cd $PSScriptRoot
cd ..\..

cmake --preset=release-windows
cmake --build build_release_windows

makensis .\assets\windows\installer\windows_installer.nsi
