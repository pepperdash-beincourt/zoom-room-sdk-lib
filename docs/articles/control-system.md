# Crestron Integration

## ControlSystem Setup

The recommended pattern for Crestron `ControlSystem` integration:

```csharp
using Crestron.SimplSharp;
using Crestron.SimplSharpPro;
using PepperDash.Zoom.ZrcSdk;

namespace MyApp;

public class ControlSystem : CrestronControlSystem
{
    private ZrcSdk? _zrcSdk;
    private CTimer? _startupTimer;

    public ControlSystem() : base() { }

    public override void InitializeSystem()
    {
        // Subscribe before the process can exit
        CrestronEnvironment.ProgramStatusEventHandler += OnProgramStatus;

        // Delay SDK startup to allow Crestron runtime to fully initialize
        _startupTimer = new CTimer(_ => LoadZrcSdk(), 5000);
    }

    private void LoadZrcSdk()
    {
        _zrcSdk = new ZrcSdk();

        _zrcSdk.Initialized            += (s, e) => CrestronConsole.PrintLine($"[ZrcSdk] Initialized: {e.ErrorCode}");
        _zrcSdk.PairRoomResult         += (s, e) => CrestronConsole.PrintLine($"[ZrcSdk] Pair result: {ZrcSdkCodes.GetPairRoomResultDescription(e.ErrorCode)}");
        _zrcSdk.ConnectionStateChanged += (s, e) => CrestronConsole.PrintLine($"[ZrcSdk] Connection: {ZrcSdkCodes.GetConnectionStateDescription(e.ErrorCode)}");
        _zrcSdk.MeetingStatus          += (s, e) => CrestronConsole.PrintLine($"[ZrcSdk] Meeting: {ZrcSdkCodes.GetMeetingStatusDescription(e.ErrorCode)}");
        _zrcSdk.Error                  += (s, e) => CrestronConsole.PrintLine($"[ZrcSdk] Error {e.ErrorCode}: {ZrcSdkCodes.GetErrorDescription(e.ErrorCode)}");

        bool ok = _zrcSdk.Initialize("/user/zrcsdk");
        if (!ok)
        {
            CrestronConsole.PrintLine("[ZrcSdk] Initialization failed");
            return;
        }

        if (_zrcSdk.CanRetryToPairLastRoom())
            _zrcSdk.RetryToPairRoom();
        else
            CrestronConsole.PrintLine("[ZrcSdk] No stored credentials — send 'pairroom <code>' to pair");

        // Register console commands
        CrestronConsole.AddNewConsoleCommand(
            args => { _zrcSdk?.PairRoomWithActivationCode(args.Trim()); },
            "pairroom", "Pair Zoom Room with activation code");

        CrestronConsole.AddNewConsoleCommand(
            _ => { _zrcSdk?.StartInstantMeeting(); },
            "startinstant", "Start an instant meeting");

        CrestronConsole.AddNewConsoleCommand(
            _ => { _zrcSdk?.EndMeeting(); },
            "endmeeting", "End the current meeting");
    }

    private void OnProgramStatus(eProgramStatusEventType type)
    {
        if (type == eProgramStatusEventType.Stopping)
        {
            CrestronConsole.PrintLine("[ZrcSdk] Program stopping — disposing SDK");
            _zrcSdk?.Dispose();
            _zrcSdk = null;
        }
    }
}
```

## Why `CrestronEnvironment.ProgramStatusEventHandler`?

Crestron does not guarantee that `ControlSystem` finalizers or `ShutdownProcedure()` are called
before the process is killed. `ProgramStatusEventHandler` with `eProgramStatusEventType.Stopping`
is the only reliable hook for cleanup code.

The SDK's `Dispose()` stops the HeartBeat loop, calls `IZRCSDK::ForceFlushLog()`, and then
`IZRCSDK::DestroyInstance()` — all on the SDK owner thread. This ensures encrypted credentials are
written to `/user/zrcsdk/ZRCSDK.conf` before exit, allowing `RetryToPairRoom()` to succeed on the
next boot.

## Console Commands

The PoC project (`ZrcSdkTest`) registers these commands via `CrestronConsole.AddNewConsoleCommand`:

| Command | Action |
|---|---|
| `pairroom <code>` | Pair with activation code |
| `retrypair` | Reconnect with stored credentials |
| `unpairroom` | Unpair and clear credentials |
| `startinstant` | Start instant meeting |
| `endmeeting` | End meeting (host) |
| `leavemeeting` | Leave meeting |
| `joinmeeting <num>` | Join by meeting number |
| `joinurl <url>` | Join via invite URL |
| `starthostkey <key>` | Start meeting with host key |
| `meetingpw <pw>` | Send meeting password |
| `recstart/recstop/recpause/recresume` | Recording control |
| `partcount` | Print participant count |
| `zrwake` | Wake Zoom Room display |
| `zrlogout` | Logout Zoom Room device |
| `zrrestart` | Restart Zoom Room OS |
| `zrcsenabled` | Check ZRCS enabled |
| `zrcsopen` | Open room controls |
| `zrcsdevice <d> <m> <p> <v>` | Control ZRCS device |
| `zrcsscene <id>` | Execute ZRCS scene |
| `zrstate` | Print connection state |

## noexec Mount Points

All writable paths on Crestron devices (`/simpl/app`, `/user`, `/var/volatile`) are mounted
`noexec`. The library handles this automatically by loading itself via Linux `memfd_create`
(syscall 385) and `dlopen(/proc/self/fd/N)` — no configuration required.
