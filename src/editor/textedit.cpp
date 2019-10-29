#include <QStringList>
#include <QSignalMapper>
#include "textedit.h"
#include <QCompleter>
#include <QKeyEvent>
#include <QAbstractItemView>
#include <QtDebug>
#include <QApplication>
#include <QModelIndex>
#include <QAbstractItemModel>
#include <QScrollBar>
#include <QDebug>
#include <QMenu>


#include "src/phonetic/phonetic.h"
#include "src/profile/profile.h"
#include "src/utils/utilities.h"


//extern int validSoftware;

void TextEdit::deleteOldWord( int i) {
 //   qDebug() << "deleting connected through  word no " << i;
    Profile *currProfile = Profile::getkeyBoard();
    currProfile->deleteWord( Utilities::getUnicodeString(oldWords[i]->text()));
}


void TextEdit::addNewWord( int i) {
   // qDebug() << "connected through  word no " << i;
    Profile *currProfile = Profile::getkeyBoard();
    currProfile->addWord( Utilities::getUnicodeString( newWords[i]->text()));
   // qDebug()  << "Romanized " << Romanization::convert2Roman(  Utilities::getUnicodeString( newWords[i]->text())  );
    //Phonetic::insertWordFromOutside(  newWords[i]->text());
    /*foreach(QString unicodeWord, it.value() ) {

        charList.clear();
        foreach(QChar c, it.key())charList.append( Phonetic::phoneticEquivString(QString(c).toLower()));

       // qDebug() << charList << "  " << unicodeWord;
       insertWord(roman2UnicodeTree, charList, unicodeWord);
    }
    */

}

void TextEdit::replaceNewWord( int i) {
   QString replacement = replaceWords[i]->text();
   QTextCursor cursor = cursorForPosition(lastPosition);
   cursor.select(QTextCursor::WordUnderCursor);
   cursor.deleteChar();
   cursor.insertText(replacement);
}


 TextEdit::TextEdit(QWidget *parent) : QTextEdit(parent), c(0)
     {
         setPlainText(trUtf8("")) ;
         /*
         setPlainText(tr("This TextEdit provides autocompletions for words that have more than"
                              " 3 characters. You can trigger autocompletion using ") +
                              QKeySequence("Ctrl+E").toString(QKeySequence::NativeText));
         */
         rightClickCount=0;
         setPlainText(tr(""));
     }


 TextEdit::~TextEdit(){}

 void TextEdit::setCompleter(QCompleter *completer)
 {
     if (c)
         QObject::disconnect(c, 0, this, 0);

     c = completer;

     if (!c)
         return;

     c->setWidget(this);
     c->setCompletionMode(QCompleter::PopupCompletion);
     c->setCompletionRole(2);
     c->setModelSorting(QCompleter::CaseInsensitivelySortedModel);
 //    qDebug()<<"Total Number of words in completer: "<<c->completionCount();
 //    qDebug()<<"Index: "<<c->currentCompletion();

     //c->setCaseSensitivity(Qt::CaseInsensitive);
     //c->setMaxVisibleItems(10);

     QObject::connect(c, SIGNAL(activated(QString)), this, SLOT(insertCompletion(QString)));
 }

 QCompleter *TextEdit::completer() const
 {
     return c;
 }


 void TextEdit::insertCharSelection(const QString selectedChar, const QString prefix)
 {
     if (c->widget() != this)
         return;
     QTextCursor tc = textCursor();

     tc.movePosition(QTextCursor::Left);
     tc.select( QTextCursor::WordUnderCursor );

     QString word =tc.selectedText();

     QString prefixstr;
     QString suffix;

     for(int i = 0;  i < word.size();  ++i) {
         if( word.at(i).isLetter() && ( (word.at(i) >= 'A' && word.at(i) <= 'Z') || (word.at(i) >= 'a' && word.at(i) <= 'z') ))
             suffix.append(word.at(i));
         else
             prefixstr.append(word.at(i));
     }

     tc.insertText(prefixstr + selectedChar);
    // setTextCursor(tc);
 }


 void TextEdit::insertVowelModSelection(const QString selectedChar, const QString prefix)
 {
     if (c->widget() != this)
         return;
     QTextCursor tc = textCursor();

     tc.movePosition(QTextCursor::Left);
     tc.select( QTextCursor::WordUnderCursor );

     QString word =tc.selectedText();

     QString prefixstr;



     int moveBack=-1;
     for(int i = 0;  i < word.size();  ++i) {
         prefixstr.append(word.at(i));

     }

     prefixstr.append(selectedChar);
     tc.insertText(prefixstr);
     tc.movePosition( QTextCursor::PreviousCharacter, QTextCursor::MoveAnchor,moveBack );
    // qDebug() << "Set textcursor";
     setTextCursor(tc);


 }



 void TextEdit::insertMidCharSelection(const QString selectedChar)
 {
     if (c->widget() != this)
         return;
     QTextCursor tc = textCursor();

     tc.movePosition(QTextCursor::Left);
     tc.select( QTextCursor::WordUnderCursor );

     QString word =tc.selectedText();

     QString prefixstr;
     QString suffix="";


     int moveBack=-1;
     for(int i = 0;  i < word.size();  ++i) {
         if(
             ( word.at(i).isDigit() &&
               (word.at(i) >= '0' && word.at(i) <= '9')
               ) ||
             (word.at(i).isLetter()
             && ( (word.at(i) >= 'A' && word.at(i) <= 'Z') || (word.at(i) >= 'a' && word.at(i) <= 'z') )
             )
           ) {


             if( suffix.size() == 0 ) {
                suffix.append(selectedChar);
                prefixstr.append(suffix);

                moveBack=0;
             }
         }
         else {
             prefixstr.append(word.at(i));
         //    qDebug() << Utilities::getUnicodeString(word.at(i));
           //  qDebug() << Utilities::zeroLengthChar->contains( Utilities::getUnicodeString(word.at(i)));


             if(! Utilities::zeroLengthChar->contains( Utilities::getUnicodeString(word.at(i))) ) {
                 moveBack++;

             }
             else if(  Utilities::getUnicodeString(word.at(i))==QString("0x9cd") ) {
                 moveBack--;
             }
         }
     }


     tc.insertText(prefixstr);
     tc.movePosition( QTextCursor::PreviousCharacter, QTextCursor::MoveAnchor,moveBack );
    // qDebug() << "Set textcursor";
     setTextCursor(tc);


 }


 void TextEdit::insertCompletion(const QString& completion)
 {


     if (c->widget() != this)
         return;
     QTextCursor tc = textCursor();
     tc.movePosition(QTextCursor::Left);

     int extra;
     QString newWord;
     ToolTipSingleton *toolTipControl=ToolTipSingleton::GetToolTipSingleton("" );


     if( _state ==F2) {
       tc.movePosition(QTextCursor::EndOfWord);
       extra =  completion.length() - c->completionPrefix().length();
       tc.insertText(completion.right(extra));
     }
     else {
       tc.movePosition(QTextCursor::EndOfWord);
       tc.select( QTextCursor::WordUnderCursor );
       if(  toolTipControl->hasAssamesePrefix(this)) {
           newWord= toolTipControl->trimRomanSuffix(this);
           tc.insertText(newWord + Utilities::getField(completion,"\t",0));
        }
       else {

       //tc.removeSelectedText();
          tc.insertText(Utilities::getField(completion,"\t",0));
       }
     //  qDebug() << "<<" << Utilities::getField(completion,"\t",0) <<">>";
     }
     setTextCursor(tc);
 }

 QString TextEdit::textUnderCursor() const
 {
     QTextCursor tc = textCursor();
     tc.select(QTextCursor::WordUnderCursor);

   //  qDebug() << Utilities::getUnicodeString(tc.selectedText());

     return tc.selectedText();
 }

 QString TextEdit::charUnderCursor() const
 {
     QTextCursor tc = textCursor();
//     tc.select(QTextCursor::WordUnderCursor);

     tc.movePosition( QTextCursor::PreviousCharacter, QTextCursor::KeepAnchor, 1 );

     return tc.selectedText();
 }

 //cursor.movePosition( QTextCursor::NextCharacter, QTextCursor::KeepAnchor, 2 );

 void TextEdit::focusInEvent(QFocusEvent *e)
 {
     if (c)
         c->setWidget(this);
     QTextEdit::focusInEvent(e);
 }




 void TextEdit::charToolTip(QKeyEvent *e) {
     if (c && c->popup()->isVisible()) {
         // The following keys are forwarded by the completer to the widget
        switch (e->key()) {
            case Qt::Key_Enter:
            case Qt::Key_Return:
               e->ignore();
               return; // let the completer do default behavior
        default:
            break;
        }
     }

     bool isShortcut = isShortcut = (e->key() == Qt::Key_Control);
     QTextEdit::keyPressEvent(e);
     if(! isShortcut )
         return;

     QString completionPrefix = textUnderCursor();
     QRect cr = cursorRect();

     if( e->key()!=Qt::Key_Enter) {
          if (completionPrefix != c->completionPrefix() && completionPrefix.size() > 0) {
      //   qDebug() << "Completion prefix  " << "\"" << completionPrefix << "\"";
             c->setCompletionPrefix(completionPrefix);
             c->popup()->setCurrentIndex(c->completionModel()->index(0, 0));
          }
          cr.setWidth(c->popup()->sizeHintForColumn(0) + c->popup()->verticalScrollBar()->sizeHint().width());
          QPoint q = this->viewport()->mapToGlobal(QPoint(TextEdit::cursorRect().x(),TextEdit::cursorRect().y()));
          QCursor::setPos(q.x() - 2,q.y());
          c->complete(cr); // popup it up!
     }

     if( e->key() == Qt::Key_Space  || e->key() == Qt::Key_Tab) {
         c->popup()->hide();
     }

 }

 void TextEdit::wordToolTip(QKeyEvent *e) {
     QTextEdit::keyPressEvent(e);
 }

 void TextEdit::keyPressEvent(QKeyEvent *e)
 {
     if( e->key()!= Qt::Key_Backspace  &&  this->toPlainText().length() > 250 ) {
         if( this->toPlainText().length() < 270) {
             this->setText( this->toPlainText() + QString(" Maimum limit exceeded! Please try the full version"));
         }
         Utilities::warningBox("You have exceeded the 250 characters limit per window for the trial version\n For full version please see text in the 2nd window.");
         return;
     }


     if( _state == F2) {
        charToolTip(e);
     }
     else {
        wordToolTip(e);
     }
 }

 bool TextEdit::canInsertFromMimeData(const QMimeData* source) const
 {
     return source->hasImage() || source->hasUrls() ||
         QTextEdit::canInsertFromMimeData(source);
 }

 void TextEdit::insertFromMimeData(const QMimeData* source)
 {
  //   qDebug() << " Insert into the textedit";
     if (source->hasImage())
     {
         static int i = 1;
         QUrl url(QString("dropped_image_%1").arg(i++));
         dropImage(url, qvariant_cast<QImage>(source->imageData()));
     }
     else if (source->hasUrls())
     {
         foreach (QUrl url, source->urls())
         {
             QFileInfo info(url.toLocalFile());
             if (QImageReader::supportedImageFormats().contains(info.suffix().toLower().toLatin1()))
                 dropImage(url, QImage(info.filePath()));
             else
                 dropTextFile(url);
         }
     }
     else
     {
         QTextEdit::insertFromMimeData(source);
     }
 }


 void TextEdit::dropImage(const QUrl& url, const QImage& image)
 {
     if (!image.isNull())
     {
         document()->addResource(QTextDocument::ImageResource, url, image);
      //   qDebug() << "inserting image " << url.toString();
         textCursor().insertImage(url.toString());
     }
 }

 void TextEdit::dropTextFile(const QUrl& url)
 {
     QFile file(url.toLocalFile());
     if (file.open(QIODevice::ReadOnly | QIODevice::Text))
         textCursor().insertText(file.readAll());
 }


 void TextEdit::copyForAsciiFn(){
     Translator *translator = Translator::getTranslator();
     this->copy();
     QClipboard *clipboard = QApplication::clipboard();
     QString copiedText = clipboard->text();
     QString asciiText  = translator->translateTextToAscii(copiedText);

     clipboard->setText(asciiText);

 }

 void TextEdit::pasteFromAsciiFn(){
     Translator *translator = Translator::getTranslator();
     QClipboard *clipboard = QApplication::clipboard();
     QString tempString = clipboard->text();

     QString translatedText = translator->translateTextToUnicode(tempString);

     clipboard->setText(translatedText);
     this->paste();
     clipboard->setText(tempString);

 }

 /*

 void TextEdit::Ignore() {
     QTextCursor cursor = cursorForPosition(lastPosition);
     cursor.select(QTextCursor::WordUnderCursor);
     QString ignoreWord = textUnderCursor();
     if(ignoreWord.size() > 1) ignoreDictionary.insert(ignoreWord,true);
    // highlighter->rehighlight();
  //   highlighter->highlightBlock();
     qDebug() << "Ignore list size in Text edit " <<  ignoreDictionary.size();
    // qDebug() << "Cursor " << Utilities::getUnicodeString(ignoreWord);
 }
*/


 QString TextEdit::getIgnoreWordsString(){
     QHash<QString, bool>::const_iterator it;
     QString ignoreWordsString;
     ignoreWordsString.clear();

     ignoreWordsString = QString( "\n<ignore_list>\n");
     for(it = ignoreDictionary.constBegin(); it!=ignoreDictionary.constEnd(); ++it){
        ignoreWordsString.append(it.key() + "\n");
     }
     ignoreWordsString.append("</ignore_list>\n");
     return ignoreWordsString;
 }
