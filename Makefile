#############################################################################
# Makefile for building: LuitPad
# Generated by qmake (2.01a) (Qt 4.8.1) on: Thu Mar 14 07:07:43 2013
# Project:  LuitPad.pro
# Template: app
# Command: f:\qtsdk\desktop\qt\4.8.1\msvc2010\bin\qmake.exe -spec ..\..\QtSDK\Desktop\Qt\4.8.1\msvc2010\mkspecs\win32-msvc2010 CONFIG+=release -o Makefile LuitPad.pro
#############################################################################

first: release
install: release-install
uninstall: release-uninstall
MAKEFILE      = Makefile
QMAKE         = f:\qtsdk\desktop\qt\4.8.1\msvc2010\bin\qmake.exe
DEL_FILE      = del
CHK_DIR_EXISTS= if not exist
MKDIR         = mkdir
COPY          = copy /y
COPY_FILE     = $(COPY)
COPY_DIR      = xcopy /s /q /y /i
INSTALL_FILE  = $(COPY_FILE)
INSTALL_PROGRAM = $(COPY_FILE)
INSTALL_DIR   = $(COPY_DIR)
DEL_FILE      = del
SYMLINK       = 
DEL_DIR       = rmdir
MOVE          = move
CHK_DIR_EXISTS= if not exist
MKDIR         = mkdir
SUBTARGETS    =  \
		release \
		debug

release: $(MAKEFILE).Release FORCE
	@set MAKEFLAGS=$(MAKEFLAGS)
	$(MAKE) -f $(MAKEFILE).Release
release-make_default: $(MAKEFILE).Release FORCE
	@set MAKEFLAGS=$(MAKEFLAGS)
	$(MAKE) -f $(MAKEFILE).Release 
release-make_first: $(MAKEFILE).Release FORCE
	@set MAKEFLAGS=$(MAKEFLAGS)
	$(MAKE) -f $(MAKEFILE).Release first
release-all: $(MAKEFILE).Release FORCE
	@set MAKEFLAGS=$(MAKEFLAGS)
	$(MAKE) -f $(MAKEFILE).Release all
release-clean: $(MAKEFILE).Release FORCE
	@set MAKEFLAGS=$(MAKEFLAGS)
	$(MAKE) -f $(MAKEFILE).Release clean
release-distclean: $(MAKEFILE).Release FORCE
	@set MAKEFLAGS=$(MAKEFLAGS)
	$(MAKE) -f $(MAKEFILE).Release distclean
release-install: $(MAKEFILE).Release FORCE
	@set MAKEFLAGS=$(MAKEFLAGS)
	$(MAKE) -f $(MAKEFILE).Release install
release-uninstall: $(MAKEFILE).Release FORCE
	@set MAKEFLAGS=$(MAKEFLAGS)
	$(MAKE) -f $(MAKEFILE).Release uninstall
debug: $(MAKEFILE).Debug FORCE
	@set MAKEFLAGS=$(MAKEFLAGS)
	$(MAKE) -f $(MAKEFILE).Debug
debug-make_default: $(MAKEFILE).Debug FORCE
	@set MAKEFLAGS=$(MAKEFLAGS)
	$(MAKE) -f $(MAKEFILE).Debug 
debug-make_first: $(MAKEFILE).Debug FORCE
	@set MAKEFLAGS=$(MAKEFLAGS)
	$(MAKE) -f $(MAKEFILE).Debug first
debug-all: $(MAKEFILE).Debug FORCE
	@set MAKEFLAGS=$(MAKEFLAGS)
	$(MAKE) -f $(MAKEFILE).Debug all
debug-clean: $(MAKEFILE).Debug FORCE
	@set MAKEFLAGS=$(MAKEFLAGS)
	$(MAKE) -f $(MAKEFILE).Debug clean
debug-distclean: $(MAKEFILE).Debug FORCE
	@set MAKEFLAGS=$(MAKEFLAGS)
	$(MAKE) -f $(MAKEFILE).Debug distclean
debug-install: $(MAKEFILE).Debug FORCE
	@set MAKEFLAGS=$(MAKEFLAGS)
	$(MAKE) -f $(MAKEFILE).Debug install
debug-uninstall: $(MAKEFILE).Debug FORCE
	@set MAKEFLAGS=$(MAKEFLAGS)
	$(MAKE) -f $(MAKEFILE).Debug uninstall

Makefile: LuitPad.pro  ..\..\QtSDK\Desktop\Qt\4.8.1\msvc2010\mkspecs\win32-msvc2010\qmake.conf ..\..\QtSDK\Desktop\Qt\4.8.1\msvc2010\mkspecs\qconfig.pri \
		..\..\QtSDK\Desktop\Qt\4.8.1\msvc2010\mkspecs\modules\qt_webkit_version.pri \
		..\..\QtSDK\Desktop\Qt\4.8.1\msvc2010\mkspecs\features\qt_functions.prf \
		..\..\QtSDK\Desktop\Qt\4.8.1\msvc2010\mkspecs\features\qt_config.prf \
		..\..\QtSDK\Desktop\Qt\4.8.1\msvc2010\mkspecs\features\exclusive_builds.prf \
		..\..\QtSDK\Desktop\Qt\4.8.1\msvc2010\mkspecs\features\default_pre.prf \
		..\..\QtSDK\Desktop\Qt\4.8.1\msvc2010\mkspecs\features\win32\default_pre.prf \
		..\..\QtSDK\Desktop\Qt\4.8.1\msvc2010\mkspecs\features\release.prf \
		..\..\QtSDK\Desktop\Qt\4.8.1\msvc2010\mkspecs\features\debug_and_release.prf \
		..\..\QtSDK\Desktop\Qt\4.8.1\msvc2010\mkspecs\features\default_post.prf \
		..\..\QtSDK\Desktop\Qt\4.8.1\msvc2010\mkspecs\features\win32\default_post.prf \
		..\..\QtSDK\Desktop\Qt\4.8.1\msvc2010\mkspecs\features\win32\rtti.prf \
		..\..\QtSDK\Desktop\Qt\4.8.1\msvc2010\mkspecs\features\win32\exceptions.prf \
		..\..\QtSDK\Desktop\Qt\4.8.1\msvc2010\mkspecs\features\win32\stl.prf \
		..\..\QtSDK\Desktop\Qt\4.8.1\msvc2010\mkspecs\features\shared.prf \
		..\..\QtSDK\Desktop\Qt\4.8.1\msvc2010\mkspecs\features\win32\embed_manifest_exe.prf \
		..\..\QtSDK\Desktop\Qt\4.8.1\msvc2010\mkspecs\features\win32\embed_manifest_dll.prf \
		..\..\QtSDK\Desktop\Qt\4.8.1\msvc2010\mkspecs\features\warn_on.prf \
		..\..\QtSDK\Desktop\Qt\4.8.1\msvc2010\mkspecs\features\qt.prf \
		..\..\QtSDK\Desktop\Qt\4.8.1\msvc2010\mkspecs\features\win32\thread.prf \
		..\..\QtSDK\Desktop\Qt\4.8.1\msvc2010\mkspecs\features\moc.prf \
		..\..\QtSDK\Desktop\Qt\4.8.1\msvc2010\mkspecs\features\win32\windows.prf \
		..\..\QtSDK\Desktop\Qt\4.8.1\msvc2010\mkspecs\features\resources.prf \
		..\..\QtSDK\Desktop\Qt\4.8.1\msvc2010\mkspecs\features\uic.prf \
		..\..\QtSDK\Desktop\Qt\4.8.1\msvc2010\mkspecs\features\yacc.prf \
		..\..\QtSDK\Desktop\Qt\4.8.1\msvc2010\mkspecs\features\lex.prf \
		..\..\QtSDK\Desktop\Qt\4.8.1\msvc2010\mkspecs\features\include_source_dir.prf \
		f:\QtSDK\Desktop\Qt\4.8.1\msvc2010\lib\qtmain.prl
	$(QMAKE) -spec ..\..\QtSDK\Desktop\Qt\4.8.1\msvc2010\mkspecs\win32-msvc2010 CONFIG+=release -o Makefile LuitPad.pro
..\..\QtSDK\Desktop\Qt\4.8.1\msvc2010\mkspecs\qconfig.pri:
..\..\QtSDK\Desktop\Qt\4.8.1\msvc2010\mkspecs\modules\qt_webkit_version.pri:
..\..\QtSDK\Desktop\Qt\4.8.1\msvc2010\mkspecs\features\qt_functions.prf:
..\..\QtSDK\Desktop\Qt\4.8.1\msvc2010\mkspecs\features\qt_config.prf:
..\..\QtSDK\Desktop\Qt\4.8.1\msvc2010\mkspecs\features\exclusive_builds.prf:
..\..\QtSDK\Desktop\Qt\4.8.1\msvc2010\mkspecs\features\default_pre.prf:
..\..\QtSDK\Desktop\Qt\4.8.1\msvc2010\mkspecs\features\win32\default_pre.prf:
..\..\QtSDK\Desktop\Qt\4.8.1\msvc2010\mkspecs\features\release.prf:
..\..\QtSDK\Desktop\Qt\4.8.1\msvc2010\mkspecs\features\debug_and_release.prf:
..\..\QtSDK\Desktop\Qt\4.8.1\msvc2010\mkspecs\features\default_post.prf:
..\..\QtSDK\Desktop\Qt\4.8.1\msvc2010\mkspecs\features\win32\default_post.prf:
..\..\QtSDK\Desktop\Qt\4.8.1\msvc2010\mkspecs\features\win32\rtti.prf:
..\..\QtSDK\Desktop\Qt\4.8.1\msvc2010\mkspecs\features\win32\exceptions.prf:
..\..\QtSDK\Desktop\Qt\4.8.1\msvc2010\mkspecs\features\win32\stl.prf:
..\..\QtSDK\Desktop\Qt\4.8.1\msvc2010\mkspecs\features\shared.prf:
..\..\QtSDK\Desktop\Qt\4.8.1\msvc2010\mkspecs\features\win32\embed_manifest_exe.prf:
..\..\QtSDK\Desktop\Qt\4.8.1\msvc2010\mkspecs\features\win32\embed_manifest_dll.prf:
..\..\QtSDK\Desktop\Qt\4.8.1\msvc2010\mkspecs\features\warn_on.prf:
..\..\QtSDK\Desktop\Qt\4.8.1\msvc2010\mkspecs\features\qt.prf:
..\..\QtSDK\Desktop\Qt\4.8.1\msvc2010\mkspecs\features\win32\thread.prf:
..\..\QtSDK\Desktop\Qt\4.8.1\msvc2010\mkspecs\features\moc.prf:
..\..\QtSDK\Desktop\Qt\4.8.1\msvc2010\mkspecs\features\win32\windows.prf:
..\..\QtSDK\Desktop\Qt\4.8.1\msvc2010\mkspecs\features\resources.prf:
..\..\QtSDK\Desktop\Qt\4.8.1\msvc2010\mkspecs\features\uic.prf:
..\..\QtSDK\Desktop\Qt\4.8.1\msvc2010\mkspecs\features\yacc.prf:
..\..\QtSDK\Desktop\Qt\4.8.1\msvc2010\mkspecs\features\lex.prf:
..\..\QtSDK\Desktop\Qt\4.8.1\msvc2010\mkspecs\features\include_source_dir.prf:
f:\QtSDK\Desktop\Qt\4.8.1\msvc2010\lib\qtmain.prl:
qmake: qmake_all FORCE
	@$(QMAKE) -spec ..\..\QtSDK\Desktop\Qt\4.8.1\msvc2010\mkspecs\win32-msvc2010 CONFIG+=release -o Makefile LuitPad.pro

qmake_all: FORCE

make_default: release-make_default debug-make_default FORCE
make_first: release-make_first debug-make_first FORCE
all: release-all debug-all FORCE
clean: release-clean debug-clean FORCE
	-$(DEL_FILE) ".\LuitPad.intermediate.manifest"
	-$(DEL_FILE) LuitPad.exp
distclean: release-distclean debug-distclean FORCE
	-$(DEL_FILE) Makefile

check: first

release-mocclean: $(MAKEFILE).Release
	@set MAKEFLAGS=$(MAKEFLAGS)
	$(MAKE) -f $(MAKEFILE).Release mocclean
debug-mocclean: $(MAKEFILE).Debug
	@set MAKEFLAGS=$(MAKEFLAGS)
	$(MAKE) -f $(MAKEFILE).Debug mocclean
mocclean: release-mocclean debug-mocclean

release-mocables: $(MAKEFILE).Release
	@set MAKEFLAGS=$(MAKEFLAGS)
	$(MAKE) -f $(MAKEFILE).Release mocables
debug-mocables: $(MAKEFILE).Debug
	@set MAKEFLAGS=$(MAKEFLAGS)
	$(MAKE) -f $(MAKEFILE).Debug mocables
mocables: release-mocables debug-mocables
FORCE:

$(MAKEFILE).Release: Makefile
$(MAKEFILE).Debug: Makefile
