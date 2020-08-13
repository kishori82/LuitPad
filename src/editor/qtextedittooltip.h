#ifndef QTEXTEDITTOOLTIP_H
#define QTEXTEDITTOOLTIP_H
#include <QTextEdit>


class QTextEditToolTip : public QTextEdit
{
private:
    QTextEditToolTip();

    static QTextEditToolTip *object;
public:
    static QTextEditToolTip *getQTextEditToolTip();
    void showTextEditToolTip(QString html);

};

#endif // QTEXTEDITTOOLTIP_H
