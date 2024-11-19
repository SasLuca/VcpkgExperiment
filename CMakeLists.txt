cmake_minimum_required(VERSION 3.29)
project(VcpkgExperiment)

find_package(fmt CONFIG REQUIRED)
find_package(ffmpeg REQUIRED)
find_package(SDL3 CONFIG REQUIRED)

# set(SDL_TEST OFF CACHE BOOL "Disable SDL3 test building")
# set(SDL_EXAMPLES OFF CACHE BOOL "Disable SDL3 examples")
# set(SDL3_DISABLE_DOCS ON CACHE BOOL "Disable building SDL3 documentation")
# set(SDL_SHARED OFF CACHE BOOL "Build SDL3 as a shared library")
# set(SDL_STATIC ON CACHE BOOL "Build SDL3 as a static library")
# add_subdirectory(thirdparty/SDL)

if (WIN32)
add_executable(VcpkgExperiment sources/main.cpp)
target_compile_features(VcpkgExperiment PRIVATE cxx_std_20)
target_include_directories(VcpkgExperiment PRIVATE ${FFMPEG_INCLUDE_DIRS})
target_link_directories(VcpkgExperiment PRIVATE ${FFMPEG_LIBRARY_DIRS})
target_link_libraries(VcpkgExperiment PRIVATE fmt::fmt ${FFMPEG_LIBRARIES} SDL3::SDL3)
endif()

if (IOS)

if(NOT DEFINED MY_IOS_TEAM_ID)
    message(FATAL_ERROR "The variable MY_IOS_TEAM_ID is not set. Please set it to your Apple Developer Team ID.")
endif()

add_executable(
    VcpkgExperiment
    MACOSX_BUNDLE
    sources/main.cpp
    sources/LaunchScreen.storyboard
)
target_compile_features(VcpkgExperiment PRIVATE cxx_std_20)
set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/Info.plist PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
target_link_libraries(
    VcpkgExperiment
    PRIVATE
    fmt::fmt
    SDL3::SDL3
    ${FFMPEG_LIBRARIES}
    "-framework UIKit"
    "-framework AVFoundation"
    "-framework CoreGraphics"
    "-framework GameController"
    "-framework OpenGLES"
    "-framework AudioToolbox"
    "-framework QuartzCore"
    "-framework CoreMotion"
    "-framework CoreAudio"
    "-framework Metal"
)
set_target_properties(VcpkgExperiment PROPERTIES
    # Set the Info.plist file for Apple Mobile platforms. Without this file, your app will not launch.
    MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/Info.plist
    # in Xcode, create a Scheme in the schemes dropdown for the app.
    XCODE_GENERATE_SCHEME TRUE
    XCODE_ATTRIBUTE_CURRENT_PROJECT_VERSION "1.0"  # Set project version
    XCODE_ATTRIBUTE_MARKETING_VERSION "1.0" # Set project version
    XCODE_ATTRIBUTE_VERSIONING_SYSTEM "apple-generic"
    XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "iPhone Developer"
    XCODE_ATTRIBUTE_DEVELOPMENT_TEAM ${MY_IOS_TEAM_ID}
    XCODE_ATTRIBUTE_TARGETED_DEVICE_FAMILY "1" # 1=iPhone, 2=iPad
    XCODE_ATTRIBUTE_PRODUCT_BUNDLE_IDENTIFIER "com.lucasas.vcpkgexperiment"
    XCODE_ATTRIBUTE_BUNDLE_IDENTIFIER "com.lucasas.vcpkgexperiment"
    XCODE_ATTRIBUTE_CURRENTYEAR "${CURRENTYEAR}"
)
endif()