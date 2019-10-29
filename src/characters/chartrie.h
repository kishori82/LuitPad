#ifndef CHARTRIE_H
#define CHARTRIE_H

#include <QHash>
#include <QList>
#include <QTextStream>
#include <QStringList>
#include <QMessageBox>
#include <QFile>
#include <QDebug>
#include <QStack>
//#include "profile.h"

#include "src/core/constants.h"
#include "src/core/TreeNode.h"





class CharTrie
{

private:
    //variables
    TreeNode *charTree;
    CharTrie();
    // methods
    void load_saved_maps(QString, QHash<QString,QString> &);
    bool insertWord(TreeNode * , QList<QString> & , QString );
    void printTree( TreeNode *, QStack<QString> &);


    void get_choices_from_subtree(TreeNode *root, QList<QKeyValue> & choices, QStack<QString> & wordstack, int max_choices);

    void get_choice_words(TreeNode *root,  QList<QKeyValue> & choices, QList<QString> & words, QStack<QString> wordstack, int choicesize, bool autoselect);

    QString  _getPrefixMatch(TreeNode *root,  QList<QString> &prefix_chars_list);
    void _whatPrefixMatch(TreeNode *root,  QList<QString> &prefix_chars_list, QString &whatMatch);

//	private static PrefixTreeMap Singleton_PrefixTree = null;
public:
    void LoadCharTreeFromProfile(QHash<QString, QString> &charMap);
    void LoadCharTreeMap(QString);
    void get_choice( QString prefix, int choicesize, bool autoselect, QList<QKeyValue> & choices);

    bool doesPrefixMatch( QString prefix );
    QString getPrefixMatch( QString prefix );
    QString whatPrefixMatch( QString prefix );
    bool foundPrefixMatch(TreeNode *root,  QList<QString> &prefix_chars_list);

    static CharTrie *getCharTrie();

    static CharTrie *charMapTree ;
};

#endif // CHARTRIE_H
