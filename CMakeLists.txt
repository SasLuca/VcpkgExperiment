cmake_minimum_required(VERSION 3.29)
project(VcpkgExperiment)

find_package(fmt CONFIG REQUIRED)


add_executable(VcpkgExperiment main.cpp)
target_compile_features(VcpkgExperiment PRIVATE cxx_std_20)
target_link_libraries(VcpkgExperiment PRIVATE fmt::fmt)