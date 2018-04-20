#ifndef PROFILE_H
#define PROFILE_H

#include "QAction"
#include "QGridLayout"
#include "QLabel"
#include "QToolButton"
#include "QActionGroup"
#include "QComboBox"
#include "QGroupBox"
#include "QPushButton"
#include "QButtonGroup"
#include "QCheckBox"
#include "chartrie.h"


#include "mdichild.h"
#include "QMessageBox"
#include "QToolTip"
#include "QPushButton"
#include "QButtonGroup"
#include <QDebug>
#include "QLineEdit"
#include <sstream>
#include "QDir"
#include "QDirIterator"

#include "QAction"
#include "QGridLayout"
#include "QLabel"
#include "QToolButton"
#include "QActionGroup"
#include "QComboBox"
#include "QGroupBox"
#include "QPushButton"
#include <QScrollArea>
#include <QDockWidget>
#include "Constants.h"

class Profile: public QWidget
{
    Q_OBJECT

private:
     Profile(QWidget *parent=0);
public:
    //keyboard(QWidget *parent=0);
    static Profile *getkeyBoard();

    void display();
    bool addWord(const QString &newWord);
    bool deleteWord(const QString &word);

   // static QWidget *kbd;

    static QWidget * kbd;
    QGridLayout *grid;
    QLabel *label;
    QComboBox *profCombo;
    QGroupBox *gbox ;

    QActionGroup *keyAct;
    QButtonGroup *qbg;

    static Profile *keyBoard;
    static QString currentProfile;


    void newProfile();
    void auto_fill_profile_combo(QString profilename ="");

public slots:
       bool fill_keyboard(const  QString  & profName);

private slots:
    bool changeProfile();
    bool saveProfile();
    bool deleteProfile();
    void changeModifierOrder(int state);

private:
    void deSign();
    void textEnable();
    void textDisable();

    void setChangeEnable();
    void setChangeDisable();
    bool isChangeEnable();
    bool changeEnDisableFlag;

    bool modifierOrder;
    void setModifierOrder(bool flag);
    bool isValidProfile( const QString &profName);
    QPushButton *save_profile, *change_profile, *delButton;
    QCheckBox *modifierOrderCheckbox;

    QLineEdit *lbl[54] ;

    /*
    QLineEdit *lbl_54;
    QLineEdit *lbl_55 ;

    QLineEdit *lbl_56 ;
    QLineEdit *lbl_57 ;
    QLineEdit *lbl_58;
    QLineEdit *lbl_59 ;
    QLineEdit *lbl_60 ;
    QLineEdit *lbl_61;
    QLineEdit *lbl_62 ;
    QLineEdit *lbl_63 ;
    */

private slots:
        //void triggeredChar(QAction * a);

        void buttonClick(QAbstractButton* button);


};
#endif // KEYBOARD_H

