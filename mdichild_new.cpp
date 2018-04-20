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

#include <QtGui>

#include "mdichild.h"
#include "Constants.h"
#include "profile.h"

extern bool _auto_matra_setting ;


MdiChild::MdiChild()
{
    setAttribute(Qt::WA_DeleteOnClose);
    isUntitled = true;
    c = NULL;
    //this->setSizeIncrement(200,150);
    //this->setLineWidth(300);
    //this->setFixedSize(200,150);



    Profile *profile = Profile::getkeyBoard();

    profile->fill_keyboard("DEFAULT");

    charMapTree = CharTrie::getCharTrie();

    wordMapTree = WordsTrie::getWordsTrie();

    connect(this, SIGNAL(textChanged()), this, SLOT(updateCompleterModelOnTextChange()));

}
void MdiChild::copy() {
    qDebug() << "Being copied";

}
void MdiChild::newFile()
{
    static int sequenceNumber = 1;

    isUntitled = true;
    curFile = trUtf8("Untitled%1").arg(sequenceNumber++);
    setWindowTitle(curFile + "[*]");



    connect(document(), SIGNAL(contentsChanged()),this, SLOT(documentWasModified()));
}

bool MdiChild::loadFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("LuitPad"),tr("Cannot read file %1:\n%2.").arg(fileName).arg(file.errorString()));
        return false;
    }

    QFileInfo fileInfo(fileName);

    QTextStream in(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);

    if( fileInfo.suffix()== QString("pad") )
        setHtml(in.readAll());
    else
        setPlainText(in.readAll());

    QApplication::restoreOverrideCursor();

    setCurrentFile(fileName);

    connect(document(), SIGNAL(contentsChanged()),this, SLOT(documentWasModified()));

    return true;
}

bool MdiChild::save()
{
    if (isUntitled) {
        return saveAs();
    } else {
       // return saveFile(curFile);
        return saveFileHtml(curFile);
    }
}

bool MdiChild::saveAs()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"),curFile,".pad");
    if (fileName.isEmpty())
        return false;
    return saveFileHtml(fileName);
    //return saveFile(fileName);
}

bool MdiChild::saveAsHtml()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save As Html"),curFile, ".html");
    if (fileName.isEmpty())
        return false;

    return saveFileHtml(fileName);
}


bool MdiChild::saveAsPDF()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save As Html"),curFile, ".pdf");
    if (fileName.isEmpty())
        return false;

    return saveFilePDF(fileName);
}


bool MdiChild::saveFile(const QString &fileName)
{
    QFile file(fileName);
    if( file.exists() ) file.remove(file.fileName());
    if( !file.open(file.ReadWrite) ) return false;
    file.write("\xEF\xBB\xBF");
    file.setTextModeEnabled(true);

    QTextStream out(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    out.setCodec(QTextCodec::codecForName("UTF-8"));
    out << toPlainText();

    QApplication::restoreOverrideCursor();

    setCurrentFile(fileName);
    return true;
}


bool MdiChild::saveFileHtml(const QString &fileName)
{
    QFile file(fileName);
    if( file.exists() ) file.remove(file.fileName());
    if( !file.open(file.ReadWrite) ) return false;
    file.write("\xEF\xBB\xBF");
    file.setTextModeEnabled(true);

    QTextStream out(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    out.setCodec(QTextCodec::codecForName("UTF-8"));
    out << toHtml();
    QApplication::restoreOverrideCursor();

    setCurrentFile(fileName);
    return true;
}
bool MdiChild::saveFilePDF(const QString &fileName)
{
    QString filename = fileName;
    if(QFileInfo(filename).suffix().isEmpty()) {
            filename.append(".pdf");
    }
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(filename);
    QPrintDialog *dlg = new QPrintDialog(&printer,this);
    dlg->setWindowTitle(QObject::tr("Print Document"));

    if(dlg->exec() == QDialog::Accepted) {
       this->document()->print(&printer);
    }
    delete dlg;
    return true;


}

QString MdiChild::userFriendlyCurrentFile()
{
    return strippedName(curFile);
}

void MdiChild::closeEvent(QCloseEvent *event)
{
    if (maybeSave()) {
        event->accept();
    } else {
        event->ignore();
    }
}

void MdiChild::documentWasModified()
{
    setWindowModified(document()->isModified());
}

bool MdiChild::maybeSave()
{
    if (document()->isModified()) {
	QMessageBox::StandardButton ret;
        ret = QMessageBox::warning(this, tr("LuitPad"),
                     tr("'%1' has been modified.\n Do you want to save your changes?")
                     .arg(userFriendlyCurrentFile()),
                     QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        if (ret == QMessageBox::Save)
            return save();
        else if (ret == QMessageBox::Cancel)
            return false;
    }
    return true;
}

void MdiChild::setCurrentFile(const QString &fileName)
{
    curFile = QFileInfo(fileName).canonicalFilePath();
    isUntitled = false;
    document()->setModified(false);
    setWindowModified(false);
    setWindowTitle(userFriendlyCurrentFile() + "[*]");
}

QString MdiChild::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}


void MdiChild::setCompleter(QCompleter *compltr)
{

    if (c) QObject::disconnect(c, 0, this, 0);

    c = compltr;

    if (!c) return;

    c->setWidget(this);
    c->setCompletionMode(QCompleter::PopupCompletion);
    c->setCaseSensitivity(Qt::CaseInsensitive);
    QObject::connect(c, SIGNAL(activated(QString)), this, SLOT(insertCompletion(QString)));

    //this->setCaseSensitivity(Qt::CaseInsensitive);

}

void MdiChild::setToolTipSingleton(ToolTipSingleton *tooltipSingleton) {

    if( tooltipSingleton != NULL)
        toolTipControl = tooltipSingleton;

}

bool MdiChild::eventFilter(QObject *obj, QEvent *event)
 {
    if (event->type() == QEvent::KeyPress) {
        //QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);

        return true;
    } else {
        // standard event processing
        if( event->type() == QEvent::Resize ) {
            qDebug() << "Resize event";

            return false;
        }
        return QObject::eventFilter(obj, event);
    }
 }


void  MdiChild::showToolTip()
   {
    //   QToolTip::showText(editor->mapToGlobal(QPoint()), "A tool tip");

       return;
       QString text;

     //  QTimer *timer = new QTimer(this);
    //   connect(timer, SIGNAL(timeout()), this, SLOT(myEventSender()));
   //    timer->start(1000);

       if( QToolTip::isVisible() ) {
          QToolTip::hideText();
         // qDebug() << "Tooltip 260";
       }


       QList<QKeyValue>  choices;
       charMapTree->get_choice( toolTipControl->charPrefix(this), 4,false, choices ) ;
     //  qDebug() << toolTipControl->charPrefix(this);
       QList<QKeyValue>::iterator it;
       toolTipControl->clearKeyValueList();
       QKeyValue keyValue;
       for (it = choices.begin(); it != choices.end(); it++) {
            qDebug() << it->value;
          // toolTipControl->addChar( Utilities::getUnicode(*it));
           toolTipControl->addQKeyValue( *it);

           if( Utilities::vowelMap->contains(it->value)) {
               keyValue.key = it->key;
               keyValue.value = Utilities::vowelMap->contains(it->value);
               toolTipControl->addQKeyValue(keyValue);
           }
       }
       toolTipControl->resetIndex();
    //   qDebug() << text;

       QRect q = this->cursorRect();
       int x,y,z,w;
       q.getCoords(&x,&y,&z,&w);
       text=toolTipControl->createToolTipText();
       QToolTip::showText(this->mapToGlobal(QPoint())+ QPoint(x + 20,y-50),  text);
   }

void MdiChild::computeToolTipText() {
    QList<QKeyValue>  choices;
   // charMapTree->get_choice( toolTipControl->charPrefix(this), 4,false, choices ) ;

    charMapTree->get_choice( toolTipControl->charMidPrefix(this), 4,false, choices ) ;

    QList<QKeyValue>::iterator it;
    toolTipControl->clearKeyValueList();
    QKeyValue keyValue;
    for (it = choices.begin(); it != choices.end(); it++) {
        toolTipControl->addQKeyValue( *it);
        if( Utilities::vowelMap->contains(it->value)) {
            keyValue.key = it->key;
            keyValue.value = Utilities::vowelMap->value(it->value);
            toolTipControl->addQKeyValue(keyValue);
         }
    }



    toolTipControl->sortByUsage();
    toolTipControl->resetIndex();
    if( !toolTipControl->isEmptyCharList()) {
      showToolTipText();

    }
    else {
        if( QToolTip::isVisible()) {
            QToolTip::hideText();
        }
    }

}

void MdiChild::showToolTipText() {
    QRect q = this->cursorRect();
    int x,y,z,w;
    q.getCoords(&x,&y,&z,&w);
    QString text=toolTipControl->createToolTipText();
    if(QToolTip::isVisible()) QToolTip::hideText();
    QToolTip::showText(this->mapToGlobal(QPoint())+ QPoint(x + 20,y-50),  text);
}




void MdiChild::showVowelModToolTipText() {
    toolTipControl->createVowelModToolTipText();
    QRect q = this->cursorRect();
    int x,y,z,w;
    q.getCoords(&x,&y,&z,&w);
    QString text=toolTipControl->createVowelModToolTipText();
    if(QToolTip::isVisible()) QToolTip::hideText();
    QToolTip::showText(this->mapToGlobal(QPoint())+ QPoint(x + 20,y-50),  text);
}


void MdiChild::characterToolTipText(QKeyEvent *event) {
    if( event->key() == 124 ) {
       replaceDanda();
       return;
    }

    if(event->key() == Qt::Key_Right && QToolTip::isVisible() ) {
       if(toolTipControl->state == 0 ) {
          toolTipControl->incrementIndex();
          showToolTipText();
       }
       if(toolTipControl->state == 1 ) {
          toolTipControl->incrementCol();
          showVowelModToolTipText();
       }

    }
    else if(event->key() == Qt::Key_Left && QToolTip::isVisible()) {
        if(toolTipControl->state == 0 ) {
           toolTipControl->decrementIndex();
           showToolTipText();
        }
        if(toolTipControl->state == 1 ) {
           toolTipControl->decrementCol();
           showVowelModToolTipText();
        }

    }
    else if(event->key() == Qt::Key_Down && QToolTip::isVisible()) {
        if(toolTipControl->state == 1 ) {
           toolTipControl->incrementRow();
           showVowelModToolTipText();
        }

    }
    else if(event->key() == Qt::Key_Up && QToolTip::isVisible()) {
        if(toolTipControl->state == 1 ) {
            toolTipControl->decrementRow();
            showVowelModToolTipText();
        }
    }
    else if(event->key() == Qt::Key_Return && QToolTip::isVisible()) {
        if( toolTipControl->state == 0 ) {
            insertMidCharSelection( Utilities::getUnicode(toolTipControl->getAtIndex(), "0x")  );
            if(QToolTip::isVisible()) QToolTip::hideText();

            if(_auto_matra_setting && Utilities::consonantMap->contains(toolTipControl->getAtIndex())) {
               toolTipControl->state = 1;
               qDebug() << "Showing matra = " << _auto_matra_setting;
               toolTipControl->resetRowCol();
               showVowelModToolTipText();
            }
         }
         else{
            insertVowelModSelection( Utilities::getUnicode(toolTipControl->getAtRowCol(), "0x"), toolTipControl->consonantPrefix(this)  );
            if(QToolTip::isVisible()) QToolTip::hideText();
            toolTipControl->state = 0;
         }
    }
    else if(event->key() == Qt::Key_Escape) {
        insertCompositeLetter();
        event->ignore();
    }
    else {
       if(event->key() == Qt::Key_Backspace) {
          if(QToolTip::isVisible()) QToolTip::hideText();
       }
       TextEdit::keyPressEvent(event);
       if(event->key() != Qt::Key_Space &&  event->key() != Qt::Key_Backspace && event->key() != Qt::Key_Tab ) {
           computeToolTipText() ;
       }
       toolTipControl->state = 0;
    }


}

void MdiChild::wordToolTipText(QKeyEvent *e) {


    if( e->key() == 124 ) {
       replaceDanda();
       return;
    }
    if (c && c->popup()->isVisible()) {
       switch (e->key()) {
           case Qt::Key_Enter:
           case Qt::Key_Return:
              e->ignore();
              return; // let the completer do default behavior
       default:
           break;
       }
    }
    TextEdit::keyPressEvent(e);
  /*  switch (e->key()) {
        case Qt::Key_Backspace:
           if(c->popup()->isVisible())
              c->popup()->hide();
           e->ignore();
           return; // let the completer do default behavior
    default:
        break;
    }
*/


    QString newWord = Phonetic::phoneticEquivString(toolTipControl->charPrefix(this).toLower());
    QStringList newWordList;
    if( newWord.size() > 0 ) {
        Phonetic::phoneticWordChoices(newWord, newWordList,1);
    }

     c->setModel(new QStringListModel(newWordList, c));


    QString completionPrefix = newWord; //textUnderCursor();
    QRect cr = cursorRect();

    if( e->key()!=Qt::Key_Enter) {
        if(completionPrefix.size() > 0) {
           // qDebug() << "Completion prefix  " << completionPrefix ;
            c->setCompletionPrefix("");
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

   // qDebug() << "Tooltip word " << toolTipControl->charPrefix(this).toLower();

}

void MdiChild::keyPressEvent( QKeyEvent *event )
{
    if( _state == F2 ) {
       characterToolTipText(event);
    }
    else if( _state == F3) {

       // TextEdit::keyPressEvent(event);
        wordToolTipText(event);
    }

}

void MdiChild::replaceDanda() {
    QTextCursor tc = textCursor();
    QString prefixstr;
    prefixstr.append(QString(QChar(0x964)));
    tc.insertText(prefixstr);
}


void MdiChild::insertCompositeLetter()
{
    if (c->widget() != this) {
        qDebug() << "Not the rigth widget";
        return;

    }

    QTextCursor tc = textCursor();
    tc.movePosition(QTextCursor::Left);
    tc.select( QTextCursor::WordUnderCursor );

    QString word =this->textUnderCursor();

    QString prefixstr;
    QChar firstLast, secondLast;

    if( word.size() > 1 ) {
        firstLast = word.at( word.size()- 1);
        secondLast = word.at( word.size()- 2);
     //   qDebug() << "bool" << Utilities::isValidForComposition(firstLast) <<"  " <<  Utilities::isValidForComposition(secondLast);
        if( Utilities::isValidForComposition(firstLast)  &&  Utilities::isValidForComposition(secondLast) ) {
            for(int i = 0;  i < word.size()-1;  ++i) {
                prefixstr.append(word.at(i));
            }
     //       qDebug() << "Can be composited";
            prefixstr.append( QChar(0x9cd));
            prefixstr.append(firstLast);
            tc.insertText(prefixstr);
            return;
        }

        if( Utilities::isValidForComposition(firstLast)\
           &&  Utilities::isDelinkCharacter(secondLast) ) {
            for(int i = 0;  i < word.size()-2;  ++i) {
                prefixstr.append(word.at(i));
            }
            prefixstr.append(firstLast);
            tc.insertText(prefixstr);
       //     qDebug() << "Can be uncomposited";
            return;
        }

    }
    tc.clearSelection();
   // setTextCursor(tc);
}


void MdiChild::updateCompleterModelOnTextChange(){
    QStringList words;


 //   if( _state == F3 ) { c->setModel(new QStringListModel(words, c));return; }

    if( _state== F3 ) {
      // wordToolTipText() ;
       return;
    }



    QList<QString>  choices;
    QStringList stringlist;

    QString completerWord = Utilities::wordUnderCursor(this);
   // qDebug() << "  XXX " << Utilities::getUnicodeString(completerWord);
    if( !Utilities::isValidCompleterPrefix(completerWord)  ||  completerWord.size() < 2) {
        return;
    }

    foreach(QChar str, completerWord) {
        stringlist.append( Utilities::convertToHexString( str.unicode()));
   //     qDebug() << "Code : " << Utilities::convertToHexString( str.unicode()) ;
    }

    wordMapTree->get_choice( stringlist, 1000,false, choices ) ;

    foreach( QString choice, choices) {
       words.append(Utilities::createStringFromCodeList(Utilities::split(choice, "0x")));
    }

    choices.clear();
    WordsTrie *profileWords = WordsTrie::getProfileWordsTrie();
      //QList<QString> keys1 = dictionaryWords->charTree->links.keys();
    profileWords->get_choice(stringlist,20, false,choices);
     // QList<QString> keys2 = dictionaryWords->charTree->links.keys();

    foreach( QString choice, choices) {
       words.append(Utilities::createStringFromCodeList(Utilities::split(choice, "0x")));
    }

  //  QList<QString> keys = dictionaryWords->charTree->links.keys();

    c->setModel(new QStringListModel(words, c));
  //  qDebug() << "Completer model update " << words.size() ;
}



