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

#include "searchreplace.h"

SearchReplaceDialog *SearchReplaceDialog::search = NULL;
MdiChild *SearchReplaceDialog::document = NULL;

SearchReplaceDialog *SearchReplaceDialog::getSearchReplace(MdiChild *doc) {
  if (document != doc) {
    document = doc;
  }

  if (search == NULL) {
    search = new SearchReplaceDialog();
  }
  return search;
}

SearchReplaceDialog::SearchReplaceDialog() {

  searchNum = 0;

  QGridLayout *layout = new QGridLayout(this);

  search_box = new QLineEdit(this);
  replace_box = new QLineEdit(this);

  search_lbl = new QLabel(tr("Search "));
  replace_lbl = new QLabel(tr("Replace"));

  QPushButton *next = new QPushButton("Next", this);
  QPushButton *replace = new QPushButton("Replace", this);
  QPushButton *replaceAll = new QPushButton("Replace All", this);
  QPushButton *cancel = new QPushButton("Cancel", this);

  layout->addWidget(search_lbl, 0, 0);
  layout->addWidget(replace_lbl, 1, 0);

  layout->addWidget(search_box, 0, 1);
  layout->addWidget(replace_box, 1, 1);

  search_box->setText("");
  replace_box->setText("");

  layout->addWidget(next, 0, 2);
  layout->addWidget(replace, 1, 2);
  layout->addWidget(replaceAll, 2, 2);
  layout->addWidget(cancel, 3, 2);

  setLayout(layout);

  connect(next, SIGNAL(clicked()), SLOT(nextProcess()));
  connect(replace, SIGNAL(clicked()), SLOT(replaceProcess()));
  connect(replaceAll, SIGNAL(clicked()), SLOT(replaceAllProcess()));
  connect(cancel, SIGNAL(clicked()), SLOT(accept()));
}

void SearchReplaceDialog::nextProcess() {

  if (cursor.isNull())
    cursor = document->textCursor();
  //= document->document();
  QString origWord;
  QRegExp re;

  re.setPattern(search_box->text());

  if (!(cursor = document->document()->find(search_box->text(), cursor))
           .isNull()) {

    QTextCharFormat highlightFormat;
    highlightFormat.setBackground(Qt::blue);
    // highlightFormat.setUnderlineColor(Qt::darkGreen);
    // highlightFormat.setUnderlineStyle(QTextCharFormat::SpellCheckUnderline);
    // highlightCursor.mergeCharFormat(highlightFormat);

    // cursor.setCharFormat(highlightFormat);

    cursor.select(QTextCursor::WordUnderCursor);
    origWord = cursor.selectedText();

    //  cursor.insertText( origWord );
    // cursor.setCharFormat(highlightFormat);

    document->setTextCursor(cursor);
    document->ensureCursorVisible();

  } else {
    Utilities::warningBox("Cannot find any more instance of " +
                          search_box->text());
  }
}

void SearchReplaceDialog::replaceProcess() {

  if (cursor.isNull())
    cursor = document->textCursor();
  //= document->document();
  QString origWord;
  QRegExp re;

  re.setPattern(search_box->text());

  if (!(cursor = document->document()->find(search_box->text(), cursor))
           .isNull()) {
    cursor.select(QTextCursor::WordUnderCursor);
    origWord = cursor.selectedText();

    origWord = origWord.replace(re, replace_box->text());
    qDebug() << "One " << origWord;

    cursor.insertText(origWord);
  }
}

void SearchReplaceDialog::replaceAllProcess() {

  if (cursor.isNull())
    cursor = document->textCursor();
  QString origWord;
  QRegExp re;

  re.setPattern(search_box->text());

  unsigned int count = 0;

  while (!(cursor = document->document()->find(search_box->text(), cursor))
              .isNull()) {
    cursor.select(QTextCursor::WordUnderCursor);
    origWord = cursor.selectedText();

    origWord = origWord.replace(re, replace_box->text());
    //  qDebug() << "After " << origWord;

    cursor.insertText(origWord);
    count++;
  }
  Utilities::warningBox("Replaced " + QString::number(count) +
                        " instances of " + search_box->text() + " with " +
                        replace_box->text());

  /*
  for( QTextBlock currentBlock = document->document()->firstBlock();\
      currentBlock!=document->document()->lastBlock(); currentBlock=
  currentBlock.next()) {

      QTextBlock::iterator it;
      for (it = currentBlock.begin(); !(it.atEnd()); ++it) {
          QTextFragment fragment = it.fragment();
          if (fragment.isValid()) {
              if(fragment.charFormat().isCharFormat()  ) {
                  qDebug() << "new " << fragment.text();
              }
          }
      }

  }
  */
}
