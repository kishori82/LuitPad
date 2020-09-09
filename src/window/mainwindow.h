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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QFileDialog>
#include <QFontComboBox>
#include <QMainWindow>
#include <QMdiSubWindow>
#include <QMenuBar>
#include <QPrintDialog>
#include <QShortcut>
#include <QStatusBar>
#include <QToolBar>

#include "src/characters/chartrie.h"
#include "src/editor/highlighter.h"
#include "src/editor/searchreplace.h"
#include "src/editor/textedit.h"
#include "src/editor/tooltipsingleton.h"
#include "src/phonetic/romanization.h"
#include "src/phonetic/translator.h"
#include "src/phonetic/wordstrie.h"
#include "src/utils/utilities.h"

class MdiChild;
QT_BEGIN_NAMESPACE
class QAction;
class QMenu;
class QMdiArea;
class QMdiSubWindow;
class QSignalMapper;
class QTextCharFormat;
class QTimer;
class QToolTip;
class QComboBox;
class QFontComboBox;
class QTextCursor;
class QCompleter;
class QAbstractItemModel;
class QAbstractButton;
QT_END_NAMESPACE

typedef enum _FILETYPE { NEW, TRIAL, INFO } FILETYPE;

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow();

  QMdiArea *editor;

protected:
  void closeEvent(QCloseEvent *event);

public slots:
  // void triggeredChar(QAbstractButton*);
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
  // void textColor();

  void textFamily(const QString &f);
  void textSize(const QString &p);
  void textQCompleterFont(const QString &p);
  void mergeFormatOnWordOrSelection(const QTextCharFormat &format);
  // void colorChanged(const QColor &c);

  QAbstractItemModel *modelFromFile(const QString &fileName);

  void f2Action();
  void f3Action();

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
  void createDevelopmentMenus();
  void createActions();
  void createMenus();
  void createToolBars();
  void createStatusBar();
  QMenu *getNewChildMenu();

  void readSettings();
  void writeSettings();
  MdiChild *activeMdiChild();
  QMdiSubWindow *findMdiChild(const QString &fileName);
  bool eventFilter(QObject *obj, QEvent *event);
  // void keyReleaseEvent(QKeyEvent *kv);
  // void keyPressEvent(QKeyEvent *kv);
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
  QAction *kbdDisplayAct;

  QAction *lAlign;
  QAction *rAlign;
  QAction *cAlign;
  QAction *jAlign;

  QAction *iText;
  QAction *bText;
  QAction *uText;

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
  QAction *hideToolTipMode;

  QComboBox *comboStyle;
  QFontComboBox *comboFont;
  QComboBox *comboSize;

  QCompleter *completer;
  QList<QString> choices;
  CharTrie *charMapTree;
  WordsTrie *wordsMapTree;

  ToolTipSingleton *tooltipSingleton;

  QHash<QString, bool> storedKeys;

protected:
  //   bool event(QEvent *event);

  bool event(QEvent *event);

private:
  void keyPressEvent(QKeyEvent *e);

private:

};

#endif
