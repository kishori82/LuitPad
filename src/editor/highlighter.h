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
*****************************************************************************/

#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H

#include <QSyntaxHighlighter>

#include <QHash>
#include <QTextCharFormat>
#include "src/phonetic/wordstrie.h"
#include "src/phonetic/phonetic.h"


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
