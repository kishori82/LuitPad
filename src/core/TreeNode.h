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
***************************************************************************/

#ifndef PREFIXTREECHAR_H
#define PREFIXTREECHAR_H

#include <QHash>
#include <QStringList>
#include <QStack>
#include <QDebug>


class TreeNode {

public:
    TreeNode();
    TreeNode(QString, bool);

    QStringList roman2UnicodeList;
    bool used;

    QString unicode;
    QHash<QString, TreeNode *> links;
    bool fullWord;
    int children;
    static void delete_Tree(TreeNode *tree );
    static void printData(TreeNode *asciiTree);
    static void printTree( TreeNode *root, QStack<QString> & partword);
private:


};

#endif // PREFIXTREECHAR_H
