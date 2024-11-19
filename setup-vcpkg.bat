@echo off
setlocal enabledelayedexpansion

:: Save the original directory
set ORIGINAL_DIR=%cd%

:: Define the target directory for vcpkg
set VCPKG_DIR=thirdparty\vcpkg

:: Ensure the thirdparty directory exists
if not exist thirdparty (
    echo Creating thirdparty directory...
    mkdir thirdparty
)

:: Clone vcpkg if it doesn't already exist
if not exist %VCPKG_DIR% (
    echo Cloning vcpkg into %VCPKG_DIR%...
    git clone https://github.com/microsoft/vcpkg.git %VCPKG_DIR%
) else (
    echo vcpkg is already cloned in %VCPKG_DIR%.
)

:: Change to the vcpkg directory
cd %VCPKG_DIR%

:: Make sure the bootstrap script is executable and run it
if exist bootstrap-vcpkg.bat (
    echo Running the bootstrap script...
    call bootstrap-vcpkg.bat
) else (
    echo ERROR: Bootstrap script not found. Please check your vcpkg setup.
    exit /b 1
)

:: Return to the original directory
cd %ORIGINAL_DIR%

echo vcpkg has been successfully bootstrapped!
endlocal
