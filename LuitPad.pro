
QT += core gui
#unix|win32: LIBS += -lQtCore4

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport
greaterThan(QT_MAJOR_VERSION, 4): DEFINES = QT_VERSION_5





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
#    src/profile/encryption.h \
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
#    src/profile/encryption.cpp \
    \
    src/utils/utilities.cpp \
    \
    src/dictionary/dictionary.cpp

RESOURCES     = \
    mdi.qrc

TARGET = LuitPad
# install
target.path = $$[qt]/mainwindows/mdi
sources.files = $$SOURCES $$HEADERS $$RESOURCES $$FORMS LuitPad.pro images
sources.path = $$[qt]/mainwindows/mdi
INSTALLS += target sources

OTHER_FILES += \
    luitpad.rc

RC_FILE = luitpad.rc





#FORMS += luitbrowser.ui

DISTFILES += \
    files/DEFAULT.dat \
    files/Lohit-Assamese.pfb \
    files/Lohit-Assamese.sfd \
    files/Lohit-Assamese.ttf \
    files/Nabaprabhat.ttf \
    files/Siyam Rupali ANSI.ttf \
    files/Siyamrupali.ttf \
    files/Siyamrupali_1_01.ttf \
    files/SolaimanLipi.ttf \
    files/T_IdeaBase.csv \
    files/T_Idioms.tsv \
    files/T_Poribhasha.tsv \
    files/T_WrdASM.csv \
    files/T_WrdASMIdea.csv \
    files/T_WrdENG.csv \
    files/T_WrdENGIdea.csv \
    files/T_WrdExamples.tsv \
    files/ambiguous.txt \
    files/ambiguous1.txt \
    files/char_usage.txt \
    files/dictionary-file.txt \
    files/inflexions_comb.txt \
    files/kalpurush ANSI.ttf \
    files/kalpurush.ttf \
    files/poribhasha-file-1.txt \
    files/poribhasha-file.txt \
    files/processed_dictionary.prefix_len_5.txt \
    files/processed_dictionary.txt \
    files/processed_dictionary_1.txt \
    files/processed_dictionary_6.txt \
    files/processed_dictionary_Sumu_corrected_Feb_2013.txt \
    files/processed_dictionary_old.txt \
    files/processed_temp.txt \
    files/romanized.txt \
    files/unicode_to_roman_override.txt \
    files/user_keymap.txt
