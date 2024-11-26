The src is just  main.c and  vector_add.cl 



Those are the two files that get compiled. 
The other files are build script.

I work from Arch WSL, so it has a .sh wrapper for both
Theres a make with cmake, and one with cl, the microsoft studio compiler. 
It's for windows for now, but changing the build script should allow for linux idk.

My Arch Boot is corrupted by Windows LOL, so can't test it there for now. 

---
Instructions: 

just execute in either a WSL the .sh files, and you'll need to change the command to get to powershell
or execute the bat files from developper powershell 

You'll need cmake, so install that. 
You can do it through winget. 

Here's a link and some info:
https://github.com/KhronosGroup/OpenCL-Guide/blob/main/chapters/getting_started_windows.md 

Copy paste of his:
winget install "Visual Studio Build Tools 2022"
& "C:\Program Files (x86)\Microsoft Visual Studio\Installer\setup.exe" install --passive --norestart --productId Microsoft.VisualStudio.Product.BuildTools --channelId VisualStudio.17.Release --add Microsoft.VisualStudio.Component.VC.Tools.x86.x64 --add Microsoft.VisualStudio.Component.VC.Redist.14.Latest


winget install Git.Git
winget install Kitware.CMake
winget install "Visual Studio Code" --source msstore

----
My stuff

Move the build files from The Operating System to the root dir. 
(This is just to save clutter)


Then windows: 
.\build.bat 
or 
.\build_cmake.bat 

from developper powershell. 
.\build_linux.sh 
or 
.\build_cmake_linux.sh 





