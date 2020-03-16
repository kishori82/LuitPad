#include <QtGui>

#include "QPixmap"
#include "QSplashScreen"
#include "QThread"
#include <QSplashScreen>
#include <QSizePolicy>
#include <QFontDatabase>
#include <QThreadPool>

#include "src/window/mainwindow.h"
#include "src/window/mdichild.h"
#include "src/profile/profile.h"
#include "src/phonetic/wordstrie.h"
#include "src/phonetic/phonetic.h"
#include "src/core/constants.h"
#include "src/core/loaddatathread.h"

#ifdef _WINDOWS
#include "security.h"


#include <Windows.h>
#endif





#undef PRINT_DICTIONARY


#undef  EDITOR_LAYOUT
#undef PRINT_OPTION

#undef PRINT_PREVIEW_OPTION

#define DEVELOPMENT_MENUS
#define ROOT_WORDS_ONLY

#undef EDIT_DICTIONARY_READ


extern APP_STATES _state = F2;

extern bool _auto_matra_setting = false;
extern bool _auto_character_fill_setting = false;
extern bool _single_character_mode_setting = false;
extern bool _hide_tooltip_mode_setting = false;

#ifdef _WINDOWS
#include <windows.h>
#include <conio.h>
#endif



extern QStringList  inputIds;
extern QHash<QString, bool> validIds;


//extern const char **szFeatures;
MainWindow::MainWindow() : completer(0)
{


    Utilities::initializeVowelMap();
    Utilities::initializeConsonantMap();
    Utilities::initializeZeroLengthChars();

  //  wordsMapTree = WordsTrie::getWordsTrie();

  //  wordsMapTree->LoadDictionaryWords(":/files/composite_dictionary.txt" );
  //  wordsMapTree->printData();


    tooltipSingleton = ToolTipSingleton::GetToolTipSingleton(":/files/char_usage.txt");

    editor = new QMdiArea;
    editor->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    editor->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    connect(editor, SIGNAL(subWindowActivated(QMdiSubWindow*)), this, SLOT(updateMenus()));

    windowMapper = new QSignalMapper(this);
    connect(windowMapper, SIGNAL(mapped(QWidget*)), this, SLOT(setActiveSubWindow(QWidget*)));

    QFontDatabase fontDB;
    fontDB.addApplicationFont(":/files/Siyamrupali_1_01.ttf");
    fontDB.addApplicationFont(":/files/kalpurush.ttf");
    fontDB.addApplicationFont(":/files/Lohit-Assamese.ttf");


    //fontDB.addApplicationFont("font/Lohit-Assamese.ttf");
 //   fontDB.addApplicationFont("Lohit-Assamese_mono.ttf");
   //  fontDB.addApplicationFont("kalpurush_new.ttf");

   // fontDB.addApplicationFont(":/files/Nabaprabhat.ttf");
    //QFont mainFont = QFont("Siyam Rupali",14,0,false);
    QFont mainFont = QFont("Kalpurush",14,0,false);
   // QFont mainFont = QFont("Lohit Assamese",14,0,false);

 //   QApplication::setFont(mainFont);
    setFont(mainFont);
    QApplication::setFont(mainFont, "QAbstractItemView");

    QFont toolTipFont = QFont("kalpurush",20,0,false);
    QToolTip::setFont(toolTipFont);


    initCompleter();
    createActions();
    createMenus();
    createToolBars();
    createStatusBar();
    updateMenus();
    readSettings();

#include <QThreadPool>
#ifdef  DEVELOPMENT_MENUS
    createDevelopmentMenus();
#endif


  //  ShowTutorialThread *showtutorial = new ShowTutorialThread;
    //showtutorial->start();
   // QThreadPool::start(showtutorial);

    LoadDataThread  *loadData = new LoadDataThread;
    loadData->start();

    Utilities::ShowSplashScreen(":/images/logo3.png");
    Utilities::ShowSplashScreen(":/images/welcome1.png");

    QLabel *waitScreen = Utilities::ShowWaitScreen(":/images/wait.png");

    loadData->wait();
    waitScreen->hide();

    setWindowTitle(tr("LuitPad 2.0.0"));
    setUnifiedTitleAndToolBarOnMac(true);

    setCentralWidget(editor);
    setWindowIcon(QIcon(":/images/logo3.png"));

    newFile(NEW);
}


void MainWindow::initCompleter() {
    completer = new QCompleter(this);
    QStringList words;
    completer->setModel(new QStringListModel(words, completer));
    completer->setModelSorting(QCompleter::CaseInsensitivelySortedModel);
    completer->setCaseSensitivity(Qt::CaseSensitive);
    completer->setWrapAround(false);
    completer->setCompletionMode(QCompleter::PopupCompletion);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    editor->closeAllSubWindows();
    if (editor->currentSubWindow()) {
        event->ignore();
    } else {
        writeSettings();
        event->accept();
    }
}


void loadWords(MdiChild *child) {
    QHash<QString, QString>::const_iterator it;
    QString output;
    QList<QKeyInt> outputList;
    int unsigned i=0;
    QKeyInt k;

    for(it = Phonetic::allWords->begin(); it!= Phonetic::allWords->end(); ++it) {
      //  qDebug() << it.key();
        k.key = it.key(); k.size = k.key.size();
        outputList.append(k);
        i++;
        if( i%5000 == 0) qDebug() << "i = " << i;
    }

    QStringList stringList ;
    foreach( QKeyInt k, outputList) {
        stringList.append(Utilities::getUnicode(k.key,"0x"));
    }

    qDebug() << "Sorting ... now ";
   // qSort(outputList.begin(), outputList.end(), &toAscending);
   //  qSort(stringList);

     i =0;
     qDebug() << "Writing ... now ";

     unsigned int lim = 100000;
     foreach(QString k, stringList) {
   //      if( lim > i &&  i >= lim - 25000 )
  //       qDebug() << k;
         if( !Utilities::hasCharAnywhere(k,0x9f1) && Utilities::hasCharAnywhere(k,0x9ac) ) {
         //  if( Utilities::bisargahAnywhere(k)) {
         //if( Utilities::hyphenPresent(k)) {
         //if( Utilities::bisargahAtEnd(k) || Utilities::hyphenPresent(k)) {
         //if( Utilities::shortWord(k,2)) {
             output = output.append( "\n" + QString::number(i) + "\t" +  k);
         }
         ++i;
     }
     i=0;
     foreach(QString k, stringList) {
   //      if( lim > i &&  i >= lim - 25000 )
  //       qDebug() << k;
         if( !(!Utilities::hasCharAnywhere(k,0x9f1) && Utilities::hasCharAnywhere(k,0x9ac)) ) {
        // if(!Utilities::bisargahAnywhere(k)) {
         // if( !Utilities::hyphenPresent(k)) {
        // if( !(Utilities::bisargahAtEnd(k)  || Utilities::hyphenPresent(k))) {
         //if( !(Utilities::shortWord(k,2))) {
             output = output.append("\n" + QString::number(i) + "\t" +  k);
         }
         ++i;
         if( i%5000 == 0) qDebug() << "i = " << i;
     }
     /*
    foreach(QKeyInt k, outputList) {
        i++;
        output = output + "\n" + QString::number(i) + "\t" +  k.key;
        if(i > 20000 ) break;
    }
*/

    qDebug() << "Number of words " << Phonetic::allWords->size();

    child->setText(output);
}

void MainWindow::loadUndefinedWords() {

    MdiChild *child = createMdiChild();
    QHash<QString, QString>::const_iterator it;

    QList<QKeyInt> outputList;
    int unsigned i=0;
    QKeyInt k;

    for(it = Phonetic::allWords->begin(); it!= Phonetic::allWords->end(); ++it) {
      //  qDebug() << it.key();
        k.key = it.key(); k.size = k.key.size();
        outputList.append(k);
        i++;
        //if( i%5000 == 0) qDebug() << "i = " << i;
    }
    i = 0;
    QStringList stringList ;
    foreach( QKeyInt k, outputList) {
       // stringList.append(Utilities::getUnicode(k.key,"0x"));
        stringList.append(k.key);
    //    output.append(QString::number(++i) +  "\t" + Utilities::getUnicode(k.key,"0x") +"\n" );
    }


    Dictionaries *dicts = Dictionaries::getDictionaries();
    Dictionary *dictionary;


    QString out;

    i=0;
    out.clear();

    bool found = false;
    dictionary= dicts->getDictionary(0);
    foreach(QString word, stringList) {
       found = false;
       foreach(QString assmWord, Phonetic::getInflectionalFormsX(word)) {
           if( dictionary->hasWord(assmWord)) {
               found = true;
           }
           if(found) break;
       }
       if(found==false) {
           out.append(QString::number(i) + " \t " +  Utilities::getUnicode(word,"0x")  + "\n");
           i++;
       }
    }

    child->setText(out);
}

QMenu* MainWindow::getNewChildMenu() {
    QMenu *childMenu= new QMenu();
    childMenu->addAction(selectAllAct);
    childMenu->addAction(cutAct);
    childMenu->addAction(copyAct);
    childMenu->addAction(pasteAct);
    childMenu->addAction(searchAct);
    childMenu->addSeparator();
    return childMenu;
 }



void MainWindow::newFile(FILETYPE type)
{
    MdiChild *child = createMdiChild(type);

    if( child ==NULL ) return;
    qreal pointSize = comboSize->currentText().toFloat();
    if(pointSize > 0 ) child->setFontPointSize( pointSize );

#ifdef EDIT_DICTIONARY_READ
    loadWords(child);
#endif

    QMenu *childMenu= getNewChildMenu();

    if( childMenu != NULL) child->addContextMenu(childMenu);

    child->newFile();
    child->show();
}

void MainWindow::open()
{


    QString fileName = QFileDialog::getOpenFileName(this);
    if (!fileName.isEmpty()) {
        QMdiSubWindow *existing = findMdiChild(fileName);
        if (existing) {
            editor->setActiveSubWindow(existing);
            return;
        }

        MdiChild *child = createMdiChild();

        if(child == NULL) return;

        //qDebug() << "Current size " <<
        qreal pointSize = comboSize->currentText().toFloat();
        if(pointSize > 0 ) child->setFontPointSize( pointSize    );

        QMenu *childMenu= getNewChildMenu();

        if( childMenu != NULL) child->addContextMenu(childMenu);

      //  child->setGeometry( QRect(0,0,200,400 ));

    //    child->repaint();

        if (child->loadFile(fileName)) {
            statusBar()->showMessage(tr("File loaded"), 2000);
            child->show();
        } else {
            child->close();
        }
    }
}

void MainWindow::save()
{
    if (activeMdiChild() && activeMdiChild()->save())
        statusBar()->showMessage(tr("File saved as Plain text"), 2000);
}

void MainWindow::saveAs()
{
    if (activeMdiChild() && activeMdiChild()->saveAs())
        statusBar()->showMessage(tr("File saved"), 2000);
}

void MainWindow::saveAsHtml()
{   qDebug() << "Saving as html";
    if (activeMdiChild() && activeMdiChild()->saveAsHtml())
        statusBar()->showMessage(tr("File saved as Html"), 2000);
}

void MainWindow::saveAsPDF()
{
    if (activeMdiChild() && activeMdiChild()->saveAsPDF())
        statusBar()->showMessage(tr("File saved as PDF"), 2000);
}



void MainWindow::printFile()
{
     if (activeMdiChild())
        {
           QPrinter *printer = new QPrinter;
           QPrintDialog *dialog = new QPrintDialog(printer, this);

           dialog->setWindowTitle(tr("Print Document"));
           if (activeMdiChild()->textCursor().hasSelection())
             dialog->addEnabledOption(QAbstractPrintDialog::PrintSelection);
           if (dialog->exec() != QDialog::Accepted)
             return;

           activeMdiChild()->print(printer);
        }
        statusBar()->showMessage(tr("Printing File"), 2000);
}

void MainWindow::printPrvFile()
{
   // if (activeMdiChild() && activeMdiChild()->print())
        statusBar()->showMessage(tr("Print preview.... "), 2000);
}

void MainWindow::selectAll()
{
    if (activeMdiChild())
        activeMdiChild()->selectAll();
}

void MainWindow::cut()
{
    if (activeMdiChild())
        activeMdiChild()->cut();
}

void MainWindow::copy()
{
    if (activeMdiChild()) {
        activeMdiChild()->copy();
    }
}

void MainWindow::paste()
{
    if (activeMdiChild())
        activeMdiChild()->paste();
}

void MainWindow::search()
{
\
    if(!activeMdiChild()) return;

    SearchReplaceDialog *search = SearchReplaceDialog::getSearchReplace(activeMdiChild());
    search->exec();
   // qDebug() << "Trying to get new keyboard";
}



void MainWindow::about()
{
   QMessageBox::about(this, tr("About LuitPad"),
            trUtf8("<font face='Lohit Assamese' size=13 color='#DA0D07'>লুইতপেড</font>  সংষ্কৰণ ১.০<br><br><font face='Lohit Assamese' size=9>অসমীয়া ভাষাৰ বাবে সম্পূৰ্ণৰূপে ইউনিক'ডত সজোঁৱা একমাত্ৰ লিখন সঁজুলি<br><br>নৱপ্ৰভাতৰ দ্বাৰা সৰ্বস্বত্ব সংৰক্ষিত <br> http://www.luitpad.com</font>"));
}

void MainWindow::rightAlign()
{
   if (activeMdiChild())
        activeMdiChild()->setAlignment(Qt::AlignRight | Qt::AlignAbsolute);
    rAlign->isChecked();
    lAlign->setChecked(false);
    cAlign->setChecked(false);
    jAlign->setChecked(false);

}

void MainWindow::leftAlign()
{
   if (activeMdiChild())
        activeMdiChild()->setAlignment(Qt::AlignLeft | Qt::AlignAbsolute);
    lAlign->isChecked();
    rAlign->setChecked(false);
    jAlign->setChecked(false);
    cAlign->setChecked(false);
}

void MainWindow::centerAlign()
{
    if (activeMdiChild())
        activeMdiChild()->setAlignment(Qt::AlignCenter | Qt::AlignAbsolute);
    cAlign->isChecked();
    rAlign->setChecked(false);
    cAlign->setChecked(false);
    jAlign->setChecked(false);
}

void MainWindow::justifyAlign()
{

    if(activeMdiChild())
        activeMdiChild()->setAlignment(Qt::AlignJustify | Qt::AlignAbsolute);
    jAlign->isChecked();
    rAlign->setChecked(false);
    cAlign->setChecked(false);
    lAlign->setChecked(false);
}

void MainWindow::boldText()
{
    QTextCharFormat fmt;
    fmt.setFontWeight(bText->isChecked() ? QFont::Bold : QFont::Normal);
    setFormat(fmt);
}

void MainWindow::italicText()
{
    QTextCharFormat fmt;
    fmt.setFontItalic(iText->isChecked());
    setFormat(fmt);
}



void MainWindow::underlineText()
{
    QTextCharFormat fmt;
    fmt.setFontUnderline(uText->isChecked());
    setFormat(fmt);
}

void MainWindow::setFormat(QTextCharFormat &format)
{
    QTextCursor cursor = activeMdiChild()->textCursor();
    if (!cursor.hasSelection())
        cursor.select(QTextCursor::WordUnderCursor);
    cursor.mergeCharFormat(format);
    activeMdiChild()->mergeCurrentCharFormat(format);
}



void MainWindow::updateMenus()
{
    bool hasMdiChild = (activeMdiChild() != 0);
#ifdef PRINT_OPTION
    printAct->setEnabled(hasMdiChild);
#endif

#ifdef PRINT_PREVIEW_OPTION
    printPrvAct->setEnabled(hasMdiChild);
#endif
    saveAct->setEnabled(hasMdiChild);
    saveAsAct->setEnabled(hasMdiChild);
    saveAsHtmlAct->setEnabled(hasMdiChild);
    saveAsPDFAct->setEnabled(hasMdiChild);
    pasteAct->setEnabled(hasMdiChild);
    copyAct->setEnabled(hasMdiChild);
    closeAct->setEnabled(hasMdiChild);
    closeAllAct->setEnabled(hasMdiChild);
    tileAct->setEnabled(hasMdiChild);
    cascadeAct->setEnabled(hasMdiChild);
    nextAct->setEnabled(hasMdiChild);
    previousAct->setEnabled(hasMdiChild);
    separatorAct->setVisible(hasMdiChild);
    f2Act->setEnabled(hasMdiChild);
  //  f3Act->setEnabled(hasMdiChild);
    /*
    f4Act->setEnabled(hasMdiChild);
    f5Act->setEnabled(hasMdiChild);
    f6Act->setEnabled(hasMdiChild);
    */
    bText->setEnabled(hasMdiChild);
    uText->setEnabled(hasMdiChild);
    iText->setEnabled(hasMdiChild);

    rAlign->setEnabled(hasMdiChild);
    lAlign->setEnabled(hasMdiChild);
    cAlign->setEnabled(hasMdiChild);
    jAlign->setEnabled(hasMdiChild);

    comboFont->setEnabled(hasMdiChild);
    comboSize->setEnabled(hasMdiChild);

    //bool hasSelection = (activeMdiChild() && activeMdiChild()->textCursor().hasSelection());
 //   bool hasSelection= true;
 //   cutAct->setEnabled(hasSelection);
   // copyAct->setEnabled(hasSelection);

   // cutAct->setEnabled(true);
   // copyAct->setEnabled(true);
}


void MainWindow::updateSettingsMenu()
{
    autoVowelAct->setCheckable(true);
    autoVowelAct->setChecked(false);
    settingsMenu->addAction(autoVowelAct);
    settingsMenu->addSeparator();

    autoCharacterSubs->setCheckable(true);
    autoCharacterSubs->setChecked(false);
  //  settingsMenu->addAction(autoCharacterSubs);

    settingsMenu->addSeparator();

    singleCharacterMode->setCheckable(true);
    singleCharacterMode->setChecked(false);
    settingsMenu->addAction(singleCharacterMode);

    hideToolTipMode->setCheckable(true);
    hideToolTipMode->setChecked(false);
    settingsMenu->addAction(hideToolTipMode);

    /*
    windowMenu->addAction(nextAct);
    windowMenu->addAction(previousAct);
    windowMenu->addAction(separatorAct);
*/
}


void MainWindow::updateWindowMenu()
{
    windowMenu->clear();
    windowMenu->addAction(closeAct);
    windowMenu->addAction(closeAllAct);
    windowMenu->addSeparator();
    windowMenu->addAction(tileAct);
    windowMenu->addAction(cascadeAct);
    windowMenu->addSeparator();
    windowMenu->addAction(nextAct);
    windowMenu->addAction(previousAct);
    windowMenu->addAction(separatorAct);

    QList<QMdiSubWindow *> windows = editor->subWindowList();
    separatorAct->setVisible(!windows.isEmpty());

    for (int i = 0; i < windows.size(); ++i) {
        MdiChild *child = qobject_cast<MdiChild *>(windows.at(i)->widget());

        QString text;
        if (i < 9) {
            text = tr("&%1 %2").arg(i + 1).arg(child->userFriendlyCurrentFile());
        } else {
            text = tr("%1 %2").arg(i + 1) .arg(child->userFriendlyCurrentFile());
        }
        QAction *action  = windowMenu->addAction(text);
        action->setCheckable(true);
        action ->setChecked(child == activeMdiChild());
        connect(action, SIGNAL(triggered()), windowMapper, SLOT(map()));
        windowMapper->setMapping(action, windows.at(i));
    }
}


void MainWindow::createMessageText(MdiChild *child) {

#ifdef _WINDOWS
    child->setHtml(
                      QString("THIS IS A LIMITED TRIAL VERSION!<br><br>") +\
                      QString("     To register for the full version, copy the following <font color=\"blue\"> blue </font> line <br>") +\
                      QString("         ") + QString("<font color=\"blue\" >") + generateSignature() + QString("</font>") + QString("<br>") +\
                      QString("     and mail it to luitpad@gmail.com<br>") +\
                      QString("     For more information visit <font color=\"blue\" > http://www.luitpad.com </font> <br><br>") +\
                      QString("IMPORTANT:<br>") +\
                      QString("1. For LuitPad tutorials, visit <font color=\"blue\" > http://www.luitpad.com </font> <br>") +\
                      QString("2. Full version has many more interesting useful features.<br>") +\
                      QString("3. Trial version allows only a max of 250 characters.<br>")

                   );
#endif
    child->setFixedSize(800,350);

}

MdiChild *MainWindow::createMdiChild(FILETYPE type)
{

    MdiChild *child =  new MdiChild;
    setCentralWidget(editor);
    QMdiSubWindow *s= editor->addSubWindow(child);

    editor->setWindowIcon(QIcon(":/images/logo3.png"));
    s->setSizePolicy(QSizePolicy::MinimumExpanding   ,QSizePolicy::Expanding);


    connect(child, SIGNAL(copyAvailable(bool)), cutAct, SLOT(setEnabled(bool)));
    connect(child, SIGNAL(copyAvailable(bool)), copyAct, SLOT(setEnabled(bool)));

    child->setCompleter(completer);
    child->setToolTipSingleton(tooltipSingleton);
    if(type==NEW) child->showMaximized();
    child->setStyleSheet(
                        "QTextEdit {"
                        " selection-color: white;"
                        //" margin-left: 150px solid grey;"
                        " border: 2px solid grey;"
                        " selection-background-color: #005bbf;"
                        " color: black" // text color
                        "}");

    if(type == TRIAL ) {
      child->setFixedSize(600,100);
      child->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
      //child->setVerticalScrollBarPolicy (Qt::ScrollBarAlwaysOff);
      child->setText("Trial version window, maximum 250 characters...");
      child->setDisable(false);
      //createMessageText();
    }
    if(type==NEW) {
      child->showMaximized();
      child->setDisable(false);
    }

    if(type==INFO) {
      createMessageText(child);
      child->setDisable(true);
    }

#ifdef  EDITOR_LAYOUT
    child->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
   // child->setStyleSheet("padding: 50px grey; background-color: white; border: 2px solid grey;");
    QPrinter printer(QPrinter::HighResolution);
    printer.setPaperSize( QPrinter::B4 );
    child->setMaximumSize((printer.paperSize(QPrinter::Point)).toSize());
#endif
   // child->setMinimumSize(600, 500);



#ifdef PRINT_DICTIONARY
    Dictionaries *dicts = Dictionaries::getDictionaries();
    Dictionary *assam= dicts->getDictionary(1);

    QString out;
    unsigned int  i =0;
    foreach( QString a, assam->getWords()) {
       // qDebug() << assam->getMeanings(a);
      //  out.append(QString::number(i) + '\t' + Utilities::getUnicode(a,"0x") );
        out.append(QString::number(i) + '\t' + a);
        foreach(QString m, assam->getMeanings(a)) {
            out.append('\t' + Utilities::getUnicode(m,"0x"));
        }
        out.append('\n');
        i++;
    }
    child->setText(out);
#endif
  //   editor->activeSubWindow()->move(QPoint(40,40));
   // child->show();

    return child;
}

void MainWindow::createActions()
{
    newAct = new QAction(QIcon(":/images/new.png"), tr("&New"), this);
    newAct->setShortcuts(QKeySequence::New);
    newAct->setStatusTip(tr("Create a new file"));
    connect(newAct, SIGNAL(triggered()), this, SLOT(newFile()));

    openAct = new QAction(QIcon(":/images/open.png"), tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open an existing file"));
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

    saveAct = new QAction(QIcon(":/images/save.png"), tr("&Save"), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Save the document to disk"));
    connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));

    saveAsAct = new QAction(tr("Save &As..."), this);
    saveAsAct->setShortcuts(QKeySequence::SaveAs);
    saveAsAct->setStatusTip(tr("Save the document under a new name"));
    connect(saveAsAct, SIGNAL(triggered()), this, SLOT(saveAs()));

#ifdef PRINT_OPTION
    printAct = new QAction(QIcon(":/images/print.png"), tr("&Print"), this);
    printAct->setShortcuts(QKeySequence::Print);
    printAct->setStatusTip(tr("Print a file"));
    connect(printAct, SIGNAL(triggered()), this, SLOT(printFile()));
#endif

#ifdef PRINT_PREVIEW_OPTION
    printPrvAct = new QAction(tr("Print Preview"), this);
    //printPrvAct->setShortcuts(QKeySequence::;
    printPrvAct->setStatusTip(tr("Print preview a file"));
    connect(printPrvAct, SIGNAL(triggered()), this, SLOT(printPrvFile()));
#endif

    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    exitAct->setStatusTip(tr("Exit the application"));
    connect(exitAct, SIGNAL(triggered()), qApp, SLOT(closeAllWindows()));

    selectAllAct = new QAction(tr("&Select All"), this);
  //  selectAllAct->setShortcuts(QKeySequence::Copy);
    selectAllAct->setStatusTip(tr("Select the entire contents " "clipboard"));
    connect(selectAllAct, SIGNAL(triggered()), this, SLOT(selectAll()));

    cutAct = new QAction(QIcon(":/images/cut.png"), tr("Cu&t"), this);
    cutAct->setShortcuts(QKeySequence::Cut);
    cutAct->setStatusTip(tr("Cut the current selection's contents to the "
                           "clipboard"));
    connect(cutAct, SIGNAL(triggered()), this, SLOT(cut()));

    copyAct = new QAction(QIcon(":/images/copy.png"), tr("&Copy"), this);
    copyAct->setShortcuts(QKeySequence::Copy);
    copyAct->setStatusTip(tr("Copy the current selection's contents to the " "clipboard"));
    connect(copyAct, SIGNAL(triggered()), this, SLOT(copy()));

    pasteAct = new QAction(QIcon(":/images/paste.png"), tr("&Paste"), this);
    pasteAct->setShortcuts(QKeySequence::Paste);
    pasteAct->setStatusTip(tr("Paste the clipboard's contents into the current " "selection"));
    connect(pasteAct, SIGNAL(triggered()), this, SLOT(paste()));

    searchAct = new QAction( tr("&Find/Replace"), this);
    searchAct->setShortcuts(QKeySequence::Find);
    searchAct->setStatusTip(tr("Search and replace " "selection"));
    connect(searchAct, SIGNAL(triggered()), this, SLOT(search()));

    docStateAct = new QAction(tr("Document Statistics"), this);
    docStateAct->setStatusTip(tr("Document Statistics"));
   // connect(docStateAct, SIGNAL(triggered()), this, SLOT(docState()));

    closeAct = new QAction(tr("Cl&ose"), this);
    closeAct->setStatusTip(tr("Close the active window"));
    connect(closeAct, SIGNAL(triggered()), editor, SLOT(closeActiveSubWindow()));

    closeAllAct = new QAction(tr("Close &All"), this);
    closeAllAct->setStatusTip(tr("Close all the windows"));
    connect(closeAllAct, SIGNAL(triggered()), editor, SLOT(closeAllSubWindows()));

    tileAct = new QAction(tr("&Tile"), this);
    tileAct->setStatusTip(tr("Tile the windows"));
    connect(tileAct, SIGNAL(triggered()), editor, SLOT(tileSubWindows()));

    cascadeAct = new QAction(tr("&Cascade"), this);
    cascadeAct->setStatusTip(tr("Cascade the windows"));
    connect(cascadeAct, SIGNAL(triggered()), editor, SLOT(cascadeSubWindows()));

    nextAct = new QAction(tr("Ne&xt"), this);
    nextAct->setShortcuts(QKeySequence::NextChild);
    nextAct->setStatusTip(tr("Move the focus to the next window"));
    connect(nextAct, SIGNAL(triggered()),editor, SLOT(activateNextSubWindow()));

    previousAct = new QAction(tr("Pre&vious"), this);
    previousAct->setShortcuts(QKeySequence::PreviousChild);
    previousAct->setStatusTip(tr("Move the focus to the previous " "window"));
    connect(previousAct, SIGNAL(triggered()), editor, SLOT(activatePreviousSubWindow()));

    separatorAct = new QAction(this);
    separatorAct->setSeparator(true);

    aboutAct = new QAction(QIcon(":/images/logo3.png"), trUtf8("&About"), this);
    aboutAct->setStatusTip(tr("About box"));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

    kbdDisplayAct = new QAction(QIcon(":/images/keyboard.png"), tr("&Keyboard"), this);
    kbdDisplayAct->setStatusTip(tr("Display Keyboard"));
    connect(kbdDisplayAct, SIGNAL(triggered()), this, SLOT(kbdDisplay()));

    rAlign = new QAction(QIcon(":/images/right.png"), tr("&Right"), this);
    rAlign->setStatusTip(tr("Right Align"));
    connect(rAlign, SIGNAL(triggered()),this, SLOT(rightAlign()));
    rAlign->setCheckable(true);

    lAlign = new QAction(QIcon(":/images/left.png"), tr("&Left"), this);
    lAlign->setStatusTip(tr("Left Align"));
    connect(lAlign, SIGNAL(triggered()),this, SLOT(leftAlign()));
    lAlign->setCheckable(true);

    cAlign = new QAction(QIcon(":/images/center.png"), tr("C&enter"), this);
    cAlign->setStatusTip(tr("Center Align"));
    connect(cAlign, SIGNAL(triggered()),this, SLOT(centerAlign()));
    cAlign->setCheckable(true);

    jAlign = new QAction(QIcon(":/images/justify.png"), tr("&Justified"), this);
    jAlign->setStatusTip(tr("Justified Align"));
    connect(jAlign, SIGNAL(triggered()),this, SLOT(justifyAlign()));
    jAlign->setCheckable(true);

    iText = new QAction(QIcon(":/images/italic.png"), tr("Italic Text"), this);
    iText->setStatusTip(tr("Italic Text"));
    connect(iText, SIGNAL(triggered()),this, SLOT(italicText()));
    iText->setCheckable(true);
    iText->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_I));

    bText = new QAction(QIcon(":/images/bold.png"), tr("Bold Text"), this);
    bText->setStatusTip(tr("Bold Text"));
    connect(bText, SIGNAL(triggered()),this, SLOT(boldText()));
    bText->setCheckable(true);
    bText->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_B));

    uText = new QAction(QIcon(":/images/underline.png"), tr("UnderLine Text"), this);
    uText->setStatusTip(tr("Underline text"));
    connect(uText, SIGNAL(triggered()),this, SLOT(underlineText()));
    uText->setCheckable(true);
    uText->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_U));

    f2Act = new QAction(QIcon(":/images/mode2.png"), tr("Toggle Character->Phonetic mode"), this);
    f2Act->setStatusTip(tr("Character mode"));
    connect(f2Act, SIGNAL(triggered()),this, SLOT(f2Action()));
    f2Act->setCheckable(true);
   // f2Act->setShortcut(QKeySequence::QKeySequence(Qt::Key_F2));


    //QShortcut* del = new QShortcut(Qt:: , this);
    QShortcut *del = new QShortcut(Qt::Key_F1, this);
    connect(del, SIGNAL(activated()), f2Act, SLOT(trigger()));
 //   QShortcut* bak = new QShortcut(Qt::Key_F3, this);
//    connect(bak, SIGNAL(activated()), f2Act, SLOT(trigger()));

    /*
    f4Act = new QAction(QIcon(":/images/f4.png"), tr("Yet to set"), this);
    f4Act->setStatusTip(tr("Yet to set"));
    connect(f4Act, SIGNAL(triggered()),this, SLOT(f4Action()));
    f4Act->setCheckable(true);
    f4Act->setShortcut(QKeySequence::QKeySequence(Qt::Key_F4));

*/
    saveAsHtmlAct = new QAction(QIcon(":/images/save_html.png"), tr("Save as Html"), this);
    saveAsHtmlAct->setStatusTip(tr("Convert to HTML"));
    connect(saveAsHtmlAct , SIGNAL(triggered()),this, SLOT(saveAsHtml()));

    saveAsPDFAct = new QAction(QIcon(":/images/save_pdf.png"), tr("Save as PDF"), this);
    saveAsPDFAct ->setStatusTip(tr("Convert to PDF"));
    connect(saveAsPDFAct , SIGNAL(triggered()),this, SLOT(saveAsPDF()));
    //saveAsHtmlAct ->setShortcut(QKeySequence::QKeySequence(Qt::Key_Control + Qt::Key_H));


     autoVowelAct = new QAction(tr("&Automatic Matra"), this);
     autoVowelAct->setStatusTip(tr("Turn automatic vowel mode on/off"));
     connect(autoVowelAct, SIGNAL(triggered()), this, SLOT(toggleAutoVowelMode()));

     autoCharacterSubs = new QAction(tr("&Automatic Character Fill"), this);
     autoCharacterSubs->setStatusTip(tr("Turn automatic character fill mode on/off"));
     connect(autoCharacterSubs, SIGNAL(triggered()), this, SLOT(toggleAutoCharacterFillMode()));

     singleCharacterMode = new QAction(tr("&Fast Character Mode"), this);
     singleCharacterMode->setStatusTip(tr("Turn Fast character  mode on/off"));
     connect(singleCharacterMode, SIGNAL(triggered()), this, SLOT(toggleSingleCharacterMode()));

     hideToolTipMode = new QAction(tr("&Hide ToolTip Mode"), this);
     hideToolTipMode->setStatusTip(tr("Hide ToolTip  mode on/off"));
     connect(hideToolTipMode, SIGNAL(triggered()), this, SLOT(toggleHideToolTipMode()));
}


void MainWindow::createDevelopmentMenus() {
    developmentMenu  = menuBar()->addMenu(tr("&Developers"));
   // connect(settingsMenu, SIGNAL(aboutToShow()), this, SLOT(updateSettingsMenu()));

    printInternalDictionaryAct = new QAction(tr("&Print Internal Dictionary"), this);
    connect( printInternalDictionaryAct, SIGNAL(triggered()), this, SLOT(printInternalDictionarySlot()) );

    writeNewDictionaryAct = new QAction(tr("&Write Words to Dictionary"), this);
    connect( writeNewDictionaryAct, SIGNAL(triggered()), this, SLOT(writeNewDictionarySlot()) );

    loadUndefinedWordsAct = new QAction(tr("&Load Undefined Dictionary Words"), this);
    connect( loadUndefinedWordsAct, SIGNAL(triggered()), this, SLOT( loadUndefinedWords() ) );

    developmentMenu->addAction( printInternalDictionaryAct );
    developmentMenu->addAction( writeNewDictionaryAct );
    developmentMenu->addAction(loadUndefinedWordsAct);
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAct);
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);
    fileMenu->addSeparator();
    fileMenu->addAction(saveAsAct);
    fileMenu->addAction(saveAsHtmlAct);
    fileMenu->addAction(saveAsPDFAct);
    fileMenu->addSeparator();
#ifdef PRINT_PREVIEW_OPTION
    fileMenu->addSeparator();
    fileMenu->addAction(printPrvAct);
#endif

#ifdef PRINT_OPTION
    fileMenu->addSeparator();
    fileMenu->addAction(printAct);
#endif

    fileMenu->addSeparator();
    fileMenu->addAction(kbdDisplayAct);
    fileMenu->addSeparator();
    QAction *action = fileMenu->addAction(tr("Switch layout direction"));
    connect(action, SIGNAL(triggered()), this, SLOT(switchLayoutDirection()));
    fileMenu->addAction(exitAct);

    editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(selectAllAct);
    editMenu->addAction(cutAct);
    editMenu->addAction(copyAct);
    editMenu->addAction(pasteAct);
    editMenu->addAction(searchAct);
    editMenu->addSeparator();
    editMenu->addAction(f2Act);
//    editMenu->addAction(f3Act);

    editMenu->addSeparator();
    editMenu->addAction(docStateAct);

    //menuBar()->addSeparator();
    formatMenu = menuBar()->addMenu("&Format");
    formatMenu ->addAction(rAlign);
    formatMenu ->addAction(lAlign);
    formatMenu ->addAction(cAlign);
    formatMenu ->addAction(jAlign);
    formatMenu->addSeparator();
    formatMenu ->addAction(bText);
    formatMenu ->addAction(iText);
    formatMenu ->addAction(uText);
    formatMenu->addSeparator();

    windowMenu = menuBar()->addMenu(tr("&Window"));
    updateWindowMenu();
    connect(windowMenu, SIGNAL(aboutToShow()), this, SLOT(updateWindowMenu()));
    menuBar()->addSeparator();

    settingsMenu = menuBar()->addMenu(tr("&Settings"));
   // connect(settingsMenu, SIGNAL(aboutToShow()), this, SLOT(updateSettingsMenu()));
    updateSettingsMenu();

    /*
    tutorialsMenu = menuBar()->addMenu(tr("&Video Tutorials"));
    tutorialSignalMapper = new QSignalMapper(this);

    for(int j =0 ; j < 10; j++) {
        videos[j] = new QAction(videoTitles.at(j), this);
       tutorialsMenu->addAction(videos[j]);
       tutorialSignalMapper->setMapping(videos[j],j);
    }
    for(int j =0 ; j < 10; j++) {
        connect(videos[j], SIGNAL(triggered()), tutorialSignalMapper, SLOT (map()));
    }
    if( validSoftware > 0)
       connect(tutorialSignalMapper, SIGNAL(mapped(int)), this, SLOT(playVideo( int )));
     */

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);


}

/*
void MainWindow::playVideo( int i ) {

    SimplePlayer playerWindow;
    playerWindow.setWindowTitle("Luit Player");
    playerWindow.setMediaFile("videos\\" + videoFiles.at(i));
    playerWindow.play();

    QEventLoop loop;
    loop.exec();
}
*/

void MainWindow::createToolBars()
{
    fileToolBar = addToolBar(tr("File"));
    fileToolBar->addAction(newAct);
    fileToolBar->addAction(openAct);
    fileToolBar->addAction(saveAct);
    fileToolBar->addAction(saveAsHtmlAct);

#ifdef PRINT_OPTION
    fileToolBar->addAction(printAct);
#endif

#ifdef PRINT_PREVIEW_OPTION
    fileToolBar->addAction(printPrvAct);
#endif

    fileToolBar->addSeparator();
    fileToolBar->addAction(kbdDisplayAct);
   // fileToolBar->addAction(printPrvAct);

    //fileToolBar->addSeparator();

    editToolBar = addToolBar(tr("Edit"));
    editToolBar->addAction(cutAct);
    editToolBar->addAction(copyAct);
    editToolBar->addAction(pasteAct);
    editToolBar->addSeparator();
    editToolBar->addAction(f2Act);
  //  editToolBar->addAction(f3Act);

    formatToolBar = addToolBar(tr("Format"));

    formatToolBar->addAction(lAlign);
    formatToolBar->addAction(rAlign);
    formatToolBar->addAction(cAlign);
    formatToolBar ->addAction(jAlign);
    formatToolBar->addSeparator();
    formatToolBar ->addAction(uText);
    formatToolBar ->addAction(bText);
    formatToolBar ->addAction(iText);

    //................................................................................................................
    comboFont = new QFontComboBox(formatToolBar);
    connect(comboFont, SIGNAL(activated(QString)),this, SLOT(textFamily(QString)));
    comboFont->setFont(QApplication::font());
    comboFont->setAutoCompletion(true);
    comboFont->adjustSize();

    comboSize = new QComboBox(formatToolBar);
    comboSize->setObjectName("comboSize");
    comboSize->setEditable(true);
    comboSize->setFont(QFont("Times New Roman",12,0,false));


    QFontDatabase db;
    foreach(int size, db.standardSizes())
        comboSize->addItem(QString::number(size));

    connect(comboSize, SIGNAL(activated(QString)),this, SLOT(textSize(QString)));

    // set Qcompleter Font
    connect(comboSize, SIGNAL(activated(QString)),this, SLOT(textQCompleterFont(QString)));
    //comboSize->setCurrentIndex(comboSize->findText(QString::number(QApplication::font().pointSize())));
    comboSize->setCurrentIndex(comboSize->findText(QString::number(16)));

    formatToolBar->addWidget(comboSize);
    formatToolBar->addWidget(comboFont);
    formatToolBar->addSeparator();

    formatToolBar->addAction(aboutAct);
}

void MainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}

void MainWindow::readSettings()
{
    QSettings settings("NavaPrabhat", "LuitPad");
    QPoint pos = settings.value("pos", QPoint(200, 200)).toPoint();
    QSize size = settings.value("size", QSize(800, 600)).toSize();
    move(pos);
    resize(size);
}

void MainWindow::writeSettings()
{
    QSettings settings("NavaPrabhat", "LuitPad");
    settings.setValue("pos", pos());
    settings.setValue("size", size());
}

MdiChild *MainWindow::activeMdiChild()
{
    if (QMdiSubWindow *activeSubWindow = editor->activeSubWindow())
        return qobject_cast<MdiChild *>(activeSubWindow->widget());

  //  qDebug()  << "Returning the activeMdiChild";
    return 0;
}

QMdiSubWindow *MainWindow::findMdiChild(const QString &fileName)
{
    QString canonicalFilePath = QFileInfo(fileName).canonicalFilePath();

    foreach (QMdiSubWindow *window, editor->subWindowList()) {
        MdiChild *mdiChild = qobject_cast<MdiChild *>(window->widget());
        if (mdiChild->currentFile() == canonicalFilePath)
            return window;
    }
    return 0;
}

void MainWindow::switchLayoutDirection()
{
    if (layoutDirection() == Qt::LeftToRight)
        qApp->setLayoutDirection(Qt::RightToLeft);
    else
        qApp->setLayoutDirection(Qt::LeftToRight);
}

void MainWindow::setActiveSubWindow(QWidget *window)
{
    if (!window)
        return;
    editor->setActiveSubWindow(qobject_cast<QMdiSubWindow *>(window));
}

void MainWindow::kbdDisplay()
{
    Profile *kbd = Profile::getkeyBoard();
   // qDebug() << "Trying to get new keyboard";
    kbd->display();
}


void MainWindow::textFamily(const QString &f)
{
    QTextCharFormat fmt;
    fmt.setFontFamily(f);
    mergeFormatOnWordOrSelection(fmt);
}

void MainWindow::textSize(const QString &p)
{
    qreal pointSize = p.toFloat();
   // qDebug() << "Format text size " << pointSize;
    if (p.toFloat() > 0) {
        QTextCharFormat fmt;
        fmt.setFontPointSize(pointSize);
        mergeFormatOnWordOrSelection(fmt);
    }
}


void MainWindow::textQCompleterFont(const QString &p) {

   qreal pointSize = p.toFloat();
   if( p.toInt())
     completer->popup()->setFont(QFont("kalpurush",pointSize ,0,false));
}


void MainWindow::mergeFormatOnWordOrSelection(const QTextCharFormat &format)
{
    QTextCursor cursor = activeMdiChild()->textCursor();
    if (!cursor.hasSelection())
        cursor.select(QTextCursor::WordUnderCursor);
    cursor.mergeCharFormat(format);

    activeMdiChild()->mergeCurrentCharFormat(format);
}

bool MainWindow::isValidConfig() {
    if(
        (_auto_matra_setting == true && _single_character_mode_setting == false && _hide_tooltip_mode_setting == false) ||\
        (_auto_matra_setting == false && _single_character_mode_setting == false && _hide_tooltip_mode_setting == false) ||\
        (_auto_matra_setting == false && _single_character_mode_setting == true && _hide_tooltip_mode_setting == false) ||\
        (_auto_matra_setting == false && _single_character_mode_setting == true && _hide_tooltip_mode_setting == true)
     )
        return true;
    else {
        return false;
    }
}

void MainWindow::resetConfig() {
    _auto_matra_setting = false;
    _single_character_mode_setting = false;
    _hide_tooltip_mode_setting = false;
     singleCharacterMode->setChecked(false);
     autoVowelAct->setChecked(false);
     hideToolTipMode->setChecked(false);

}


void MainWindow::toggleAutoVowelMode() {
    _auto_matra_setting = autoVowelAct->isChecked();
    bool before_reset = _auto_matra_setting;
    if(!isValidConfig()) resetConfig();
    _auto_matra_setting= before_reset;
    autoVowelAct->setChecked(_auto_matra_setting);
     if(!isValidConfig()) {
         resetConfig();
         Utilities::warningBox("Make sure you check/tick mark them in the correct order!");
     }

}


void MainWindow::writeNewDictionarySlot() {
      Utilities::writeStringToUnicode(QString("files/recently_processed_dictionary_filtered.txt"), activeMdiChild()->document()->toPlainText());
}


QStringList MainWindow::pickRootWords(QStringList &wordList) {

    QStringList newList;
    int size = wordList.size();
    for( int i = 0, j = 0;i < size ; ) {
        do {
            j++;
            if(j > size - 1) break;
        } while( Utilities::isAnInflectionalForm( wordList[i], wordList[j]) );
        i = j;
     /*   if( i%1000 == 0 ) {
            qDebug() << "counted " + QString::number(i);
        } */
        if( i < size) newList.append(wordList[i]);
    }
    qDebug() << "Number of words picked " << QString::number(newList.size());
    return newList;
}

void MainWindow::printInternalDictionarySlot() {

    MdiChild *child = createMdiChild();

    if( child ==NULL ) return;
    qreal pointSize = comboSize->currentText().toFloat();
    if(pointSize > 0 ) child->setFontPointSize( pointSize );

    WordsTrie *words = WordsTrie::getWordsTrie();
    QStringList wordList;

    words->getWordList(wordList);

    QString output;

    unsigned int i = 0;
#ifdef ROOT_WORDS_ONLY
    wordList.sort();
    qDebug() << "Debug before sort;";
    wordList = pickRootWords(wordList);
#endif

    foreach(QString s, wordList) {
     //   output.append(s +"\n");

       if(s.contains("0x2d0")) continue;
       i++;

       output.append(QString::number(i) + "\t"  + s + "\t"+  Utilities::getUnicode(s,"0x") +"\n");
    }
    child->setText(output);

    qDebug()<< "Number of words retrieved " << wordList.size();

    QMenu *childMenu= getNewChildMenu();

    if( childMenu != NULL) child->addContextMenu(childMenu);

    child->newFile();
    child->show();
    qDebug() << "Printing internal dictionary";
}


/* do not use this function */
void MainWindow::toggleAutoCharacterFillMode() {
    _auto_character_fill_setting = autoCharacterSubs->isChecked();
    if(!isValidConfig()) resetConfig();

}

void MainWindow::toggleSingleCharacterMode() {

    _single_character_mode_setting = singleCharacterMode->isChecked();
     bool before_reset = _single_character_mode_setting;
    if(!isValidConfig()) resetConfig();

    _single_character_mode_setting = before_reset;
    singleCharacterMode->setChecked(_single_character_mode_setting);
    if(!isValidConfig()) resetConfig();

}

void MainWindow::toggleHideToolTipMode() {

    _hide_tooltip_mode_setting = hideToolTipMode->isChecked();
    bool before_reset = _hide_tooltip_mode_setting;
    if(!isValidConfig()) resetConfig();
    _hide_tooltip_mode_setting = before_reset;
    hideToolTipMode->setChecked(_hide_tooltip_mode_setting);
    if(!isValidConfig()) {
        resetConfig();
        Utilities::warningBox("Make sure you check/tick mark them in the correct order!\nFirst mark \"Fast Character Mode\" and next \"Hide Tooltop Mode\"");
    }
}


void MainWindow::f2Action(){

    if( _state == F3 ) {
        f2Act->setIcon(QIcon(":/images/mode2.png"));
        f2Act->setText( tr("Change Character->Phonetic mode (F1)"));
        f2Act->setStatusTip(tr("Character mode"));
      //  f3Act->setIcon(QIcon::QIcon(":/images/_mode3.png"));
         _state = F2;
    }
    else {
        f2Act->setIcon(QIcon(":/images/_mode2.png"));
        f2Act->setText(tr("Change Phonetic->Character mode  (F1)"));
        f2Act->setStatusTip(tr("Phonetic mode"));
    //    f3Act->setIcon(QIcon::QIcon(":/images/mode3.png"));
        _state = F3;
     }

     //   f2Act->setChecked(true);
      //  f3Act->setChecked(false);
}


void MainWindow::f3Action(){
    qDebug() << "line break ";



    if( !activeMdiChild() ) return;

    QTextCursor tc = activeMdiChild()->textCursor();
 //   tc.insertText(QString("hello"));

    /*
    f2Act->setIcon(QIcon::QIcon(":/images/mode2.png"));
    f3Act->setIcon(QIcon::QIcon(":/images/_mode3.png"));
    _state = F3;
  //  qDebug() << " State F3";
    f2Act->setChecked(false);
   // f3Act->setChecked(true);

*/


}


QAbstractItemModel *MainWindow::modelFromFile(const QString& fileName)
 {
     QFile file(fileName);
     if (!file.open(QFile::ReadOnly))
         return new QStringListModel(completer);

 #ifndef QT_NO_CURSOR
     QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
 #endif
     QStringList words;

     while (!file.atEnd()) {
         QByteArray line = file.readLine();
         if (!line.isEmpty()){
             words << line.trimmed();
         }
     }
     qDebug()<<"The List: " <<words;

 #ifndef QT_NO_CURSOR
     QApplication::restoreOverrideCursor();
 #endif
     return new QStringListModel(words, completer);
 }

/*
void TextEdit::colorChanged(const QColor &c)
{
    QPixmap pix(16, 16);
    pix.fill(c);
    actionTextColor->setIcon(pix);
}

void MainWindow::textColor()
{
    QColor col = QColorDialog::getColor(activeMdiChild()->textColor(), this);
    if (!col.isValid())
        return;
    QTextCharFormat fmt;
    fmt.setForeground(col);
    setFormat(fmt);
}
*/


bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    return true;




    if(!obj->isWidgetType())
        {
       //  qDebug()<<"False: "<<obj->objectName();
        return false;
        }
    if(activeMdiChild()){activeMdiChild()->setFocusPolicy(Qt::StrongFocus); activeMdiChild()->installEventFilter(this);}
    const QEvent::Type type = event->type();
    //qDebug()<<"Event type: ";
    bool result = false;
    QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
    switch(type){
         case QEvent::KeyPress:  if(keyEvent->key() ==Qt::Key_Return||keyEvent->key()  == Qt::Key_Enter)
                                                {qDebug()<<"Handling later";
                                                keyEvent->ignore();}
                                                 else {
                                                    keyEvent = dynamic_cast<QKeyEvent*>(event);
                                                 //  qDebug("KeyPress event : %c",keyEvent->key());
                                                  // qDebug()<<keyEvent->text();
                                                   keyMap(keyEvent->text());
                                                   //keyEvent->ignore();
                                                   result =true;}
                                                   break;

         case QEvent::KeyRelease: keyEvent = dynamic_cast<QKeyEvent*>(event);
                                                     //qDebug("KeyRelese event : %c",keyEvent->key());
                                                     keyEvent->ignore();
                                                     result=true;
                                                     //keyMap(keyEvent->key());
                                                     break;
        default :   result = QObject::eventFilter(obj, event);
                      //  qDebug()<<"default result: "<<result;
                        break;
            }

return result;
}//eventFilter



void MainWindow::keyMap(QString ch)
{

qDebug()<<"Within keyMap: "<<ch;

if(ch == "A") {activeMdiChild()->insertPlainText(trUtf8("আ"));}
else if(ch == "B") {activeMdiChild()->insertPlainText(trUtf8("ঝ")); }
else if(ch == "C") {activeMdiChild()->insertPlainText(trUtf8("ছ")); }
else if(ch == "D") {activeMdiChild()->insertPlainText(trUtf8("ধ")); }
else if(ch == "E") {activeMdiChild()->insertPlainText(trUtf8("এ")); }
else if(ch == "F") {activeMdiChild()->insertPlainText(trUtf8("ফ")); }
else if(ch == "G") {activeMdiChild()->insertPlainText(trUtf8("ঘ")); }
else if(ch == "H") {activeMdiChild()->insertPlainText(trUtf8("ঢ়")); }
else if(ch == "I") {activeMdiChild()->insertPlainText(trUtf8("ই")); }
else if(ch == "J") {activeMdiChild()->insertPlainText(trUtf8("ঝ")); }
else if(ch == "K") {activeMdiChild()->insertPlainText(trUtf8("খ")); }
else if(ch == "L") {activeMdiChild()->insertPlainText(trUtf8("ল")); }
else if(ch == "M") {activeMdiChild()->insertPlainText(trUtf8("ম")); }
else if(ch == "N") {activeMdiChild()->insertPlainText(trUtf8("ণ")); }
else if(ch == "O") {activeMdiChild()->insertPlainText(trUtf8("ও")); }
else if(ch == "P") {activeMdiChild()->insertPlainText(trUtf8("ফ")); }
else if(ch == "Q") {activeMdiChild()->insertPlainText(trUtf8("খ")); }
else if(ch == "R") {activeMdiChild()->insertPlainText(trUtf8("ড়")); }
else if(ch == "S") {activeMdiChild()->insertPlainText(trUtf8("স")); }
else if(ch == "T") {activeMdiChild()->insertPlainText(trUtf8("ট")); }
else if(ch == "U") {activeMdiChild()->insertPlainText(trUtf8("উ")); }
else if(ch == "V") {activeMdiChild()->insertPlainText(trUtf8("ভ")); }
else if(ch == "W") {activeMdiChild()->insertPlainText(trUtf8("ৱ")); }
else if(ch == "X") {activeMdiChild()->insertPlainText(trUtf8("ক্ষ")); }
else if(ch == "Y") {activeMdiChild()->insertPlainText(trUtf8("য়")); }
else if(ch == "Z") {activeMdiChild()->insertPlainText(trUtf8("ঝ")); }

else if(ch == "a") {activeMdiChild()->insertPlainText(trUtf8("অ")); }
else if(ch == "b") {activeMdiChild()->insertPlainText(trUtf8("ব")); }
else if(ch == "c") {activeMdiChild()->insertPlainText(trUtf8("চ")); }
else if(ch == "d") {activeMdiChild()->insertPlainText(trUtf8("দ")); }
else if(ch == "e") {activeMdiChild()->insertPlainText(trUtf8("ে")); }
else if(ch == "f") { activeMdiChild()->insertPlainText(trUtf8("ফ")); }
else if(ch == "g") { activeMdiChild()->insertPlainText(trUtf8("গ")); }
else if(ch == "h") { activeMdiChild()->insertPlainText(trUtf8("হ")); }
//else if(ch == "i") {activeMdiChild()->insertPlainText(trUtf8("ই")); }
else if(ch == "i") {activeMdiChild()->insertPlainText("i"); }
else if(ch == "j") {activeMdiChild()->insertPlainText(trUtf8("জ")); }
else if(ch == "k") {activeMdiChild()->insertPlainText(trUtf8("ক")); }
else if(ch == "l") {activeMdiChild()->insertPlainText(trUtf8("ল")); }
else if(ch == "m") {activeMdiChild()->insertPlainText(trUtf8("ম")); }
else if(ch == "n") {activeMdiChild()->insertPlainText(trUtf8("ন")); }
else if(ch == "o") { activeMdiChild()->insertPlainText(trUtf8("ো")); }
else if(ch == "p") {activeMdiChild()->insertPlainText(trUtf8("প")); }
else if(ch == "q") {activeMdiChild()->insertPlainText(trUtf8("ক")); }
else if(ch == "r") {activeMdiChild()->insertPlainText(trUtf8("ৰ")); }
else if(ch == "s") {activeMdiChild()->insertPlainText(trUtf8("ছ")); }
else if(ch == "t") {activeMdiChild()->insertPlainText(trUtf8("ত")); }
else if(ch == "u") {activeMdiChild()->insertPlainText(trUtf8("ু")); }
else if(ch == "v") {activeMdiChild()->insertPlainText(trUtf8("ভ")); }
else if(ch == "w") { activeMdiChild()->insertPlainText(trUtf8("ৱ")); }
else if(ch == "x") {activeMdiChild()->insertPlainText(trUtf8("ষ")); }
else if(ch == "y") {activeMdiChild()->insertPlainText(trUtf8("য়")); }
else if(ch == "z") {activeMdiChild()->insertPlainText(trUtf8("ঝ")); }

//blank space
else if(ch == " ") {activeMdiChild()->insertPlainText(trUtf8(" ")); }
//else if(ch == Qt::Key_Return) {activeMdiChild()->insertPlainText(trUtf8(" ")); }
}

bool MainWindow::event(QEvent *event)
{

    //qDebug() << "Event" << event->type();


    if (event->type() == QEvent::KeyPress ) {
       tooltipSingleton->charPrefix(activeMdiChild());
    }

    return QWidget::event(event);
}



void MainWindow::keyPressEvent(QKeyEvent* e)
    {
          return;
          QMessageBox* box = new QMessageBox();
          box->setWindowTitle(QString("Hello"));
          box->setText(QString("You Pressed: ")+ e->text());
          box->show();
    }

void MainWindow::docState()
{
//docStatistics dstate;
//dstate.show();
}

class SleepThread : public QThread{
     Q_OBJECT
     // private run
     void run (){}

     public :
     static void usleep(long iSleepTime){
          QThread::usleep(iSleepTime);
     }
     static void sleep(long iSleepTime){
          QThread::sleep(iSleepTime);
     }
     static void msleep(long iSleepTime){
          QThread::msleep(iSleepTime);
     }
};

#ifdef  _WINDOWS
QString MainWindow::hardDiskID() {
    DWORD dwVolSerial;
    BOOL bIsRetrieved;
    bIsRetrieved = GetVolumeInformation(L"C:\\", NULL, NULL, &dwVolSerial, NULL, NULL, NULL, NULL);
    QString hardDiskSerial = QString::number(dwVolSerial, 16).toUpper();;
    QString hashID = QCryptographicHash::hash(hardDiskSerial.toAscii().constData()  ,QCryptographicHash::Sha1 ).toBase64();
    return hashID;
}



QString MainWindow::generateSignature()
{

    SimpleCrypt *crypto = SimpleCrypt::getEncryptor(Q_UINT64_C(0x23bacf02473fdea0));
    QString cpuinfo_string = cpuinfo();
    QStringList mac_adressList = GetMACaddress();
    QString serial;
    foreach(QString macaddr, mac_adressList) {
        QString encMacAddr = macaddr);
        if(encMacAddr.size()==0) continue;
        serial.append(encMacAddr);
        serial.append(";");
    }
    serial.append("@");
    serial.append(cpuinfo_string);
    return(QString("&lt;&lt;") + serial + QString("&gt;&gt;"));
}

#endif
