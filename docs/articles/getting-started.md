# Getting Started

## Requirements

- .NET 8.0
- Crestron CP4N (or any Crestron ARMv7-A hard-float Linux appliance)
- ZRC SDK 6.5.5+ — `libZRCSdk.so` installed at `/usr/lib/` on the device
- Zoom Rooms activation code (from the Zoom admin portal)

## Installation

Add the package to your Crestron project:

```
dotnet add package PepperDash.ZoomRoom.Sdk
```

The NuGet package bundles the compiled C wrapper (`libzrcsdkwrapperpdt.so`) as a
`runtimes/linux-arm/native/` asset, which is automatically copied to the output directory.
The proprietary `libZRCSdk.so` must be separately installed on the device.

## Basic Setup

```csharp
using PepperDash.ZoomRoom.Sdk;

var sdk = new ZrcSdk();

sdk.Initialized            += (s, e) => Console.WriteLine($"SDK ready, result: {e.ErrorCode}");
sdk.ConnectionStateChanged += (s, e) => Console.WriteLine(ZrcSdkCodes.GetConnectionStateDescription(e.ErrorCode));
sdk.PairRoomResult         += (s, e) => Console.WriteLine(ZrcSdkCodes.GetPairRoomResultDescription(e.ErrorCode));
sdk.MeetingStatus          += (s, e) => Console.WriteLine(ZrcSdkCodes.GetMeetingStatusDescription(e.ErrorCode));
sdk.Error                  += (s, e) => Console.WriteLine($"SDK error {e.ErrorCode}: {ZrcSdkCodes.GetErrorDescription(e.ErrorCode)}");

// "/user/zrcsdk" is a persistent writable path on Crestron devices
bool ok = sdk.Initialize("/user/zrcsdk");
if (!ok)
{
    Console.WriteLine("SDK initialization failed");
    return;
}

// Reconnect if credentials are stored, otherwise prompt for activation code
if (sdk.CanRetryToPairLastRoom())
    sdk.RetryToPairRoom();
else
    sdk.PairRoomWithActivationCode("XXXXXX");
```

## SDK Threading Model

The ZRC SDK requires that `IZRCSDK::CreateInstance()` and all `HeartBeat()` calls originate from
the **same thread**. This library manages a dedicated `ZrcSdkThread` internally — you never need to
call `HeartBeat` or manage threading manually.

`Initialize()` blocks until the SDK instance is created, then starts the 150 ms HeartBeat loop in
the background. All event callbacks are invoked from that same thread.

## Shutdown

Always call `Dispose()` before process exit to flush credentials to disk. Without this the SDK
cannot re-pair on the next boot (error 5002).

```csharp
sdk.Dispose();
```
