#!/bin/bash

# Use Terminal, go to settings, and it will give you the command to go into microsoft visual studio 2022, or whatever version you have

devps() {
    local command="$1"
    powershell.exe -Command "& {
        Import-Module 'C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\Tools\Microsoft.VisualStudio.DevShell.dll';
        Enter-VsDevShell d51c1c00 -SkipAutomaticLocation -DevCmdArguments '-arch=x64 -host_arch=x64';
        $command
    }"
}

devps ".\\build.bat"
