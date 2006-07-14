; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CNewSymbols
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "TMViewer.h"
LastPage=0

ClassCount=14
Class1=CTMViewerApp
Class2=CChildView
Class3=CMainFrame
Class4=CAboutDlg
Class5=CUrlText

ResourceCount=22
Resource1=IDD_ABOUTBOX
Resource2=IDD_StylesList (Neutral)
Resource3=IDD_EditStyle
Resource4=IDD_TextProps
Class6=CTextProps
Resource5=IDR_EDITOR_TOOLBAR (Neutral)
Resource6=IDD_TB_DIALOG (Neutral)
Resource7=IDD_ABOUTBOX (Neutral)
Resource8=IDD_MergeMaps (English (U.S.))
Class7=CStylesListDlg
Class8=CEditStyleDlg
Resource9=IDR_VIEWER_TOOLBAR (Neutral)
Resource10=IDR_VIEWER_TOOLBAR (English (U.S.))
Resource11=IDR_EDITOR_TOOLBAR (English (U.S.))
Resource12=IDD_ABOUTBOX (English (U.S.))
Resource13=IDR_POPUPS (English (U.S.))
Resource14=IDD_NewMark (English (U.S.))
Resource15=IDD_ExportTo (English (U.S.))
Class9=CMarksList
Resource16=IDD_StylesList (English (U.S.))
Class10=CEditMarkCat
Resource17=IDD_MarksList (English (U.S.))
Class11=CNewMarkDlg
Resource18=IDD_NewSymbols (English (U.S.))
Resource19=IDD_EditMarkCat (English (U.S.))
Class12=CNewSymbols
Resource20=IDD_TextProps (English (U.S.))
Class13=CMergeDlg
Resource21=IDD_EditStyle (English (U.S.))
Class14=CExportDlg
Resource22=IDR_MAINFRAME (English (U.S.))

[CLS:CTMViewerApp]
Type=0
HeaderFile=TMViewer.h
ImplementationFile=TMViewer.cpp
Filter=W
BaseClass=CWinApp
VirtualFilter=AC
LastObject=ID_OpenDesc

[CLS:CChildView]
Type=0
HeaderFile=ChildView.h
ImplementationFile=ChildView.cpp
Filter=C
BaseClass=CWnd 
VirtualFilter=WC
LastObject=IDC_ClearCats

[CLS:CMainFrame]
Type=0
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=C
BaseClass=CFrameWnd
VirtualFilter=fWC
LastObject=ID_FILE_MRU_FILE1

[CLS:CAboutDlg]
Type=0
HeaderFile=TMViewer.cpp
ImplementationFile=TMViewer.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=CAboutDlg

[DLG:IDD_ABOUTBOX]
Type=1
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308352
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889
Class=CAboutDlg

[MNU:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command3=ID_FILE_SAVE
Command4=ID_FILE_SAVE_AS
Command5=ID_FILE_MRU_FILE1
Command6=ID_APP_EXIT
Command10=ID_EDIT_PASTE
Command11=ID_VIEW_TOOLBAR
Command12=ID_VIEW_STATUS_BAR
Command13=ID_APP_ABOUT
CommandCount=13
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command7=ID_EDIT_UNDO
Command8=ID_EDIT_CUT
Command9=ID_EDIT_COPY

[MNU:IDR_MAINFRAME (English (U.S.))]
Type=1
Class=CMainFrame
Command1=ID_FILE_OPEN
Command2=ID_FILE_EXPORTTOBMP
Command3=ID_MERGE_MAPS
Command4=ID_OpenDesc
Command5=ID_SaveDescr
Command6=ID_SaveDescAs
Command7=ID_NewMapDesc
Command8=ID_LOADPREVIOUSMAPSYMBOLS
Command9=ID_FILE_MRU_FILE1
Command10=ID_APP_EXIT
Command11=IDC_EDITOR_MODE
Command12=ID_INSERT_TEXT
Command13=ID_InsertMark
Command14=ID_ToFront
Command15=ID_ToBack
Command16=ID_DELETE
Command17=ID_PROPERTIES
Command18=ID_EditStyles
Command19=ID_MARKCATEGORIES
Command20=ID_VIEW_TEXTLABELS
Command21=ID_VIEW_MARKS
Command22=ID_ShowAllMarks
Command23=ID_HideAllMarks
Command24=ID_HIDEUNEXPLOREDSYMBOLS
Command25=ID_VIEW_HIDEALLSYMBOLS
Command26=IDC_CHECK_OVERLAY
Command27=ID_VIEW_FILTERING
Command28=ID_VIEW_STATUS_BAR
Command29=ID_VIEW_NAVIGATION
Command30=ID_HELP_VIEWREADME
Command31=ID_APP_ABOUT
CommandCount=31

[TB:IDR_MAINFRAME (English (U.S.))]
Type=1
Class=?
Command1=ID_FILE_OPEN
CommandCount=1

[ACL:IDR_MAINFRAME (English (U.S.))]
Type=1
Class=?
Command1=ID_FILE_EXPORTTOBMP
Command2=ID_VIEW_FILTERING
Command3=ID_OpenDesc
Command4=ID_InsertMark
Command5=ID_FILE_OPEN
Command6=ID_SaveDescr
Command7=ID_INSERT_TEXT
Command8=ID_DELETE
Command9=ID_HELP_VIEWREADME
Command10=ID_LOADPREVIOUSMAPSYMBOLS
Command11=IDC_EDITOR_MODE
Command12=ID_HIDEUNEXPLOREDSYMBOLS
Command13=ID_VIEW_TEXTLABELS
Command14=ID_VIEW_MARKS
Command15=ID_VIEW_HIDEALLSYMBOLS
Command16=IDC_CHECK_OVERLAY
CommandCount=16

[CLS:CUrlText]
Type=0
HeaderFile=tmviewer.cpp
ImplementationFile=tmviewer.cpp
BaseClass=CStatic
Filter=W
VirtualFilter=WC

[DLG:IDD_ABOUTBOX (Neutral)]
Type=1
Class=CAboutDlg
ControlCount=10
Control1=IDC_STATIC,static,1342177283
Control2=IDC_VERSION,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,static,1342308352
Control7=IDC_EMAIL,static,1342308608
Control8=IDC_STATIC,static,1342308352
Control9=IDC_FTP,static,1342308608
Control10=IDC_STATIC,static,1342177298

[DLG:IDD_TB_DIALOG (Neutral)]
Type=1
Class=CMainFrame
ControlCount=11
Control1=IDC_CB_LEVEL,combobox,1344274435
Control2=IDC_STATIC,static,1342308352
Control3=IDC_STATIC,static,1342308352
Control4=IDC_CB_ZOOM,combobox,1344274435
Control5=IDC_STATIC,static,1342308352
Control6=IDC_CHECK_OVERLAY,button,1342210051
Control7=IDC_SLIDER_SHADING,msctls_trackbar32,1342177281
Control8=IDC_STATIC,static,1342177297
Control9=IDC_INTENSITY_LABEL,static,1342308352
Control10=IDC_EDITOR_MODE,button,1342247043
Control11=IDC_STATIC,static,1342177297

[DLG:IDD_TextProps]
Type=1
Class=CTextProps
ControlCount=3
Control1=IDC_EDIT_TEXT,edit,1350635716
Control2=IDOK,button,1342242817
Control3=IDCANCEL,button,1342242816

[CLS:CTextProps]
Type=0
HeaderFile=TextProps.h
ImplementationFile=TextProps.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_EDIT_TEXT

[DLG:IDD_EditStyle]
Type=1
Class=CEditStyleDlg
ControlCount=9
Control1=IDC_edStyleName,edit,1350631552
Control2=IDC_btChangeFont,button,1342242816
Control3=IDC_chkShadow,button,1342242819
Control4=IDOK,button,1342242817
Control5=IDCANCEL,button,1342242816
Control6=IDC_STATIC,static,1342177298
Control7=IDC_STATIC,static,1342308352
Control8=IDC_edFontName,edit,1350568064
Control9=IDC_STATIC,static,1342308352

[DLG:IDD_StylesList (Neutral)]
Type=1
Class=CStylesListDlg
ControlCount=5
Control1=IDC_STYLES_LIST,listbox,1352728835
Control2=IDC_btNewStyle,button,1342242816
Control3=IDC_btEditStyle,button,1342242816
Control4=IDC_btDeleteStyle,button,1342242816
Control5=IDCANCEL,button,1342242816

[CLS:CStylesListDlg]
Type=0
HeaderFile=StylesListDlg.h
ImplementationFile=StylesListDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDOK

[CLS:CEditStyleDlg]
Type=0
HeaderFile=EditStyleDlg.h
ImplementationFile=EditStyleDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_chkShadow
VirtualFilter=dWC

[TB:IDR_EDITOR_TOOLBAR (Neutral)]
Type=1
Class=?
Command1=ID_SelectMode
Command2=ID_INSERT_TEXT
Command3=ID_STYLES_COMBO
CommandCount=3

[TB:IDR_VIEWER_TOOLBAR (Neutral)]
Type=1
Class=?
Command1=IDC_CB_ZOOM
Command2=IDC_CB_LEVEL
Command3=IDC_CHECK_OVERLAY
Command4=IDC_SLIDER_SHADING
Command5=IDC_EDITOR_MODE
CommandCount=5

[DLG:IDD_StylesList (English (U.S.))]
Type=1
Class=CStylesListDlg
ControlCount=5
Control1=IDC_STYLES_LIST,listbox,1352728833
Control2=IDC_btNewStyle,button,1342242816
Control3=IDC_btEditStyle,button,1342242816
Control4=IDC_btDeleteStyle,button,1342242816
Control5=IDCANCEL,button,1342242816

[DLG:IDD_ABOUTBOX (English (U.S.))]
Type=1
Class=CAboutDlg
ControlCount=10
Control1=IDC_STATIC,static,1342177283
Control2=IDC_VERSION,static,1342308492
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,static,1342308352
Control7=IDC_EMAIL,static,1342308608
Control8=IDC_STATIC,static,1342308352
Control9=IDC_FTP,static,1342308608
Control10=IDC_STATIC,static,1342177298

[DLG:IDD_TextProps (English (U.S.))]
Type=1
Class=CTextProps
ControlCount=3
Control1=IDC_EDIT_TEXT,edit,1350635716
Control2=IDOK,button,1342242817
Control3=IDCANCEL,button,1342242816

[DLG:IDD_EditStyle (English (U.S.))]
Type=1
Class=CEditStyleDlg
ControlCount=9
Control1=IDC_edStyleName,edit,1350631552
Control2=IDC_btChangeFont,button,1342242816
Control3=IDC_chkShadow,button,1342242819
Control4=IDOK,button,1342242817
Control5=IDCANCEL,button,1342242816
Control6=IDC_FRAME,static,1342177298
Control7=IDC_STATIC,static,1342308352
Control8=IDC_edFontName,edit,1350568064
Control9=IDC_STATIC,static,1342308352

[TB:IDR_EDITOR_TOOLBAR (English (U.S.))]
Type=1
Class=?
Command1=ID_SaveDescr
Command2=ID_SelectMode
Command3=ID_INSERT_TEXT
Command4=ID_InsertMark
Command5=ID_STYLES_COMBO
Command6=ID_EditStyles
Command7=ID_MARKCATEGORIES
CommandCount=7

[TB:IDR_VIEWER_TOOLBAR (English (U.S.))]
Type=1
Class=?
Command1=IDC_CB_ZOOM
Command2=IDC_CB_LEVEL
Command3=IDC_CHECK_OVERLAY
Command4=IDC_SLIDER_SHADING
Command5=ID_VIEW_HIDEALLSYMBOLS
Command6=ID_VIEW_TEXTLABELS
Command7=ID_VIEW_MARKS
Command8=IDC_EDITOR_MODE
CommandCount=8

[CLS:CMarksList]
Type=0
HeaderFile=MarksList.h
ImplementationFile=MarksList.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_Marks
VirtualFilter=dWC

[DLG:IDD_MarksList (English (U.S.))]
Type=1
Class=CMarksList
ControlCount=7
Control1=IDC_Marks,SysListView32,1350649933
Control2=IDC_btNewMark,button,1342242816
Control3=IDC_btEditMark,button,1342242816
Control4=IDC_btDeleteMark,button,1342242816
Control5=IDCANCEL,button,1342242816
Control6=IDC_MoveUp,button,1342242816
Control7=IDC_MoveDown,button,1342242816

[DLG:IDD_EditMarkCat (English (U.S.))]
Type=1
Class=CEditMarkCat
ControlCount=9
Control1=IDC_btSelBmp,button,1342242816
Control2=IDC_edDescr,edit,1350631552
Control3=IDOK,button,1342242817
Control4=IDCANCEL,button,1342242816
Control5=IDC_MarkImg,static,1342182148
Control6=IDC_STATIC,static,1342308352
Control7=IDC_STATIC,static,1342177298
Control8=IDC_STATIC,static,1342308352
Control9=IDC_STATIC,static,1342308352

[CLS:CEditMarkCat]
Type=0
HeaderFile=EditMarkCat.h
ImplementationFile=EditMarkCat.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_edDescr

[DLG:IDD_NewMark (English (U.S.))]
Type=1
Class=CNewMarkDlg
ControlCount=9
Control1=IDC_cbCategory,ComboBoxEx32,1344340227
Control2=IDC_ShortText,edit,1352732868
Control3=IDC_LongText,edit,1352732868
Control4=IDOK,button,1342242817
Control5=IDCANCEL,button,1342242816
Control6=IDC_STATIC,static,1342308352
Control7=IDC_STATIC,static,1342177298
Control8=IDC_STATIC,static,1342308352
Control9=IDC_STATIC,static,1342308352

[CLS:CNewMarkDlg]
Type=0
HeaderFile=NewMarkDlg.h
ImplementationFile=NewMarkDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC

[MNU:IDR_POPUPS (English (U.S.))]
Type=1
Class=?
Command1=ID_PROPERTIES
Command2=ID_ToFront
Command3=ID_ToBack
Command4=ID_DELETE
CommandCount=4

[CLS:CNewSymbols]
Type=0
HeaderFile=NewSymbols.h
ImplementationFile=NewSymbols.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_ClearCats

[CLS:CMergeDlg]
Type=0
HeaderFile=MergeDlg.h
ImplementationFile=MergeDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CMergeDlg
VirtualFilter=dWC

[DLG:IDD_MergeMaps (English (U.S.))]
Type=1
Class=CMergeDlg
ControlCount=8
Control1=IDC_Browse,button,1342242816
Control2=IDC_Fogged,button,1342242819
Control3=IDOK,button,1342242817
Control4=IDCANCEL,button,1342242816
Control5=IDC_MergeWith,edit,1350631552
Control6=IDC_STATIC,static,1342177298
Control7=IDC_STATIC,static,1342308352
Control8=IDC_STATIC,static,1342308352

[DLG:IDD_NewSymbols (English (U.S.))]
Type=1
Class=CNewSymbols
ControlCount=6
Control1=IDC_ClearStyles,button,1342242819
Control2=IDC_ClearCats,button,1342242819
Control3=IDOK,button,1342242817
Control4=IDCANCEL,button,1342242816
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,static,1342177298

[DLG:IDD_ExportTo (English (U.S.))]
Type=1
Class=CExportDlg
ControlCount=8
Control1=IDC_Browse,button,1342242816
Control2=IDC_ExportSymbols,button,1342242819
Control3=IDC_ExportZoom,button,1342242819
Control4=IDOK,button,1342242817
Control5=IDCANCEL,button,1342242816
Control6=IDC_ExportFile,edit,1350631552
Control7=IDC_STATIC,static,1342177298
Control8=IDC_STATIC,static,1342308352

[CLS:CExportDlg]
Type=0
HeaderFile=ExportDlg.h
ImplementationFile=ExportDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=IDOK
VirtualFilter=dWC

