#include "loaddatathread.h"
LoadDataThread::LoadDataThread(QObject *parent) :
    QThread(parent)
{
}


void LoadDataThread::LoadDictionary(
        QString IdeasFile,
        QString engWrdIdFile,
        QString asmWrdWrdId,
        QString examplesFile,
        QString idiomsFile,
        QString poribhashaFile) {


    Dictionaries *dictionaries = Dictionaries::getDictionaries();

    dictionaries->loadAssameseEnglishDictionaries(
                IdeasFile,
                engWrdIdFile,
                asmWrdWrdId,
                examplesFile,
                idiomsFile,
                poribhashaFile
             );




    //dictionaries->setAssameseEnglishDictionaries(dictionaryFile);
    //dictionaries->setPoriBhashaDictionaries(poribhashaFile);
}


