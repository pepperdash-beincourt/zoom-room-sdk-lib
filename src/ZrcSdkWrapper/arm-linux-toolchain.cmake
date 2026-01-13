# CMake toolchain file for cross-compiling to ARM Linux (Crestron 4-series)
# Usage: cmake -DCMAKE_TOOLCHAIN_FILE=arm-linux-toolchain.cmake ..

set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR arm)

# Specify the cross compiler
# Option 1: Using arm-linux-gnueabihf toolchain (most common for ARM Linux)
set(TOOLCHAIN_PREFIX arm-linux-gnueabihf)

# Option 2: Uncomment if using Crestron's specific toolchain
# set(TOOLCHAIN_PREFIX arm-crestron-linux-gnueabi)

# Set compiler paths
# Adjust these paths based on where your toolchain is installed
# Common locations:
#   - Homebrew: /opt/homebrew/bin or /usr/local/bin
#   - Manual install: /usr/local/arm-toolchain/bin
#   - Crestron SDK: /opt/crestron/toolchain/bin

find_program(CMAKE_C_COMPILER NAMES ${TOOLCHAIN_PREFIX}-gcc)
find_program(CMAKE_CXX_COMPILER NAMES ${TOOLCHAIN_PREFIX}-g++)
find_program(CMAKE_AR NAMES ${TOOLCHAIN_PREFIX}-ar)
find_program(CMAKE_RANLIB NAMES ${TOOLCHAIN_PREFIX}-ranlib)
find_program(CMAKE_STRIP NAMES ${TOOLCHAIN_PREFIX}-strip)

# If compilers not found in PATH, try common locations
if(NOT CMAKE_C_COMPILER)
    set(CMAKE_C_COMPILER /opt/homebrew/bin/${TOOLCHAIN_PREFIX}-gcc)
    set(CMAKE_CXX_COMPILER /opt/homebrew/bin/${TOOLCHAIN_PREFIX}-g++)
endif()

# Verify compiler exists
if(NOT EXISTS ${CMAKE_C_COMPILER})
    message(FATAL_ERROR "Cross compiler not found: ${CMAKE_C_COMPILER}\n"
        "Please install the ARM toolchain:\n"
        "  brew install arm-linux-gnueabihf-binutils\n"
        "Or download from: https://developer.arm.com/downloads/-/gnu-a")
endif()

message(STATUS "Using C compiler: ${CMAKE_C_COMPILER}")
message(STATUS "Using C++ compiler: ${CMAKE_CXX_COMPILER}")

# Set the target root path (sysroot) - where target libraries are located
# Adjust this path if you have a Crestron sysroot
# set(CMAKE_SYSROOT /path/to/crestron/sysroot)

# Search for programs only in the build host directories
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)

# Search for libraries and headers in the target directories
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

# ARM-specific compiler flags for Crestron 4-series
set(CMAKE_C_FLAGS_INIT "-march=armv7-a -mfpu=neon -mfloat-abi=hard -mtune=cortex-a9")
set(CMAKE_CXX_FLAGS_INIT "-march=armv7-a -mfpu=neon -mfloat-abi=hard -mtune=cortex-a9")

# Additional flags for shared libraries
set(CMAKE_SHARED_LINKER_FLAGS_INIT "-Wl,-rpath-link,${CMAKE_SYSROOT}/lib:${CMAKE_SYSROOT}/usr/lib")

# Build type defaults
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Set PIC for shared libraries
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
