
#include <QString>
#include <QStringList>
#include <QList>
#include <QDebug>
#include <QTextCursor>
#include "src/editor/textedit.h"
#include <QHash>
#include <QMessageBox>
#include <QLabel>
#include <QPixmap>
#include <QCryptographicHash>
#include <QThread>
#include <QPainter>

#include <QSize>
#include <QVector>

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <QBuffer>
#include <QFile>
#include "src/core/constants.h"


#ifndef UTILITIES_H
#define UTILITIES_H


float Min(float a, float b);

class Utilities
{
public:
    Utilities();

    static QString reverse(QString s);
    static QString reverseX(QString s);
    static QList<QString> split(QString str, QString delim);
    static void trimRomanEnd(QString &newWord);
    static QString getField(QString str, QString delim, unsigned int i);
    static QString  createStringFromCodeList(QList<QString> list);
    static QStringList  getStringListFromUnicodeString(QString unicodeString);

    static QString getCharacterUnicodeForHTML(QString string);
    static QString getHTMLStringFromMixedHexString(QString text);
    static QString getExamplesForToolTip(QString str);
    static QString getUnicode(QString str, QString delim);
    static QString getUnicodeForWebPage(QString str, QString delim);

    static bool isAnInflectionalForm(QString a, QString b) ;
    static QString wordUnderCursor(QTextEdit *textDocument);
    static QString padWithSpaces(QString s, unsigned int n) ;
    static bool isValidCompleterPrefix(QString word);
    static bool isAssamese( const QString &str );
    static QString createStringFromCodeList( QList<QString> list, int base) ;
    static QStringList getStringListFromHexString(QString word, QString delim, int len=4);

    static QString convertToHexString(ushort value);
    static bool YesNoDialog(QString infoText, QString confimText);
    static bool warningBox(QString infoText);

    static QHash<QString, QString> *vowelMap;  
    static QHash<QString, QString> *inverseVowelMap;
    static QHash<QString, QString> *consonantMap;
    static QHash<QString, bool> *zeroLengthChar;

    static QString getUnicodeString(QString str);
    static QString getUnicodeStringX(QString str);

    static void initializeVowelMap( );
    static void initializeConsonantMap( );
    static void initializeZeroLengthChars();
    static bool isRomanAlphabet(QChar c) ;
    static bool isRomanAlphabetDigit(QChar c) ;


    static bool isValidForComposition(QChar qchar);
    static bool isValidForVowelComposition(QChar qchar);
    static bool isDelinkCharacter(QChar qchar);

    static QString driveIDs();
    static QString driveID(QString driveLetter);

    static void CreateRandomVector(QPixmap &px, QVector<QPoint> &rVector);
    static void ShowSplashScreen(QString pngFile);
    static QLabel* ShowWaitScreen(QString pngFile);
    static QPixmap UpdateAlphaChannel(QPixmap &px, unsigned int i, unsigned int dissolve_span,  QVector<QPoint> &rVector);


    static QString readAmbiguousSpellings(QString fileName);


    static QString toLuitPadFormat(QTextDocument *doc, QString htmlCode,  QString ignoreWordString="") ;
    static QString toPdfConvertibleLuitPadFormat(QTextDocument *doc, QString htmlCode) ;
    static QString getImageFileName(QString str);
    static QString getImageDatainBase64(QTextDocument *doc, QString imageFileName);

    static bool IsTag(QString str, QString tag);
    static QString getAttribute(QString str,QString attr);

    static void writeStringToUnicode(QString fileName, QString output, unsigned int col=1);
   /*************************************/

    static bool toAsending( QKeyFloat &s1 , QKeyFloat &s2 );
    static bool toDesending( QKeyFloat & s1 , QKeyFloat & s2 );
    static void sortQKeyFloatList(QList<QKeyFloat> &list, bool Ascending= true);


    static QHash<QString, unsigned int> *alphabetOrder;
    static void initializeAlphabetOrder();
    static bool compareUnicode(QString &s1, QString &s2) ;
    static bool toUnicodeAsending( QString &s1 , QString &s2 );
    static bool toUnicodeDesending( QString &s1 , QString &s2 );
    static void sortQStringList(QStringList &list, bool Ascending) ;

    ///////////////////////////////////
    static bool bisargahAtEnd(QString &str);
    static bool bisargahAnywhere(QString &str);
    static bool hasCharAnywhere(QString &str, ushort uCode);
    static QString getSectorNumber(QString deiveLetter);
    static bool hyphenPresent(QString &str);
    static bool hoHoPresent(QString &str);
    static bool shortWord(QString &str, unsigned int len);
};

#endif // UTILITIES_H
