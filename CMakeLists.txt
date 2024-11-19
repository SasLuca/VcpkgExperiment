cmake_minimum_required(VERSION 3.29)
project(VcpkgExperiment)

find_package(fmt CONFIG REQUIRED)
find_package(ffmpeg REQUIRED)
find_package(SDL3 CONFIG REQUIRED)

add_executable(VcpkgExperiment sources/main.cpp)
target_compile_features(VcpkgExperiment PRIVATE cxx_std_20)
target_include_directories(VcpkgExperiment PRIVATE ${FFMPEG_INCLUDE_DIRS})
target_link_directories(VcpkgExperiment PRIVATE ${FFMPEG_LIBRARY_DIRS})
target_link_libraries(VcpkgExperiment PRIVATE fmt::fmt ${FFMPEG_LIBRARIES} SDL3::SDL3)