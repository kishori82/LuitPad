/********************************************************************************
** Form generated from reading UI file 'registerbox.ui'
**
** Created: Thu May 9 07:31:11 2013
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_REGISTERBOX_H
#define UI_REGISTERBOX_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPlainTextEdit>

QT_BEGIN_NAMESPACE

class Ui_RegisterBox
{
public:
    QDialogButtonBox *buttonBox;
    QPlainTextEdit *plainTextEdit;
    QLabel *label;

    void setupUi(QDialog *RegisterBox)
    {
        if (RegisterBox->objectName().isEmpty())
            RegisterBox->setObjectName(QString::fromUtf8("RegisterBox"));
        RegisterBox->resize(488, 175);
        buttonBox = new QDialogButtonBox(RegisterBox);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(80, 130, 341, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        buttonBox->setCenterButtons(true);
        plainTextEdit = new QPlainTextEdit(RegisterBox);
        plainTextEdit->setObjectName(QString::fromUtf8("plainTextEdit"));
        plainTextEdit->setGeometry(QRect(40, 70, 411, 41));
        label = new QLabel(RegisterBox);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(110, 20, 281, 41));

        retranslateUi(RegisterBox);
        QObject::connect(buttonBox, SIGNAL(accepted()), RegisterBox, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), RegisterBox, SLOT(reject()));

        QMetaObject::connectSlotsByName(RegisterBox);
    } // setupUi

    void retranslateUi(QDialog *RegisterBox)
    {
        RegisterBox->setWindowTitle(QApplication::translate("RegisterBox", "Registration Key", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("RegisterBox", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:10pt; font-weight:600;\">Please paste the key you received from </span></p>\n"
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:10pt; font-weight:600;\">NabaPrabhat below and press OK</span></p></body></html>", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class RegisterBox: public Ui_RegisterBox {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REGISTERBOX_H
