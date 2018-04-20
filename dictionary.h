#ifndef DICTIONARY_H
#define DICTIONARY_H
#include <QVector>
#include <QHash>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QDebug>

class Dictionary;


class Dictionaries
{
private:
    Dictionaries();
    static Dictionaries *dictionaries;
    Dictionary *assamese, *english, *assamesePoribhasha, *englishPoribhasha;

public:
    static Dictionaries *getDictionaries();
    void setAssameseEnglishDictionaries(QString dictionaryFile);
    void setPoriBhashaDictionaries(QString poribhashaFile) ;
    Dictionary *getDictionary(unsigned int i);

};



class Dictionary
{
public:
    Dictionary();
    Dictionary(QString dictionaryFile, QString poribhashaFile);
    void set_New_Entry(const QString str);
    void set_New_Entry_Reversed(const QString str);
    QStringList getMeanings(const QString str);
    QStringList getWords();
    bool hasWord(QString word);


private:
    QHash<QString, unsigned int> tempHash, localTempHash;

    QHash<QString, QHash<QString, unsigned int> > words;
    QVector<QString> meanings;


};

#endif // DICTIONARY_H
