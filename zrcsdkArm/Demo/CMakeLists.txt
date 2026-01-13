cmake_minimum_required(VERSION 3.5)
project(Demo)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)

# Set source files
if(UNIX AND APPLE)
set(SOURCES
    src/main.mm
    src/MyZRCSDKApp.cpp
)
else()
set(SOURCES
    src/main.cpp
    src/MyZRCSDKApp.cpp
)
endif()

# Add executable
add_executable(Demo ${SOURCES})

# Link directories
target_link_directories(Demo PUBLIC ${PROJECT_SOURCE_DIR}/libs)

# Set include directories
target_include_directories(Demo PUBLIC ${PROJECT_SOURCE_DIR}/)
target_include_directories(Demo PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_include_directories(Demo PUBLIC ${PROJECT_SOURCE_DIR}/include/libuv/include)
target_include_directories(Demo PUBLIC ${PROJECT_SOURCE_DIR}/include/include)
target_include_directories(Demo PUBLIC ${PROJECT_SOURCE_DIR}/include/include/ServiceComponents)
target_include_directories(Demo PUBLIC ${PROJECT_SOURCE_DIR}/libs)

# Link the executable with libuv
if (MSVC) # win

ADD_DEFINITIONS(-DUNICODE -D_UNICODE)

target_link_libraries(Demo
    PUBLIC
    ZRCSdk.lib
)

elseif(UNIX AND APPLE) # mac

ADD_DEFINITIONS(-D__MACOS__)

set(CMAKE_CXX_FLAGS "-x objective-c++")
set(CMAKE_MACOSX_RPATH OFF)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH FALSE)

set(LIBDIR ${PROJECT_SOURCE_DIR}/libs)
set_target_properties(Demo PROPERTIES
    XCODE_ATTRIBUTE_LD_RUNPATH_SEARCH_PATHS "${LIBDIR}"
    LINK_FLAGS "-Wl -F${LIBDIR}")

target_link_libraries(Demo
    PUBLIC
    "-framework Cocoa"
    "-framework ZRCSdk"
)

elseif(UNIX AND NOT APPLE) # linux

target_link_libraries(Demo
    PUBLIC
    libuv.a
    atomic
    pthread
    dl
    libZRCSdk.so
)

endif()
