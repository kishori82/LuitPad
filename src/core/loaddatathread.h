#ifndef LOADDATATHREAD_H
#define LOADDATATHREAD_H

#include <QThread>
#include <QRunnable>
#include <QDebug>

#include <QApplication>
#include <QDesktopWidget>
#include <QtGui/QApplication>

#include "src/phonetic/phonetic.h"
#include "src/phonetic/wordstrie.h"
#include "src/core/inflexTrie.h"
#include "src/phonetic/romanization.h"
#include "src/dictionary/dictionary.h"

class LoadDataThread : public QThread
{
    Q_OBJECT
    void run (){
     //   qDebug() << "Loading data.....";

        Romanization::InitializeMaps();
        QString dictionaryFile = ":/files/processed_dictionary.txt";
    //    dictionaryFile = ":/files/processed_dictionary_Sumu_corrected_Feb_2013.txt";


        QString unicodeToRomanOverrideFile = ":/files/unicode_to_roman_override.txt";

        Romanization::InitializeUnicodeToRomanOverrideMaps(unicodeToRomanOverrideFile);

    //    qDebug() << " Initialized maps";
        WordsTrie *wordsMapTree = WordsTrie::getWordsTrie();

        wordsMapTree->LoadDictionaryWords(dictionaryFile);

     //   qDebug() << " Loaded words";

        Phonetic::setInflexTypes(":/files/inflexions_comb.txt");
        Phonetic::createSingleInflections();
        Phonetic::createInflexCombinations();

        Phonetic::initializeCharPhoneticMap();
        Phonetic::loadAllWords(dictionaryFile );
    //    qDebug() << " Loaded all words";

        Phonetic::createPhoneticTree(dictionaryFile);
        Phonetic::initializeDistances();

        Phonetic::initializeDeleteCharMap();


        InflexTrie *inflexTree = InflexTrie::getInflexTrie();

        inflexTree->LoadInflections( Phonetic::singleInflexionsReverse);

        Utilities::initializeAlphabetOrder();

        LoadDictionary(":files/dictionary-file.txt", ":files/poribhasha-file.txt");

        //inflexTree->printData();

      //  qDebug() << "Finished loading data";

    }
public:
    explicit LoadDataThread(QObject *parent = 0);
    
signals:
    
public slots:

private:
    void LoadDictionary(QString dictionaryFile, QString poribhashaFile);

    
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
