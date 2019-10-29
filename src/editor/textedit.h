#ifndef TEXTEDIT_H
#define TEXTEDIT_H

#include <QTextEdit>
#include <QSignalMapper>
#include <QUrl>
#include <QMimeData>
#include <QImage>
#include <QImageReader>
#include <QFileInfo>
#include <QIODevice>
#include <QTextDocument>

#include <QTextBlock>
#include <QTextFragment>
#include <QTextCursor>
#include <QPair>
#include <QLabel>
#include "src/phonetic/translator.h"
#include <QDebug>
#include <QClipboard>
#include <QHash>
//#include "highlighter.h"

 class QCompleter;

 class TextEdit : public QTextEdit
 {
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


 private slots:
     void insertCompletion(const QString &completion);
     void addNewWord( int i);
     void replaceNewWord( int i);
     void deleteOldWord( int i);

 protected:
     QString textUnderCursor() const;
     QString charUnderCursor() const;

 public:
     QCompleter *c;
     void insertCharSelection(const QString selectedChar, const QString prefix);
     void insertMidCharSelection(const QString selectedChar);
     void insertVowelModSelection(const QString selectedChar, const QString prefix);


 public slots:
     void copyForAsciiFn();
     void pasteFromAsciiFn();
    // void Ignore();
   //  void Replace();

protected:
     QString getIgnoreWordsString();

protected:
     QMenu *menu;
     QMenu *addWord, *replaceWord;
     QMenu *deleteWord;

     QAction *resizeImage;

     QAction *pasteFromAscii;
     QAction *copyForAscii;

     QAction *ignoreWord;
     QMenu *meaning;
     QAction *meaningText;


     QAction *newWords[10], *oldWords[10], *replaceWords[10];
     QSignalMapper *deletesignalMapper, *addsignalMapper, *signalMapperReplaceWords;
     QPoint lastPosition;
     int rightClickCount;
   //  Highlighter *highlighter;

 public:
     QHash<QString, bool> ignoreDictionary;

 private:
     bool canInsertFromMimeData(const QMimeData* source) const;
     void insertFromMimeData(const QMimeData* source);
     void dropImage(const QUrl& url, const QImage& image);
     void dropTextFile(const QUrl& url);



 };

#endif // TEXTEDIT_H
