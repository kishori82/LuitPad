
QT += core gui


TEMPLATE = app

HEADERS       = mainwindow.h \
                            mdichild.h \
    TreeNode.h \
    wordstrie.h \
    Constants.h \
    tooltipsingleton.h \
    chartrie.h \
    textedit.h \
    phonetic.h \
    profile.h \
    romanization.h \
    tablemodel.h \
    encryption.h \
    inputdialog.h \
    loaddatathread.h \
    utilities.h \
    asciiTrie.h \
    Translator.h \
    dongle.h \
    searchreplace.h \
    highlighter.h \
    inflexTrie.h \
    dictionary.h \
    security.h \
    registerbox.h


SOURCES       = main.cpp \
                            mainwindow.cpp \
                            mdichild.cpp \
    TreeNode.cpp \
    wordstrie.cpp \
    tooltipsingleton.cpp \
    chartrie.cpp \
    textedit.cpp \
    phonetic.cpp \
    profile.cpp \
    romanization.cpp \
    tablemodel.cpp \
    encryption.cpp \
    inputdialog.cpp \
    loaddatathread.cpp \
    utilities.cpp \
    asciiTrie.cpp \
    Translator.cpp \
    dongle.cpp \
    searchreplace.cpp \
    highlighter.cpp \
    inflexTrie.cpp \
    dictionary.cpp \
    security.cpp \
    registerbox.cpp
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



unix|win32: LIBS += -lQtCore4

FORMS += \
    registerbox.ui
