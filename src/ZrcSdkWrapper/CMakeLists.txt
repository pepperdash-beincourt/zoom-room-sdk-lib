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
if(UNIX AND NOT APPLE)
    # For ARM builds, include path depends on architecture
    if(CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64")
        target_include_directories(ZrcSdkWrapper PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}
            ${CMAKE_SOURCE_DIR}/../../zrcsdkArm64/include
        )
    else()
        target_include_directories(ZrcSdkWrapper PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}
            "${CMAKE_SOURCE_DIR}/../../zrcsdk 2/include"
        )
    endif()
else()
    # For macOS/Windows development
    target_include_directories(ZrcSdkWrapper PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/../../zrcsdk/include
    )
endif()

# Define export macro
target_compile_definitions(ZrcSdkWrapper PRIVATE
    ZRCSDKWRAPPER_EXPORTS
)

# Platform-specific settings
if(UNIX AND NOT APPLE)
    # Linux-specific settings (including Crestron ARM)
    target_compile_options(ZrcSdkWrapper PRIVATE
        -Wall
        -Wextra
        -fvisibility=hidden
    )
    
    # Link with pthread and dl for dynamic loading
    target_link_libraries(ZrcSdkWrapper PRIVATE
        pthread
        dl
    )
    
    # For ARM processors, optimize accordingly
    if(CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64")
        message(STATUS "Building for ARM64 (aarch64) architecture")
        target_compile_options(ZrcSdkWrapper PRIVATE
            -march=armv8-a
        )
    elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "arm")
        message(STATUS "Building for ARM32 hard-float ABI (matching Crestron device)")
        target_compile_options(ZrcSdkWrapper PRIVATE
            -march=armv7-a
        )
    endif()
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
# Adjust the path to ZRC SDK libraries based on platform
if(UNIX AND NOT APPLE)
    # For ARM64 Linux (Crestron 4-series)
    if(CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64")
        set(ZRCSDK_LIB_DIR "${CMAKE_SOURCE_DIR}/../../zrcsdkArm64/libs")
    else()
        # For ARM32 Linux
        set(ZRCSDK_LIB_DIR "${CMAKE_SOURCE_DIR}/../../zrcsdk 2/libs/armhf")
    endif()
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
