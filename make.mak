OUTDIR=.\bin
INTDIR=.\Release

ALL : "$(OUTDIR)\TMViewer.exe"

CLEAN :
	-@erase "$(OUTDIR)\TMViewer.exe"
	-@erase "$(INTDIR)\*.obj"
	-@erase "$(INTDIR)\*.res"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /Gz /Gy /MT /W3 /GX /O1 /Oy /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /Fo"$(INTDIR)\\" /c

.c{$(INTDIR)}.obj:
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj:
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj:
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.rc{$(INTDIR)}.res:
   $(RC) $(RFLAGS) /fo $@ $<

LINK32=link.exe
LINK32_FLAGS=version.lib /nologo /incremental:no /machine:I386  /subsystem:windows /out:"$(OUTDIR)\TMViewer.exe"
LINK32_OBJS= \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\ChildView.obj" \
	"$(INTDIR)\Content.obj" \
	"$(INTDIR)\EditMarkCat.obj" \
	"$(INTDIR)\EditStyleDlg.obj" \
	"$(INTDIR)\ExportDlg.obj" \
	"$(INTDIR)\MainFrm.obj" \
	"$(INTDIR)\Mapper.obj" \
	"$(INTDIR)\MarksList.obj" \
	"$(INTDIR)\MergeDlg.obj" \
	"$(INTDIR)\NewMarkDlg.obj" \
	"$(INTDIR)\NewSymbols.obj" \
	"$(INTDIR)\StylesListDlg.obj" \
	"$(INTDIR)\TextProps.obj" \
	"$(INTDIR)\TMViewer.obj"
	
RES="$(INTDIR)\TMViewer.res"

"$(OUTDIR)\TMViewer.exe" : "$(OUTDIR)" "$(INTDIR)" $(LINK32_OBJS) $(RES)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS) $(RES)
<<
