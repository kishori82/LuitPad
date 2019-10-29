#ifndef INPUTDIALOG_H
#define INPUTDIALOG_H

#include <QGridLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QString>
#include <QDialog>
#include <QtGui>
#include <QPair>

#include "src/utils/utilities.h"

class InputDialog: public QDialog
{
    Q_OBJECT
public:
    InputDialog();

    void incImageNum();
    void setSize( unsigned int w, unsigned int h);

    QPair<unsigned int, unsigned int>  getSize() const;
private:
    QLineEdit *width, *height;
    QLabel *width_lbl, *height_lbl, *imageName;
    unsigned int imageNum;
};

#endif
