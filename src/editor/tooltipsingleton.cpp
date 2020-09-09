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

#include "tooltipsingleton.h"

ToolTipSingleton *ToolTipSingleton::object = NULL;
QHash<QString, int> *ToolTipSingleton::charUsageFrequency = NULL;

ToolTipSingleton::ToolTipSingleton() {
  state = F2;

  this->resetRowCol();
  rowSizes[0] = 3;
  rowSizes[1] = 3;
  rowSizes[2] = 4;

  vowelList[0].append(QString("0x9be"));
  vowelList[0].append(QString("0x9bf"));
  vowelList[0].append(QString("0x9c0"));

  vowelList[1].append(QString("0x9c1"));
  vowelList[1].append(QString("0x9c2"));
  vowelList[1].append(QString("0x9c3"));

  vowelList[2].append(QString("0x9c7"));
  vowelList[2].append(QString("0x9c8"));
  vowelList[2].append(QString("0x9cb"));
  vowelList[2].append(QString("0x9cc"));
}

void ToolTipSingleton::setState(APP_STATES _state) { this->state = _state; }

APP_STATES ToolTipSingleton::getState() { return this->state; }

int ToolTipSingleton::SetCharacterList(QList<QString> list) {

  //    charlist = list;
  return charlist.size();
}

ToolTipSingleton *ToolTipSingleton::GetToolTipSingleton(QString char_freq) {
  if (object == 0) {
    object = new ToolTipSingleton();
    Utilities::initializeVowelMap();
    if (char_freq.length() > 0) {
      initializeCharFrequency(char_freq);
    }
  }

  return object;
}

void ToolTipSingleton::initializeCharFrequency(QString char_freq_fileName) {
  QFile file(char_freq_fileName);
  if (!file.open(QIODevice::ReadOnly)) {
    QMessageBox::information(0, "error", file.errorString());
  }

  charUsageFrequency = new QHash<QString, int>();

  QTextStream in(&file);
  QStringList list;

  QString line;
  while (!in.atEnd()) {
    line = in.readLine();
    list = line.split("\t");
    //  qDebug() << list;
    if (list.size() >= 1) {
      if (list.at(0).trimmed().size() != 0)
        charUsageFrequency->insert(list.at(0).trimmed().toLower(),
                                   list.at(1).trimmed().toInt());
    }
  }

  file.close();
}

void ToolTipSingleton::incrementIndex() {
  index++;
  index = index % keyvaluelist.size();
}

void ToolTipSingleton::decrementIndex() {

  index = index + keyvaluelist.size();
  index--;
  index = index % keyvaluelist.size();
}

void ToolTipSingleton::incrementRow() {
  row++;
  row = row % 3;
}

void ToolTipSingleton::decrementRow() {
  row = row + 3;
  row--;
  row = row % 3;
}

void ToolTipSingleton::incrementCol() {
  col++;
  col = col % rowSizes[row];
}

void ToolTipSingleton::decrementCol() {
  col = col + rowSizes[row];
  col--;
  col = col % rowSizes[row];
}

unsigned int ToolTipSingleton::getSize() { return keyvaluelist.size(); }

QString ToolTipSingleton::getAtIndex() {
  if (index > -1 && keyvaluelist.size() > 0)
    return (keyvaluelist.at(index).value);
  else
    return ("");
}

QString ToolTipSingleton::getAtRowCol() {
  //   qDebug() << "vowel " << vowelList[row].at(col);
  return (vowelList[row].at(col));
}

QList<QKeyValue> ToolTipSingleton::getKeyValueList() {
  return this->keyvaluelist;
}

QString ToolTipSingleton::getAssamesePrefix(TextEdit *textDocument) {
  if (!this->hasAssamesePrefix(textDocument))
    return QString("");
  QTextCursor tc = textDocument->textCursor();
  tc.movePosition(QTextCursor::Left);
  tc.select(QTextCursor::WordUnderCursor);
  return tc.selectedText();
}

bool ToolTipSingleton::hasAssamesePrefix(TextEdit *textDocument) {
  QTextCursor tc = textDocument->textCursor();

  tc.movePosition(QTextCursor::Left);
  tc.select(QTextCursor::WordUnderCursor);
  QString word = Utilities::reverse(tc.selectedText());
  unsigned int romanCount = 0;
  for (int i = 0; i < word.size() && word.at(i).isLetter(); ++i) {
    romanCount++;
  }
  return (romanCount < word.size() ? true : false);
}

QString ToolTipSingleton::trimRomanDigitSuffix(TextEdit *textDocument) {

  QTextCursor tc = textDocument->textCursor();

  tc.movePosition(QTextCursor::Left);

  tc.select(QTextCursor::WordUnderCursor);

  QString word = tc.selectedText();
  // QChar *data= tc.selectedText().data() ;

  unsigned int j = 0;
  for (int i = word.size() - 1;
       (i >= 0 && Utilities::isRomanAlphabetDigit(word.at(i))); i--) {
    //  qDebug() << QChar(word.at(i)).unicode();
    j++;
  }
  // qDebug() << "Number of suffix to trim" << j;
  return word.left(word.size() - j);
}

QString ToolTipSingleton::charPrefix(TextEdit *textDocument) {

  QTextCursor tc = textDocument->textCursor();

  tc.movePosition(QTextCursor::Left);

  tc.select(QTextCursor::WordUnderCursor);

  QString word = Utilities::reverse(tc.selectedText());
  // QChar *data= tc.selectedText().data() ;

  QString keyChar = "";

  for (int i = 0; i < word.size() && word.at(i).isLetterOrNumber(); ++i) {
    keyChar.append(word.at(i));
  }

  return Utilities::reverse(keyChar);
}

QString ToolTipSingleton::consonantPrefix(TextEdit *textDocument) {

  QTextCursor tc = textDocument->textCursor();

  tc.movePosition(QTextCursor::Left);

  tc.select(QTextCursor::WordUnderCursor);

  QString word = Utilities::reverse(tc.selectedText());
  // QChar *data= tc.selectedText().data() ;

  QString keyChar = "";

  for (int i = 0; i < word.size() && !word.at(i).isLetter(); ++i) {
    keyChar.append(word.at(i));
  }
  return Utilities::reverse(keyChar);
}

QString ToolTipSingleton::charMidPrefix(TextEdit *textDocument) {

  QTextCursor tc = textDocument->textCursor();

  tc.movePosition(QTextCursor::Left);

  tc.select(QTextCursor::WordUnderCursor);

  QString word = Utilities::reverse(tc.selectedText());
  // QChar *data= tc.selectedText().data() ;

  QString keyChar = "";

  for (int i = 0; i < word.size(); ++i) {
    if (word.at(i).isLetter() || word.at(i).isDigit())
      keyChar.append(word.at(i));
  }

  return Utilities::reverse(keyChar);
}

void ToolTipSingleton::clearCharList() {
  charlist.clear();
  index = -1;
}

bool ToolTipSingleton::isEmptyCharList() {
  if (keyvaluelist.size() > 0)
    return false;
  else
    return true;
}

void ToolTipSingleton::clearKeyValueList() {
  keyvaluelist.clear();
  index = -1;
}

void ToolTipSingleton::addChar(QString c) { charlist.append(c); }

void ToolTipSingleton::addQKeyValue(QKeyValue kv) { keyvaluelist.append(kv); }

bool ToolTipSingleton::toAssending(QKeyValue &s1, QKeyValue &s2) {
  return (ToolTipSingleton::charUsageFrequency->value(s1.value) <
          ToolTipSingleton::charUsageFrequency->value(s2.value));
}

bool ToolTipSingleton::toDesending(QKeyValue &s1, QKeyValue &s2) {

  return (ToolTipSingleton::charUsageFrequency->value(s1.value) >
          ToolTipSingleton::charUsageFrequency->value(s2.value));
}
void ToolTipSingleton::sortByUsage() {

  qSort(keyvaluelist.begin(), keyvaluelist.end(),
        &ToolTipSingleton::toDesending);
}

void ToolTipSingleton::resetIndex() {
  if (keyvaluelist.size() > 0)
    index = 0;
  else
    index = -1;
}

void ToolTipSingleton::resetRowCol() {
  row = 1;
  col = 1;
}

bool toAsendingLength(QKeyValue &s1, QKeyValue &s2) {
  return s1.key.size() < s2.key.size();
}

QStringList ToolTipSingleton::createQcompleterList() {

  QStringList charList;

  qSort(keyvaluelist.begin(), keyvaluelist.end(), &toAsendingLength);

  for (int i = 0; i < keyvaluelist.size(); i++) {
    charList.append(Utilities::getUnicode(keyvaluelist.at(i).value, "0x") +
                    "\t" + keyvaluelist.at(i).key);
  }
  return charList;
}

void ToolTipSingleton::setSelectCharacters(const QList<QKeyValue> &choices) {
  QKeyValue keyValue;
  for (QList<QKeyValue>::const_iterator it = choices.begin();
       it != choices.end(); it++) {
    this->addQKeyValue(*it);
    if (Utilities::inverseVowelMap->contains(it->value)) {
      keyValue.key = it->key;
      keyValue.value = Utilities::inverseVowelMap->value(it->value);
      this->addQKeyValue(keyValue);
    }
  }
}

QString ToolTipSingleton::createToolTipText() {

  QString text = toolTipPrefix;
  text += "<table border=\"0\" align=\"center\" bgcolor=\"#FFFFCa\"  >";

  text += "<tr>";
  for (int i = 0; i < keyvaluelist.size(); i++) {
    text += "<td  style=\"color:#0000ff\">";
    text += keyvaluelist.at(i).key;
    text += " ";
    text += "</td>";
  }
  text += "</tr>";

  text += "<tr>";

  for (int i = 0; i < keyvaluelist.size(); i++) {
    text += "<td  style=\"color:#0000ff\" >";
    if (index == i)
      text += "<span style=\"background-color: #99FF66\">";
    text += Utilities::getUnicode(keyvaluelist.at(i).value, "0x");
    if (index == i)
      text += "</span>";
    text += "</td>";
  }

  text += "</tr>";
  text += "</table>";
  text += toolTipSuffix;

  return text;
}

QString ToolTipSingleton::createVowelModToolTipText() {

  QString text;

  text = toolTipPrefix;
  text += "<table border=\"0\" align=\"center\" bgcolor=\"#00FF00\">";

  for (int i = 0; i < 3; i++) {
    text += "<tr>";
    for (int j = 0; j < vowelList[i].size(); j++) {
      text += "<td  style=\"color:#8C001A\">";
      if (row == i && col == j)
        text += "<span>";
      text += Utilities::getUnicode(vowelList[i].at(j), "0x");
      if (row == i && col == j)
        text += "</span>";
      text += " ";
      text += "</td>";
    }
    text += "</tr>";
  }

  text += "</table>";
  text += toolTipSuffix;
  //   text += "<font size=\"30\">" + text + "</font>";

  return text;
}
