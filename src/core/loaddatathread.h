#ifndef LOADDATATHREAD_H
#define LOADDATATHREAD_H

#include <QThread>
#include <QRunnable>
#include <QDebug>

#include <QApplication>
#include <QDesktopWidget>

#ifdef QT_VERSION_5
#include <QApplication>
#else
#include <QtGui/QApplication>
#endif


#include "src/phonetic/phonetic.h"
#include "src/phonetic/wordstrie.h"
#include "src/core/inflexTrie.h"
#include "src/phonetic/romanization.h"
#include "src/dictionary/dictionary.h"

class LoadDataThread : public QThread
{
    Q_OBJECT
    void run (){
        qDebug() << "Loading data.....";

        Romanization::InitializeMaps();
        //QString dictionaryFile = ":/files/processed_dictionary.txt";
        QString dictionaryFile = ":/files/T_WrdASMIdea.csv";

        // dictionaryFile = ":/files/processed_temp.txt";
        //dictionaryFile = ":/files/processed_dictionary.txt";
      //  dictionaryFile  = ":/files/processed_dictionary.prefix_len_5.txt";

    //    dictionaryFile = ":/files/processed_dictionary_Sumu_corrected_Feb_2013.txt";

        QString unicodeToRomanOverrideFile = ":/files/unicode_to_roman_override.txt";

        Romanization::InitializeUnicodeToRomanOverrideMaps(unicodeToRomanOverrideFile);

        WordsTrie *wordsMapTree = WordsTrie::getWordsTrie();

        wordsMapTree->LoadDictionaryWords(dictionaryFile);


        Phonetic::setInflexTypes(":/files/inflexions_comb.txt");
        Phonetic::createSingleInflections();
        Phonetic::createInflexCombinations();

        Phonetic::initializeCharPhoneticMap();
        Phonetic::loadAllWords(dictionaryFile );

        Phonetic::createPhoneticTree(dictionaryFile);
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

        LoadDictionary(
                    IdeasFile,
                    engWrdIdFile,
                    asmWrdWrdId,
                    examplesFile,
                    idiomsFile,
                    poribhashaFile
                  );



        //inflexTree->printData();
    }
public:
    explicit LoadDataThread(QObject *parent = 0);
    
signals:
    
public slots:

private:
    void LoadDictionary(
            QString IdeasFile,
            QString engWrdIdFile,
            QString asmWrdWrdId,
            QString examplesFile,
            QString idiomsFile,
            QString poribhashaFile
         );
};


class ShowTutorialThread : public QRunnable
{
    void run (){
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
