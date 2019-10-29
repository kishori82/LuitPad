#ifndef ASCIITRIE_H
#define ASCIITRIE_H

#include "src/core/TreeNode.h"
#include <QHash>
#include <QList>
#include <QTextStream>
#include <QStringList>
#include <QMessageBox>
#include <QFile>
#include <QDebug>
#include <QStack>
#include <QStringList>
//#include "Translator.h"



class AsciiTrie
{
public:
     TreeNode *asciiTree;

private:
    //variables
 //   TreeNode *asciiTree;
   // TreeNode *profileWordsTree;

    // methods

    bool insertWord(TreeNode * , QList<QString> & , QString );
    void printTree( TreeNode *, QStack<QString> &);


    void get_choices_from_subtree(TreeNode *root, QList<QString> & choices, QStack<QString> & wordstack, int max_choices);

    void get_choice_words(TreeNode *root,  QList<QString> & choices, QList<QString> & words, int choicesize, bool autoselect);

    int _findPrefix(TreeNode *root,  QStringList chars);
    bool _deleteWord(TreeNode *root,  QStringList & chars);

public:
    AsciiTrie();

    void get_choice( QStringList prefix, int choicesize, bool autoselect, QList<QString> & choices);

    static AsciiTrie *getAsciiTrie();
    static AsciiTrie *getUnicodeTrie();
    static AsciiTrie *wordMapTree;
    static AsciiTrie *unicodeTree;

    static AsciiTrie *getProfileAsciiTrie();

    void printData();

    void addWords(QHash<QString,QString> charMap);

    QString findPrefix(QStringList &chars);


    void delete_Tree(TreeNode *tree );

};

#endif //ASCIITRIE_H
