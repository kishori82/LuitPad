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
***************************************************************************/

#ifndef CHARTRIE_H
#define CHARTRIE_H

#include <QDebug>
#include <QFile>
#include <QHash>
#include <QList>
#include <QMessageBox>
#include <QStack>
#include <QStringList>
#include <QTextStream>
//#include "profile.h"

#include "src/core/TreeNode.h"
#include "src/core/constants.h"

class CharTrie {

private:
  // variables
  TreeNode *charTree;
  CharTrie();
  // methods
  void load_saved_maps(QString, QHash<QString, QString> &);
  bool insertWord(TreeNode *, QList<QString> &, QString);
  void printTree(TreeNode *, QStack<QString> &);

  void get_choices_from_subtree(TreeNode *root, QList<QKeyValue> &choices,
                                QStack<QString> &wordstack, int max_choices);

  void get_choice_words(TreeNode *root, QList<QKeyValue> &choices,
                        QList<QString> &words, QStack<QString> wordstack,
                        int choicesize, bool autoselect);

  QString _getPrefixMatch(TreeNode *root, QList<QString> &prefix_chars_list);
  void _whatPrefixMatch(TreeNode *root, QList<QString> &prefix_chars_list,
                        QString &whatMatch);

  //	private static PrefixTreeMap Singleton_PrefixTree = null;
public:
  void LoadCharTreeFromProfile(QHash<QString, QString> &charMap);
  void LoadCharTreeMap(QString);
  void get_choice(QString prefix, int choicesize, bool autoselect,
                  QList<QKeyValue> &choices);

  bool doesPrefixMatch(QString prefix);
  QString getPrefixMatch(QString prefix);
  QString whatPrefixMatch(QString prefix);
  bool foundPrefixMatch(TreeNode *root, QList<QString> &prefix_chars_list);

  static CharTrie *getCharTrie();

  static CharTrie *charMapTree;
};

#endif // CHARTRIE_H
