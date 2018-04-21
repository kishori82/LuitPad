#include "registerbox.h"
#include "ui_registerbox.h"

RegisterBox::RegisterBox(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RegisterBox)
{
    ui->setupUi(this);
}

RegisterBox::~RegisterBox()
{
    delete ui;
}


void RegisterBox::clearKey() {
    ui->plainTextEdit->clear();
}

void RegisterBox::on_buttonBox_accepted()
{

#ifdef _WINDOWS
    QSettings storedKey("LuitPad", "Nabaprabhat");
    QString serial = ui->plainTextEdit->toPlainText();
    if(serial.size() ==0 ) {
        Utilities::warningBox("Not a valid registration key for this computer!");
        return;
    }
    //remove << and >>

    serial.remove(QRegExp("<<"));
    serial.remove(QRegExp(">>"));

    QStringList serialParts = serial.split(";");
    QStringList mac_addressesList = GetMACaddress();

    QHash<QString, bool> serialPartsDict;
/*
    qDebug() << mac_addressesList;
    qDebug() << serialParts;
  */
   foreach(QString s, serialParts) {
        if( s.size()==0) continue;
        serialPartsDict.insert(s,true);
    }

    foreach(QString m, mac_addressesList) {
        if( m.size()==0) continue;
        if( serialPartsDict.contains(m) ) {
            storedKey.setValue("registration", ui->plainTextEdit->toPlainText());
            Utilities::warningBox("Successfully registered LuitPad!\n Please restart LuitPad!");
            return;
        }
    }

    Utilities::warningBox("Not a valid registration key for this computer!");
#endif
}

/*
void RegisterBox::generateSignature()
{

    SimpleCrypt *crypto = SimpleCrypt::getEncryptor(Q_UINT64_C(0x23bacf02473fdea0));
    QString cpuinfo_string = cpuinfo();
    QStringList mac_adressList = GetMACaddress();
    QString serial;
    foreach(QString macaddr, mac_adressList) {
        QString encMacAddr = crypto->encryptToString(macaddr);
        if(encMacAddr.size()==0) continue;
        serial.append(encMacAddr);
        serial.append(";");
    }
    serial.append("@");
    serial.append(cpuinfo_string);
    ui->signatureText->setText(QString("<<") + serial + QString(">>"));
}
*/

QString  getStoredKey() {
    QSettings storedKey("LuitPad", "Nabaprabhat");
    QString value = storedKey.value("registration", "null").toString();
    return value;
}


void getStoredKeys(QHash<QString, bool> &keys) {
    QSettings storedKey("LuitPad", "Nabaprabhat");
    QString value = storedKey.value("registration", "null").toString();
    keys.clear();
    value.remove(QRegExp("<<"));
    value.remove(QRegExp(">>"));

    QStringList storedKeys = value.split(";");
    foreach(QString s, storedKeys) {
        keys.insert(s.trimmed(), true);
    }
}
