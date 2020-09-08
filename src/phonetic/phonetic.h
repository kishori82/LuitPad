/**************************************************************************
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

#ifndef PHONETIC_H
#define PHONETIC_H

#include <QHash>
#include <QString>
#include <QStringList>
#include <QList>

#include <QTextStream>
#include <QMessageBox>
#include <QFile>
#include "src/core/constants.h"
#include <QDebug>
#include <qmath.h>
#include "romanization.h"
#include "src/core/TreeNode.h"
#include <QStack>
#include "src/core/inflexTrie.h"


class Phonetic
{
public:
    Phonetic();


    static QHash<QString, QList<QKeyValue> *> *inflexTypes;
    static QStringList *inflexCombinations;

    static QHash<QString, QString> *charPhoneticMap;

    static QHash<QString, QString> *allInflexions;
    static QHash<QString, QString> *singleInflexions;
    static QHash<QString, QString> *singleInflexionsReverse;
    static TreeNode *roman2UnicodeTree;
    static TreeNode *roman2UnicodeTreeProfile;

    static QList<TreeNode *> treeNodeList;
    static QList<TreeNode *> treeNodeListProfile;

    static QHash<QString, QString> *allWords;
    static QHash<QString, QString> *phoneticMap;
    static QHash<QString, QString> *deleteCharMap;

    static QStringList getInflectionalForms(QString newWord);
    static QStringList getInflectionalFormsX(QString newWord);
    static QHash<QString, QString> getInflectionalFormsPairs(QString newWord);
    static QString getRootWord(QString newWord);

    static void setInflexTypes(QString fileName);
    static void createInflexCombinations();
    static void createSingleInflections();
    static void initializeCharPhoneticMap();
    static void loadAllWords(QString fileName);
    static int LD(QString s, QString t);
    static QString replaceChars(QString a );


    static void clearForNewWord(QString newWord,QList<TreeNode *> *visitedNodeList, TreeNode *roman2UnicodeTreeGeneric);
    static bool toAsending( QWordRank & s1 , QWordRank & s2 );
    static bool toAsendingInflex( QKeyDistance& s1 , QKeyDistance & s2 );
    static void rankWords(QList<QWordUnicode> &words, QString word, int max=15);
    static bool markUsedWord(TreeNode * curNode, QStringList &charList, bool used, unsigned int depth);

    static void createPhoneticTree(QString fileName);
    static void createPhoneticTreeProfile(QStringList &wordList);
    static bool insertWord(TreeNode * curNode, QStringList & charList, QString unicodeWord);
    static bool insertWordFromOutside(QString unicodeWord);

    static void printTree( TreeNode *root, QStack<QString> & partword);
    static QString phoneticEquivString(QString orig);
    static QString processPhoneticInput(QString orig);
   // static void initializePhoneticMap();
    static void phoneticWordChoices(QString newWord, QList<QWordUnicode> &wordList, bool ProfileTree = false);
    static void phoneticWordChoicesLengthBased(QString newWord, QList<QWordUnicode> &wordList, bool ProfileTree = false);

    static void arrangeWordChoices(QList<QWordUnicode> &words, QStringList &wordList, QString newWord) ;
    static void phoneticInflexChoices(QString newWord, QStringList &wordList);
    static void searchRoman2UnicodeTree( TreeNode *root, QStack<QString> &seen, QStack<QString> &notSeen, QStack<QString> &word, QList<QWordUnicode> & words, int depth, int traversal_depth=0);
    static void childrenOfTree( TreeNode *root, QStack<QString> & partword, QList<QWordUnicode> &words, QString preWord,  int depth=0);
    static void initializeDistances();
    static void initializeDeleteCharMap();
    static float wordDistance(QString s, QString t);

private:
     static QHash<QChar, QHash<QChar, float> * > *distances;

};

#endif // PHONETIC_H
