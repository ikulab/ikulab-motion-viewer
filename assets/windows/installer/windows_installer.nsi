!include "MUI2.nsh"

;--------------------------------
;General

Name "ikulab-motion-viewer"
OutFile "..\..\..\ikulab-motion-viewer_Installer.exe"
Unicode True

InstallDir "$PROGRAMFILES\ikulab-motion-viewer"

;--------------------------------
;Interface Settings

!define MUI_ABORTWARNING

;Show all languages, despite user's codepage
!define MUI_LANGDLL_ALLLANGUAGES

;--------------------------------
;Pages

!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES

!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES

;Support Languages
!insertmacro MUI_LANGUAGE "English"
!insertmacro MUI_LANGUAGE "Japanese"
!insertmacro MUI_LANGUAGE "SimpChinese"
!insertmacro MUI_LANGUAGE "TradChinese"
!insertmacro MUI_LANGUAGE "Korean"

;--------------------------------
;Installer Sections

Section

  SetOutPath "$INSTDIR"
  File "..\..\..\build_release_windows\app\ikulab-motion-viewer.exe"
  File "..\..\..\build_release_windows\app\vulkan-1.dll"
  File "..\..\..\build_release_windows\app\glfw3.dll"

  SetOutPath "$INSTDIR\fonts"
  File "..\..\..\assets\fonts\NotoSansJP-Medium.otf"

  ; VC++ Runtime Installer
  SetOutPath "$INSTDIR"
  File "..\..\..\dist_pyinstaller\check_vcpp_version_and_install.exe"
  File "..\..\..\msvc_version.txt"
  File "..\..\..\vc_redist.x64.exe"

  ;Start Menu Shortcut
  CreateShortcut "$SMPROGRAMS\ikulab-motion-viewer.lnk" "$INSTDIR\ikulab-motion-viewer.exe" ""
  ;Desktop Shortcut
  CreateShortcut "$DESKTOP\ikulab-motion-viewer.lnk" "$INSTDIR\ikulab-motion-viewer.exe" ""

  ;Create uninstaller
  WriteUninstaller "$INSTDIR\Uninstall.exe"
  ;Registry for Uninstalling from Windows Settings
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\ikulab-motion-viewer" "DisplayName" "ikulab-motion-viewer"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\ikulab-motion-viewer" "UninstallString" '"$INSTDIR\Uninstall.exe"'

  ExecWait "$INSTDIR\check_vcpp_version_and_install.exe"

SectionEnd

;--------------------------------
;Installer Functions

Function .onInit

  ;Show Language Selector
  !insertmacro MUI_LANGDLL_DISPLAY

FunctionEnd

;--------------------------------
;Uninstaller Section

Section "Uninstall"

  RMDir /r "$INSTDIR"

  Delete "$SMPROGRAMS\ikulab-motion-viewer.lnk"
  Delete "$DESKTOP\ikulab-motion-viewer.lnk"

  ;Registry for Uninstalling from Windows Settings
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\ikulab-motion-viewer"

SectionEnd