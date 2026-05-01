#!/bin/bash

# Build script for ZrcSdkWrapper - Cross-compile for Crestron ARM Linux
# This script builds the native wrapper library for deployment on Crestron 4-series processors

set -e  # Exit on error

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Script directory
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
PROJECT_ROOT="$( cd "$SCRIPT_DIR/../.." && pwd )"
WRAPPER_DIR="$SCRIPT_DIR"

echo -e "${GREEN}========================================${NC}"
echo -e "${GREEN}ZrcSdkWrapper Build Script${NC}"
echo -e "${GREEN}========================================${NC}"

# Parse command line arguments
BUILD_TYPE="Release"
TARGET="arm64"
CLEAN=false

while [[ $# -gt 0 ]]; do
    case $1 in
        --debug)
            BUILD_TYPE="Debug"
            shift
            ;;
        --release)
            BUILD_TYPE="Release"
            shift
            ;;
        --native)
            TARGET="native"
            shift
            ;;
        --arm)
            TARGET="arm"
            shift
            ;;
        --arm64)
            TARGET="arm64"
            shift
            ;;
        --clean)
            CLEAN=true
            shift
            ;;
        --help)
            echo "Usage: $0 [options]"
            echo "Options:"
            echo "  --debug         Build in Debug mode (default: Release)"
            echo "  --release       Build in Release mode"
            echo "  --native        Build for native platform (macOS)"
            echo "  --arm           Build for ARM32 Linux (older Crestron)"
            echo "  --arm64         Build for ARM64 Linux (Crestron 4-series - default)"
            echo "  --clean         Clean build directories before building"
            echo "  --help          Show this help message"
            exit 0
            ;;
        *)
            echo -e "${RED}Unknown option: $1${NC}"
            exit 1
            ;;
    esac
done

# Set build directory based on target
if [ "$TARGET" == "arm" ]; then
    BUILD_DIR="$WRAPPER_DIR/build-arm"
    TOOLCHAIN_FILE="$WRAPPER_DIR/arm-linux-toolchain.cmake"
    COMPILER_PREFIX="arm-linux-gnueabihf"
    echo -e "${YELLOW}Building for: ARM32 Linux hard-float (matching Crestron device ABI)${NC}"
elif [ "$TARGET" == "arm64" ]; then
    BUILD_DIR="$WRAPPER_DIR/build-arm64"
    TOOLCHAIN_FILE="$WRAPPER_DIR/arm64-linux-toolchain.cmake"
    COMPILER_PREFIX="aarch64-linux-gnu"
    echo -e "${YELLOW}Building for: ARM64 Linux (Crestron 4-series)${NC}"
else
    BUILD_DIR="$WRAPPER_DIR/build"
    TOOLCHAIN_FILE=""
    COMPILER_PREFIX=""
    echo -e "${YELLOW}Building for: Native platform ($(uname -s))${NC}"
fi

echo -e "${YELLOW}Build type: $BUILD_TYPE${NC}"

# Clean if requested
if [ "$CLEAN" = true ]; then
    echo -e "${YELLOW}Cleaning build directory...${NC}"
    rm -rf "$BUILD_DIR"
fi

# Check for ARM toolchain if cross-compiling
if [ "$TARGET" == "arm" ]; then
    if ! command -v arm-linux-gnueabihf-gcc &> /dev/null; then
        echo -e "${RED}ERROR: ARM32 hard-float cross-compiler not found!${NC}"
        echo ""
        echo "Please install the ARM32 hard-float toolchain:"
        echo ""
        echo "On macOS with Homebrew:"
        echo -e "  ${GREEN}brew tap messense/macos-cross-toolchains${NC}"
        echo -e "  ${GREEN}brew install arm-unknown-linux-gnueabihf${NC}"
        echo ""
        echo "This hard-float toolchain matches the ABI of the Crestron device."
        echo ""
        exit 1
    fi
    echo -e "${GREEN}✓ ARM32 hard-float toolchain found${NC}"
elif [ "$TARGET" == "arm64" ]; then
    if ! command -v aarch64-linux-gnu-gcc &> /dev/null; then
        echo -e "${RED}ERROR: ARM64 cross-compiler not found!${NC}"
        echo ""
        echo "Please install the ARM64 toolchain:"
        echo ""
        echo "On macOS with Homebrew:"
        echo -e "  ${GREEN}brew tap messense/macos-cross-toolchains${NC}"
        echo -e "  ${GREEN}brew install aarch64-unknown-linux-gnu${NC}"
        echo ""
        echo "On Ubuntu/Debian:"
        echo -e "  ${GREEN}sudo apt-get install gcc-aarch64-linux-gnu g++-aarch64-linux-gnu${NC}"
        echo ""
        exit 1
    fi
    echo -e "${GREEN}✓ ARM64 toolchain found${NC}"
fi

# Check for CMake
if ! command -v cmake &> /dev/null; then
    echo -e "${RED}ERROR: CMake not found!${NC}"
    echo "Please install CMake:"
    echo "  brew install cmake"
    exit 1
fi
echo -e "${GREEN}✓ CMake found: $(cmake --version | head -n1)${NC}"

# Create build directory
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

# Configure with CMake
echo ""
echo -e "${YELLOW}Configuring build...${NC}"
if [ "$TARGET" == "arm" ] || [ "$TARGET" == "arm64" ]; then
    cmake "$WRAPPER_DIR" \
        -DCMAKE_TOOLCHAIN_FILE="$TOOLCHAIN_FILE" \
        -DCMAKE_BUILD_TYPE="$BUILD_TYPE" \
        -DCMAKE_VERBOSE_MAKEFILE=ON
else
    cmake "$WRAPPER_DIR" \
        -DCMAKE_BUILD_TYPE="$BUILD_TYPE"
fi

# Build
echo ""
echo -e "${YELLOW}Building...${NC}"
cmake --build . --config "$BUILD_TYPE" -j$(sysctl -n hw.ncpu 2>/dev/null || echo 4)

# Check if build succeeded
if [ $? -eq 0 ]; then
    echo ""
    echo -e "${GREEN}========================================${NC}"
    echo -e "${GREEN}Build successful!${NC}"
    echo -e "${GREEN}========================================${NC}"
    echo ""
    
    # Find and display the output library
    if [ "$TARGET" == "arm" ]; then
        LIB_FILE=$(find "$BUILD_DIR" -name "libzrcsdkwrapperpdt.so*" -type f | head -n1)
    elif [[ "$OSTYPE" == "darwin"* ]]; then
        LIB_FILE=$(find "$BUILD_DIR" -name "libzrcsdkwrapperpdt.dylib" -type f | head -n1)
    else
        LIB_FILE=$(find "$BUILD_DIR" -name "libzrcsdkwrapperpdt.so*" -type f | head -n1)
    fi
    
    if [ -n "$LIB_FILE" ]; then
        echo -e "${GREEN}Output library:${NC}"
        echo "  $LIB_FILE"
        echo ""
        
        # Show file info
        file "$LIB_FILE"
        echo ""
        
        # Show size
        SIZE=$(du -h "$LIB_FILE" | cut -f1)
        echo -e "${GREEN}Size: $SIZE${NC}"
        
        if [ "$TARGET" == "arm" ]; then
            echo ""
            echo -e "${YELLOW}Deployment Instructions:${NC}"
            echo "1. Transfer the library to your Crestron processor:"
            echo "   scp $LIB_FILE crestron@<processor-ip>:/usr/lib/"
            echo ""
            echo "2. Ensure the ZRC SDK libraries are also on the processor"
            echo ""
            echo "3. Deploy your .NET application (.cpz) with the P/Invoke wrapper"
        fi
    else
        echo -e "${YELLOW}Warning: Output library not found in build directory${NC}"
    fi
else
    echo ""
    echo -e "${RED}========================================${NC}"
    echo -e "${RED}Build failed!${NC}"
    echo -e "${RED}========================================${NC}"
    exit 1
fi
