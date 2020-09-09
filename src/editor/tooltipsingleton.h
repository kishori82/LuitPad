/***************************************************************************
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

#ifndef TOOLTIPSINGLETON_H
#define TOOLTIPSINGLETON_H

#include <QChar>
#include <QList>
#include <QString>
#include <QTextCursor>

#include "src/core/constants.h"
#include "src/editor/textedit.h"
#include "src/utils/utilities.h"
#include <QDebug>

class ToolTipSingleton {
private:
  ToolTipSingleton();
  QList<QString> charlist;
  QList<QKeyValue> keyvaluelist;
  int index;

  int row, col;
  int rowSizes[3];
  QList<QString> vowelList[3];

public:
  APP_STATES state;
  static ToolTipSingleton *object;

  static ToolTipSingleton *GetToolTipSingleton(QString char_freq = "");

  APP_STATES getState();
  void setState(APP_STATES state);

  int SetCharacterList(QList<QString> list);
  QString charPrefix(TextEdit *);
  QString trimRomanDigitSuffix(TextEdit *textDocument);
  QString consonantPrefix(TextEdit *);
  bool hasAssamesePrefix(TextEdit *textDocument);
  QString getAssamesePrefix(TextEdit *textDocument);

  QString charMidPrefix(TextEdit *);
  void addChar(QString c);
  void addQKeyValue(QKeyValue kv);
  void clearCharList();
  void clearKeyValueList();
  void sortByUsage();

  QString createToolTipText();

  void setSelectCharacters(const QList<QKeyValue> &choices);

  QStringList createQcompleterList();

  unsigned int getSize();
  void resetIndex();
  void incrementIndex();
  void decrementIndex();
  bool isEmptyCharList();
  QString getAtIndex();
  QList<QKeyValue> getKeyValueList();

  QString createVowelModToolTipText();
  void resetRowCol();
  void incrementRow();
  void decrementCol();
  void incrementCol();
  void decrementRow();
  QString getAtRowCol();

private:
  static bool toAssending(QKeyValue &s1, QKeyValue &s2);
  static bool toDesending(QKeyValue &s1, QKeyValue &s2);
  static void initializeCharFrequency(QString char_freq_fileName);
  static QHash<QString, int> *charUsageFrequency;
};

#endif // TOOLTIPSINGLETON_H
