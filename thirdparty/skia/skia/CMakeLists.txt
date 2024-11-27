cmake_minimum_required(VERSION 3.14)
project(skia)

add_library(skia INTERFACE)
target_include_directories(skia INTERFACE
    # Include directory for Skia
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "include"

    # Include directories for dependencies
    "third_party/externals/harfbuzz/src"
    "third_party/externals/icu/source"
    "third_party/externals/libjpeg-turbo"
    "third_party/externals/libpng"
    "third_party/externals/libjxl/lib/include"
    "third_party/externals/zlib"
)

# Specify the libraries to link
target_link_libraries(skia INTERFACE
    # Skia library
    "${CMAKE_CURRENT_SOURCE_DIR}/out/win-x64-debug/skia.lib"

    # Dependency libraries
    "${CMAKE_CURRENT_SOURCE_DIR}/out/win-x64-debug/bentleyottmann.lib"
    "${CMAKE_CURRENT_SOURCE_DIR}/out/win-x64-debug/brotli.lib"
    "${CMAKE_CURRENT_SOURCE_DIR}/out/win-x64-debug/compression_utils_portable.lib"
    "${CMAKE_CURRENT_SOURCE_DIR}/out/win-x64-debug/expat.lib"
    "${CMAKE_CURRENT_SOURCE_DIR}/out/win-x64-debug/harfbuzz.lib"
    "${CMAKE_CURRENT_SOURCE_DIR}/out/win-x64-debug/highway.lib"
    "${CMAKE_CURRENT_SOURCE_DIR}/out/win-x64-debug/icu_bidi.lib"
    "${CMAKE_CURRENT_SOURCE_DIR}/out/win-x64-debug/icu.lib"
    "${CMAKE_CURRENT_SOURCE_DIR}/out/win-x64-debug/libgrapheme.lib"
    "${CMAKE_CURRENT_SOURCE_DIR}/out/win-x64-debug/libjpeg.lib"
    "${CMAKE_CURRENT_SOURCE_DIR}/out/win-x64-debug/libjxl.lib"
    "${CMAKE_CURRENT_SOURCE_DIR}/out/win-x64-debug/libpng.lib"
    "${CMAKE_CURRENT_SOURCE_DIR}/out/win-x64-debug/libwebp_sse41.lib"
    "${CMAKE_CURRENT_SOURCE_DIR}/out/win-x64-debug/libwebp.lib"
    "${CMAKE_CURRENT_SOURCE_DIR}/out/win-x64-debug/pathkit.lib"
    "${CMAKE_CURRENT_SOURCE_DIR}/out/win-x64-debug/skcms.lib"
    "${CMAKE_CURRENT_SOURCE_DIR}/out/win-x64-debug/skottie.lib"
    "${CMAKE_CURRENT_SOURCE_DIR}/out/win-x64-debug/skparagraph.lib"
    "${CMAKE_CURRENT_SOURCE_DIR}/out/win-x64-debug/skresources.lib"
    "${CMAKE_CURRENT_SOURCE_DIR}/out/win-x64-debug/sksg.lib"
    "${CMAKE_CURRENT_SOURCE_DIR}/out/win-x64-debug/skshaper.lib"
    "${CMAKE_CURRENT_SOURCE_DIR}/out/win-x64-debug/skunicode_core.lib"
    "${CMAKE_CURRENT_SOURCE_DIR}/out/win-x64-debug/skunicode_icu.lib"
    "${CMAKE_CURRENT_SOURCE_DIR}/out/win-x64-debug/skunicode_libgrapheme.lib"
    "${CMAKE_CURRENT_SOURCE_DIR}/out/win-x64-debug/spvtools_link.lib"
    "${CMAKE_CURRENT_SOURCE_DIR}/out/win-x64-debug/spvtools_opt.lib"
    "${CMAKE_CURRENT_SOURCE_DIR}/out/win-x64-debug/spvtools_reduce.lib"
    "${CMAKE_CURRENT_SOURCE_DIR}/out/win-x64-debug/spvtools_val.lib"
    "${CMAKE_CURRENT_SOURCE_DIR}/out/win-x64-debug/spvtools.lib"
    "${CMAKE_CURRENT_SOURCE_DIR}/out/win-x64-debug/svg.lib"
    "${CMAKE_CURRENT_SOURCE_DIR}/out/win-x64-debug/wuffs.lib"
    "${CMAKE_CURRENT_SOURCE_DIR}/out/win-x64-debug/zlib.lib"
)

# Optionally, specify any compile definitions if required
target_compile_definitions(skia INTERFACE
    # Add any required compile definitions here
)

# Optionally, specify any compile options if required
target_compile_options(skia INTERFACE
    # Add any required compile options here
)
