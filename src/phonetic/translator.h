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

#ifndef Translator_H
#define Translator_H

#include <QDebug>
#include <QList>
#include <QString>
#include <QStringList>
#include <QTextCursor>

#include "src/characters/asciiTrie.h"
#include <QHash>
#include <QMessageBox>
#include <QStack>

//#include "mdichild.h"
#include <QFile>

struct QKeyValuePair {
  QString ascii;
  unsigned int len;
  QKeyValuePair(QString key, unsigned int length) {
    ascii = key;
    len = length;
  }
};

struct QLeftRightPair {
  QString left;
  QString right;
  QLeftRightPair(QString _left, QString _right) {
    left = _left;
    right = _right;
  }

  QLeftRightPair() {
    left = QString("");
    right = QString("");
  }
};

class Translator {
private:
  static Translator *translator;

public:
  Translator();

  static QString reverse(QString s);
  static QList<QString> split(QString str, QString delim);
  static QString createStringFromCodeList(QList<QString> list);
  static QStringList getStringListFromUnicodeString(QString unicodeString);
  static QString getUnicode(QString str, QString delim);

  static QString createStringFromCodeList(QList<QString> list, int base);
  static QStringList getStringListFromHexString(QString word, QString delim,
                                                int len = 4);

  static QString convertToHexString(ushort value);

  static QHash<QString, QString> *vowelModifierMap;

  static QHash<QString, QString> *unicode2AsciiMap;
  static QHash<QString, QString> *unicode2AsciiMapExt;
  static QHash<QString, QString> *unicode2AsciiMatraRightMap;
  static QHash<QString, QString> *ascii2UnicodeMatraRightMap;
  static QHash<QString, QString> *unicode2AsciiMatraLeftMap;
  static QHash<QString, QString> *ascii2UnicodeMatraLeftMap;

  static QHash<QString, QString> *unicode2AsciiMatraRefMap;
  static QHash<QString, QString> *ascii2UnicodeMatraRefMap;

  static QHash<QString, QString> *ascii2UnicodeExoticMatraMap;
  static QHash<QString, QString> *unicode2AsciiExoticMatraMap;

  static QHash<QString, QString> *numericUnicode2AsciiMap;
  static QHash<QString, QString> *numericAscii2UnicodeMap;

  static QHash<QString, QString> *ascii2UnicodeMap;
  static QList<QKeyValuePair> *ascii2UnicodeList;
  static QList<QKeyValuePair> *unicode2AsciiList;

  static QHash<QString, QString> *romanAsciiMap;
  static QHash<QString, QLeftRightPair> *unicode2MatraCombMap;
  static QHash<QString, QLeftRightPair> *unicode2PrefixMatraCombMap;
  static QHash<QString, QLeftRightPair> *unicode2SuffixMatraCombMap;

  static QString getUnicodeString(QString str);

  static bool isRoman(QString word);
  static bool isNumeric(QString word);
  static QString translateNumeric(QString word);
  static QString translateRoman(QString word);

  void initializeUnicodeToAsciiMap();
  void initializeUnicodeToAsciiMapExt();

  void initializeVowelModifierMap();

  void initializeAsciiToUnicodeMap();
  void initializeMatraCombinations();
  void initializePrefixMatraCombinations();
  void initializeSuffixMatraCombinations();
  void initializeMatraCombinationsExt();
  void initializeRomanAsciiMap();
  void intializeAsciiTree();
  void intializeUnicodeTree();

  static QString handle0x9cd(QString unicodeChar, QStack<QString> &matraStack,
                             QString &output);

  static bool isValidAssameseChar(QChar qchar);
  // QStringList getAssameseWords(QString line);
  QString readGeetanjaliFile(QString fileName);
  void createAsciiWords(QString inputString, QList<QString> &words);
  static QString getCode(QString line, QString delim = "");
  QString translateToUnicode(QString word);

  QString translateAssamese(QString &word);
  QString translateNumericAndAssamese(QString &word);

  void createUnicodeWords(QString string, QList<QString> &words);
  QString translateTextToUnicode(QString word);
  QString translateTextToAscii(QString word);
  QString translateToAscii(QString word);
  bool isUnicodeEnglish(QString word);
  bool isUnicodeNumeric(QString word);
  QString translateToAssameseAscii(QString word);
  QString translateToEnglishAscii(QString word);
  QString translateToNumericAscii(QString word);

  static Translator *getTranslator();
};

#endif // Translator_H
