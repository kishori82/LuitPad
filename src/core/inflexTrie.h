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
*************************************************************************/

#ifndef INFLEXTRIE_H
#define INFLEXTRIE_H

#include "TreeNode.h"
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

class InflexTrie {
public:
  TreeNode *inflexTreeNode;

private:
  // methods
  bool insertWord(TreeNode *, QList<QString> &, QString);
  void printTree(TreeNode *, QStack<QString> &);

  QString _hasInflection(TreeNode *root, QStringList &chars);
  bool _deleteWord(TreeNode *root, QStringList &chars);

public:
  InflexTrie();
  void LoadInflections(QHash<QString, QString> *charMa);
  static InflexTrie *getInflexTrie();
  static InflexTrie *inflexMapTree;

  void printData();

  void addWords(QHash<QString, QString> charMap);

  QString hasInflection(QString word);

  bool deleteWord(const QString &word);

  void delete_Tree(TreeNode *tree);
};

#endif // WORDSTRIE_H
