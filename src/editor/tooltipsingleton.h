#ifndef TOOLTIPSINGLETON_H
#define TOOLTIPSINGLETON_H

#include <QList>
#include <QString>
#include <QTextCursor>
#include <QChar>

#include <QDebug>
#include "src/editor/textedit.h"
#include "src/utils/utilities.h"
#include "src/core/constants.h"


class ToolTipSingleton
{
private:
    ToolTipSingleton();
    QList<QString> charlist;
    QList<QKeyValue> keyvaluelist;
    int index;

    int row, col;
    int rowSizes[3];
    QList<QString> vowelList[3];



public:

    unsigned short int state;
    static ToolTipSingleton *object;

    static ToolTipSingleton * GetToolTipSingleton(QString char_freq = "");

    int SetCharacterList(QList<QString> list);
    QString charPrefix(TextEdit *);
    QString trimRomanSuffix(TextEdit *textDocument);
    QString consonantPrefix(TextEdit *);
    bool hasAssamesePrefix(TextEdit *textDocument);

    QString charMidPrefix(TextEdit *);
    void addChar(QString c);
    void addQKeyValue(QKeyValue kv);
    void clearCharList();
    void clearKeyValueList();
    void sortByUsage();

    QString createToolTipText();

    unsigned int getSize();
    void resetIndex();
    void incrementIndex();
    void decrementIndex();
    bool isEmptyCharList();
    QString getAtIndex();


    QString createVowelModToolTipText();
    void resetRowCol();
    void incrementRow();
    void decrementCol();
    void incrementCol();
    void decrementRow();
    QString getAtRowCol();

private:
    static bool toAssending( QKeyValue & s1 , QKeyValue & s2 );
    static bool toDesending( QKeyValue & s1 , QKeyValue & s2 );
    static void initializeCharFrequency(QString char_freq_fileName);
    static  QHash<QString, int> *charUsageFrequency;

};



#endif // TOOLTIPSINGLETON_H
