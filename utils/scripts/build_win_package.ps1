Set-Location $PSScriptRoot
Set-Location ..\..

cmake --preset=release-windows
cmake --build build_release_windows
cmake --build build_release_windows --target write_version_info

makensis .\assets\windows\installer\windows_installer.nsi
