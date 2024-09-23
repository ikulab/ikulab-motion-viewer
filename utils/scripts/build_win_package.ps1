cd $PSScriptRoot
cd ..\..

cmake --preset=release-windows
cmake --build build_release_windows

makensis .\utils\scripts\windows_installer.nsi
