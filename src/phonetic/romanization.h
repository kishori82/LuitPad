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
****************************************************************************/

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
