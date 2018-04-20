#ifndef INFLEXTRIE_H
#define INFLEXTRIE_H

#include "TreeNode.h"
#include <QHash>
#include <QList>
#include <QTextStream>
#include <QStringList>
#include <QMessageBox>
#include <QFile>
#include <QDebug>
#include <QStack>
#include <QStringList>
#include "utilities.h"
#include <QThread>


class InflexTrie
{
public:
     TreeNode *inflexTreeNode;
private:

    // methods
    bool insertWord(TreeNode * , QList<QString> & , QString );
    void printTree( TreeNode *, QStack<QString> &);

    QString _hasInflection(TreeNode *root,  QStringList &chars);
    bool _deleteWord(TreeNode *root,  QStringList & chars);

public:
    InflexTrie();
    void LoadInflections( QHash<QString, QString> *charMa);
    static InflexTrie *getInflexTrie();
    static InflexTrie *inflexMapTree;


    void printData();

    void addWords(QHash<QString,QString> charMap);

    QString hasInflection(QString word);

    bool deleteWord(const QString &word);

    void delete_Tree(TreeNode *tree );

};

#endif // WORDSTRIE_H
