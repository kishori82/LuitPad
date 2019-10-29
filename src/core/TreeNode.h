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
