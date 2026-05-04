# PepperDash.ZoomRoom.Sdk — Developer Guide

C# P/Invoke wrapper for the **Zoom Rooms Controller SDK (ZRC SDK)**, targeting Crestron control systems running ARM32 Linux (e.g. CP4N).

> **Looking for consumer docs?** See the NuGet package page or the `example/ZrcSdkExample/` project for usage.

## Repository Structure

```
src/
  ZoomRoomSdk/          # C# library (net8.0) — the NuGet package
  ZoomRoomSdk.Tests/    # xUnit tests for pure C# logic
  ZrcSdkWrapper/        # C++ → C wrapper, cross-compiled to ARM32
example/
  ZrcSdkExample/        # Standalone Crestron program using the library
  ZrcSdkExample.slnx    # Open this separately — not part of the main solution
zrcsdkArm/              # ZRC SDK 6.5.5 headers and ARM32 .so (armhf)
zrcsdkArm64/            # ZRC SDK 6.5.5 headers and ARM64 .so
PepperDash.ZoomRoom.Sdk.slnx  # Main solution (library + tests only)
```

## Architecture

```
ZRC SDK (libZRCSdk.so)         — proprietary, pre-installed on device
        ↑
ZrcSdkWrapper (C)              — thin C shim exposing a C ABI
  libzrcsdkwrapperpdt.so         cross-compiled ARM32 via arm-linux-gnueabihf-g++
        ↑
ZoomRoomSdk (C#)               — P/Invoke wrapper, partial class per domain
  PepperDash.ZoomRoom.Sdk.dll    net8.0, loaded via memfd_create on Crestron
```

The C wrapper is necessary because the ZRC SDK exposes a C++ ABI that cannot be P/Invoked directly. The C# library loads itself via `memfd_create` + `dlopen(/proc/self/fd/N)` at runtime to work around Crestron's `noexec` filesystem mounts.

## Prerequisites

| Tool | Version | Notes |
|---|---|---|
| .NET SDK | 10.x | `dotnet --version` |
| CMake | 3.x+ | `brew install cmake` |
| ARM32 cross-compiler | GCC 15.x | See below |

### ARM32 Cross-Compiler (macOS)

```bash
brew tap messense/macos-cross-toolchains
brew install arm-unknown-linux-gnueabihf
```

Verify:
```bash
arm-linux-gnueabihf-g++ --version
```

> Full cross-compilation details are in [src/ZrcSdkWrapper/README.md](src/ZrcSdkWrapper/README.md).

## Building

### C# library + tests (no native build required)

```bash
dotnet build PepperDash.ZoomRoom.Sdk.slnx
```

### Native ARM32 wrapper

```bash
src/ZrcSdkWrapper/build.sh --arm --release --clean
```

Output: `src/ZrcSdkWrapper/build-arm/libzrcsdkwrapperpdt.so.1.0.0`

Copy it into the library's native asset directory before building the NuGet package:
```bash
cp src/ZrcSdkWrapper/build-arm/libzrcsdkwrapperpdt.so.1.0.0 \
   src/ZoomRoomSdk/native/linux-arm/libzrcsdkwrapperpdt.so
```

## Running Tests

```bash
dotnet test src/ZoomRoomSdk.Tests/
```

Tests cover `ZrcSdkCodes` translation methods, enum integer values, and POCO models. No native library or Crestron SDK required.

## Project Layout — C# Library

The `ZrcSdk` class is split into partial class files by domain:

| File | Domain |
|---|---|
| `ZrcSdk.Core.cs` | Lifecycle, P/Invoke declarations, DllImportResolver |
| `ZrcSdk.Connection.cs` | Connection state, wake, logout, restart |
| `ZrcSdk.Pairing.cs` | Activation code pairing, retry, unpair |
| `ZrcSdk.Meeting.cs` | Start, join, leave, end, password |
| `ZrcSdk.Audio.cs` | Mute, unmute, mute-on-entry |
| `ZrcSdk.Video.cs` | Video state, pin, spotlight |
| `ZrcSdk.Participants.cs` | Participant list and count events |
| `ZrcSdk.Recording.cs` | Start, stop, pause, resume, request events |
| `ZrcSdk.Share.cs` | Screen sharing |
| `ZrcSdk.Layout.cs` | Screen layout, video order |
| `ZrcSdk.BreakoutRoom.cs` | Breakout room management |
| `ZrcSdk.Chat.cs` | In-meeting chat |
| `ZrcSdk.Camera.cs` | Camera control, far-end camera |
| `ZrcSdk.Phone.cs` | SIP / Zoom Phone calls |
| `ZrcSdk.WaitingRoom.cs` | Admit, put back, silent mode |
| `ZrcSdk.Reactions.cs` | Hand raise/lower, emoji reactions |
| `ZrcSdk.QA.cs` | Q&A, closed captions |
| `ZrcSdk.Zrcs.cs` | Zoom Room Control System devices and scenes |
| `ZrcSdk.Polling.cs` | Polling stub |
| `ZrcSdk.Settings.cs` | Settings stub |
| `ZrcSdk.ProAV.cs` | Pro AV stub |

## CI

Two jobs in `.github/workflows/build.yml`:

1. **`testZoomRoomSdkLibrary`** — runs `dotnet test`, publishes a trx report via `dorny/test-reporter`. Must pass before the build job starts.
2. **`buildZoomRoomSdkLibrary`** — installs ARM toolchain, builds native wrapper, builds solution, publishes NuGet to GitHub Packages.

Both jobs clean up the tag created by `GetVersion` if they fail.

## Example Program

`example/ZrcSdkExample/` is a standalone Crestron program that demonstrates library usage. Open `example/ZrcSdkExample.slnx` separately — it is not part of the main solution and is excluded from CI.

## License

MIT — see [LICENSE](LICENSE)

The ZRC SDK itself (`libZRCSdk.so`) is proprietary software © Zoom Video Communications, Inc.

