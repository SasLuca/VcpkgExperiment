# VCPKG Experiment

A simple experiment to try and compile SDL3 and ffmpeg into an ios application using cmake.

## How to use

### Setting up vcpkg
```bash
cd thirdparty/vcpkg
./bootstrap-vcpkg.sh # On Unix/macOS
./bootstrap-vcpkg.bat # On Windows
``` 

### Windows
On windows simply load the project in clion/vs/vscode and it should work. Alternatively generate a visual studio solution or whatever you prefer.

### iOS
Use the following command to generate an xcode solution and replace `"YOUR_TEAM_ID"` with your actual apple team id.
```bash
cmake --preset ios-xcode -DMY_IOS_TEAM_ID="YOUR_TEAM_ID" --output-dir ./xcode
```
