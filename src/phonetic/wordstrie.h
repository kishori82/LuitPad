#ifndef WORDSTRIE_H
#define WORDSTRIE_H

#include "src/core/TreeNode.h"
#include "src/phonetic/phonetic.h"

#include <QHash>
#include <QList>
#include <QTextStream>
#include <QStringList>
#include <QMessageBox>
#include <QFile>
#include <QDebug>
#include <QStack>
#include <QStringList>
#include "src/utils/utilities.h"
#include <QThread>


class WordsTrie
{
public:
     TreeNode *charTree;

private:
     static QHash<QChar, QHash<QChar, float> * > *distances;
private:

    // methods
    void load_dictionary(QString, QHash<QString,QString> &);
    void load_dictionary_asm(QString, QHash<QString,QString> &);

    bool insertWord(TreeNode * , QList<QString> & , QString );
    void getWordsFromTree( TreeNode *, QStack<QString> &, QStringList & wordList);
    void initializeDistances();

    void get_choices_from_subtree(TreeNode *root, QList<QString> & choices, QStack<QString> & wordstack, int max_choices);

    void get_choice_words(TreeNode *root,  QList<QString> & choices, QList<QString> & words, int choicesize, bool autoselect);

    bool _hasWord(TreeNode *root,  QStringList &chars);
    bool _deleteWord(TreeNode *root,  QStringList & chars);
    void getPossibleCandidates(TreeNode *root, QString uniWord, QStack<QString> &leftStack, QStack<QString> &rightStack, QStack<QString> &partWord, QStringList &candidateList);
    static float wordDistance(QString s, QString t);
public:
    WordsTrie();
    void LoadDictionaryWords(QString);
    void get_choice( QStringList prefix, int choicesize, bool autoselect, QList<QString> & choices);

    static WordsTrie *getWordsTrie();
    static WordsTrie *wordMapTree;

    static WordsTrie *getProfileWordsTrie();
    static WordsTrie *profileWordMapTree;
    static QString dictionaryFile;

    void  LoadProfileDictionaryWords(QHash<QString,QString> charMap);
  //  void printData();

    void getWordList(QStringList &wordList);

    void addWords(QHash<QString,QString> charMap);

    bool hasWord(QString word);

    bool deleteWord(const QString &word);

    void delete_Tree(TreeNode *tree );

    void getPossibleWords(QString word, QList<QKeyFloat> &possibleList, unsigned int numMax);

};

#endif // WORDSTRIE_H
