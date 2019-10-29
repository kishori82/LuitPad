#ifndef SEARCHREPLACE_H
#define SEARCHREPLACE_H

#include <QGridLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QString>
#include <QDialog>
#include <QtGui>
#include <QPair>
#include "src/utils/utilities.h"
#include "src/window/mdichild.h"

class SearchReplaceDialog: public QDialog
{
    Q_OBJECT

private slots:
    void nextProcess();
    void replaceProcess();
    void replaceAllProcess();

public:
    static SearchReplaceDialog *getSearchReplace(MdiChild *doc);
    QPair<unsigned int, unsigned int>  getSize() const;
private:
    SearchReplaceDialog();
    static SearchReplaceDialog *search;
    static MdiChild *document;

    QLineEdit *search_box, *replace_box;
    QLabel *search_lbl, *replace_lbl;
    unsigned int searchNum;
    QTextCursor cursor;

};

#endif

