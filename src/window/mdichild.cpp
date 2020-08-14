
#include <QtGui>
#include <QGraphicsWidget>
#include <QListView>
#include <QMainWindow>
#include <QPlainTextEdit>
#include "src/editor/qtextedittooltip.h"


#include "src/window/mdichild.h"
#include "src/core/constants.h"
#include "src/profile/profile.h"
#include "src/core/inputdialog.h"
#include "src/dictionary/dictionary.h"

extern bool _auto_matra_setting ;
//extern int validSoftware;

void MdiChild::addContextMenu(QMenu *childMenu) {
    menu= childMenu;
}

void MdiChild::contextMenuEvent(QContextMenuEvent *event) {
    //QMenu *menu = new QMenu();
    try {
        QStringList candidateWordsList; //,
        QHash<QString, QString> candidateWordInflexPairs;

        QStringList suggestedWordList;
        QStringList newWordsList;
        QStringList oldWordsList;

        WordsTrie *profileWords = WordsTrie::getProfileWordsTrie();
        WordsTrie *dictionaryWords = WordsTrie::getWordsTrie();

        QString newWord;
        newWord= textUnderCursor();

        candidateWordsList = Phonetic::getInflectionalFormsX(newWord);
        bool ignoreReplaceTest = false;
        foreach( QString str, candidateWordsList) {
            if(!dictionaryWords->hasWord(str)) {
               if( profileWords->hasWord(str)) {
                 oldWordsList.append(str);
                 ignoreReplaceTest = true;
               }
               else {
                 newWordsList.append(str);
               }
            }
            else {
                ignoreReplaceTest = true;
            }
        }

        candidateWordInflexPairs = Phonetic::getInflectionalFormsPairs(newWord);
        suggestedWordList = getSpellingSuggestions(candidateWordInflexPairs,10);


        // if for the first time
        if( rightClickCount==0 ) {
          //  menu = TextEdit::createStandardContextMenu();
            resizeImage=new QAction(tr("Resize Image"),this);
            connect(resizeImage, SIGNAL(triggered()), this, SLOT(resizeImageSize()));
            menu->addAction(resizeImage);

            pasteFromAscii=new QAction(tr("Paste from Ascii"),this);
            connect(pasteFromAscii, SIGNAL(triggered()), this, SLOT(pasteFromAsciiFn()));
            menu->addAction(pasteFromAscii);


            copyForAscii=new QAction(tr("Copy for Ascii"),this);
            connect(copyForAscii, SIGNAL(triggered()), this, SLOT(copyForAsciiFn()));
            menu->addAction(copyForAscii);

            addWord = menu->addMenu("Add to Dictionary");
            addsignalMapper = new QSignalMapper(this);
            QFont contextMenuFont = QFont("kalpurush",14,0,false);

            for(int j =0 ; j < 10; j++) {
               newWords[j] = new QAction(tr("New"), this);
               newWords[j]->setFont(contextMenuFont);
               newWords[j]->setStatusTip(tr("Add a new word"));
               addWord->addAction(newWords[j]);
               addsignalMapper->setMapping(newWords[j],j);
            }
            for(int j =0 ; j < 10; j++) {
                connect(newWords[j], SIGNAL(triggered()), addsignalMapper, SLOT (map()));
            }
            connect(addsignalMapper, SIGNAL(mapped(int)), this, SLOT(addNewWord( int )));

            deleteWord = menu->addMenu("Delete from Dictionary");

            deletesignalMapper = new QSignalMapper(this);

            for(int j =0 ; j < 10; j++) {
               oldWords[j] = new QAction(tr("New"), this);
               oldWords[j]->setFont(contextMenuFont);
               oldWords[j]->setStatusTip(tr("Add a new word"));
               deleteWord->addAction(oldWords[j]);
               deletesignalMapper->setMapping(oldWords[j],j);
            }

            for(int j =0 ; j < 10; j++) {
                connect(oldWords[j], SIGNAL(triggered()), deletesignalMapper, SLOT (map()));
            }
            connect(deletesignalMapper, SIGNAL(mapped(int)), this, SLOT(deleteOldWord( int )));

            menu->addSeparator();

            meaning = menu->addMenu("Meanings");
            meaningText = new QAction(tr(""), this);
            meaning->setToolTip(tr("<p></p>") );
            meaningText->setFont(contextMenuFont);
            meaning->addAction(meaningText);
            meaning->setFont(contextMenuFont);
            menu->addMenu(meaning);

            synonyms = menu->addMenu("Synonyms");
            synonymsText = new QAction(tr(""), this);
            synonyms->setToolTip(tr("<p></p>") );
            synonymsText->setFont(contextMenuFont);
            synonyms->addAction(synonymsText);
            synonyms->setFont(contextMenuFont);
            menu->addMenu(synonyms);

            /*
            examples = menu->addMenu("Examples");
            examplesText = new QAction(tr(""), this);
            examples->setToolTip(tr("<p></p>") );
            examplesText->setFont(contextMenuFont);
            examples->addAction(examplesText);
            examples->setFont(contextMenuFont);
            menu->addMenu(examples);
            */

            examples=new QAction(tr("Examples"),this);
            connect(examples, SIGNAL(triggered()), this, SLOT(showExamples()));
            menu->addAction(examples);


            officialwords = menu->addMenu("Official Words");
            officialwordsText = new QAction(tr(""), this);
            officialwords->setToolTip(tr("<p></p>") );
            officialwordsText->setFont(contextMenuFont);
            officialwords->addAction(officialwordsText);
            officialwords->setFont(contextMenuFont);
            menu->addMenu(officialwords);

           // idioms = menu->addMenu("Idioms");
           // idiomsText = new QAction(tr(""), this);
           // idioms->setToolTip(tr("<p></p>") );
           // idiomsText->setFont(contextMenuFont);
           // idioms->addAction(idiomsText);
            //idioms->setFont(contextMenuFont);
           // menu->addMenu(idioms);


            idioms=new QAction(tr("Idioms"),this);
            connect(idioms, SIGNAL(triggered()), this, SLOT(showIdioms()));
            menu->addAction(idioms);

            menu->addSeparator();
            ignoreWord=new QAction(tr("Ignore"),this);
            connect(ignoreWord, SIGNAL(triggered()), this, SLOT(Ignore()));
            menu->addAction(ignoreWord);

            replaceWord = menu->addMenu("Replace...");
            signalMapperReplaceWords = new QSignalMapper(this);
            for(int j =0 ; j < 10; j++) {
               replaceWords[j] = new QAction(tr("New"), this);
               replaceWords[j]->setFont(contextMenuFont);
               replaceWords[j]->setStatusTip(tr("Add a new word"));
               replaceWord->addAction(replaceWords[j]);
               signalMapperReplaceWords->setMapping(replaceWords[j],j);
            }
            for(int j =0 ; j < 10; j++) {
                connect(replaceWords[j], SIGNAL(triggered()), signalMapperReplaceWords, SLOT (map()));
            }
            connect(signalMapperReplaceWords, SIGNAL(mapped(int)), this, SLOT(replaceNewWord( int )));
        }

        int newWordListSize = newWordsList.size();

        for(int j =0 ; j < 10; j++) {

           if( j <  newWordListSize ) {
               newWords[j]->setVisible(true);
               newWords[j]->setText(newWordsList.at(j));
           }
           else {
               newWords[j]->setVisible(false);
           }
        }
       // if( ignoreReplaceTest==false ) { ignoreWord->setVisible(false); replaceWord->setVisible(false); }

        int oldWordListSize = oldWordsList.size();

        for(int j =0 ; j < 10; j++) {
           if( j <  oldWordListSize) {
               oldWords[j]->setVisible(true);
               oldWords[j]->setText(oldWordsList.at(j));
           }
           else {
               oldWords[j]->setVisible(false);
           }
        }

        // add the possible correct spellings
        for(int j =0 ; j < 10; j++) {
           if( j <  suggestedWordList.size() ) {
               replaceWords[j]->setVisible(true);
               replaceWords[j]->setText(suggestedWordList.at(j));
           }
           else {
               replaceWords[j]->setVisible(false);
           }
        }

        getWordInfo();
        lastPosition = event->pos();

        rightClickCount++;
        menu->exec(event->globalPos());
    }
    catch(...) {

        Utilities::warningBox("Exception handled!");

    }
}


void MdiChild::showIdioms() {
    QTextEditToolTip *tooltip = QTextEditToolTip::getQTextEditToolTip();
    tooltip->showTextEditToolTip(this->idioms->toolTip(), this->mapToGlobal(this->pos()) );
}

void MdiChild::showExamples() {
    QTextEditToolTip *tooltip = QTextEditToolTip::getQTextEditToolTip();
    tooltip->showTextEditToolTip(this->examples->toolTip(), this->mapToGlobal(this->pos()) );
}


QStringList MdiChild::getSpellingSuggestions(QHash<QString, QString> &candidateWordsInflectionPairs, unsigned int nMax) {
    QStringList suggestionList;
    WordsTrie *dictionaryWords = WordsTrie::getWordsTrie();
    QList<QKeyFloat> possibleList;
    QHash<QString, QString>::const_iterator it;

    for( it=candidateWordsInflectionPairs.constBegin(); it!= candidateWordsInflectionPairs.constEnd(); ++it) {
        dictionaryWords->getPossibleWords(it.key(), possibleList, 10);
    }
    Utilities::sortQKeyFloatList(possibleList, true);

    QHash<QString, bool>  uniqueList;



    for(unsigned int i=0, j=0; j < 10 && i < possibleList.size(); i++ ) {
        if( uniqueList.contains( possibleList.at(i).key )) continue;
        uniqueList.insert( possibleList.at(i).key, true ); j++;

        suggestionList.append(possibleList.at(i).key + candidateWordsInflectionPairs.value(possibleList.at(i).key));
    }

   // qDebug() <<"Spelling choices " << suggestionList.size();

    return suggestionList;
}



MdiChild::MdiChild()
{
    setAttribute(Qt::WA_DeleteOnClose);
    isUntitled = true;
    autocompleter  = NULL;
    disable = false;
    //this->setSizeIncrement(200,150);
    //this->setLineWidth(300);
    //this->setFixedSize(200,150);

 //   this->setLineWrapMode(QTextOption::WordWrap);
   /*
    this->setWordWrapMode(QTextOption::WordWrap);
    this->setLineWrapColumnOrWidth (10);
    this->setStyleSheet("margin: 20px");
   */
   this->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);

    Profile *profile = Profile::getkeyBoard();
    profile->fill_keyboard("DEFAULT");

    charMapTree = CharTrie::getCharTrie();

    wordMapTree = WordsTrie::getWordsTrie();

    highlighter = new Highlighter(this->document(),true);
    highlighter->setIgnoreWords( &(this->ignoreDictionary) );

//	connect(editor,SIGNAL(addWord(QString)),highlighter,SLOT(slot_addWord(QString)));
    connect(this, SIGNAL(textChanged()), this, SLOT(updateCompleterModelOnTextChange()));

}
/*
void MdiChild::copy() {
    qDebug() << "Being copied";

}*/



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

    if( fileInfo.suffix()== QString("pad") ) {
        //setHtml(in.readAll());
        readAsLuitPadFormat(in);
    }
    else
        setPlainText(in.readAll());

    QApplication::restoreOverrideCursor();

    setCurrentFile(fileName);

    connect(document(), SIGNAL(contentsChanged()),this, SLOT(documentWasModified()));

    return true;
}


void MdiChild::readAsLuitPadFormat( QTextStream &in){
    QString imageFileName;

    QHash<QString, QString> imageDataHash;
    bool imageSeen = false;

   // SimpleCrypt *crypto = SimpleCrypt::getEncryptor(Q_UINT64_C(0x23bacf02473fdea0));

    QString input = in.readAll();
   // input =  crypto->decryptToString(input);
    if( input.isEmpty() ) Utilities::warningBox("Corrupt .pad file!");

    //qDebug() << input;

    QString str;
    QStringList inputLines = input.split("\n");

    foreach(QString str, inputLines) {
        str = str + "\n";
        if(Utilities::IsTag(str, "LuitPad_image") && Utilities::getAttribute(str,"name").length()> 0) {
            imageFileName =  Utilities::getAttribute(str,"name") ;
            imageSeen = true;
        }
        if( imageSeen && str.length() > 1000 ) {
            imageDataHash.insert(imageFileName, str.trimmed());
        }
    }
    in.seek(0);

    QString htmlCode;
    foreach(QString str, inputLines) {
        str = str + "\n";
        if(Utilities::IsTag(str, "LuitPad_images")) {
            break;
        }
        htmlCode.append(str);
    }


    QHash<QString, QString>::const_iterator it;

    QImage image;
    QUrl url;
    for(it = imageDataHash.begin(); it != imageDataHash.end(); ++it) {
        image.loadFromData( QByteArray::fromBase64(it.value().toLatin1()), "PNG");
//            image.loadFromData( QByteArray::fromBase64(it.value().toAscii()), "PNG");
        url.setUrl(it.key());
        document()->addResource(QTextDocument::ImageResource, url, image);
    }

   // htmlCode.replace( QRegExp("image_2"), "C:\\Documents and Settings\\kishori\\Desktop\\test.PNG");
  //  qDebug() << htmlCode;
    bool start_ignore_list = false;
    foreach(QString str, inputLines) {
        if( start_ignore_list &&   !Utilities::IsTag(str, "/ignore_list")) {
          //  qDebug() << str <<"  "  << Utilities::getUnicodeString(str);
            if( str.size() > 1) ignoreDictionary.insert(str, true);
        }
        if(Utilities::IsTag(str, "ignore_list")) {
            start_ignore_list = true;
        }

    }

    setHtml(htmlCode);
}



bool MdiChild::save()
{
    if (isUntitled) {
        return saveAs();
    } else {
       // return saveFile(curFile);
        if( curFile.endsWith(".pad", Qt::CaseInsensitive)) {
           return saveFile(curFile);
        }
        else {
           return saveFileHtml(curFile);
        }
    }
}

bool MdiChild::saveAs()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"),curFile,".pad");
    if (fileName.isEmpty())
        return false;
    //return saveFileHtml(fileName);
    return saveFile(fileName);
}

bool MdiChild::saveAsHtml()
{

    QString truncatedCurFile = curFile;
    truncatedCurFile.chop(truncatedCurFile.lastIndexOf(QRegExp("pad")));
//    qDebug() <<"Current file = " <<  truncatedCurFile;
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save As Html"),truncatedCurFile, ".html");
    if (fileName.isEmpty())
        return false;

    QFile file(fileName);
    if( file.exists() ) file.remove(file.fileName());
    if( !file.open(file.ReadWrite) ) return false;
    file.write("\xEF\xBB\xBF");
    file.setTextModeEnabled(true);

    QTextStream out(&file);
  //  QApplication::setOverrideCursor(Qt::WaitCursor);
    out.setCodec(QTextCodec::codecForName("UTF-8"));
    out << toHtml();
   // qDebug() << toHtml();
    file.close();


    /*QApplication::restoreOverrideCursor();

    setCurrentFile(fileName);

    return saveFileHtml(fileName);*/

    return true;
}


bool  MdiChild::exportOdf()
{
    QString truncatedCurFile = curFile;
    truncatedCurFile.chop(truncatedCurFile.lastIndexOf(QRegExp("pad")));
    //qDebug() <<"Current file = " <<  truncatedCurFile;
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save As Open Document Format"),truncatedCurFile, ".odt");
    if (fileName.isEmpty())
        return false;

     Q_ASSERT(fileName.endsWith(".odt"));
     QTextDocumentWriter writer(fileName);
     return writer.write(this->document());
}


bool MdiChild::saveAsPDF()
{
    QString truncatedCurFile = curFile;
    truncatedCurFile.chop(truncatedCurFile.lastIndexOf(QRegExp("pad")));
//    qDebug() <<"Current file = " <<  truncatedCurFile;
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save As Html"),truncatedCurFile, ".pdf");
    if (fileName.isEmpty())
        return false;

    Q_ASSERT(fileName.endsWith(".ps") || fileName.endsWith(".pdf"));
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFileName(fileName);

    highlighter->setDocument(0);
    printer.setOutputFormat(fileName.endsWith(".pdf") ? QPrinter::PdfFormat : QPrinter::PdfFormat);
// printer.setOutputFormat(fileName.endsWith(".pdf") ? QPrinter::PdfFormat : QPrinter::PostScriptFormat);
    this->document()->print(&printer);
    highlighter->setDocument(this->document());   
    // test end

    return true;
   // return saveFilePDF(fileName);
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
   // out << toPlainText();
  //  SimpleCrypt *crypto = SimpleCrypt::getEncryptor(Q_UINT64_C(0x23bacf02473fdea0));
   // out <<  Utilities::toLuitPadFormat(document(), toHtml());
    QString ignoreWordString;
    ignoreWordString = getIgnoreWordsString();
  //  qDebug() << "ignore \n" << ignoreWordString;
    out<<   Utilities::toLuitPadFormat(document(), toHtml(),ignoreWordString);

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

void printPage(int index, QPainter *painter, const QTextDocument *doc, const QRectF &body, const QPointF &pageNumberPos)
{
    painter->save();
    painter->translate(body.left(), body.top() - (index - 1) * body.height());
    QRectF view(0, (index - 1) * body.height(), body.width(), body.height());

    QAbstractTextDocumentLayout *layout = doc->documentLayout();
    QAbstractTextDocumentLayout::PaintContext ctx;

    painter->setClipRect(view);
    ctx.clip = view;

    // don't use the system palette text as default text color, on HP/UX
    // for example that's white, and white text on white paper doesn't
    // look that nice
    ctx.palette.setColor(QPalette::Text, Qt::black);

    layout->draw(painter, ctx);

    if (!pageNumberPos.isNull()) {
        painter->setClipping(false);
        painter->setFont(QFont(doc->defaultFont()));
        const QString pageString = QString::number(index);

        painter->drawText(
                          qRound(pageNumberPos.x() - painter->fontMetrics().width(pageString)),
                          qRound(pageNumberPos.y() + view.top()),
                          pageString
                         );
    }

    painter->restore();
}
bool MdiChild::saveFilePDF(const QString &fileName)
{
    QString filename = fileName;
    if(QFileInfo(filename).suffix().isEmpty()) {
            filename.append(".pdf");
    }


    QPrinter printer; QPainter painter;
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setPaperSize(QPrinter::Letter); //set paper size
    printer.setFullPage(false); //coordinates based on printable area
    printer.setPageMargins( 0.5, 0.5, 0.5, 0.5, QPrinter::Inch);
   // printer.setResolution(72); //72 is standard
  //  printer.setPageOrder();

    printer.setOutputFileName(filename);
    if (! painter.begin(&printer)) { // failed to open file
        qWarning("failed to open file, is it writable?");
         return false;
    }

    painter.setPen( Qt::black );
    QRect rect = QRect(10, 20, 700, 815*0.90);
   // QFont font = QApplication::font();
    QFont font = this->font();
    painter.setFont(font);
    font.setPixelSize( rect.height());
    rect.setHeight( QFontMetrics( font ).height() );
    QTextDocument doc;

    QString rawHtml = this->toHtml();

    QString imageInlinedHtml = Utilities::toPdfConvertibleLuitPadFormat(this->document(), rawHtml);

    doc.setHtml(imageInlinedHtml);

    doc.setPageSize(QSizeF(rect.width(), rect.height() ));
 //   qDebug() << "  Pages needed = " << doc.pageCount();

    QPointF q(rect.width(), rect.height());
 // QRect questionView( 0, 0, printer.pageRect().width(), printer.pageRect().height());
    for (int i = 1; i <= doc.pageCount(); i++){
       QRect questionView( 10, 20, printer.pageRect().width(), printer.pageRect().height()*0.92);
       printPage(i, &painter, &doc, questionView, q );
       if( i < doc.pageCount() )  printer.newPage();
    }

    painter.end();

    return true;
}





void MdiChild::testPrinting() {


    QPrinter printer;QPainter painter;
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName("testingpdf.pdf");
    if (! painter.begin(&printer)) { // failed to open file
        qWarning("failed to open file, is it writable?");
         return ;
    }

      painter.setPen( Qt::black );

      QPoint point = QPoint( 10, 20 );

      QRect rect = QRect(10, 20, 700, 800);


      QFont font = QApplication::font();

      QFont printFont = QFont("kalpurush",14,0,false);
      printFont.setPixelSize( rect.height() );
      painter.setFont(font);

      font.setPixelSize( rect.height() );



    //  rect.translate( 0, rect.height()+10 );
      rect.setHeight( QFontMetrics( font ).height() );


   QTextDocument doc;



  doc.setHtml( this->toHtml() );
  doc.setPageSize(QSizeF(rect.width(), rect.height() ));
  qDebug() << "  Pages needed = " << doc.pageCount();
  //doc.setTextWidth( rect.width() );
 // painter.save();
 // painter.translate( rect.topLeft() );
  /*QPointF pageNumberPos = QPointF(body.width() - margin,
                          body.height() - margin
                          + QFontMetrics(doc->defaultFont(), p.device()).ascent()
                          + 5 * dpiy / 72.0);
  */
  QPointF q(rect.width(), rect.height());

  for (int i = 1; i <= doc.pageCount(); i++){
      QRect questionView( 0,
                             0,
                             printer.pageRect().width(),
                             printer.pageRect().height());
      printPage(i, &painter, &doc, questionView, q );
      if( i < doc.pageCount() )  printer.newPage();

  }
//  doc.drawContents( &painter, rect.translated(-rect.topLeft() ) );

  //doc.drawContents( &painter );


 // painter.restore();
  //rect.translate(-10,0);
 // painter.drawRect( rect );

  //int maxHeight = painter->window().height();
 // unsigned int SmallGap = 0;
  //QRect textRect = painter.boundingRect(SmallGap, SmallGap, textWidth, maxHeight,Qt::TextWordWrap, str);


 // rect.translate( 0, 160 );
 // rect.setHeight( doc.size().height()-160 );
 // painter.setBrush( Qt::gray );
  //painter.drawRect( rect );

  //pixmap.save( "text.pdf" );

  }


/* SAVE it
void MdiChild::testPrinting() {


    QPrinter printer;QPainter painter;
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName("testingpdf.pdf");
    if (! painter.begin(&printer)) { // failed to open file
        qWarning("failed to open file, is it writable?");
         return ;
    }

       QPixmap pixmap( 200, 330 );
      pixmap.fill( Qt::white );

     // QPainter painter( &pixmap );
      painter.setPen( Qt::black );

      QPoint point = QPoint( 10, 20 );
      painter.drawText( point, "You can draw text from a point..." );
      painter.drawLine( point+QPoint(-5, 0), point+QPoint(5, 0) );
      painter.drawLine( point+QPoint(0, -5), point+QPoint(0, 5) );

      QRect rect = QRect(10, 30, 180, 20);
      painter.drawText( rect, Qt::AlignCenter,
                        "...or you can draw it inside a rectangle." );
      painter.drawRect( rect );

      rect.translate( 0, 30 );

      QFont font = QApplication::font();
      font.setPixelSize( rect.height() );
      painter.setFont( font );

      painter.drawText( rect, Qt::AlignRight, "Right." );
      painter.drawText( rect, Qt::AlignLeft, "Left." );
      painter.drawRect( rect );

      rect.translate( 0, rect.height()+10 );
      rect.setHeight( QFontMetrics( font ).height() );

      painter.drawText( rect, Qt::AlignRight, "Right." );
      painter.drawText( rect, Qt::AlignLeft, "Left." );
      painter.drawRect( rect );

QTextDocument doc;
  doc.setHtml( "<p>A QTextDocument can be used to present formatted text "
               "in a nice way.</p>"
               "<p align=center>It can be <b>formatted</b> "
               "<font size=+2>in</font> <i>different</i> ways.</p>"
               "<p>The text can be really long and contain many "
               "paragraphs. It is properly wrapped and such...</p>" );



  rect.translate( 0, rect.height()+10 );
  rect.setHeight( 160 );
  doc.setTextWidth( rect.width() );
  painter.save();
  painter.translate( rect.topLeft() );
  doc.drawContents( &painter, rect.translated( -rect.topLeft() ) );
  painter.restore();
  painter.drawRect( rect );

  rect.translate( 0, 160 );
  rect.setHeight( doc.size().height()-160 );
  painter.setBrush( Qt::gray );
  painter.drawRect( rect );

  //pixmap.save( "text.pdf" );

  }

*/


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

    if (autocompleter) QObject::disconnect(autocompleter, 0, this, 0);

    autocompleter = compltr;

    if (! autocompleter ) return;

    autocompleter->setWidget(this);
    autocompleter->setCompletionMode(QCompleter::PopupCompletion);
    autocompleter->setCaseSensitivity(Qt::CaseInsensitive);
    QObject::connect(autocompleter, SIGNAL(activated(QString)), this, SLOT(insertCompletion(QString)));

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
           // qDebug() << "Resize event";

            return false;
        }
        return QObject::eventFilter(obj, event);
    }
 }


CONFIG_STATES MdiChild::getConfigState() {

    if(_auto_matra_setting == true && _single_character_mode_setting == false && _hide_tooltip_mode_setting == false) return C1;

    if(_auto_matra_setting == false && _single_character_mode_setting == false && _hide_tooltip_mode_setting == false) return C2;

    if(_auto_matra_setting == false && _single_character_mode_setting == true && _hide_tooltip_mode_setting == false) return C3;

    if(_auto_matra_setting == false && _single_character_mode_setting == true && _hide_tooltip_mode_setting == true) return C4;

    return C0;
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
           qDebug() << "hide-2";
          QToolTip::hideText();
       }


       QList<QKeyValue>  choices;
       charMapTree->get_choice( toolTipControl->charPrefix(this), 4,false, choices ) ;
     //  qDebug() << toolTipControl->charPrefix(this);
       QList<QKeyValue>::iterator it;
       toolTipControl->clearKeyValueList();
       QKeyValue keyValue;
       for (it = choices.begin(); it != choices.end(); it++) {
           // qDebug() << it->value;
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

       if( getConfigState()!=C4)
          QToolTip::showText(this->mapToGlobal(QPoint())+ QPoint(x + 20,y-50),  text);
   }

void MdiChild::computeToolTipTextOnSpace() {
    //qDebug() << "text before space " << toolTipControl->charMidPrefix(this);
    if( (getConfigState()==C3 || getConfigState()==C4) ) {
         QString match="";
         QString prefix = toolTipControl->charMidPrefix(this);
         QString whatPrefix;
         bool foundMatch = false;
         if( charMapTree->doesPrefixMatch(prefix) ) {
              whatPrefix = charMapTree->whatPrefixMatch(prefix);
              match= charMapTree->getPrefixMatch(prefix);
              //qDebug() << " prefix " << prefix << " match " << whatPrefix;
              prefix= prefix.remove(QRegExp(QString("^") + whatPrefix));
            //  qDebug() << "Trimmed prefix " << prefix;
              foundMatch = true;
         }

         if(prefix.size()==0) prefix=" ";
         if(foundMatch)  insertMidCharSelection( Utilities::getUnicode(match, "0x") + prefix );
    }
    if( QToolTip::isVisible()) {
        qDebug() << "hide-7";
        QToolTip::hideText();
    }

}


///////////
/// \brief MdiChild::computeToolTipText
/* original
void MdiChild::computeToolTipText() {
    QList<QKeyValue>  choices;

    charMapTree->get_choice( toolTipControl->charMidPrefix(this), 4,false, choices ) ;


    QList<QKeyValue>::iterator it;
    toolTipControl->clearKeyValueList();
    QKeyValue keyValue;
    for (it = choices.begin(); it != choices.end(); it++) {
        toolTipControl->addQKeyValue( *it);
        if( Utilities::inverseVowelMap->contains(it->value) && (getConfigState()==C1 || getConfigState()==C2) ) {
            keyValue.key = it->key;
            keyValue.value = Utilities::inverseVowelMap->value(it->value);
            toolTipControl->addQKeyValue(keyValue);
        }
    }

    toolTipControl->sortByUsage();
    toolTipControl->resetIndex();
    if( !toolTipControl->isEmptyCharList()) {
        if( toolTipControl->getSize()==1 && (getConfigState()==C3 || getConfigState()==C4) ){
         //   qDebug() << " tooltip control";
           if( QToolTip::isVisible() ) QToolTip::hideText();
           insertMidCharSelection( Utilities::getUnicode(toolTipControl->getAtIndex(), "0x")  );
           if(getConfigState()==C1 && Utilities::consonantMap->contains(toolTipControl->getAtIndex())) {
              toolTipControl->state = 1;
              toolTipControl->resetRowCol();
              showVowelModToolTipText();
           }
        }
        else {
           showToolTipText();
        }
    }
    else {
        //qDebug()<< "Empty list " << toolTipControl->charMidPrefix(this);
        if( (getConfigState()==C3 || getConfigState()==C4) ) {
            QString match="";
            QString prefix = toolTipControl->charMidPrefix(this);
            QString whatPrefix;
            bool foundMatch = false;
            if( charMapTree->doesPrefixMatch(prefix) ) {
                whatPrefix = charMapTree->whatPrefixMatch(prefix);
                match= charMapTree->getPrefixMatch(prefix);
            //    qDebug() << " prefix " << prefix << " match " << whatPrefix;
                prefix= prefix.remove(QRegExp(QString("^") + whatPrefix));
          //      qDebug() << "Trimmed prefix " << prefix;
                foundMatch = true;
            }


            charMapTree->get_choice(prefix, 4,false, choices ) ;
            if( charMapTree->doesPrefixMatch(prefix) && choices.size()==1 ) {
                match.append(charMapTree->getPrefixMatch(prefix));
                whatPrefix = charMapTree->whatPrefixMatch(prefix);
                prefix= prefix.remove(QRegExp(QString("^") + whatPrefix));
              //  qDebug()<< "matched more ";
            }

            if(foundMatch)  insertMidCharSelection( Utilities::getUnicode(match, "0x") + prefix );
        }
        if( QToolTip::isVisible()) {
            QToolTip::hideText();
        }
    }
}
*/

//new
void MdiChild::computeToolTipText() {
    QList<QKeyValue>  choices;
    charMapTree->get_choice( toolTipControl->charMidPrefix(this), 4,false, choices ) ;


    QList<QKeyValue>::iterator it;
    toolTipControl->clearKeyValueList();
    QKeyValue keyValue;
    for (it = choices.begin(); it != choices.end(); it++) {
        toolTipControl->addQKeyValue( *it);
        if( Utilities::inverseVowelMap->contains(it->value) ) {
            keyValue.key = it->key;
            keyValue.value = Utilities::inverseVowelMap->value(it->value);
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

    QToolTip::setFont(QFont("Kalpurush",  this->textCursor().charFormat().fontPointSize(), 0, false));

    QToolTip::showText(this->mapToGlobal(QPoint())+ QPoint(x + 20,y-50),  text);

}


void MdiChild::showVowelModToolTipText() {
    toolTipControl->createVowelModToolTipText();
    QRect q = this->cursorRect();

    QPoint t = this->viewport()->mapToGlobal(QPoint(TextEdit::cursorRect().x(),TextEdit::cursorRect().y()));
    int x,y,z,w;
    q.getCoords(&x,&y,&z,&w);
 //   qDebug() << q.x()+20 << " " << t.x() <<"  "  <<  q.x() + 100 << "||" << q.y()-50 << "  " << t.y() << "  " << q.y() + 200;
  //  if( x +20 < t.x() && t.x() < x + 110 && y-50 < t.y() && t.y() < y + 200 ) {
    //   qDebug() << q.x() << " --- "  << q.y() << "  " << t.x() << "  " << t.y();
       QCursor::setPos(t.x() +15,t.y() + 200);
   // }

    QString text=toolTipControl->createVowelModToolTipText();
    if(QToolTip::isVisible()) {  qDebug() << "hide-4"; QToolTip::hideText(); }
    QToolTip::showText(this->mapToGlobal(QPoint())+ QPoint(x + 20,y-50),  text);
}

/* Original
void MdiChild::characterToolTipText(QKeyEvent *event) {
    if( event->key() == 124 ) {
       replaceDanda();
       return;
    }
    if(event->key() == Qt::Key_Right && QToolTip::isVisible() ) {
          toolTipControl->incrementIndex();
          showToolTipText();
    }
    else if(event->key() == Qt::Key_Left && QToolTip::isVisible()) {
           toolTipControl->decrementIndex();
           showToolTipText();

    }
    else if( event->key() ==  Qt::Key_Return  && QToolTip::isVisible()  ) {
         insertMidCharSelection( Utilities::getUnicode(toolTipControl->getAtIndex(), "0x")  );
      //   qDebug() << "Fecthing" << toolTipControl->getAtRowCol() << " " <<  toolTipControl->consonantPrefix(this) ;
         if(QToolTip::isVisible()) {         qDebug() << "hide-3";  QToolTip::hideText(); }

    }
    else if(event->key() == Qt::Key_Escape) {
        insertCompositeLetter();
        event->ignore();
    }
    else {
    //   qDebug() << "This is the " << Qt::Key_Return <<"   " << event->key();
       if(event->key() == Qt::Key_Backspace) {
          if(QToolTip::isVisible()) { qDebug() << "hide-5"; QToolTip::hideText(); }
       }
       TextEdit::keyPressEvent(event);
       if( event->key() != Qt::Key_Space &&  event->key() != Qt::Key_Backspace && event->key() != Qt::Key_Tab ) {
           computeToolTipText() ;
           qDebug() << "Displaying tool tip";
       }
    }
}

*/

void MdiChild::expandListonPrefix() {

    QString newWord;

    newWord= toolTipControl->getAssamesePrefix(this);

    if( !toolTipControl->hasAssamesePrefix(this)) return;

    WordsTrie * wordstrie = WordsTrie::getWordsTrie();


    QStringList prefix;
    foreach(QChar qchar, newWord) {
        prefix.append(  Utilities::convertToHexString(qchar.unicode()).toLower() );
    }
    QList<QString> choices;
    wordstrie->get_choice(prefix, 5, true,  choices );

    QStringList newWordList;
    foreach(QString word, choices) {
        newWordList.append(Utilities::getUnicode(word, "0x"));
    }
    autocompleter->setModel(new QStringListModel(newWordList,  autocompleter));

    QRect cr = cursorRect();
    if(newWord.size() > 0) {
        autocompleter->setCompletionPrefix(newWord);
        autocompleter->popup()->setCurrentIndex( autocompleter->completionModel()->index(0, 0));
    }
    cr.setWidth( autocompleter->popup()->sizeHintForColumn(0) +  autocompleter->popup()->verticalScrollBar()->sizeHint().width());
    QPoint q = this->viewport()->mapToGlobal(QPoint(TextEdit::cursorRect().x(),TextEdit::cursorRect().y()));
    QCursor::setPos(q.x() - 2,q.y());
    autocompleter->complete(cr); // popup it up!
}

void MdiChild::characterToolTipText(QKeyEvent *event) {
    if( event->key() == 124 ) {
       replaceDanda();
       return;
    }
    toolTipControl->setState(F2);


    if(event->key() == Qt::Key_Escape) {
        insertCompositeLetter();
        event->ignore();
    }
    else {
        TextEdit::keyPressEvent(event);

        QList<QKeyValue>  choices;
        charMapTree->get_choice( toolTipControl->charPrefix(this), 4,false, choices );

        std::reverse(choices.begin(), choices.end());
        toolTipControl->clearKeyValueList();

        toolTipControl->setSelectCharacters(choices);

        QStringList newWordList = toolTipControl->createQcompleterList();

        autocompleter->setModel(new QStringListModel(newWordList,  autocompleter));


        QRect cr = cursorRect();

        if( toolTipControl->charPrefix(this).toLower().size() > 0) {
            autocompleter->setCompletionPrefix("");
            autocompleter->popup()->setCurrentIndex( autocompleter->completionModel()->index(0, 0));
        }
        cr.setWidth( autocompleter->popup()->sizeHintForColumn(0) +  autocompleter->popup()->verticalScrollBar()->sizeHint().width());
        QPoint q = this->viewport()->mapToGlobal(QPoint(TextEdit::cursorRect().x(), TextEdit::cursorRect().y()));
        QCursor::setPos(q.x() - 2, q.y());
        autocompleter->complete(cr); // popup it up
    }
}

void MdiChild::wordToolTipText(QKeyEvent *e) {
    if( e->key() == 124 ) {
       replaceDanda();
       return;
    }
    toolTipControl->setState(F3);

    TextEdit::keyPressEvent(e);
    QString newWord;

    newWord= toolTipControl->charPrefix(this).toLower();
    QStringList newWordList;
    if( newWord.size() > 0 ) {
        if(  !toolTipControl->hasAssamesePrefix(this)) {
           QList<QWordUnicode> pairedWordList;
           Phonetic::phoneticWordChoices(newWord, pairedWordList, false);
         // Phonetic::phoneticWordChoicesLengthBased(newWord, pairedWordList, false);
         // Phonetic::phoneticWordChoices(newWord, pairedWordList,true);
           Phonetic::arrangeWordChoices(pairedWordList,newWordList,Phonetic::processPhoneticInput( Phonetic::phoneticEquivString(newWord)));
        }
        else
           Phonetic::phoneticInflexChoices(newWord, newWordList);
    }

    autocompleter->setModel(new QStringListModel(newWordList, autocompleter));
    QString completionPrefix = newWord; //textUnderCursor();
    QRect cr = cursorRect();

    if(completionPrefix.size() > 0) {
        autocompleter->setCompletionPrefix("");
        autocompleter->popup()->setCurrentIndex(autocompleter->completionModel()->index(0, 0));
    }

    cr.setWidth( autocompleter->popup()->sizeHintForColumn(0) + autocompleter->popup()->verticalScrollBar()->sizeHint().width());
    QPoint q = this->viewport()->mapToGlobal(QPoint(TextEdit::cursorRect().x(),TextEdit::cursorRect().y()));
    QCursor::setPos(q.x() - 2,q.y());
    autocompleter->complete(cr); // popup it up!

    if( e->key() == Qt::Key_Space  || e->key() == Qt::Key_Tab) {
        autocompleter->popup()->hide();
    }
   // qDebug() << "Tooltip word " << toolTipControl->charPrefix(this).toLower();
}

void MdiChild::keyPressEvent( QKeyEvent *event )
{
    if(disable==true) return;

    if (autocompleter && autocompleter->popup()->isVisible()) {
         switch (event->key()) {
           case Qt::Key_Enter:
           case Qt::Key_Return:
                event->ignore();
                return; // let the completer do default behavior
         default:
            autocompleter->popup()->hide();
           break;
         }
     }
    if( _state == F2 ) {

       characterToolTipText(event);

//       wordToolTipText(event);

    }
    else if( _state == F3) {
        wordToolTipText(event);
    }
}

void MdiChild::keyReleaseEvent(QKeyEvent *event)
{

#if defined(Q_OS_MAC64) || defined(Q_OS_MAC) || defined(Q_OS_DARWIN) || defined(Q_OS_DARWIN64)
     if(event->key() == Qt::Key_Meta)
#else
    if(event->key() == Qt::Key_Control)
#endif
    {
        expandListonPrefix();
        ToolTipSingleton *tooltip = ToolTipSingleton::GetToolTipSingleton();
        tooltip->setState(F4);
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
    if (autocompleter->widget() != this) {
        qDebug() << "Not the rigth widget";
        return;

    }

    QTextCursor tc = textCursor();
    tc.movePosition(QTextCursor::Left);
    tc.select( QTextCursor::WordUnderCursor );

    QString word =this->textUnderCursor();

    QString prefixstr;
    QChar firstLast, secondLast;
    if( word.size() == 1) {
          firstLast = word.at( word.size()- 1);
          if( Utilities::isValidForVowelComposition(firstLast) ) {// &&  Utilities::isValidForComposition(secondLast) ) {
             for(int i = 0;  i < word.size()-1;  ++i) {
                prefixstr.append(word.at(i));
             }

             QString strfirstLast = Utilities::getUnicodeString(firstLast);
             if( Utilities::inverseVowelMap->contains(strfirstLast) ) {
                 prefixstr.append( Utilities::getUnicode(Utilities::inverseVowelMap->value(strfirstLast),"0x"));
              }
              else if(Utilities::vowelMap->contains(strfirstLast)) {
                prefixstr.append( Utilities::getUnicode(Utilities::vowelMap->value(strfirstLast),"0x"));
              }

              tc.insertText(prefixstr);
              return;
           }

    }
    else if( word.size() > 1 ) {
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


        if( Utilities::isValidForVowelComposition(firstLast) ) {// &&  Utilities::isValidForComposition(secondLast) ) {
            for(int i = 0;  i < word.size()-1;  ++i) {
                prefixstr.append(word.at(i));
            }

            QString strfirstLast = Utilities::getUnicodeString(firstLast);
          //  qDebug() << strfirstLast;

            if( Utilities::inverseVowelMap->contains(strfirstLast) ) {
              //  qDebug() << strfirstLast << " " << Utilities::inverseVowelMap->value(strfirstLast);
                prefixstr.append( Utilities::getUnicode(Utilities::inverseVowelMap->value(strfirstLast),"0x"));
            }
            else if(Utilities::vowelMap->contains(strfirstLast)) {
                prefixstr.append( Utilities::getUnicode(Utilities::vowelMap->value(strfirstLast),"0x"));
              //  qDebug() << strfirstLast <<  "  x";
            }

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
    }else {
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

   autocompleter->setModel(new QStringListModel(words, autocompleter));
  //  qDebug() << "Completer model update " << words.size() ;
}


void MdiChild::resizeImageSize()
{
 //   qDebug() << " So you want to resize? ";

    InputDialog b;
    QTextBlock currentBlock = this->textCursor().block();
    QTextBlock::iterator it;

  //  qDebug() << currentBlock.text();
    for (it = currentBlock.begin(); !(it.atEnd()); ++it) {
        QTextFragment fragment = it.fragment();

      //  qDebug() << "Fragment " << fragment.text();
        if (fragment.isValid()) {
            if(fragment.charFormat().isImageFormat ()) {
              //  qDebug() << "Fragment " << fragment.charFormat().toImageFormat().height();
             //   qDebug() << "Found an image";
                QTextImageFormat newImageFormat = fragment.charFormat().toImageFormat();
               /* qDebug() << "new width " << newImageFormat.width();
                qDebug() << "new height" << newImageFormat.height();
*/
                QPair<unsigned int, unsigned int> size; // = ResizeImageDialog::getNewSize(this, newImageFormat.width(), newImageFormat.height());

                b.setSize(fragment.charFormat().toImageFormat().width(), fragment.charFormat().toImageFormat().height());
                b.exec();
                size = b.getSize();
                b.incImageNum();

                newImageFormat.setWidth(size.first);
                newImageFormat.setHeight(size.second);

                if (newImageFormat.isValid()){
                   // QMessageBox::about(this, "Fragment", fragment.text());
                    //newImageFormat.setName(":/icons/text_bold.png");

        //            qDebug() <<  "Text " << fragment.text();
                    QTextCursor helper = this->textCursor();

                    helper.setPosition(fragment.position());
                    helper.setPosition(fragment.position() + fragment.length(), QTextCursor::KeepAnchor);
                    helper.setCharFormat(newImageFormat);
                 }
            }
        }
    }
}

void MdiChild::Ignore() {
    QTextCursor cursor = cursorForPosition(lastPosition);
    cursor.select(QTextCursor::WordUnderCursor);
    QString ignoreWord = textUnderCursor();
    if(ignoreWord.size() > 1) ignoreDictionary.insert(ignoreWord,true);
    highlighter->rehighlight();
 //   highlighter->highlightBlock();
   // qDebug() << "Ignore list size in Text edit " <<  ignoreDictionary.size();
   // qDebug() << "Cursor " << Utilities::getUnicodeString(ignoreWord);
}


/* old code when synonyms acted as meanings
 * void MdiChild::getMeanings() {
    QTextCursor cursor = cursorForPosition(lastPosition);
    cursor.select(QTextCursor::WordUnderCursor);
    QString word = textUnderCursor();


    Dictionaries *dicts = Dictionaries::getDictionaries();
    Dictionary *dictionary;

    QString out;

    unsigned int i=0;
    out.clear();
    if( Utilities::isAssamese(word)) {
        dictionary= dicts->getDictionary(0);
        foreach(QString assmWord, Phonetic::getInflectionalFormsX(word)) {
           QString candWord = Utilities::getUnicodeString(assmWord);
           if( dictionary->hasWord(candWord)) {
              i = 0;
              foreach( QString a, dictionary->getMeanings(candWord)) {
                  out.append( a + ";");
                  i++;
                  if(i>5) break;
              }
              break;
           }
        }
    }
    else  {
        dictionary= dicts->getDictionary(1);
        foreach( QString a, dictionary->getMeanings(word))
            out.append(Utilities::getUnicode(a,"0x") +"; ");
    }


    if( out.size()>0 )
        meaningText->setText(out + "\n");
    else
        meaningText->setText("No meanings found");

}*/


void MdiChild::getWordInfo() {
    QTextCursor cursor = cursorForPosition(lastPosition);
    cursor.select(QTextCursor::WordUnderCursor);
    QString word = textUnderCursor();


    Dictionaries *dicts = Dictionaries::getDictionaries();
    Dictionary *dictionary;

    QString out;

    unsigned int i=0;
    out.clear();

    QStringList meanings, examples, idioms, officialphrases, synonyms;


    if( Utilities::isAssamese(word)) { //for assamese words
        dictionary= dicts->getDictionary(0);

        i = 0;
        foreach(QString assmWord, Phonetic::getInflectionalFormsX(word)) {
           QString candWord = Utilities::getUnicodeString(assmWord);
           //qDebug() << "   candword " << candWord;
           if( dictionary->hasWord(candWord)) {
              //get meanings;

              if(meanings.size() == 0) meanings  = dictionary->getMeanings(candWord);
              if(examples.size() == 0) examples = dictionary->getExamples(candWord);
              if(idioms.size() == 0)  idioms = dictionary->getIdioms(candWord);
              if(officialphrases.size() == 0) officialphrases = dictionary->getOfficialUse(candWord);
              if(synonyms.size() == 0) synonyms = dictionary->getSynonyms(candWord);
              i++;
              if(i>5) break;
            }
        }
    } else  { // for english words
        dictionary = dicts->getDictionary(0);
        if( dictionary->hasWord(word)) {
            if(meanings.size() == 0) meanings = dictionary->getMeanings(word);
            if(officialphrases.size() == 0) officialphrases = dictionary->getOfficialUse(word);
            if(synonyms.size() == 0) synonyms = dictionary->getSynonyms(word);
        }
    }

    /*
    qDebug() << " meanings " << meanings;
    qDebug() << "examples " << examples;
    qDebug() << "idioms " << idioms;
    qDebug() << "official phrases " << officialphrases;
    qDebug() << "synonyms " << synonyms;
*/

    QString prefix = "<div style=\"font-size:12px;\"> <table>";
    QString suffix = "</table></div>";

    QString htmlString= prefix;
    foreach(QString item, meanings) {
        htmlString += "<tr><td>" + item + "</td></tr>";
    }
    if( meanings.size() == 0 )
        htmlString += "<tr><td>No meanings found</td></tr>";
    htmlString += suffix;
    this->meaning->setToolTip(htmlString);

    htmlString= prefix;
    foreach(QString item, synonyms) {
        htmlString += "<tr><td>" + item + "</td></tr>";
    }
    if( synonyms.size() == 0 )
        htmlString += "<tr><td>No synonyms found</td></tr>";
    htmlString += suffix;
    this->synonyms->setToolTip(htmlString);

    htmlString= prefix;
    i = 1;
    foreach(QString item, examples) {
        htmlString += "<tr><td>" + QString::number(i) + "</td><td>" + item + "</td></tr>";
        i = i + 1;
    }
    if( examples.size() == 0 )
        htmlString += "<tr><td>No examples found</td></tr>";
    htmlString += suffix;
    this->examples->setToolTip(htmlString );

    htmlString= prefix;
    i = 0;
    foreach(QString item, idioms) {
        QString num = "";
        if(i%3==0) num = QString::number(i/3 + 1);
        htmlString += "<tr><td>"+ num + "</td><td>"  + item + "</td></tr>";
        i = i + 1;
    }
    if( idioms.size() == 0 )
        htmlString += "<tr><td>No idioms found</td></tr>";
    htmlString += suffix;
    this->idioms->setToolTip(htmlString);


    htmlString= prefix;
    i = 1;
    foreach(QString item, officialphrases) {
        htmlString += "<tr><td>" + QString::number(i) + "</td><td>" + item + "</td></tr>";
        i = i + 1;
    }
    if( officialphrases.size() == 0 )
        htmlString += "<tr><td>No official phrases found</td></tr>";
    htmlString += suffix;
    this->officialwords->setToolTip(htmlString);

}

void MdiChild::setDisable(bool value) {
    this->disable = value;
}
