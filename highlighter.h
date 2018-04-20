
#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H

#include <QSyntaxHighlighter>

#include <QHash>
#include <QTextCharFormat>
#include "wordstrie.h"
#include "phonetic.h"


class QTextDocument;

class Highlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    Highlighter(QTextDocument *parent = 0,bool spellCheckState=false);
    ~Highlighter();
    void clearCheck();
    QStringList misspelled();
    QStringList correct();
    void enableSpellChecking(const bool state);
    void setSpellCheck(const bool state);
    bool setDict(const QString SpellDic);
    void setIgnoreWords( QHash<QString, bool> *ignoreDictionaryRef);
  //  void undoSpellChecking();

public slots:
	void slot_addWord(QString word);

protected:
    void highlightBlock(const QString &text);
    void spellCheck(const QString &text);
    bool checkWord(QString word);

private:
    struct HighlightingRule
    {
        QRegExp pattern;
        QTextCharFormat format;
    };
    QVector<HighlightingRule> highlightingRules;

    QRegExp commentStartExpression;
    QRegExp commentEndExpression;

    QTextCharFormat keywordFormat;
    QTextCharFormat classFormat;
    QTextCharFormat singleLineCommentFormat;
    QTextCharFormat multiLineCommentFormat;
    QTextCharFormat quotationFormat;
    QTextCharFormat functionFormat;

    QString spell_dic, spell_encoding;


    bool spellCheckActive,spellerError;
    QTextCharFormat spellCheckFormat;
    QTextCodec *codec;

    static WordsTrie *profileWords ;
    static WordsTrie *dictionaryWords;

    QHash<QString, bool> *ignoreDictionary;

   // MdiChild *textEditorWindow;

};

#endif
