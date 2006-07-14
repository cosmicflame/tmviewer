;  This file is part of Tibia Map Viewer.
;
;  Tibia Map Viewer is free software; you can redistribute it and/or modify
;  it under the terms of the GNU General Public License as published by
;  the Free Software Foundation; either version 2 of the License, or
;  (at your option) any later version.
;
;  Tibia Map Viewer is distributed in the hope that it will be useful,
;  but WITHOUT ANY WARRANTY; without even the implied warranty of
;  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;  GNU General Public License for more details.
;
;  You should have received a copy of the GNU General Public License
;  along with Tibia Map Viewer; if not, write to the Free Software
;  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

!include "MUI.nsh"

; The name of the installer
!define PRODUCT_NAME "Tibia Map Viewer"
Name "${PRODUCT_NAME} 3.2"

!define REG_ROOT "SOFTWARE\${PRODUCT_NAME}"

;--------------------------------
;Configuration

  SetCompressor /solid lzma
  OutFile "TMViewerSetup.exe"
  InstallDir "$PROGRAMFILES\${PRODUCT_NAME}"
  ; Registry key to check for directory (so if you install again, it will
  ; overwrite the old one automatically)
  InstallDirRegKey HKLM "${REG_ROOT}" "Install_Dir"

  BrandingText " "
  XPStyle on
  ShowInstDetails nevershow

  !define MUI_ICON "setup2.ico"
  !define MUI_UNICON "uninstall2.ico"

  !define MUI_COMPONENTSPAGE_NODESC
  !define MUI_FINISHPAGE_RUN "$INSTDIR\tmviewer.exe"

  !define MUI_ABORTWARNING

;--------------------------------
;Pages
  !insertmacro MUI_PAGE_WELCOME
  !insertmacro MUI_PAGE_LICENSE "..\Doc\License.txt"
  !insertmacro MUI_PAGE_DIRECTORY
  !insertmacro MUI_PAGE_COMPONENTS
  !insertmacro MUI_PAGE_INSTFILES
  !insertmacro MUI_PAGE_FINISH

; Uninstaller pages
  !insertmacro MUI_UNPAGE_CONFIRM
  !insertmacro MUI_UNPAGE_INSTFILES

;--------------------------------
;Language
  !insertmacro MUI_LANGUAGE "English"

;--------------------------------
;Installer Sections

;--------------------------------
Section "${PRODUCT_NAME} (required)"
  ; Set output path to the installation directory.
  SetOutPath $INSTDIR
  ; Put file there
  File "..\bin\TMViewer.exe"
  File "..\Doc\License.txt"
  File "..\Doc\readme.htm"
  ; Write the installation path into the registry
  WriteRegStr HKLM "${REG_ROOT}" "Install_Dir" "$INSTDIR"
  ; Write the uninstall keys for Windows
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\TMViewer" "DisplayName" "${PRODUCT_NAME}"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\TMViewer" "UninstallString" '"$INSTDIR\uninstall.exe"'
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\TMViewer" "Publisher" "Yury Sidorov"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\TMViewer" "URLUpdateInfo" "http://tmviewer.blagovest.ua"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\TMViewer" "URLInfoAbout" "http://tmviewer.blagovest.ua"
  WriteUninstaller "uninstall.exe"
SectionEnd

; optional section
Section "Default Map symbols (recommended)"
  GetFullPathName $0 "$INSTDIR\default.tmv"
  IfErrors Err NoErr
NoErr:
  MessageBox MB_YESNO|MB_ICONEXCLAMATION \
             "Setup has found the Default Map symbols file in the installation folder.$\rDo you want to overwrite it ?" \
             IDNO NoCopy

Err:
  ; Set output path to the installation directory.
  SetOutPath $INSTDIR
  ; Put file there
  File "..\bin\default.tmv"
NoCopy:
SectionEnd

; optional section
Section "Start Menu Shortcuts"
  CreateDirectory "$SMPROGRAMS\${PRODUCT_NAME}"
  CreateShortCut "$SMPROGRAMS\${PRODUCT_NAME}\${PRODUCT_NAME}.lnk" "$INSTDIR\TMViewer.exe"
  CreateShortCut "$SMPROGRAMS\${PRODUCT_NAME}\View documentation.lnk" "$INSTDIR\readme.htm"
  CreateShortCut "$SMPROGRAMS\${PRODUCT_NAME}\Uninstall.lnk" "$INSTDIR\uninstall.exe" "" "$INSTDIR\uninstall.exe" 0
SectionEnd

; optional section
Section "Desktop icon"
  CreateShortCut "$DESKTOP\${PRODUCT_NAME}.lnk" "$INSTDIR\TMViewer.exe" "" "$INSTDIR\TMViewer.exe" 0
SectionEnd

; -------- uninstall stuff --------

; special uninstall section.
Section "Uninstall"
  StrCpy $0 0

  ; remove registry keys
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\TMViewer"
  DeleteRegKey HKLM "${REG_ROOT}"

  ClearErrors
  ; remove files
  Push $INSTDIR\TMViewer.exe
  Call un.RemFile
  Push $INSTDIR\License.txt
  Call un.RemFile
  Push $INSTDIR\TMViewer.ini
  Call un.RemFile
  Push $INSTDIR\readme.htm
  Call un.RemFile
  Push $INSTDIR\default.tmv
  Call un.RemFile
  ; MUST REMOVE UNINSTALLER, too
  Push $INSTDIR\uninstall.exe
  Call un.RemFile

  ; remove shortcuts, if any.
  Push "$SMPROGRAMS\${PRODUCT_NAME}\*.*"
  Call un.RemFile
  Push "$DESKTOP\${PRODUCT_NAME}.lnk"
  Call un.RemFile
  ; remove directories used.
  RMDir "$SMPROGRAMS\${PRODUCT_NAME}"

  ClearErrors
  RMDir "$INSTDIR"
  IfErrors xErr xEnd
xErr:
  StrCpy $0 1
  SetDetailsPrint listonly
  DetailPrint "Can't delete folder $INSTDIR"
  SetDetailsPrint none
xEnd:

  StrCmp $0 1 Err

  SetAutoClose true
  Return

Err:
  SetDetailsView show
  MessageBox MB_OK|MB_ICONSTOP "Some files were not uninstalled. You need to delete them manually."

SectionEnd

Function un.onUninstSuccess
  StrCmp $0 1 End
  HideWindow
  MessageBox MB_OK "${PRODUCT_NAME} has been removed from your computer."
End:
FunctionEnd

Function un.RemFile
  Pop $R1

  SetDetailsPrint none
  ClearErrors
  Delete $R1
  IfErrors Err End
Err:
  StrCpy $0 1
  SetDetailsPrint listonly
  DetailPrint "Can't delete $R1"
  SetDetailsPrint none
End:
FunctionEnd

; eof
