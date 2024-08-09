cd $PSScriptRoot
cd ..\..

cmake --preset=default
cmake --build build

makensis .\utils\scripts\windows_installer.nsi
