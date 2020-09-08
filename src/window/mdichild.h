/************************************************************************
# MIT License

Copyright (c) 2020 Kishori M. Konwar

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
******************************************************************************/

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
