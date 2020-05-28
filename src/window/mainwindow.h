/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor
**     the names of its contributors may be used to endorse or promote
**     products derived from this software without specific prior written
**     permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "src/editor/highlighter.h"
#include "src/characters/chartrie.h"
#include "src/phonetic/wordstrie.h"
#include "src/editor/textedit.h"
#include "src/editor/tooltipsingleton.h"
#include "src/utils/utilities.h"
#include "src/phonetic/romanization.h"
#include "src/phonetic/translator.h"
#include "src/editor/searchreplace.h"

class MdiChild;
QT_BEGIN_NAMESPACE
class QAction;
class QMenu;
class QMdiArea;
class QMdiSubWindow;
class QSignalMapper;
class QTextCharFormat ;
class QTimer;
class QToolTip;
class QComboBox;
class QFontComboBox;
class QTextCursor;
class QCompleter;
class QAbstractItemModel;
class QAbstractButton;
QT_END_NAMESPACE

typedef enum _FILETYPE { NEW, TRIAL, INFO} FILETYPE;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

    QMdiArea *editor;


protected:
    void closeEvent(QCloseEvent *event);

public slots:
    //void triggeredChar(QAbstractButton*);
private:
private slots:
    void newFile(FILETYPE type = NEW);
    void open();
    void save();
    void saveAs();
    void saveAsHtml();
    void saveAsPDF();
    void printPrvFile();
    void printFile();
    void selectAll();
    void cut();
    void copy();
    void paste();
    void search();
    void about();
    void updateMenus();
    void kbdDisplay();
    void updateSettingsMenu();
    void updateWindowMenu();
    MdiChild *createMdiChild(FILETYPE type = NEW);
    void switchLayoutDirection();
    void setActiveSubWindow(QWidget *window);

    void underlineText();
    void italicText();
    void boldText();
    void setFormat(QTextCharFormat &format);

    void rightAlign();
    void leftAlign();
    void centerAlign();
    void justifyAlign();
    //void textColor();

    void textFamily(const QString &f);
    void textSize(const QString &p);
    void textQCompleterFont(const QString &p);
    void mergeFormatOnWordOrSelection(const QTextCharFormat &format);
    //void colorChanged(const QColor &c);

    QAbstractItemModel *modelFromFile(const QString& fileName);


    void f2Action();
    void f3Action();

    void toggleAutoVowelMode();
    void toggleAutoCharacterFillMode();
    void toggleSingleCharacterMode();
    void toggleHideToolTipMode();

    QStringList pickRootWords(QStringList &wordList);
//    void openLuitBrowserSlot();
    void printInternalDictionarySlot();
    void writeNewDictionarySlot();
    void loadUndefinedWords();
    void createMessageText(MdiChild *child);

private:

    QString myToolTip;
    QTimer *timer;

 //   void createLuitBrowserTab();
    void createDevelopmentMenus() ;
    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();
    QMenu* getNewChildMenu();

    void readSettings();
    void writeSettings();
    MdiChild *activeMdiChild();
    QMdiSubWindow *findMdiChild(const QString &fileName);
    bool eventFilter(QObject *obj, QEvent *event);
    //void keyReleaseEvent(QKeyEvent *kv);
    //void keyPressEvent(QKeyEvent *kv);
    void keyMap(QString ch);
    void setCurrentFile(const QString &fileName);
    void initCompleter();
    void docState();
    QString generateSignature();

    QMenu *luitbrowserMenu;
    QAction *luitbrowserAct;


    QMenu *developmentMenu;
    QAction *printInternalDictionaryAct;
    QAction *writeNewDictionaryAct;
    QAction *loadUndefinedWordsAct;

    QSignalMapper *windowMapper;

    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *formatMenu;
    QMenu *windowMenu;
    QMenu *settingsMenu;
    QMenu *helpMenu;
   // QMenu  *tutorialsMenu;


    /*
    QSignalMapper *tutorialSignalMapper;
    QAction *videos[10];
    QStringList videoTitles;
    QStringList videoFiles;
*/


    QToolBar *fileToolBar;
    QToolBar *editToolBar;
    QToolBar *formatToolBar;

    QAction *newAct;
    QAction *openAct;
    QAction *saveAct;
    QAction *saveAsAct;
    QAction *printAct;
    QAction *printPrvAct;
    QAction *exitAct;
    QAction *selectAllAct;
    QAction *cutAct;
    QAction *copyAct;
    QAction *pasteAct;
    QAction *searchAct;
    QAction *docStateAct;
    QAction *closeAct;
    QAction *closeAllAct;
    QAction *tileAct;
    QAction *cascadeAct;
    QAction *nextAct;
    QAction *previousAct;
    QAction *separatorAct;
    QAction *aboutAct;
    QAction* kbdDisplayAct;



    QAction * lAlign;
    QAction * rAlign;
    QAction * cAlign;
    QAction * jAlign;

    QAction * iText;
    QAction * bText;
    QAction * uText;

    QAction *f2Act;
    QAction *f3Act;

 /* QAction *f4Act;
    QAction *f5Act;
    QAction *f6Act;
*/
    QAction *saveAsHtmlAct;
     QAction *saveAsPDFAct;

    QAction *autoVowelAct;
    QAction *autoCharacterSubs;
    QAction *singleCharacterMode;
    QAction * hideToolTipMode;


    QComboBox *comboStyle;
    QFontComboBox *comboFont;
    QComboBox *comboSize;

    QCompleter *completer;
    QList<QString>  choices;
    CharTrie *charMapTree;
    WordsTrie *wordsMapTree;

    ToolTipSingleton *tooltipSingleton;

    QHash<QString, bool> storedKeys;

protected:
  //   bool event(QEvent *event);


    bool event(QEvent *event);

private:
    void keyPressEvent(QKeyEvent* e);

private:
    bool isValidConfig();
    void resetConfig();

};

#endif
