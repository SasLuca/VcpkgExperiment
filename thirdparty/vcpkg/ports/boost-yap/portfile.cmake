# Automatically generated by scripts/boost/generate-ports.ps1

vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO boostorg/yap
    REF boost-${VERSION}
    SHA512 81bdbe0e6d14556fbcca7c13073f2d18811288e8420e775f97e43bce00ea0d34aed61c2aec30a9b0427caa63374d60366c6437e6818b5672d91b828ed6e9d88d
    HEAD_REF master
)

set(FEATURE_OPTIONS "")
boost_configure_and_install(
    SOURCE_PATH "${SOURCE_PATH}"
    OPTIONS ${FEATURE_OPTIONS}
)