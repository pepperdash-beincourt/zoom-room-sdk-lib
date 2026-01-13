# ZrcSdkWrapper - Cross-Compilation Setup for Crestron ARM Linux

This guide covers building the native ZrcSdkWrapper library on macOS for deployment on Crestron 4-series processors (ARM Linux with BusyBox).

## Prerequisites

### 1. Install CMake

```bash
brew install cmake
```

### 2. Install ARM Cross-Compilation Toolchain

**Option A: Using Homebrew (Recommended)**

```bash
# Add the cross-toolchain tap
brew tap messense/macos-cross-toolchains

# Install ARM Linux GNU toolchain
brew install arm-unknown-linux-gnueabihf
```

**Option B: Manual Installation**

Download the ARM GNU Toolchain from:
https://developer.arm.com/downloads/-/gnu-a

Extract and add to your PATH:
```bash
export PATH="/path/to/arm-toolchain/bin:$PATH"
```

### 3. Verify Toolchain Installation

```bash
arm-linux-gnueabihf-gcc --version
```

You should see output similar to:
```
arm-linux-gnueabihf-gcc (GCC) 12.2.0
```

## Building

### Quick Build (ARM Linux for Crestron)

```bash
cd src/ZrcSdkWrapper
./build.sh --arm --release
```

### Build Options

```bash
./build.sh [options]

Options:
  --debug         Build in Debug mode (default: Release)
  --release       Build in Release mode
  --native        Build for native platform (macOS, for testing)
  --arm           Build for ARM Linux (Crestron)
  --clean         Clean build directories before building
  --help          Show help message
```

### Examples

Build for ARM Linux (Crestron) in Release mode:
```bash
./build.sh --arm --release
```

Build for native macOS for local testing:
```bash
./build.sh --native --debug
```

Clean build and rebuild:
```bash
./build.sh --arm --clean --release
```

## Output

After a successful build, you'll find the library at:
- **ARM Linux**: `build-arm/libZrcSdkWrapper.so`
- **macOS**: `build/libZrcSdkWrapper.dylib`

## Deployment to Crestron 4-Series

### 1. Transfer the Library

Use SCP to copy the library to your Crestron processor:

```bash
scp build-arm/libZrcSdkWrapper.so crestron@<processor-ip>:/usr/lib/
```

Or place it in your application directory and set LD_LIBRARY_PATH:
```bash
scp build-arm/libZrcSdkWrapper.so crestron@<processor-ip>:/home/application/libs/
```

### 2. Transfer ZRC SDK Libraries

Ensure the ZRC SDK native libraries are also on the processor:

```bash
scp zrcsdk/libs/linux-arm/*.so crestron@<processor-ip>:/usr/lib/
```

### 3. Set Library Path (if not using /usr/lib)

If you placed libraries in a custom directory:

```bash
export LD_LIBRARY_PATH=/home/application/libs:$LD_LIBRARY_PATH
```

Or add to your application startup script.

### 4. Deploy .NET Application

Build your C# project and deploy the .cpz file to the processor:

```bash
dotnet build src/ZrcSdkTest/ZrcSdkTest.csproj -c Release
# Follow Crestron's deployment process for .NET applications
```

## Troubleshooting

### Library Not Found at Runtime

If you get `DllNotFoundException` at runtime:

1. Verify the library is in a standard location:
   ```bash
   ssh crestron@<processor-ip> "ls -la /usr/lib/libZrcSdkWrapper.so"
   ```

2. Check library dependencies:
   ```bash
   arm-linux-gnueabihf-readelf -d build-arm/libZrcSdkWrapper.so
   ```

3. Verify processor architecture:
   ```bash
   ssh crestron@<processor-ip> "uname -m"
   ```

### Symbol Not Found Errors

If you get symbol resolution errors:

1. Check that ZRC SDK libraries are present
2. Verify library versions match
3. Check symbol visibility:
   ```bash
   arm-linux-gnueabihf-nm -D build-arm/libZrcSdkWrapper.so | grep ZrcSdk_
   ```

### Cross-Compilation Fails

If the toolchain can't be found:

1. Verify installation:
   ```bash
   which arm-linux-gnueabihf-gcc
   ```

2. Check the toolchain file paths in `arm-linux-toolchain.cmake`

3. Try specifying compiler manually:
   ```bash
   export CC=arm-linux-gnueabihf-gcc
   export CXX=arm-linux-gnueabihf-g++
   ./build.sh --arm
   ```

## Manual Build (Advanced)

If you need more control over the build process:

```bash
cd src/ZrcSdkWrapper
mkdir -p build-arm
cd build-arm

cmake .. \
  -DCMAKE_TOOLCHAIN_FILE=../arm-linux-toolchain.cmake \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_VERBOSE_MAKEFILE=ON

cmake --build . -j$(sysctl -n hw.ncpu)
```

## Testing on macOS

You can build a native version for testing the wrapper logic (though it won't run the actual ZRC SDK):

```bash
./build.sh --native --debug
```

Then test with a simple .NET console app:
```bash
cd ../ZrcSdkTest
dotnet run
```

## Project Structure

```
src/ZrcSdkWrapper/
├── CMakeLists.txt              # CMake build configuration
├── arm-linux-toolchain.cmake   # ARM cross-compilation toolchain
├── build.sh                    # Automated build script
├── ZrcSdkWrapper_C.h           # C API header
├── ZrcSdkWrapper_C.cpp         # C API implementation
├── build/                      # Native build output (macOS)
└── build-arm/                  # ARM cross-compilation output
```

## Additional Resources

- [ARM GNU Toolchain Documentation](https://developer.arm.com/documentation)
- [CMake Cross Compiling](https://cmake.org/cmake/help/latest/manual/cmake-toolchains.7.html)
- [Crestron 4-Series Programming Guide](https://www.crestron.com/)

## Support

For issues specific to:
- **Build system**: Check CMakeLists.txt and toolchain file
- **ZRC SDK**: Refer to ZRC SDK documentation
- **Crestron deployment**: Consult Crestron technical support
