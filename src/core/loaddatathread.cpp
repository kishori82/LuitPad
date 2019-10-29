#include "loaddatathread.h"
LoadDataThread::LoadDataThread(QObject *parent) :
    QThread(parent)
{
}


void LoadDataThread::LoadDictionary(QString  dictionaryFile, QString poribhashaFile) {
    Dictionaries *dictionaries = Dictionaries::getDictionaries();

    dictionaries->setAssameseEnglishDictionaries(dictionaryFile);

    dictionaries->setPoriBhashaDictionaries(poribhashaFile);


}


