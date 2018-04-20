#ifndef Translator_H
#define Translator_H

#include <QString>
#include <QStringList>
#include <QList>
#include <QDebug>
#include <QTextCursor>

#include <QHash>
#include <QMessageBox>
#include <QStack>
#include "asciiTrie.h"

//#include "mdichild.h"
#include <QFile>

struct QKeyValuePair {
    QString ascii;
    unsigned int len;
    QKeyValuePair(QString key,  unsigned int length) {
        ascii = key;
        len = length;
    }

};



struct QLeftRightPair {
    QString left;
    QString right;
    QLeftRightPair(QString _left,  QString _right) {
        left = _left;
        right = _right;
    }

    QLeftRightPair() {
        left = QString("");
        right = QString("");
    }
};



class Translator
{
private:
    static Translator *translator;
public:
    Translator();

    static QString reverse(QString s);
    static QList<QString> split(QString str, QString delim);
    static QString  createStringFromCodeList(QList<QString> list);
    static QStringList  getStringListFromUnicodeString(QString unicodeString);
    static QString getUnicode(QString str, QString delim);


    static QString createStringFromCodeList( QList<QString> list, int base) ;
    static QStringList getStringListFromHexString(QString word, QString delim, int len=4);

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


    void initializeVowelModifierMap( );

    void initializeAsciiToUnicodeMap();
    void initializeMatraCombinations();
    void initializePrefixMatraCombinations();
    void initializeSuffixMatraCombinations();
    void initializeMatraCombinationsExt();
    void initializeRomanAsciiMap();
    void intializeAsciiTree();
    void intializeUnicodeTree();

    static QString handle0x9cd(QString unicodeChar, QStack<QString>& matraStack, QString &output);


    static bool isValidAssameseChar(QChar qchar);
   // QStringList getAssameseWords(QString line);
    QString readGeetanjaliFile(QString fileName);
    void createAsciiWords(QString inputString, QList<QString> &words);
    static QString getCode(QString line, QString delim="");
    QString translateToUnicode(QString word);

    QString translateAssamese(QString &word);
    QString translateNumericAndAssamese(QString &word);

     void createUnicodeWords(QString string, QList<QString> &words ) ;
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
