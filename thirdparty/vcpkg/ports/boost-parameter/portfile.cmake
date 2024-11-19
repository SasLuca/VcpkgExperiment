# Automatically generated by scripts/boost/generate-ports.ps1

vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO boostorg/parameter
    REF boost-${VERSION}
    SHA512 42ca9ab48cd3f871104f9b435129ffe2e7a25c6dbb65102b4b9e3bd7ff2b5d5f69a2a876585607b6fc8199d4204839d7241eecfc5d2f9be6381c362f57124780
    HEAD_REF master
)

set(FEATURE_OPTIONS "")
boost_configure_and_install(
    SOURCE_PATH "${SOURCE_PATH}"
    OPTIONS ${FEATURE_OPTIONS}
)