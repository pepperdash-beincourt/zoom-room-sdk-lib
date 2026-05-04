# PepperDash.ZoomRoom.Sdk

C# P/Invoke wrapper for the **Zoom Rooms Controller SDK (ZRC SDK)**, targeting Crestron control systems running ARM32 Linux (e.g. CP4N).

## Requirements

| Requirement | Details |
|---|---|
| .NET | net8.0 |
| Runtime | `linux-arm` (ARMv7-A hard-float) |
| Device | Crestron CP4N or similar ARM32 Linux Crestron appliance |
| ZRC SDK | `libZRCSdk.so` installed at `/usr/lib/` on the device (ZRC SDK 6.5.5+) |

## Installation

```
dotnet add package PepperDash.ZoomRoom.Sdk
```

The NuGet package includes the compiled C wrapper (`libzrcsdkwrapperpdt.so`) as a `runtimes/linux-arm/native/` asset. The proprietary `libZRCSdk.so` must be separately installed on the target device.

## Quick Start

```csharp
using PepperDash.ZoomRoom.Sdk;

// Optionally override the library search path (useful for testing outside Crestron)
// ZrcSdk.SetLibraryPath("/path/to/libs");

var sdk = new ZrcSdk();

sdk.Initialized         += (s, e) => Console.WriteLine($"SDK initialized: {e.ErrorCode}");
sdk.PairRoomResult      += (s, e) => Console.WriteLine(ZrcSdkCodes.GetPairRoomResultDescription(e.ErrorCode));
sdk.ConnectionStateChanged += (s, e) => Console.WriteLine(ZrcSdkCodes.GetConnectionStateDescription(e.ErrorCode));
sdk.MeetingStatus       += (s, e) => Console.WriteLine(ZrcSdkCodes.GetMeetingStatusDescription(e.ErrorCode));

// Initialize — blocks until IZRCSDK::CreateInstance() returns, then starts the HeartBeat loop
bool ok = sdk.Initialize("/user/zrcsdk");

if (sdk.CanRetryToPairLastRoom())
    sdk.RetryToPairRoom();          // reconnect with stored credentials
else
    sdk.PairRoomWithActivationCode("XXXXXX");  // first-time pair

// Clean shutdown — flushes credentials to disk, calls IZRCSDK::DestroyInstance()
sdk.Dispose();
```

## On Crestron Systems

Subscribe to `CrestronEnvironment.ProgramStatusEventHandler` to ensure clean shutdown:

```csharp
CrestronEnvironment.ProgramStatusEventHandler += (type) =>
{
    if (type == eProgramStatusEventType.Stopping)
        sdk?.Dispose();
};
```

> **Note:** All writable paths on Crestron devices (`/simpl/app`, `/user`, `/var/volatile`) are mounted `noexec`. The library loads itself via `memfd_create` + `dlopen(/proc/self/fd/N)` to bypass this restriction automatically.

## Key APIs

### Lifecycle

| Method | Description |
|---|---|
| `Initialize(configPath)` | Start SDK; HeartBeat loop runs until `Dispose()` |
| `Dispose()` | Stop HeartBeat, flush credentials, destroy native instance |
| `GetSDKVersion()` | Returns the ZRC SDK version string |
| `SetLibraryPath(dir)` | (static) Override native library search directory |

### Pairing

| Method | Description |
|---|---|
| `CanRetryToPairLastRoom()` | Returns `true` if stored credentials are available |
| `RetryToPairRoom()` | Reconnect without an activation code |
| `PairRoomWithActivationCode(code)` | First-time pair |
| `UnpairRoom()` | Unpair and clear credentials |

### Meeting Control

| Method | Description |
|---|---|
| `StartInstantMeeting()` | Start a PMI/instant meeting |
| `JoinMeeting(number)` | Join by meeting number |
| `JoinMeetingWithURL(url)` | Join via invite URL |
| `StartMeetingWithHostKey(key)` | Start a scheduled meeting |
| `EndMeeting()` | End for all (host only) |
| `LeaveMeeting()` | Leave (meeting continues) |
| `SendMeetingPassword(pw)` | Respond to `MeetingNeedsPassword` |

### Audio / Video / Recording

`SetAudioMute(bool)` · `SetVideoState(bool)` · `StartRecording()` · `StopRecording()` · `PauseRecording()` · `ResumeRecording()`

### Zoom Room Control System (ZRCS)

`IsZRCSEnabled()` · `OpenRoomControls(bool)` · `ControlZRCSDevice(...)` · `ExecuteZRCSScene(sceneID)`

## Error Codes

Use the `ZrcSdkCodes` static class to convert integer codes to descriptions:

```csharp
ZrcSdkCodes.GetErrorDescription(code)
ZrcSdkCodes.GetPairRoomResultDescription(code)
ZrcSdkCodes.GetMeetingStatusDescription(code)
ZrcSdkCodes.GetConnectionStateDescription(code)
ZrcSdkCodes.GetExitMeetingReasonDescription(reason)
```

## License

MIT — see [LICENSE](../../LICENSE)

The ZRC SDK itself (`libZRCSdk.so`) is proprietary software © Zoom Video Communications, Inc.
