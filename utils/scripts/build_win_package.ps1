cd $PSScriptRoot
cd ..\..

cmake --preset=release
cmake --build build_release

makensis .\utils\scripts\windows_installer.nsi
