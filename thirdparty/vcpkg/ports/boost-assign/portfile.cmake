# Automatically generated by scripts/boost/generate-ports.ps1

vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO boostorg/assign
    REF boost-${VERSION}
    SHA512 c86e613bdd0cade0f2be229ea12fea2e869cbc220c8f95a743a0599d2c0e74a41e6feff71c40fb6197cc4f53a632c42ff8f7f2ec4e6ac8c769c6fea1820fa3cc
    HEAD_REF master
)

set(FEATURE_OPTIONS "")
boost_configure_and_install(
    SOURCE_PATH "${SOURCE_PATH}"
    OPTIONS ${FEATURE_OPTIONS}
)