
QT += core gui

#unix|win32: LIBS += -lQtCore4

win64 {
     CONFIG+= static
     DEFINES +=  _WIN32_DCOM
     DEFINES +=  _WINDOWS
}

 DEFINES -=_WIN32_DCOM

TEMPLATE = app

HEADERS  = \
    src/window/mainwindow.h \
    src/window/mdichild.h \
\
    src/core/TreeNode.h \
    src/core/loaddatathread.h \
    src/core/inflexTrie.h \
    src/core/constants.h \
    src/core/tablemodel.h \
    src/core/inputdialog.h \
\
    src/phonetic/wordstrie.h \
    src/phonetic/phonetic.h \
    src/phonetic/romanization.h \
    src/phonetic/translator.h \
\
    src/editor/tooltipsingleton.h \
    src/editor/highlighter.h \
    src/editor/searchreplace.h \
    src/editor/textedit.h \
\
    src/characters/chartrie.h \
    src/characters/asciiTrie.h \
\
    src/profile/profile.h \
    src/profile/encryption.h \
\
    src/utils/utilities.h \
\
    src/dictionary/dictionary.h

SOURCES  = main.cpp \
    src/window/mainwindow.cpp \
    src/window/mdichild.cpp \
    \
    src/core/TreeNode.cpp \
    src/core/loaddatathread.cpp \
    src/core/inflexTrie.cpp \
    src/core/tablemodel.cpp \
    src/core/inputdialog.cpp \
    \
    src/phonetic/wordstrie.cpp \
    src/phonetic/phonetic.cpp \
    src/phonetic/romanization.cpp \
    src/phonetic/translator.cpp \
    \
    src/editor/tooltipsingleton.cpp \
    src/editor/highlighter.cpp \
    src/editor/searchreplace.cpp \
    src/editor/textedit.cpp \
    \
    src/characters/chartrie.cpp \
    src/characters/asciiTrie.cpp \
    \
    src/profile/profile.cpp \
    src/profile/encryption.cpp \
    \
    src/utils/utilities.cpp \
    \
    src/dictionary/dictionary.cpp

RESOURCES     = mdi.qrc

TARGET = LuitPad
# install
target.path = $$[qt]/mainwindows/mdi
sources.files = $$SOURCES $$HEADERS $$RESOURCES $$FORMS LuitPad.pro images
sources.path = $$[qt]/mainwindows/mdi
INSTALLS += target sources

OTHER_FILES += \
    luitpad.rc

RC_FILE = luitpad.rc





FORMS +=
