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
*****************************************************************************/

#ifndef WORDSTRIE_H
#define WORDSTRIE_H

#include "src/core/TreeNode.h"
#include "src/phonetic/phonetic.h"

#include "src/utils/utilities.h"
#include <QDebug>
#include <QFile>
#include <QHash>
#include <QList>
#include <QMessageBox>
#include <QStack>
#include <QStringList>
#include <QTextStream>
#include <QThread>

class WordsTrie {
public:
  TreeNode *charTree;

private:
  static QHash<QChar, QHash<QChar, float> *> *distances;

private:
  // methods
  void load_dictionary(QString, QHash<QString, QString> &);
  void load_dictionary_asm(QString, QHash<QString, QString> &);

  bool insertWord(TreeNode *, QList<QString> &, QString);
  void getWordsFromTree(TreeNode *, QStack<QString> &, QStringList &wordList);
  void initializeDistances();

  void get_choices_from_subtree(TreeNode *root, QList<QString> &choices,
                                QStack<QString> &wordstack, int max_choices);

  void get_choice_words(TreeNode *root, QList<QString> &choices,
                        QList<QString> &words, int choicesize, bool autoselect);

  bool _hasWord(TreeNode *root, QStringList &chars);
  bool _deleteWord(TreeNode *root, QStringList &chars);
  void getPossibleCandidates(TreeNode *root, QString uniWord,
                             QStack<QString> &leftStack,
                             QStack<QString> &rightStack,
                             QStack<QString> &partWord,
                             QStringList &candidateList);
  static float wordDistance(QString s, QString t);

public:
  WordsTrie();
  void LoadDictionaryWords(QString);
  void get_choice(QStringList prefix, int choicesize, bool autoselect,
                  QList<QString> &choices);

  static WordsTrie *getWordsTrie();
  static WordsTrie *wordMapTree;

  static WordsTrie *getProfileWordsTrie();
  static WordsTrie *profileWordMapTree;
  static QString dictionaryFile;

  void LoadProfileDictionaryWords(QHash<QString, QString> charMap);
  //  void printData();

  void getWordList(QStringList &wordList);

  void addWords(QHash<QString, QString> charMap);

  bool hasWord(QString word);

  bool deleteWord(const QString &word);

  void delete_Tree(TreeNode *tree);

  void getPossibleWords(QString word, QList<QKeyFloat> &possibleList,
                        unsigned int numMax);
};

#endif // WORDSTRIE_H
