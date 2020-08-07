#ifndef DICTIONARY_H
#define DICTIONARY_H
#include <QVector>
#include <QHash>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QDebug>
#include "src/utils/utilities.h"



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

    void loadAssameseEnglishDictionaries(
            QString IdeasFile,
            QString engWrdIdFile,
            QString asmWrdWrdId,
            QString examplesFile,
            QString idiomsFile,
            QString poribhashaFile);
};



class Dictionary
{
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
    QHash<QString, QVector<QString> > idioms, examples;
    QList<QPair<QString, QString> > officialWrd;

private:
    QHash<QString, unsigned int> tempHash, localTempHash;

    QHash<QString, QHash<QString, unsigned int> > words;
    QVector<QString> meanings;



};

#endif // DICTIONARY_H
