/*************************************************************************
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
***************************************************************************/

#ifndef DICTIONARY_H
#define DICTIONARY_H
#include "src/utils/utilities.h"
#include <QDebug>
#include <QFile>
#include <QHash>
#include <QMessageBox>
#include <QTextStream>
#include <QVector>

class Dictionary;

class Dictionaries {
private:
  Dictionaries();
  static Dictionaries *dictionaries;
  Dictionary *assamese, *english, *assamesePoribhasha, *englishPoribhasha;

public:
  static Dictionaries *getDictionaries();
  void setAssameseEnglishDictionaries(QString dictionaryFile);
  void setPoriBhashaDictionaries(QString poribhashaFile);
  Dictionary *getDictionary(unsigned int i);

  void loadAssameseEnglishDictionaries(QString IdeasFile, QString engWrdIdFile,
                                       QString asmWrdWrdId,
                                       QString examplesFile, QString idiomsFile,
                                       QString poribhashaFile);
};

class Dictionary {
public:
  Dictionary();
  Dictionary(QString dictionaryFile, QString poribhashaFile);
  void set_New_Entry(const QString str);
  void set_New_Entry_Reversed(const QString str);

  QStringList getWords();
  bool hasWord(QString word);

  QStringList getMeanings(const QString str);
  QStringList getIdioms(const QString str);
  QStringList getExamples(const QString str);
  QStringList getOfficialUse(const QString str);
  QStringList getSynonyms(const QString str);

  QHash<QString, QString> asmWrdIdIdeaId, engWrdIdIdeaId;
  QHash<QString, QString> asmWrdWrdId, asmWrdIdWrd;
  QHash<QString, QString> engWrdWrdId, engWrdIdWrd;
  QHash<QString, QString> asmIdeaIdIdea, engIdeaIdIdea;
  QHash<QString, QVector<QString>> idioms, examples;
  QList<QPair<QString, QString>> officialWrd;

private:
  QHash<QString, unsigned int> tempHash, localTempHash;

  QHash<QString, QHash<QString, unsigned int>> words;
  QVector<QString> meanings;
};

#endif // DICTIONARY_H
