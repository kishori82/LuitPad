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

#ifndef MDICHILD_H
#define MDICHILD_H

#include <QTextEdit>
#include <QCompleter>
#include <QTextOption>
#include <QTextStream>
#include <QResizeEvent>
#include <QList>
#include <QMenu>
#include <QPrinter>
#include <QApplication>
#include <QAbstractItemView>
#include <QScrollBar>
#include <QFileDialog>
#include <QToolTip>

#include "src/editor/textedit.h"
#include "src/editor/tooltipsingleton.h"
#include "src/characters/chartrie.h"
#include "src/phonetic/wordstrie.h"
#include "src/utils/utilities.h"
#include "src/phonetic/phonetic.h"
#include "src/core/tablemodel.h"
#include "src/core/inputdialog.h"
#include "src/editor/highlighter.h"


class MdiChild : public TextEdit
{
    Q_OBJECT

//private slots:
  //  void copy();

public:
    ToolTipSingleton *toolTipControl;
    CharTrie *charMapTree;
    WordsTrie *wordMapTree;


    void testPrinting();

    MdiChild();
    void newFile();
    bool loadFile(const QString &fileName);
    void readAsLuitPadFormat( QTextStream &in);
    bool save();
    bool saveAs();
    bool saveFile(const QString &fileName);
    bool saveAsHtml();
    bool saveFileHtml(const QString &fileName);

    bool saveAsPDF();
    bool saveFilePDF(const QString &fileName);
    bool exportOdf();

    void addContextMenu(QMenu *childMenu);

    void setDisable(bool value);
  //  WordsTrie *wordMapTree;

    QString userFriendlyCurrentFile();
    QString currentFile() { return curFile; }
    bool eventFilter(QObject *obj, QEvent *event);
    void setCompleter(QCompleter *com);
    void setToolTipSingleton(ToolTipSingleton *);
    //void _ka();

public slots:
    //void setCompleter(QCompleter *com);
     void showToolTipText() ;
     void showVowelModToolTipText() ;
     void showToolTip();
     void computeToolTipText();
     void resizeImageSize();
     void updateCompleterModelOnTextChange();

private:
     void computeToolTipTextOnSpace();

protected:
    void closeEvent(QCloseEvent *event);

private:
    void wordToolTipText(QKeyEvent *e) ;
    void characterToolTipText(QKeyEvent *event);
    void expandListonPrefix();
    void vowelModToolTipText();
    CONFIG_STATES getConfigState();

private slots:
    void documentWasModified();

private:
    //bool event(QEvent *event);
    void keyPressEvent( QKeyEvent *event );
    void keyReleaseEvent(QKeyEvent *event);

    bool maybeSave();
    void setCurrentFile(const QString &fileName);
    QString strippedName(const QString &fullFileName);
    void insertCompositeLetter();
    void replaceDanda();


    QString curFile;
    bool isUntitled;
    void contextMenuEvent(QContextMenuEvent *event);
    QStringList getSpellingSuggestions(QHash<QString, QString> &candidateWordsInflectionPairs, unsigned int nMax);
    void getWordInfo();

public slots:
    void Ignore();
    void showIdioms();
    void showExamples();


public:
    //QStringList getWordPropositions(const QString word);
signals:
   // void addWord(QString word);

protected:
//	void createActions();
  //  void contextMenuEvent(QContextMenuEvent *event);

private slots:
//    void slot_addWord();
//    void slot_ignoreWord();
private:
    enum { MaxWords = 5 };
    QAction *misspelledWordsActs[MaxWords];

    QPoint lastPos;

    QStringList addedWords;
    Highlighter *highlighter;
    bool disable;


};


#endif
