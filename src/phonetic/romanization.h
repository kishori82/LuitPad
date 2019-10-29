#ifndef ROMANIZATION_H
#define ROMANIZATION_H
#include <QStringList>
#include <QString>
#include <QStringList>
#include <QFile>
#include <QIODevice>
#include <QMessageBox>
#include <QTextStream>
#include <QHash>
#include <QRegExp>
#include <QDebug>
#include "src/utils/utilities.h"
#include "src/core/constants.h"
#include "phonetic.h"

class Romanization
{

public:
    Romanization();
    void Romanize(QString inputFile, QHash<QString, QStringList> &roman2UnicodeMap) ;
    static QString convert2Roman(QString strC);
    static void InitializeMaps();
    static void InitializeUnicodeToRomanOverrideMaps(QString fileName);
private:
    static bool isEndWithVowel(QString token);
    static bool isVowelModifier(QString ch);
    static QString removeLastVowel(QString token);
    static bool isConjunctChar(QString ch);
   // static bool insertWord(QString &word);

private:
    static QStringList vowels;
    static QStringList Unicode;
    static QStringList Rom;
    static QStringList dlink_Rom;
    static QStringList vMod;
    static QHash<QString, QString> UnicodeToRomanMap;
//    static QHash<QString, QString> UnicodeToRomanOverrideMap;
    static QHash<QString, QString> UnicodeToRomanMapInit;
    static QHash<QString, QString> UnicodeToDlinkRomanMap;
    static QHash<QString, QHash<QString, QString> > ForwardAlterSound;
    static QHash<QString, QHash<QString, QString> > BackwardAlterSound;

public:
     static QHash<QString, QString> UnicodeToRomanOverrideMap;
     static QHash<QString, QStringList> UnicodeToRomanOverrideList;

};

#endif // ROMANIZATION_H
