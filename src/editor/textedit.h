/**************************************************************************
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
****************************************************************************/

#ifndef TEXTEDIT_H
#define TEXTEDIT_H

#include <QFileInfo>
#include <QIODevice>
#include <QImage>
#include <QImageReader>
#include <QMimeData>
#include <QSignalMapper>
#include <QTextDocument>

#include <QHelpEvent>
#include <QTextEdit>
#include <QUrl>

#include <QLabel>
#include <QPair>
#include <QTextBlock>
#include <QTextCursor>
#include <QTextFragment>

#include <QClipboard>
#include <QDebug>
#include <QHash>
#include <QMenu>
#include <QMessageBox>
#include <QToolTip>

#include "src/core/constants.h"
#include "src/phonetic/translator.h"

//#include "highlighter.h"

class Menu : public QMenu {
  Q_OBJECT
public:
  Menu() {}
  bool event(QEvent *e) {
    const QHelpEvent *helpEvent = static_cast<QHelpEvent *>(e);
    if (helpEvent->type() == QEvent::ToolTip && activeAction() != 0) {
      QToolTip::showText(helpEvent->globalPos(), activeAction()->toolTip());
    } else {
      QToolTip::hideText();
    }
    return QMenu::event(e);
  }
};

class QCompleter;

class TextEdit : public QTextEdit {
  Q_OBJECT

public:
  TextEdit(QWidget *parent = 0);
  ~TextEdit();
  //  void contextMenuEvent(QContextMenuEvent *event);
  void setCompleter(QCompleter *c);
  QCompleter *completer() const;

protected:
  void keyPressEvent(QKeyEvent *e);
  void focusInEvent(QFocusEvent *e);

private:
  void charToolTip(QKeyEvent *e);
  void wordToolTip(QKeyEvent *e);
  static const int TAB_SPACES = 4;

private slots:
  void insertCompletion(const QString &completion);
  void addNewWord();
  void replaceNewWord(int i);
  void deleteOldWord(int i);

protected:
  QString textUnderCursor() const;
  QString mouseSelectedText() const;
  QString charUnderCursor() const;

public:
  QCompleter *autocompleter;
  void insertCharSelection(const QString selectedChar, const QString prefix);
  void insertMidCharSelection(const QString selectedChar);
  void insertVowelModSelection(const QString selectedChar,
                               const QString prefix);

public slots:
  void copyForAsciiFn();
  void pasteFromAsciiFn();
  // void Ignore();
  //  void Replace();

protected:
  QString getIgnoreWordsString();

protected:
  QMenu *menu;
  //QMenu *addWord;
  QAction *addWord;

  QMenu *replaceWord;
  QMenu *deleteWord;

  QAction *resizeImage;

  QAction *pasteFromAscii;
  QAction *copyForAscii;

  QAction *ignoreWord;
  QMenu *meaning;
  QAction *meaningText;
  QMenu *synonyms, *officialwords;
  QAction *idioms, *examples;
  QAction *synonymsText, *officialwordsText; // *examplesText, *idiomsText;

  //QAction *newWords[10]
  QString newWord;

  QAction *oldWords[10], *replaceWords[10];
  QSignalMapper *deletesignalMapper, *addsignalMapper,
      *signalMapperReplaceWords;
  QPoint lastPosition;
  int rightClickCount;
  //  Highlighter *highlighter;

public:
  QHash<QString, bool> ignoreDictionary;

private:
  void insertFromMimeDataPlain(const QMimeData *source);

  bool canInsertFromMimeData(const QMimeData *source) const;
  void insertFromMimeData(const QMimeData *source);
  void dropImage(const QUrl &url, const QImage &image);
  void dropTextFile(const QUrl &url);
};

#endif // TEXTEDIT_H
