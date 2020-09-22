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

#ifndef LOADDATATHREAD_H
#define LOADDATATHREAD_H

#include <QDebug>
#include <QRunnable>
#include <QThread>

#include <QApplication>
#include <QDesktopWidget>

#ifdef QT_VERSION_5
#include <QApplication>
#else
#include <QtGui/QApplication>
#endif

#include "src/core/inflexTrie.h"
#include "src/dictionary/dictionary.h"
#include "src/phonetic/phonetic.h"
#include "src/phonetic/romanization.h"
#include "src/phonetic/wordstrie.h"

class LoadDataThread : public QThread {
  Q_OBJECT
  void run() {
    qDebug() << "Loading data.....";

    Romanization::InitializeMaps();
    // QString dictionaryFile = ":/files/processed_dictionary.txt";
    QString dictionaryFile = ":/files/T_WrdASMIdea.csv";

    QString unicodeToRomanOverrideFile =
        ":/files/unicode_to_roman_override.txt";

    Romanization::InitializeUnicodeToRomanOverrideMaps(
        unicodeToRomanOverrideFile);

    WordsTrie *wordsMapTree = WordsTrie::getWordsTrie();

    wordsMapTree->LoadDictionaryWords(dictionaryFile);

    Phonetic::initUserWordHashes();
    Phonetic::setInflexTypes(":/files/inflexions_comb.txt");

    Phonetic::createSingleInflections();
    Phonetic::createInflexCombinations();

    Phonetic::initializeCharPhoneticMap();
    Phonetic::loadAllWords(dictionaryFile);

    // add the phonetic words from the dictionary only for each character
    Phonetic::createPhoneticTree(dictionaryFile);

    // add user defined words and character mappings 
    // Phonetic::addUserWordsToPhoneticTree("profile/" + QLatin1String("DEFAULT.dat"));

    Phonetic::initializeDistances();

    Phonetic::initializeDeleteCharMap();

    InflexTrie *inflexTree = InflexTrie::getInflexTrie();

    inflexTree->LoadInflections(Phonetic::singleInflexionsReverse);
    Utilities::initializeAlphabetOrder();
    QString IdeasFile = ":/files/T_IdeaBase.tsv";
    QString engWrdIdFile = ":/files/T_WrdENGIdea.csv";
    QString asmWrdWrdId = ":/files/T_WrdASMIdea.csv";
    QString examplesFile = ":/files/T_WrdExamples.tsv.compressed";
    QString idiomsFile = ":/files/T_Idioms.tsv";
    QString poribhashaFile = ":/files/T_Poribhasha.tsv";

    LoadDictionary(IdeasFile, engWrdIdFile, asmWrdWrdId, examplesFile,
                   idiomsFile, poribhashaFile);

    // inflexTree->printData();
  }

public:
  explicit LoadDataThread(QObject *parent = 0);

signals:

public slots:

private:
  void LoadDictionary(QString IdeasFile, QString engWrdIdFile,
                      QString asmWrdWrdId, QString examplesFile,
                      QString idiomsFile, QString poribhashaFile);
};

class ShowTutorialThread : public QRunnable {
  void run() {
    showTutorial();
    qDebug() << "Debug in the thread";
  }

public:
  explicit ShowTutorialThread(QObject *parent = 0);

private:
  void center(QWidget &widget);
  void showTutorial();
};

#endif // LOADDATATHREAD_H
