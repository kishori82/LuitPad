#ifndef REGISTERBOX_H
#define REGISTERBOX_H

#include <QDialog>
#include <QSettings>
#include "security.h"
#include "encryption.h"
#include "utilities.h"

namespace Ui {
class RegisterBox;
}

class RegisterBox : public QDialog
{
    Q_OBJECT
    
public:
    explicit RegisterBox(QWidget *parent = 0);
    ~RegisterBox();
    void clearKey();

    
private slots:
    void on_buttonBox_accepted();
/*
    void generateSignature();
*/
private:
    Ui::RegisterBox *ui;
};



QString  getStoredKey() ;
void getStoredKeys(QHash<QString, bool> &keys);
#endif // REGISTERBOX_H
