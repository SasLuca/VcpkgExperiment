cmake_minimum_required(VERSION 3.29)
project(VcpkgExperiment)

#region Flags to improve debugging
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND CMAKE_CXX_SIMULATE_ID STREQUAL "MSVC")
        # message(STATUS "Compiler is Clang-CL")
        add_compile_options(
                /D_ITERATOR_DEBUG_LEVEL=0 /D_SECURE_SCL=0
                /Od        # Disable all optimizations
                /Z7        # Generate debugging information
                /Ob0       # Disable function inlining
                /Oy-       # Do not omit frame pointers
                -gcodeview # Generate CodeView debug information (Clang-specific)
                /MT       # Multi core dll runtime for debug
                /RTC1      # Enable basic runtime error checks
        )
    endif()


    if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND NOT CMAKE_CXX_SIMULATE_ID STREQUAL "MSVC")
        # message(STATUS "Compiler is Clang, but not Clang-CL")
        add_compile_options(
                /MD /Zi /Od /Ob0 /D_ITERATOR_DEBUG_LEVEL=2
                -O0                     # Disable all optimizations
                -g                      # Generate debugging information
                -fno-inline             # Disable function inlining
                -fno-omit-frame-pointer # Do not omit frame pointers
                -gcodeview              # Generate CodeView debug information
                # -fsanitize=undefined    # Enable undefined behavior checks
                # -fsanitize=address      # Enable address sanitizer (memory checks)
        )
    endif()
endif()
#endregion

find_package(fmt CONFIG REQUIRED)
#find_package(FFMPEG REQUIRED)
find_package(SDL3 CONFIG REQUIRED)
find_package(unofficial-angle REQUIRED CONFIG)

add_subdirectory(thirdparty/skia/skia)

if (WIN32)
    add_executable(VcpkgExperiment sources/main.cpp)
    target_compile_features(VcpkgExperiment PRIVATE cxx_std_20)
 #   target_include_directories(VcpkgExperiment PRIVATE ${FFMPEG_INCLUDE_DIRS})
  #  target_link_directories(VcpkgExperiment PRIVATE ${FFMPEG_LIBRARY_DIRS})
    target_link_libraries(VcpkgExperiment PRIVATE
            #${FFMPEG_LIBRARIES}
            fmt::fmt SDL3::SDL3 unofficial::angle::libEGL unofficial::angle::libGLESv2 skia)
    target_compile_definitions(VcpkgExperiment PUBLIC SK_GL SK_GANESH)
    target_compile_definitions(VcpkgExperiment PUBLIC assets_path="${CMAKE_CURRENT_SOURCE_DIR}/assets/")
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
        #${FFMPEG_LIBRARIES}
        unofficial::angle::libGLESv2
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