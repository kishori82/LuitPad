#include "mainwindow.h"
#include "profile.h"
#include <QScrollArea>
#include <QSizePolicy>
#include "encryption.h"

Profile * Profile::keyBoard = NULL ;
QWidget *Profile::kbd = NULL;
QString  Profile::currentProfile = "DEFAULT" ;



Profile *Profile::getkeyBoard() {
    if( keyBoard == NULL || kbd ==NULL) {
        keyBoard = new Profile();
    }
    return keyBoard;
}

Profile::Profile(QWidget *parent) : QWidget(parent)
{
    setModifierOrder(false);
    setChangeDisable();


    if(!QDir("profile").exists()){
        QDir().mkdir("profile");
    }
    QString fName = QLatin1String(":files") + QDir::separator() + QLatin1String("DEFAULT.dat");

    QString foutName = QLatin1String("profile") + QDir::separator()  + QLatin1String("DEFAULT.dat");

    QFile file(fName);
    QFile outfile(foutName);

    if(!isValidProfile("DEFAULT")) {
        Utilities::warningBox(QString("Do not see folder \"profile\"! \n") + \
                              QString("So we will create a new DEFAULT profile in folder \"profile\"\n\n") +\
                              QString("But you can always replace the DEFAULT.dat file in the \"profile\"") +\
                              QString("folder if have your old DEFAULT.dat!! to restore your old preferences and stored words!"));

 //   qDebug() << fName;
        if (file.open(QIODevice::ReadOnly | QIODevice::Text) && outfile.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QTextStream out(&outfile);
            QTextStream in(&file);
            QString outLine;
            while (!in.atEnd()) {
                QString inStr = in.readLine();
              //  qDebug() << inStr;
                outLine.append(inStr +"\n") ;
            }
            out << outLine;
        }
    }
    file.close();
    outfile.close();

    kbd=NULL;
}

void Profile::display()
{

    if(kbd==NULL || !kbd->isVisible() ) {
       kbd = new QWidget;
       kbd->setWindowFlags(Qt::FramelessWindowHint);
            //   kbd->setMinimumSize(kbd->size());
            //   kbd->setMaximumSize(kbd->size());
              // kbd->setSizePolicy( QSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed, TRUE ));
       kbd->setWindowTitle("User Key Map Profile");
       deSign();
    }





//=======================QACTION=====================
}



void Profile::setModifierOrder(bool flag) {

    modifierOrder = flag;

}

void Profile::deSign()
{

    profCombo = new QComboBox();

    profCombo->setEditable(true);

    auto_fill_profile_combo(currentProfile);

    save_profile = new QPushButton("Save",this);
    delButton = new QPushButton("Delete",this);
    change_profile = new QPushButton("Change Mapping",this);
    change_profile->setEnabled(true);
    save_profile->setEnabled(true);

  //  modifierOrderCheckbox = new QCheckBox( QString(QChar(0x9c7)) +QString("/") + QChar(0x9c8)+QString("/") + QChar(0x9bf) + " order change", this);


    connect(save_profile,SIGNAL(clicked()),this,SLOT(saveProfile()));
    connect(change_profile,SIGNAL(clicked()),this,SLOT(changeProfile()));
    connect(delButton,SIGNAL(clicked()),this,SLOT(deleteProfile()));
    //connect(modifierOrderCheckbox, SIGNAL(stateChanged(int)),this,SLOT(changeModifierOrder(int)));


/*    for(int i=0;i<63;i++)
        (QLineEdit *) (QString("lbl_%1").arg(i)) = new QLineEdit(this);
*/
    for(int i=1; i<=53; i++)
      lbl[i]= new QLineEdit(this);



    /*
    lbl_54= new QLineEdit(this);
    lbl_55 = new QLineEdit(this);

    lbl_56 = new QLineEdit(this);
    lbl_57 = new QLineEdit(this);
    lbl_58= new QLineEdit(this);
    lbl_59 = new QLineEdit(this);
    lbl_60 = new QLineEdit(this);

    lbl_61= new QLineEdit(this);
    lbl_62 = new QLineEdit(this);
    lbl_63 = new QLineEdit(this);
    */
//==========================================
    for(int i=1; i<=53; i++)
        lbl[i]->setFixedSize(30,20);



    /*
    lbl_54->setFixedSize(30, 20);
    lbl_55 ->setFixedSize(30, 20);

    lbl_56 ->setFixedSize(30, 20);
    lbl_57 ->setFixedSize(30, 20);
    lbl_58->setFixedSize(30, 20);
    lbl_59 ->setFixedSize(30, 20);
    lbl_60 ->setFixedSize(30, 20);

    lbl_61->setFixedSize(30, 20);
    lbl_62 ->setFixedSize(30, 20);
    lbl_63 ->setFixedSize(30, 20);
*/
//=======================================================================
    QToolButton *ka = new QToolButton(this); ka->setText(trUtf8("ক"));  ka->setFixedSize(30, 20);
    QToolButton *kha = new QToolButton(this); kha->setText(trUtf8("খ"));  kha->setFixedSize(30, 20);
    QToolButton *ga = new QToolButton(this); ga->setText(trUtf8("গ")); ga->setFixedSize(30, 20);
    QToolButton *gha = new QToolButton(this); gha->setText(trUtf8("ঘ")); gha->setFixedSize(30, 20);
    QToolButton *nga = new QToolButton(this); nga->setText(trUtf8("ঙ")); nga->setFixedSize(30, 20);

    QToolButton *ca = new QToolButton(this); ca->setText(trUtf8("চ")); ca->setFixedSize(30, 20);
    QToolButton *cha = new QToolButton(this); cha->setText(trUtf8("ছ"));  cha->setFixedSize(30, 20);
    QToolButton *ja = new QToolButton(this); ja->setText(trUtf8("জ"));     ja ->setFixedSize(30, 20);
    QToolButton *jha = new QToolButton(this); jha->setText(trUtf8("ঝ")); jha->setFixedSize(30, 20);
    QToolButton *nya = new QToolButton(this); nya->setText(trUtf8("ঞ")); nya ->setFixedSize(30, 20);

    QToolButton *ta = new QToolButton(this); ta->setText(trUtf8("ত")); ta->setFixedSize(30, 20);
    QToolButton *tha = new QToolButton(this); tha->setText(QChar(0x9A5)); tha->setFixedSize(30, 20);
  //  QToolButton *tha = new QToolButton(this); tha->setText("tha"); tha->setFixedSize(30, 20);
    QToolButton *da = new QToolButton(this); da->setText(trUtf8("দ"));  da->setFixedSize(30, 20);
    QToolButton *dha = new QToolButton(this); dha->setText(trUtf8("ধ"));    dha->setFixedSize(30, 20);
    QToolButton *na = new QToolButton(this); na->setText(trUtf8("ন")); na->setFixedSize(30, 20);

    QToolButton *Ta = new QToolButton(this); Ta->setText(trUtf8("ট")); Ta->setFixedSize(30, 20);
    //QToolButton *Tha = new QToolButton(this); Tha->setText(trUtf8("� ")); Tha->setFixedSize(30, 20);
    QToolButton *Tha = new QToolButton(this); Tha->setText( QChar(0x9A0)); Tha->setFixedSize(30, 20);
    QToolButton *Da = new QToolButton(this); Da->setText(trUtf8("ড")); Da->setFixedSize(30, 20);
    QToolButton *Dha = new QToolButton(this); Dha->setText(trUtf8("ঢ")); Dha->setFixedSize(30, 20);
    QToolButton *Na = new QToolButton(this); Na->setText(trUtf8("ণ")); Na->setFixedSize(30, 20);

    QToolButton *pa = new QToolButton(this); pa->setText(trUtf8("প")); pa->setFixedSize(30, 20);
    QToolButton *pha = new QToolButton(this); pha->setText(trUtf8("ফ")); pha->setFixedSize(30, 20);
    QToolButton *ba = new QToolButton(this); ba->setText(trUtf8("ব")); ba->setFixedSize(30, 20);
    QToolButton *bha = new QToolButton(this); bha->setText(trUtf8("ভ")); bha->setFixedSize(30, 20);
    QToolButton *ma = new QToolButton(this); ma->setText(trUtf8("ম")); ma->setFixedSize(30, 20);

    QToolButton *Ja = new QToolButton(this); Ja->setText(trUtf8("য")); Ja->setFixedSize(30, 20);
    QToolButton *ra = new QToolButton(this); ra->setText(trUtf8("ৰ")); ra->setFixedSize(30, 20);
    QToolButton *la = new QToolButton(this); la->setText(trUtf8("ল")); la->setFixedSize(30, 20);
    QToolButton *wa = new QToolButton(this); wa->setText(trUtf8("ৱ")); wa->setFixedSize(30, 20);

    QToolButton *Sa = new QToolButton(this); Sa->setText(trUtf8("শ")); Sa->setFixedSize(30, 20);
    QToolButton *sha = new QToolButton(this); sha->setText(trUtf8("ষ")); sha->setFixedSize(30, 20);
    QToolButton *sa = new QToolButton(this); sa->setText(trUtf8("স")); sa->setFixedSize(30, 20);
    QToolButton *ha = new QToolButton(this); ha->setText(trUtf8("হ")); ha->setFixedSize(30, 20);

    QToolButton *khya = new QToolButton(this); khya->setText(trUtf8("ক্ষ")); khya ->setFixedSize(30, 20);
    QToolButton *drr = new QToolButton(this); drr->setText(trUtf8("ড়")); drr->setFixedSize(30, 20);
    QToolButton *arr = new QToolButton(this); arr->setText(trUtf8("ঢ়")); arr->setFixedSize(30, 20);
    QToolButton *ya = new QToolButton(this); ya->setText(trUtf8("য়")); ya->setFixedSize(30, 20);

    QToolButton *cbindu = new QToolButton(this); cbindu->setText(trUtf8("ঁ")); cbindu->setFixedSize(30, 20);
    QToolButton *bjnt = new QToolButton(this); bjnt->setText(QChar(0x9ce)); bjnt->setFixedSize(30, 20);
    QToolButton *ansr = new QToolButton(this); ansr->setText(trUtf8("ং")); ansr->setFixedSize(30, 20);
    QToolButton *bsrg = new QToolButton(this); bsrg->setText(trUtf8("ঃ"));bsrg->setFixedSize(30, 20);

    QToolButton *a = new QToolButton(this); a->setText(trUtf8("অ")); a->setFixedSize(30, 20);
    QToolButton *aa = new QToolButton(this); aa->setText(trUtf8("আ")); aa->setFixedSize(30, 20);
    QToolButton *i = new QToolButton(this); i->setText(trUtf8("ই")); i->setFixedSize(30, 20);
    QToolButton *ii= new QToolButton(this); ii->setText(trUtf8("ঈ")); ii->setFixedSize(30, 20);

    QToolButton *u = new QToolButton(this); u->setText(trUtf8("উ")); u->setFixedSize(30, 20);
    QToolButton *uu= new QToolButton(this); uu->setText(trUtf8("ঊ")); uu->setFixedSize(30, 20);
    QToolButton *ri = new QToolButton(this); ri->setText(trUtf8("ঋ"));ri->setFixedSize(30, 20);

    QToolButton *e = new QToolButton(this); e->setText(QChar(0x98f)); e->setFixedSize(30, 20);
    QToolButton *ei = new QToolButton(this); ei->setText(QChar(0x990)); ei->setFixedSize(30, 20);
    QToolButton *o = new QToolButton(this); o->setText(QChar(0x993)); o->setFixedSize(30, 20);
    QToolButton *ou= new QToolButton(this); ou->setText(QChar(0x994));ou ->setFixedSize(30, 20);

    QToolButton *dlink = new QToolButton(this); dlink->setText(trUtf8("্")); dlink->setFixedSize(30, 20);
    QToolButton *aa_matra = new QToolButton(this); aa_matra->setText(trUtf8("া")); aa_matra->setFixedSize(30, 20);
    QToolButton *i_matra = new QToolButton(this); i_matra->setText(trUtf8("ি")); i_matra->setFixedSize(30, 20);
    QToolButton *ii_matra= new QToolButton(this); ii_matra->setText(trUtf8("ী")); ii_matra->setFixedSize(30, 20);

    QToolButton *u_matra = new QToolButton(this); u_matra->setText(trUtf8("ু")); u_matra->setFixedSize(30, 20);
    QToolButton *uu_matra= new QToolButton(this); uu_matra->setText(trUtf8("ূ"));uu_matra->setFixedSize(30, 20);
    QToolButton *ri_matra = new QToolButton(this); ri_matra->setText(trUtf8("ৃ")); ri_matra->setFixedSize(30, 20);

    QToolButton *e_matra = new QToolButton(this); e_matra->setText(trUtf8("ে")); e_matra->setFixedSize(30, 20);
    QToolButton *ei_matra = new QToolButton(this); ei_matra->setText(trUtf8("ৈ")); ei_matra->setFixedSize(30, 20);
    QToolButton *o_matra = new QToolButton(this); o_matra->setText(trUtf8("ো")); o_matra->setFixedSize(30, 20);
    QToolButton *ou_matra = new QToolButton(this); ou_matra->setText(trUtf8("ৌ")); ou_matra->setFixedSize(30, 20);
//=======================================================================
    //connect(ka_act,SIGNAL(triggered()),MainWindow,SLOT(_ka()));

//=================================================================
    fill_keyboard("DEFAULT");
    //qDebug() << "Connecting the Change to the fill_kbd";
    connect(profCombo,SIGNAL(currentIndexChanged(QString)),this,SLOT(fill_keyboard(QString)));

//=================================================================
    grid = new QGridLayout(this);
 //    setWindowFlags() with Qt::WindowStaysOnTopHint is

//    grid->setWindowFlags(Qt::WindowStaysOnTopHint);



    grid->addWidget(profCombo,0,0,1,2,Qt::AlignCenter);
    grid->addWidget(save_profile,0,2,1,3,Qt::AlignCenter);
    grid->addWidget(delButton,1,0,1,2,Qt::AlignLeft);
    grid->addWidget(change_profile,1,2,1,3,Qt::AlignLeft);
  //  grid->addWidget(modifierOrderCheckbox ,2,0,2,6,Qt::AlignCenter);


    grid->addWidget(a, 6, 0);
    grid->addWidget(aa, 6, 1);
    grid->addWidget(i, 6, 2);
    grid->addWidget(ii, 6, 3);


    grid->addWidget(u, 8, 0);
    grid->addWidget(uu, 8, 1);
    grid->addWidget(ri, 8, 2);
    grid->addWidget(e, 10, 0);
    grid->addWidget(ei, 10, 1);
    grid->addWidget(o, 10, 2);
    grid->addWidget(ou, 10, 3);

    grid->addWidget(lbl[42], 7, 0);
    grid->addWidget(lbl[43], 7, 1);
    grid->addWidget(lbl[44], 7, 2);
    grid->addWidget(lbl[45], 7, 3);


    grid->addWidget(lbl[46], 9, 0);
    grid->addWidget(lbl[47], 9, 1);
    grid->addWidget(lbl[48], 9, 2);
    grid->addWidget(lbl[49], 11, 0);
    grid->addWidget(lbl[50], 11, 1);
    grid->addWidget(lbl[51], 11, 2);
    grid->addWidget(lbl[52], 11, 3);


    grid->addWidget(ka, 12, 0);
    grid->addWidget(kha, 12, 1);
    grid->addWidget(ga, 12, 2);
    grid->addWidget(gha, 12, 3);
    grid->addWidget(nga, 12, 4);


    grid->addWidget(lbl[1], 13, 0);
    grid->addWidget(lbl[2], 13, 1);
    grid->addWidget(lbl[3], 13, 2);
    grid->addWidget(lbl[4], 13, 3);
    grid->addWidget(lbl[5], 13, 4);

    grid->addWidget(ca, 14, 0);
    grid->addWidget(cha, 14, 1);
    grid->addWidget(ja, 14, 2);
    grid->addWidget(jha, 14, 3);
    grid->addWidget(nya, 14, 4);

    grid->addWidget(lbl[6], 15, 0);
    grid->addWidget(lbl[7], 15, 1);
    grid->addWidget(lbl[8], 15, 2);
    grid->addWidget(lbl[9], 15, 3);
    grid->addWidget(lbl[10], 15, 4);

    grid->addWidget(Ta, 16, 0);
    grid->addWidget(Tha, 16, 1);
    grid->addWidget(Da, 16, 2);
    grid->addWidget(Dha, 16, 3);
    grid->addWidget(Na, 16, 4);

    grid->addWidget(lbl[11], 17, 0);
    grid->addWidget(lbl[12], 17, 1);
    grid->addWidget(lbl[13], 17, 2);
    grid->addWidget(lbl[14], 17, 3);
    grid->addWidget(lbl[15], 17, 4);

    grid->addWidget(ta, 18, 0);
    grid->addWidget(tha, 18, 1);
    grid->addWidget(da, 18, 2);
    grid->addWidget(dha, 18, 3);
    grid->addWidget(na, 18, 4);

    grid->addWidget(lbl[16], 19, 0);
    grid->addWidget(lbl[17], 19, 1);
    grid->addWidget(lbl[18], 19, 2);
    grid->addWidget(lbl[19], 19, 3);
    grid->addWidget(lbl[20], 19, 4);

    grid->addWidget(pa, 20, 0);
    grid->addWidget(pha, 20, 1);
    grid->addWidget(ba, 20, 2);
    grid->addWidget(bha, 20, 3);
    grid->addWidget(ma, 20, 4);

    grid->addWidget(lbl[21], 21, 0);
    grid->addWidget(lbl[22], 21, 1);
    grid->addWidget(lbl[23], 21, 2);
    grid->addWidget(lbl[24], 21, 3);
    grid->addWidget(lbl[25], 21, 4);

    grid->addWidget(Ja, 22, 0);
    grid->addWidget(ra, 22, 1);
    grid->addWidget(la, 22, 2);
    grid->addWidget(wa, 22, 3);

    grid->addWidget(lbl[26], 23, 0);
    grid->addWidget(lbl[27], 23, 1);
    grid->addWidget(lbl[28], 23, 2);
    grid->addWidget(lbl[29], 23, 3);

    grid->addWidget(Sa, 24, 0);
    grid->addWidget(sha, 24, 1);
    grid->addWidget(sa, 24, 2);
    grid->addWidget(ha, 24, 3);

    grid->addWidget(lbl[30], 25, 0);
    grid->addWidget(lbl[31], 25, 1);
    grid->addWidget(lbl[32], 25, 2);
    grid->addWidget(lbl[33], 25, 3);

    grid->addWidget(khya, 26, 0);
    grid->addWidget(drr, 26, 1);
    grid->addWidget(arr, 26, 2);
    grid->addWidget(ya, 26, 3);

    grid->addWidget(lbl[34], 27, 0);
    grid->addWidget(lbl[35], 27, 1);
    grid->addWidget(lbl[36], 27, 2);
    grid->addWidget(lbl[37], 27, 3);

    grid->addWidget(cbindu, 28, 0);
    grid->addWidget(bjnt, 28, 1);
    grid->addWidget(ansr, 28, 2);
    grid->addWidget(bsrg, 28, 3);

    grid->addWidget(lbl[38], 29, 0);
    grid->addWidget(lbl[39], 29, 1);
    grid->addWidget(lbl[40], 29, 2);
    grid->addWidget(lbl[41], 29, 3);

    grid->addWidget(dlink, 28, 4);
    grid->addWidget(lbl[53], 29, 4);



    kbd->setLayout(grid);
    QFont keyboardFont = QFont("kalpurush",10,3,false);
    kbd->setFont(keyboardFont);
        textDisable();

    kbd->setWindowFlags(Qt::WindowStaysOnTopHint);


    QScrollArea *tagCloudScrollArea = new QScrollArea;
    tagCloudScrollArea->setWidget(kbd);

    tagCloudScrollArea->setWindowFlags(Qt::WindowStaysOnTopHint);
    tagCloudScrollArea->show();
}


void Profile::changeModifierOrder(int state) {
  //  setModifierOrder( modifierOrderCheckbox->isChecked());
}

void Profile::buttonClick(QAbstractButton* button)
    {

        QMessageBox* box = new QMessageBox();
        box->setWindowTitle("Hello");
        box->setText("You clicked " + button->text());
        box->show();
    }

void Profile::newProfile(){}

bool Profile::changeProfile()
 {
     profCombo->setEditable(true);

     if( isChangeEnable() ) { // toggle
         textDisable();
        // save_profile->setEnabled(false);
     }
     else {
         textEnable();
        // save_profile->setEnabled(true);
     }

   //  qDebug("change profile");
   //  connect(sve,SIGNAL(clicked()),this,SLOT(saveProfile()));
     return true;
 }

bool Profile::deleteProfile(){
        //qDebug()<<"New edited text"<<profCombo->currentText();
        if( QString("DEFAULT") == profCombo->currentText() ) {
            return false;
        }
        if(!Utilities::YesNoDialog(QString("The deleted profile with be lost."), QString("Do you want to want to delete it?"))) {
            return false ;
        }

        QDir myDir("profile");

        QString fname = QLatin1String("profile") + QDir::separator() + profCombo->currentText()+QLatin1String(".dat");


        QFile file(fname);
        if(myDir.exists()) {
            if( file.exists() ) {
                file.remove();
            }
        }

        auto_fill_profile_combo("DEFAULT");
        return true;
}

bool Profile::saveProfile(){
        //qDebug()<<"New edited text"<<profCombo->currentText();
        SimpleCrypt *crypto = SimpleCrypt::getEncryptor(Q_UINT64_C(0x23bacf02473fdea0));
        if(!Utilities::YesNoDialog(QString("If you save previous mapping will be overwritten."), QString("Do you want to want save the new maps?"))) {
           return false ;
        }


        QDir myDir("profile");
        if(!myDir.exists()){
            myDir.mkpath("profile");
        }


        QString fn = QLatin1String("profile") + QDir::separator() + profCombo->currentText()+QLatin1String(".dat");

        QHash<QString, bool> uniqueText;
        for(int i = 1; i <=53; i++) {
            if(lbl[i]->text().trimmed()==0 ) {
                Utilities::warningBox("Some characters have empty mapping!");
                return false;
            }
        }

        QString message;
        for(int i = 1; i <=53; i++) {
            if( !uniqueText.contains(lbl[i]->text().trimmed()) ) {
                uniqueText.insert(lbl[i]->text().trimmed(),true);
            }
            else {
                message += QString("\"") + lbl[i]->text() + QString("\"   ");
            }
        }
        if( uniqueText.size() < 53 ) {
            Utilities::warningBox("Conflicts with the following mappings \n" + message);
            return false;
        }



        QFile outFile(fn);
        outFile.open(QIODevice::Append);
        QString outText;
      //  qDebug() << "File to save in " << fn;
        if(!outFile.isOpen()) {
            qDebug() << "Error, unable to open" << fn << "for output";
            return false;
        }
        else{
            QTextStream outStream(&outFile);
            outText =  QString("<char>\t") + QString("0x995\t") + lbl[1]->text()+"\n"; outStream << crypto->encryptToString(outText) + "\n";
            outText =  QString("<char>\t") + QString("0x996\t") + lbl[2]->text()+"\n"; outStream << crypto->encryptToString(outText) + "\n";
            outText =  QString("<char>\t") + QString("0x997\t") + lbl[3]->text()+"\n"; outStream << crypto->encryptToString(outText) + "\n";
            outText =  QString("<char>\t") + QString("0x998\t") + lbl[4]->text()+"\n"; outStream << crypto->encryptToString(outText) + "\n";
            outText =  QString("<char>\t") + QString("0x999\t") + lbl[5]->text()+"\n"; outStream << crypto->encryptToString(outText) + "\n";

            outText =  QString("<char>\t") + QString("0x99A\t") + lbl[6]->text()+"\n"; outStream << crypto->encryptToString(outText) + "\n";
            outText =  QString("<char>\t") + QString("0x99B\t") + lbl[7]->text()+"\n"; outStream << crypto->encryptToString(outText) + "\n";
            outText =  QString("<char>\t") + QString("0x99C\t") + lbl[8]->text()+"\n"; outStream << crypto->encryptToString(outText) + "\n";
            outText =  QString("<char>\t") + QString("0x99D\t") + lbl[9]->text()+"\n"; outStream << crypto->encryptToString(outText) + "\n";
            outText =  QString("<char>\t") + QString("0x99E\t") + lbl[10]->text()+"\n"; outStream << crypto->encryptToString(outText) + "\n";

            outText =  QString("<char>\t") + QString("0x99F\t") + lbl[11]->text()+"\n"; outStream << crypto->encryptToString(outText) + "\n";
            outText =  QString("<char>\t") + QString("0x9A0\t") + lbl[12]->text()+"\n"; outStream << crypto->encryptToString(outText) + "\n";
            outText =  QString("<char>\t") + QString("0x9A1\t") + lbl[13]->text()+"\n"; outStream << crypto->encryptToString(outText) + "\n";
            outText =  QString("<char>\t") + QString("0x9A2\t") + lbl[14]->text()+"\n"; outStream << crypto->encryptToString(outText) + "\n";
            outText =  QString("<char>\t") + QString("0x9A3\t") + lbl[15]->text()+"\n"; outStream << crypto->encryptToString(outText) + "\n";

            outText =  QString("<char>\t") + QString("0x9A4\t") + lbl[16]->text()+"\n"; outStream << crypto->encryptToString(outText) + "\n";
            outText =  QString("<char>\t") + QString("0x9A5\t") + lbl[17]->text()+"\n"; outStream << crypto->encryptToString(outText) + "\n";
            outText =  QString("<char>\t") + QString("0x9A6\t") + lbl[18]->text()+"\n"; outStream << crypto->encryptToString(outText) + "\n";
            outText =  QString("<char>\t") + QString("0x9A7\t") + lbl[19]->text()+"\n"; outStream << crypto->encryptToString(outText) + "\n";
            outText =  QString("<char>\t") + QString("0x9A8\t") + lbl[20]->text()+"\n"; outStream << crypto->encryptToString(outText) + "\n";

            outText =  QString("<char>\t") + QString("0x9AA\t") + lbl[21]->text()+"\n"; outStream << crypto->encryptToString(outText) + "\n";
            outText =  QString("<char>\t") + QString("0x9AB\t") + lbl[22]->text()+"\n"; outStream << crypto->encryptToString(outText) + "\n";
            outText =  QString("<char>\t") + QString("0x9AC\t") + lbl[23]->text()+"\n"; outStream << crypto->encryptToString(outText) + "\n";
            outText =  QString("<char>\t") + QString("0x9AD\t") + lbl[24]->text()+"\n"; outStream << crypto->encryptToString(outText) + "\n";
            outText =  QString("<char>\t") + QString("0x9AE\t") + lbl[25]->text()+"\n"; outStream << crypto->encryptToString(outText) + "\n";

            outText =  QString("<char>\t") + QString("0x9AF\t") + lbl[26]->text()+"\n"; outStream << crypto->encryptToString(outText) + "\n";
            outText =  QString("<char>\t") + QString("0x9F0\t") + lbl[27]->text()+"\n"; outStream << crypto->encryptToString(outText) + "\n";
            outText =  QString("<char>\t") + QString("0x9B2\t") + lbl[28]->text()+"\n"; outStream << crypto->encryptToString(outText) + "\n";
            outText =  QString("<char>\t") + QString("0x9F1\t") + lbl[29]->text()+"\n"; outStream << crypto->encryptToString(outText) + "\n";

            outText = QString("<char>\t") +  QString("0x9B6\t") + lbl[30]->text()+"\n"; outStream << crypto->encryptToString(outText) + "\n";
            outText =  QString("<char>\t") + QString("0x9B7\t") + lbl[31]->text()+"\n"; outStream << crypto->encryptToString(outText) + "\n";
            outText =  QString("<char>\t") + QString("0x9B8\t") + lbl[32]->text()+"\n"; outStream << crypto->encryptToString(outText) + "\n";
            outText =  QString("<char>\t") + QString("0x9B9\t") + lbl[33]->text()+"\n"; outStream << crypto->encryptToString(outText) + "\n";

            outText =  QString("<char>\t") + QString("0x9950x9CD0x9B7\t") + lbl[34]->text()+"\n"; outStream << crypto->encryptToString(outText) + "\n";
            outText =  QString("<char>\t") + QString("0x9DC\t") + lbl[35]->text()+"\n"; outStream << crypto->encryptToString(outText) + "\n";
            outText =  QString("<char>\t") + QString("0x9DD\t") + lbl[36]->text()+"\n"; outStream << crypto->encryptToString(outText) + "\n";
            outText =  QString("<char>\t") + QString("0x9DF\t") +  lbl[37]->text()+"\n"; outStream << crypto->encryptToString(outText) + "\n";

            outText =  QString("<char>\t") + QString("0x981\t") +  lbl[38]->text()+"\n"; outStream << crypto->encryptToString(outText) + "\n";
            outText =  QString("<char>\t") + QString("0x9CE\t") +  lbl[39]->text()+"\n"; outStream << crypto->encryptToString(outText) + "\n";
            outText =  QString("<char>\t") + QString("0x982\t") +  lbl[40]->text()+"\n"; outStream << crypto->encryptToString(outText) + "\n";
            outText =  QString("<char>\t") + QString("0x983\t") +  lbl[41]->text()+"\n"; outStream << crypto->encryptToString(outText) + "\n";

            outText =  QString("<char>\t") + QString("0x985\t") +  lbl[42]->text()+"\n"; outStream << crypto->encryptToString(outText) + "\n";

            outText =  QString("<char>\t") + QString("0x9be\t") +  lbl[43]->text()+"\n"; outStream << crypto->encryptToString(outText) + "\n";
            outText =  QString("<char>\t") + QString("0x9bf\t") +  lbl[44]->text()+"\n"; outStream << crypto->encryptToString(outText) + "\n";
            outText =  QString("<char>\t") + QString("0x9c0\t") +  lbl[45]->text()+"\n"; outStream << crypto->encryptToString(outText) + "\n";

            outText =  QString("<char>\t") + QString("0x9c1\t") +  lbl[46]->text()+"\n"; outStream << crypto->encryptToString(outText) + "\n";
            outText =  QString("<char>\t") + QString("0x9c2\t") +  lbl[47]->text()+"\n"; outStream << crypto->encryptToString(outText) + "\n";
            outText =  QString("<char>\t") + QString("0x9c3\t") +  lbl[48]->text()+"\n"; outStream << crypto->encryptToString(outText) + "\n";

            outText =  QString("<char>\t") + QString("0x9c7\t") +  lbl[49]->text()+"\n"; outStream << crypto->encryptToString(outText) + "\n";
            outText =  QString("<char>\t") + QString("0x9c8\t") +  lbl[50]->text()+"\n"; outStream << crypto->encryptToString(outText) + "\n";
            outText =  QString("<char>\t") + QString("0x9cb\t") +  lbl[51]->text()+"\n"; outStream << crypto->encryptToString(outText) + "\n";
            outText =  QString("<char>\t") + QString("0x9cc\t") +  lbl[52]->text()+"\n"; outStream << crypto->encryptToString(outText) + "\n";

            qDebug() << "Saving ";

/*
            outText =  QString("<char>\t") + QString("0x986\t") +  lbl[43]->text()+"\n"; outStream << crypto->encryptToString(outText) + "\n";
            outText =  QString("<char>\t") + QString("0x987\t") +  lbl[44]->text()+"\n"; outStream << crypto->encryptToString(outText) + "\n";
            outText =  QString("<char>\t") + QString("0x988\t") +  lbl[45]->text()+"\n"; outStream << crypto->encryptToString(outText) + "\n";

            outText =  QString("<char>\t") + QString("0x989\t") +  lbl[46]->text()+"\n"; outStream << crypto->encryptToString(outText) + "\n";
            outText =  QString("<char>\t") + QString("0x98A\t") +  lbl[47]->text()+"\n"; outStream << crypto->encryptToString(outText) + "\n";
            outText =  QString("<char>\t") + QString("0x98B\t") +  lbl[48]->text()+"\n"; outStream << crypto->encryptToString(outText) + "\n";

            outText =  QString("<char>\t") + QString("0x98F\t") +  lbl[49]->text()+"\n"; outStream << crypto->encryptToString(outText) + "\n";
            outText =  QString("<char>\t") + QString("0x990\t") +  lbl[50]->text()+"\n"; outStream << crypto->encryptToString(outText) + "\n";
            outText =  QString("<char>\t") + QString("0x993\t") +  lbl[51]->text()+"\n"; outStream << crypto->encryptToString(outText) + "\n";
            outText =  QString("<char>\t") + QString("0x994\t") +  lbl[52]->text()+"\n"; outStream << crypto->encryptToString(outText) + "\n";       
  */

            outText =  QString("<char>\t") + QString("0x9CD\t") +  lbl[53]->text()+"\n"; outStream << crypto->encryptToString(outText) + "\n";

            outText =  QString("<char>\t") + QString("0x9E6\t") +  "0"+"\n"; outStream << crypto->encryptToString(outText) + "\n";
            outText =  QString("<char>\t") + QString("0x9E7\t") +  "1"+"\n"; outStream << crypto->encryptToString(outText) + "\n";
            outText =  QString("<char>\t") + QString("0x9E8\t") +  "2"+"\n"; outStream << crypto->encryptToString(outText) + "\n";
            outText =  QString("<char>\t") + QString("0x9E9\t") +  "3"+"\n"; outStream << crypto->encryptToString(outText) + "\n";
            outText =  QString("<char>\t") + QString("0x9EA\t") +  "4"+"\n"; outStream << crypto->encryptToString(outText) + "\n";

            outText =  QString("<char>\t") + QString("0x9EB\t") +  "5"+"\n"; outStream << crypto->encryptToString(outText) + "\n";
            outText =  QString("<char>\t") + QString("0x9EC\t") +  "6"+"\n"; outStream << crypto->encryptToString(outText) + "\n";
            outText =  QString("<char>\t") + QString("0x9ED\t") +  "7"+"\n"; outStream << crypto->encryptToString(outText) + "\n";
            outText =  QString("<char>\t") + QString("0x9EE\t") +  "8"+"\n"; outStream << crypto->encryptToString(outText) + "\n";
            outText =  QString("<char>\t") + QString("0x9EF\t") +  "9"+"\n"; outStream << crypto->encryptToString(outText) + "\n";

            outText =  QString("<vowelmodifier_order>\t") + ( this->modifierOrder ? "true" : "false") + "\n"; outStream << crypto->encryptToString(outText) + "\n";

            textDisable();
        }

        outFile.flush();
        outFile.close();
        auto_fill_profile_combo( profCombo->currentText());
        return true;
        }

bool Profile::addWord(const QString &newWord)
    {

  //  qDebug() << "before new word";
    if(newWord.length() ==0  ) return false;
   // qDebug() << " new word ";
    QString fName =  QLatin1String("profile/")+  currentProfile + QLatin1String(".dat");
    QFile outFile(fName);
    QString outText;
    SimpleCrypt *crypto = SimpleCrypt::getEncryptor(Q_UINT64_C(0x23bacf02473fdea0));
    if (outFile.open(QIODevice::Append | QIODevice::Text))  {
        QTextStream outStream(&outFile);
        outText =  QString("<word>\t") + newWord;
        outText = crypto->encryptToString(outText);
        outStream << outText + "\n";

        outFile.flush();
        outFile.close();
    }

 //   qDebug() << "In profile adding a word  Profile 661";
    QHash<QString,QString> charMap;
    charMap[newWord] = newWord;

    WordsTrie *profileWords = WordsTrie::getProfileWordsTrie();
   //  qDebug() << "In profile adding a word  Profile 665";
    profileWords->addWords(charMap);
   // Phonetic::insertWordFromOutside(newWord);

  //  qDebug() << "Done adding a word  Profile 665";

    return true;

}


bool Profile::deleteWord(const QString &word) {
    SimpleCrypt *crypto = SimpleCrypt::getEncryptor(Q_UINT64_C(0x23bacf02473fdea0));
    if(word.length() ==0  ) return false;

    QString outText;

    QString fName =  QLatin1String("profile/")+  currentProfile + QLatin1String(".dat");
    QFile outFile(fName);
    if (outFile.open(QIODevice::Append | QIODevice::Text))  {
        QTextStream outStream(&outFile);
        outText =  "\n<worddel>\t" + word  +"\n";
        outStream << crypto->encryptToString(outText) + "\n";
              //qDebug() << "<worddel>\t" <<word <<"\n";
        outFile.flush();
        outFile.close();
    }

 //   QHash<QString,QString> charMap;
   // charMap[newWord] = newWord;

    WordsTrie *profileWords = WordsTrie::getProfileWordsTrie();

    profileWords->deleteWord(word);
     QList<QString> keys = profileWords->charTree->links.keys();
    return true;

}


bool Profile::isValidProfile( const QString &profName) {
    SimpleCrypt *crypto = SimpleCrypt::getEncryptor(Q_UINT64_C(0x23bacf02473fdea0));

    if(profName.length() ==0 ) return false;

    QString fName = QLatin1String("profile/")+ profName + QLatin1String(".dat");
    QFile file(fName);

    if( !file.exists() ) return false;

    unsigned int valid_lines =0 ;
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
    //    qDebug() << "have opened ";
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString inStr = in.readLine().trimmed();
            QString str = crypto->decryptToString(inStr);
            if( str.length() >0 ) valid_lines++;
           // if( str.length() == 0) { file.close(); return false;}
        }
        file.close();
        return(valid_lines > 40 ?  true :  false);
    }
    return false;
}

bool Profile::fill_keyboard(const QString &profName)
    {

    SimpleCrypt *crypto = SimpleCrypt::getEncryptor(Q_UINT64_C(0x23bacf02473fdea0));
    if(profName.length() ==0 ) return false;

    QString fName = QLatin1String("profile/")+ profName.toLower() + QLatin1String(".dat");

    QFile file(fName);
    //qDebug()<<"Reading Profile :  "<<fName;

    QHash<QString, QString> charMap;
    QHash<QString, QString> profileWordMap;

    QStringList list ;

    CharTrie *charTrie = CharTrie::getCharTrie();
    bool matra_order = false;

    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        currentProfile = profName;
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString inStr = in.readLine().trimmed();
            //qDebug() << inStr;
            QString str = crypto->decryptToString(inStr);

            if( str.length() > 0) {
                //qDebug() << str;
                list = str.split("\t");


                // if it is a characted then add it
                if( list.at(0).trimmed() == QString("<char>")) {
                  charMap[list.at(1).trimmed().toLower()] = list.at(2 ).trimmed();
                //  qDebug() << "<char> " << list.at(1 ).trimmed() <<"\t" << list.at(2 ).trimmed();
                }


                // add a word on <word>
                if( list.at(0).trimmed() == QString("<word>")) {
                  profileWordMap[list.at(1).trimmed().toLower()] = list.at(1 ).trimmed().toLower();
                  // qDebug() << "adding a word";
                }

                // delete a word on <worddel>
                if( list.at(0).trimmed() == QString("<worddel>")) {
                  if(profileWordMap.contains(list.at(1).trimmed().toLower())) {
                     profileWordMap.remove(list.at(1).trimmed().toLower());
                   //  qDebug() << "removing a word";

                  }
                }


                if( list.at(0).trimmed() == QString("<vowelmodifier_order>")) {
                    if( list.at(1).trimmed()=="true" ) {
                      //  qDebug() << "Matra order " << matra_order;
                        matra_order = true;
                    }
                    else {
                        matra_order = false;
                    }
                }
            }
         }
       // charMap.remove(QString("0x9cd"));


        WordsTrie *profileWords = WordsTrie::getProfileWordsTrie();
        profileWords->LoadProfileDictionaryWords(profileWordMap);
        QHash<QString, QString>::const_iterator it;
        QStringList wordList;
       // qDebug() << "prof " << profileWordMap.size();
        for(it=profileWordMap.begin(); it!= profileWordMap.end(); it++) {
           wordList.append(it.key());
        //   qDebug() << it.key();
        }
      //  qDebug() <<  wordList.size();
        Phonetic::createPhoneticTreeProfile(wordList);
     //   profileWords->printData();
        charTrie->LoadCharTreeFromProfile(charMap);
    }

    file.close();
  //  qDebug() << "XXX" << charMap.key(QString("0x995"));

    if( kbd == NULL ) return false;

   // modifierOrderCheckbox->setChecked(matra_order);

    lbl[1]->setText(charMap.value(QString("0x995")));
    lbl[2]->setText(charMap.value(QString("0x996")));
    lbl[3] ->setText(charMap.value(QString("0x997")));
    lbl[4] -> setText(charMap.value(QString("0x998")));
    lbl[5] -> setText(charMap.value(QString("0x999")));
    lbl[6] -> setText(charMap.value(QString("0x99a")));
    lbl[7] -> setText(charMap.value(QString("0x99b")));
    lbl[8] -> setText(charMap.value(QString("0x99c")));
    lbl[9] -> setText(charMap.value(QString("0x99d")));
    lbl[10] -> setText(charMap.value(QString("0x99e")));
    lbl[11] -> setText(charMap.value(QString("0x99f")));
    lbl[12] -> setText(charMap.value(QString("0x9a0")));
    lbl[13]-> setText(charMap.value(QString("0x9a1")));
    lbl[14]-> setText(charMap.value(QString("0x9a2")));
    lbl[15] -> setText(charMap.value(QString("0x9a3")));
    lbl[16] -> setText(charMap.value(QString("0x9a4")));
    lbl[17] -> setText(charMap.value(QString("0x9a5")));
    lbl[18]-> setText(charMap.value(QString("0x9a6")));
    lbl[19] -> setText(charMap.value(QString("0x9a7")));
    lbl[20] -> setText(charMap.value(QString("0x9a8")));
    lbl[21] -> setText(charMap.value(QString("0x9aa")));
    lbl[22] -> setText(charMap.value(QString("0x9ab")));
    lbl[23] -> setText(charMap.value(QString("0x9ac")));
    lbl[24] -> setText(charMap.value(QString("0x9ad")));
    lbl[25] -> setText(charMap.value(QString("0x9ae")));
    lbl[26] -> setText(charMap.value(QString("0x9af")));
    lbl[27] -> setText(charMap.value(QString("0x9f0")));
    lbl[28] -> setText(charMap.value(QString("0x9b2")));
    lbl[29] -> setText(charMap.value(QString("0x9f1")));
    lbl[30] -> setText(charMap.value(QString("0x9b6")));
    lbl[31] -> setText(charMap.value(QString("0x9b7")));
    lbl[32] -> setText(charMap.value(QString("0x9b8")));
    lbl[33]-> setText(charMap.value(QString("0x9b9")));
    lbl[34]-> setText(charMap.value(QString("0x9950x9cd0x9b7")));
    lbl[35] -> setText(charMap.value(QString("0x9dc")));
    lbl[36] -> setText(charMap.value(QString("0x9dd")));
    lbl[37] -> setText(charMap.value(QString("0x9df")));
    lbl[38]-> setText(charMap.value(QString("0x981")));
    lbl[39] -> setText(charMap.value(QString("0x9ce")));
    lbl[40] -> setText(charMap.value(QString("0x982")));
    lbl[41] -> setText(charMap.value(QString("0x983")));
    lbl[42] -> setText(charMap.value(QString("0x985")));

    lbl[43] -> setText(charMap.value(QString("0x9be")));
    lbl[44] -> setText(charMap.value(QString("0x9bf")));
    lbl[45] -> setText(charMap.value(QString("0x9c0")));
    lbl[46] -> setText(charMap.value(QString("0x9c1")));
    lbl[47] -> setText(charMap.value(QString("0x9c2")));
    lbl[48] -> setText(charMap.value(QString("0x9c3")));
    lbl[49] -> setText(charMap.value(QString("0x9c7")));
    lbl[50] -> setText(charMap.value(QString("0x9c8")));
    lbl[51] -> setText(charMap.value(QString("0x9cb")));
    lbl[52] -> setText(charMap.value(QString("0x9cc")));
    /*
    lbl[43] -> setText(charMap.value(QString("0x986")));
    lbl[44] -> setText(charMap.value(QString("0x987")));
    lbl[45] -> setText(charMap.value(QString("0x988")));
    lbl[46] -> setText(charMap.value(QString("0x989")));
    lbl[47] -> setText(charMap.value(QString("0x98a")));
    lbl[48] -> setText(charMap.value(QString("0x98b")));
    lbl[49] -> setText(charMap.value(QString("0x98f")));
    lbl[50] -> setText(charMap.value(QString("0x990")));
    lbl[51] -> setText(charMap.value(QString("0x993")));
    lbl[52] -> setText(charMap.value(QString("0x994")));
    */
    lbl[53]-> setText(charMap.value(QString("0x9cd")));

     return false;
}

void Profile::textEnable()
{
    for(int i=1;i<=53; i++)
      lbl[i]->setEnabled(true);

    /*
    lbl_54->setEnabled(true);
    lbl_55 ->setEnabled(true);

    lbl_56 ->setEnabled(true);
    lbl_57 ->setEnabled(true);
    lbl_58->setEnabled(true);
    lbl_59 ->setEnabled(true);
    lbl_60 ->setEnabled(true);

    lbl_61->setEnabled(true);
    lbl_62 ->setEnabled(true);
    lbl_63 ->setEnabled(true);
    */
    setChangeEnable();
}

void Profile::textDisable()
{
    for(int i=1;i<=53; i++)
      lbl[i]->setEnabled(false);

    /*
    lbl_54->setEnabled(false);
    lbl_55 ->setEnabled(false);

    lbl_56 ->setEnabled(false);
    lbl_57 ->setEnabled(false);
    lbl_58->setEnabled(false);
    lbl_59 ->setEnabled(false);
    lbl_60 ->setEnabled(false);

    lbl_61->setEnabled(false);
    lbl_62 ->setEnabled(false);
    lbl_63 ->setEnabled(false);
    */
    setChangeDisable();

}


void Profile::auto_fill_profile_combo(QString profilename)
  {
         QString dir = QDir::currentPath();
       //  qDebug()<<"Working Directory : "<<dir;
         QString cDir = dir+ QDir::separator() +"profile";
         QDirIterator it(cDir, QDirIterator::Subdirectories);
         QString  shortname;
   //      profCombo->currentText();
          profCombo->clear();
          if( profilename.length() > 0)
              profCombo->addItem(profilename);
          while (it.hasNext()) {
              QString _nam = it.next();
              QString _name =  it.fileInfo().fileName();
                if(_name.contains(".dat")){
                 //  qDebug()<<"After choping"<<_name.replace(".dat","");
                   shortname = _name.replace(".dat","");
                   if( shortname != profilename)
                      profCombo->addItem(_name.replace(".dat",""));
                }
          }


        }



/*
    for(int i=1;i<keys.size();i++){
        if(keys.at(i).isNull() || keys.at(i).isEmpty())
            QLineEdit::QLineEdit(QString("lbl_%1").arg(i)).setText("0");
       else QLineEdit::QLineEdit(QString("lbl_%1").arg(i)).setText(keys.at(i));
        qDebug()<<QString("lbl_%1").arg(i)<<"   "<<keys.at(i);
    }
*/
void Profile::setChangeEnable(){
    changeEnDisableFlag = true;

}

void Profile::setChangeDisable(){
    changeEnDisableFlag = false;

}

bool Profile::isChangeEnable(){

   return changeEnDisableFlag;
}
