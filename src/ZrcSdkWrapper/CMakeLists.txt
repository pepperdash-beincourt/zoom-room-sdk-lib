cmake_minimum_required(VERSION 3.15)
project(ZrcSdkWrapper VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Build as shared library
add_library(ZrcSdkWrapper SHARED
    ZrcSdkWrapper_C.cpp
    ZrcSdkWrapper_C.h
)

# Set output name (will produce libZrcSdkWrapper.so on Linux)
set_target_properties(ZrcSdkWrapper PROPERTIES
    OUTPUT_NAME "zrcsdkwrapperpdt"
    VERSION ${PROJECT_VERSION}
    SOVERSION 1
    POSITION_INDEPENDENT_CODE ON
)

# Add include directories
target_include_directories(ZrcSdkWrapper PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/../../zrcsdk/include
)

# Define export macro
target_compile_definitions(ZrcSdkWrapper PRIVATE
    ZRCSDKWRAPPER_EXPORTS
)

# Platform-specific settings
if(UNIX AND NOT APPLE)
    # Linux ARM32 hard-float (Crestron device)
    message(STATUS "Building for ARM32 hard-float ABI (matching Crestron device)")
    target_compile_options(ZrcSdkWrapper PRIVATE
        -Wall
        -Wextra
        -fvisibility=hidden
        -march=armv7-a
    )
    target_link_libraries(ZrcSdkWrapper PRIVATE
        pthread
        dl
    )
    # NOTE: do NOT statically link libstdc++/libgcc here. Crestron 4-series firmware provides
    # libstdc++ up to GLIBCXX_3.4.28 and glibc up to GLIBC_2.30; static-linking pulls the build
    # toolchain's newer libstdc++ (and its modern glibc calls, GLIBC_2.38) directly into the .so,
    # which the device can't satisfy. Compatibility is instead controlled by the build TOOLCHAIN
    # (GCC 9 via the pinned ubuntu:20.04 container in CI → GLIBCXX_3.4.27). See PR #4.
elseif(APPLE)
    # macOS-specific settings for development/testing
    target_compile_options(ZrcSdkWrapper PRIVATE
        -Wall
        -Wextra
        -fvisibility=hidden
    )
elseif(WIN32)
    # Windows-specific settings
    target_compile_definitions(ZrcSdkWrapper PRIVATE
        _WINDOWS
        _USRDLL
    )
endif()

# Link with ZRC SDK libraries
if(UNIX AND NOT APPLE)
    set(ZRCSDK_LIB_DIR "${CMAKE_SOURCE_DIR}/../../zrcsdk/libs/armhf")
elseif(APPLE)
    # For macOS development
    set(ZRCSDK_LIB_DIR "${CMAKE_SOURCE_DIR}/../../zrcsdk/libs/macos")
elseif(WIN32)
    # For Windows
    set(ZRCSDK_LIB_DIR "${CMAKE_SOURCE_DIR}/../../zrcsdk/libs/windows")
endif()

# Add library search path if it exists
if(EXISTS ${ZRCSDK_LIB_DIR})
    target_link_directories(ZrcSdkWrapper PRIVATE ${ZRCSDK_LIB_DIR})
    message(STATUS "Using ZRC SDK libraries from: ${ZRCSDK_LIB_DIR}")
else()
    message(WARNING "ZRC SDK library directory not found: ${ZRCSDK_LIB_DIR}")
endif()

# Installation
install(TARGETS ZrcSdkWrapper
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
)

install(FILES ZrcSdkWrapper_C.h
    DESTINATION include
)

# Print build information
message(STATUS "Building ZrcSdkWrapper")
message(STATUS "  System: ${CMAKE_SYSTEM_NAME}")
message(STATUS "  Processor: ${CMAKE_SYSTEM_PROCESSOR}")
message(STATUS "  Compiler: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "  Build type: ${CMAKE_BUILD_TYPE}")
