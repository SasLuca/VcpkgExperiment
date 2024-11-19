# SDL3 FFMPEG Experiment

A simple experiment to try and compile SDL3 and ffmpeg into an ios application using cmake.

## How to use

### Setting up vcpkg
Run one of the following commands which will download and configure vcpkg locally for the project.
```bash
./setup-vcpkg.sh # On Unix/macOS
./setup-vcpkg.bat # On Windows
``` 

### Windows
On windows simply load the project in clion/vs/vscode and it should work. Alternatively generate a visual studio solution or whatever you prefer.

### iOS
Use the following command to generate an xcode solution and replace `"YOUR_TEAM_ID"` with your actual apple team id.
```bash
cmake --preset ios-xcode -DMY_IOS_TEAM_ID="YOUR_TEAM_ID" --output-dir ./xcode
```

Then open the generated xcode project and run it.