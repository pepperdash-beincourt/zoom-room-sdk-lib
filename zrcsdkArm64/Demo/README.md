# Zoom Rooms Controller SDK Sample App

This demo app demonstrates how to pair with a Zoom Room, start meetings, and control basic functions through command line.

## 🚀 Quick Start

Run the setup script:

**macOS / Linux:**
```bash
python3 setup.py
```

**Windows:**
```cmd
python setup.py           # Auto-detect Visual Studio
python setup.py --vs2019  # Use Visual Studio 2019
python setup.py --vs2022  # Use Visual Studio 2022
```

The script will automatically check your system, build the demo, and prompt you to run it.

## 📋 Commands

**macOS / Linux:**
```bash
python3 setup.py          # Setup and build
python3 setup.py run      # Run demo
python3 setup.py build    # Rebuild
python3 setup.py clean    # Clean
python3 setup.py help     # Help
```

**Windows:**
```cmd
python setup.py           # Setup and build (auto-detect VS)
python setup.py --vs2019  # Setup with VS 2019
python setup.py --vs2022  # Setup with VS 2022
python setup.py run       # Run demo
python setup.py build     # Rebuild
python setup.py clean     # Clean
```

## 💻 Demo Usage

Once the demo is running:

| Command | Description |
|---------|-------------|
| `pair <code>` | Pair with Zoom Room (e.g., `pair 1234-5678-9012-3456`) |
| `startmeeting` | Start meeting |
| `mute` | Mute audio |
| `endmeeting` | End meeting |
| `unpair` | Unpair |
| `quit` | Exit |

**Note for Windows users:**
- The script automatically copies DLL files from `dll/` directory to the executable location
- After setup, you can open `Demo.sln` in Visual Studio to run/debug
- Use `--vs2019` or `--vs2022` to specify Visual Studio version if auto-detection fails

## 📁 File Structure

```
Demo
├── CMakeLists.txt              # CMakeLists
├── include                     # DemoApp headers folder
│   ├── MyZRCSDKApp.h
│   ├── ZRCSDKSink.h
│   ├── include                 # ZRC SDK headers folder
│   │   ├── ServiceComponents
│   ├── libuv                   # libuv headers folder
│   │   └── include
│   └── sinks.h
├── libs                        # DemoApp libs folder, macOS and windows would be different.
│   ├── libZRCSdk.so
│   └── libuv.a
└── src                         # DemoApp src folder
    ├── MyZRCSDKApp.cpp
    └── main.cpp                ## MacOS would be main.mm
```

## 📜 Open Source Licenses

This SDK uses various open source libraries. For a complete list of third-party software licenses, see:
- [`ZRC_SDK_OSS_Attribution_11_25.txt`](ZRC_SDK_OSS_Attribution_11_25.txt)

## 📖 Documentation

Make sure to review our [document](https://developers.zoom.us/docs/zoom-rooms/zoom-rooms-controller/introduction/) as a reference when building your Zoom Rooms Controller Apps.
