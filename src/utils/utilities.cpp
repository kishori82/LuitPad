
#include <algorithm>

#include "src/phonetic/phonetic.h"
#include "src/utils/utilities.h"

using namespace std;

QHash<QString, QString> *Utilities::vowelMap=NULL;
QHash<QString, QString> *Utilities::inverseVowelMap=NULL;
QHash<QString, QString> *Utilities::consonantMap=NULL;
QHash<QString, bool> *Utilities::zeroLengthChar = NULL;
QHash<QString, unsigned int> *Utilities::alphabetOrder = NULL;

float Min(float a, float b) {
   return a<b? a: b;
}


#ifdef _WINDOWS
QString Utilities::driveIDs(){

    char  g_szDrvMsg[] = "A:\n";
    ULONG uDriveMask = _getdrives();
    QString mesg="";

    if (uDriveMask == 0)
    {
       printf( "_getdrives() failed with failure code: %d\n",
       GetLastError()); //So GetLastError retuns a sring or char*?
    }
    else
    {
        while (uDriveMask) {
            if (uDriveMask & 1) {
                printf(g_szDrvMsg);
                mesg = mesg + Utilities::driveID(QString(g_szDrvMsg[0]));
            }
            g_szDrvMsg[0] =  g_szDrvMsg[0] +1;
            uDriveMask >>= 1;
        }
    }

    return mesg;
}


QString Utilities::driveID(QString driveLetter) {

    DWORD dwVolSerial;
    BOOL bIsRetrieved;
    QString extDriveLetter= driveLetter + QString(":\\\\");
    LPCTSTR driveWord = (LPCTSTR)driveLetter.utf16();
}

#endif

bool Utilities::isAnInflectionalForm(QString a, QString b) {

    QString rootWord = Phonetic::getRootWord(b);

    if(a.compare(rootWord)==0) return true;
    return false;
}

QPixmap Utilities::UpdateAlphaChannel(QPixmap &px, unsigned int i, unsigned int dissolve_span,  QVector<QPoint> &rVector)
{

    QImage image(px.toImage());
    QVector<QPoint>::const_iterator it1, it2, it;
    it1= rVector.begin() + (unsigned long)(rVector.size()*i*1/(double)dissolve_span) ;
    it2= it1+ (unsigned long)((double)rVector.size()/(double)dissolve_span);

    for(it = it1; it != it2; ++it){
        QColor color(image.pixel(*it));
        color.setAlpha(0);
        image.setPixel(*it,color.rgba());

    }
    return  QPixmap::fromImage(image);
}


void Utilities::CreateRandomVector(QPixmap &px, QVector<QPoint> &rVector) {

    QImage image(px.toImage());

    for(int x = 0; x < image.width(); x++){
       for(int y = 0; y < image.height(); y++) {
           rVector.push_back( QPoint(x,y));
       }
    }
    return  ;

}



void Utilities::ShowSplashScreen(QString pngFile) {
    class SleeperThread : public QThread
    {
          public:
             static void msleep(unsigned long msecs) {QThread::msleep(msecs);}

    };


    QPixmap aPixmap(pngFile);
    QLabel* aWidget = new QLabel(0, Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint);

    aWidget->setAttribute(Qt::WA_TranslucentBackground);
    aWidget->setAlignment(Qt::AlignCenter );

    int w,h;
    w = aWidget->width()/0.9;
    h = aWidget->height()/0.9;
    aPixmap = aPixmap.scaled(w,h,Qt::KeepAspectRatio);
    QVector<QPoint> rVector;

    Utilities::CreateRandomVector(aPixmap, rVector) ;
    std::random_shuffle( rVector.begin(), rVector.end() );
    unsigned int dissolve_span =150;
    unsigned int i;

    for(i = 0; i < dissolve_span; i++ ){
        aPixmap = Utilities::UpdateAlphaChannel( aPixmap,i,dissolve_span, rVector);
        aWidget->setPixmap(aPixmap);
        aWidget->show();
        SleeperThread::msleep(20);
        aWidget->repaint();
    }
    aWidget->hide();
}



QLabel *Utilities::ShowWaitScreen(QString pngFile) {
    class SleeperThread : public QThread
    {
          public:
             static void msleep(unsigned long msecs) {QThread::msleep(msecs);}

    };

    QPixmap aPixmap(pngFile);
    QLabel *aWidget = new QLabel(0, Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint);

    aWidget->setAttribute(Qt::WA_TranslucentBackground);

    aWidget->setAlignment(Qt::AlignCenter );
    aWidget->setPixmap(aPixmap);
    aWidget->show();
    aWidget->repaint();
    return aWidget;
}

void Utilities::initializeAlphabetOrder() {
    if(alphabetOrder != NULL) return;
    alphabetOrder = new QHash<QString, unsigned int>();
    alphabetOrder->insert( "985", 10);
    alphabetOrder->insert( "986", 11);

    alphabetOrder->insert( "987", 12);
    alphabetOrder->insert( "988", 13);
    alphabetOrder->insert( "989", 14);
    alphabetOrder->insert( "98a", 15);
    alphabetOrder->insert( "98b", 16);
    alphabetOrder->insert( "98f", 17);
    alphabetOrder->insert( "990", 18);
    alphabetOrder->insert( "993", 19);
    alphabetOrder->insert( "994", 20);
    alphabetOrder->insert( "9be",11);
    alphabetOrder->insert( "9bf",12);
    alphabetOrder->insert(  "9c0", 12); //13
    alphabetOrder->insert(  "9c1",14);
    alphabetOrder->insert(  "9c2",14);  //15
    alphabetOrder->insert(  "9c3",16);
    alphabetOrder->insert(  "9c7", 17);
    alphabetOrder->insert(  "9c8",18);
    alphabetOrder->insert(  "9cb", 14);  //19
    alphabetOrder->insert(  "9cc", 20);

    alphabetOrder->insert("995", 21);
    alphabetOrder->insert("996", 22);
    alphabetOrder->insert("997", 23);
    alphabetOrder->insert("998", 24);
    alphabetOrder->insert("999", 25);

    alphabetOrder->insert("99a", 26);
    alphabetOrder->insert("99b", 26);  //27
    alphabetOrder->insert("99c", 28);
    alphabetOrder->insert("99d", 29);
    alphabetOrder->insert("99e", 30);

    alphabetOrder->insert("99f", 31);
    alphabetOrder->insert("9a0", 32);
    alphabetOrder->insert("9a1", 33);
    alphabetOrder->insert("9a2", 34);
    alphabetOrder->insert("9a3", 35);

    alphabetOrder->insert("9a4", 31);  //36
    alphabetOrder->insert("9a5", 32);   //37
    alphabetOrder->insert("9a6", 33);   //38
    alphabetOrder->insert("9a7", 34);    //39
    alphabetOrder->insert("9a8", 35);   //40

    alphabetOrder->insert("9aa", 41);
    alphabetOrder->insert("9ab", 42);
    alphabetOrder->insert("9ac", 43);
    alphabetOrder->insert("9ad", 44);
    alphabetOrder->insert("9ae", 45);

    alphabetOrder->insert("9af", 46);
    alphabetOrder->insert("9f0", 47);
    alphabetOrder->insert("9b2", 48);
    alphabetOrder->insert("9f1", 49);

    alphabetOrder->insert("9b6", 50);
    alphabetOrder->insert("9b7", 51);
    alphabetOrder->insert("9b8", 52);
    alphabetOrder->insert("9b9", 52);  //53

    alphabetOrder->insert("9dc", 54);
    alphabetOrder->insert("9dd", 55);
    alphabetOrder->insert("9df", 56);

}

void Utilities::initializeVowelMap() {

  if( vowelMap != NULL) return;

  vowelMap = new QHash<QString,QString>();
  vowelMap->insert( "0x986", "0x9be");

  vowelMap->insert( "0x987", "0x9bf");
  vowelMap->insert( "0x988", "0x9c0");
  vowelMap->insert( "0x989", "0x9c1");

  vowelMap->insert( "0x98a", "0x9c2");
  vowelMap->insert( "0x98b", "0x9c3");

  vowelMap->insert( "0x98f", "0x9c7");
  vowelMap->insert( "0x990", "0x9c8");

  vowelMap->insert( "0x993", "0x9cb");
  vowelMap->insert( "0x994", "0x9cc");

  inverseVowelMap = new QHash<QString,QString>();
  QHash<QString,QString>::const_iterator it;

  for(it=vowelMap->begin(); it!=vowelMap->end(); it++) {
      inverseVowelMap->insert(it.value(), it.key());
  }

}

void Utilities::initializeZeroLengthChars() {

  if( zeroLengthChar != NULL) return;

  zeroLengthChar = new QHash<QString,bool>();
  zeroLengthChar->insert( "0x9be", true);

  zeroLengthChar->insert( "0x9bf", true);
  zeroLengthChar->insert( "0x9c0", true);
  zeroLengthChar->insert( "0x9c1",true);

  zeroLengthChar->insert( "0x9c2",true);
  zeroLengthChar->insert(  "0x9c3",true);

  zeroLengthChar->insert( "0x9c7", true);
  zeroLengthChar->insert( "0x9c8", true);

  zeroLengthChar->insert( "0x9cb", true);
  zeroLengthChar->insert( "0x9cc", true);

  zeroLengthChar->insert( "0x981", true);
  zeroLengthChar->insert( "0x982", true);
  zeroLengthChar->insert( "0x983", true);
  zeroLengthChar->insert( "0x9fa", true);
  zeroLengthChar->insert( "0x9cd", true);

}

void Utilities::initializeConsonantMap() {

  if( consonantMap != NULL) return;

  consonantMap = new QHash<QString,QString>();
  consonantMap->insert("0x995","0x995");
  consonantMap->insert("0x996","0x996");
  consonantMap->insert("0x997","0x997");
  consonantMap->insert("0x998","0x998");
  consonantMap->insert("0x999","0x999");
  consonantMap->insert("0x99a","0x99a");
  consonantMap->insert("0x99b","0x99b");
  consonantMap->insert("0x99c","0x99c");
  consonantMap->insert("0x99d","0x99d");
  consonantMap->insert("0x99e","0x99e");
  consonantMap->insert("0x99f","0x99f");
  consonantMap->insert("0x9a0","0x9a0");
  consonantMap->insert("0x9a1","0x9a1");
  consonantMap->insert("0x9a2","0x9a2");
  consonantMap->insert("0x9a3","0x9a3");
  consonantMap->insert("0x9a4","0x9a4");
  consonantMap->insert("0x9a5","0x9a5");
  consonantMap->insert("0x9a6","0x9a6");
  consonantMap->insert("0x9a7","0x9a7");
  consonantMap->insert("0x9a8","0x9a8");
  consonantMap->insert("0x9aa","0x9aa");
  consonantMap->insert("0x9ab","0x9ab");
  consonantMap->insert("0x9ac","0x9ac");
  consonantMap->insert("0x9ad","0x9ad");
  consonantMap->insert("0x9ae","0x9ae");
  consonantMap->insert("0x9af","0x9af");
  consonantMap->insert("0x9f0","0x9f0");
  consonantMap->insert("0x9b2","0x9b2");
  consonantMap->insert("0x9f1","0x9f1");
  consonantMap->insert("0x9b6","0x9b6");
  consonantMap->insert("0x9b7","0x9b7");
  consonantMap->insert("0x9b8","0x9b8");
  consonantMap->insert("0x9b9","0x9b9");
  consonantMap->insert("0x9df","0x9df");
  consonantMap->insert("0x9dc","0x9dc");
  consonantMap->insert("0x9dd","0x9dd");
  consonantMap->insert("0x9ce","0x9ce");
  consonantMap->insert("0x982","0x982");
  consonantMap->insert("0x983","0x983");
}


Utilities::Utilities()
{
}

QString Utilities::reverse(QString str) {

    if( str.size() <= 0) { return (QString("")); }

    QByteArray ba = str.toLatin1();  //str.toAscii();
    char *d = ba.data();
    std::reverse(d, d+str.length());
    str = QString(d);
    return str;

}


QString Utilities::reverseX(QString str) {

    QString reverse;
    for(int i= str.length() -1; i >= 0; i--)
        reverse.append( str.at(i));

    return reverse;
}


void Utilities::trimRomanEnd(QString &newWord) {
    unsigned int len = newWord.size();

    while(len > 0 && newWord.at(len-1).isLetter() ) {
       len--;
    }
    newWord.remove(len, newWord.size());

}

 QList<QString> Utilities::split(QString str, QString delim) {

    QList<QString> str_list;

    QStringList list ;

    QStringList::const_iterator it_list;

    if( delim.size() == 0) {
       for( int i =0; i < str.size(); i++) {
           str_list.append(QString( str[i]));
       }
     //  qDebug() << "split " << str_list.size();
       return str_list;
    }

    list = str.split(delim);
    for(it_list = list.constBegin(); it_list != list.constEnd(); ++it_list ) {
      if( QString::compare(QString(""), *it_list)!=0) {
          str_list.append(*it_list);
      }
    }
    return str_list;

}


QString Utilities::createStringFromCodeList( QList<QString> list) {

    QString text("");
    bool ok;

    QList<QString>::const_iterator it_list;
    for(it_list = list.constBegin(); it_list != list.constEnd(); ++it_list ) {
        text += QChar(it_list->toUInt(&ok, 16));
    }
    //qDebug() << "List size utils 213" << list.size() << "   " << text;
    return text;

};

/*
QString Utilities::createStringFromCodeList( QList<QString> list) {

    QString text("");
    bool ok;

    QList<QString>::const_iterator it_list;
    for(it_list = list.constBegin(); it_list != list.constEnd(); ++it_list ) {
        text += QChar(it_list->toUInt(&ok, 16));
    }

    return text;

};
*/

QStringList  Utilities::getStringListFromHexString(QString word, QString delim, int len) {
    QStringList firstList = word.split(delim);
    QStringList resultList ;

    // currently we are not using the len
    foreach(QString code, firstList) {
        if( code.trimmed().length() !=0 ) {
     //       qDebug() << code.trimmed().length() << "<<" <<  code.trimmed()<< ">>";
           resultList.append( code.trimmed());
        }
    }

    return resultList;

}


QStringList  Utilities::getStringListFromUnicodeString(QString unicodeString) {
    QStringList stringlist;

    foreach(QChar str, unicodeString) {
        stringlist.append(Utilities::convertToHexString( str.unicode()));
     //   qDebug() << "Code : " << Utilities::convertToHexString( str.unicode()) ;
    }
  //  qDebug() << "String list" << stringlist;
    return stringlist;


}

QString Utilities::createStringFromCodeList( QList<QString> list, int base) {

    QString text("");
    bool ok;

    QList<QString>::const_iterator it_list;
    for(it_list = list.constBegin(); it_list != list.constEnd(); ++it_list ) {
        text += QChar(it_list->toUInt(&ok, base));
    }

    return text;

};

bool  Utilities::isRomanAlphabet(QChar c) {
   // qDebug() << "in is " << c.unicode();
    if( (c.unicode() >= 0x61 && c.unicode()<= 0x7a)  ||  (c.unicode() >= 0x41 && c.unicode()<= 0x5a) )
        return true;
    else
        return false;
}


bool  Utilities::isRomanAlphabetDigit(QChar c) {
   // qDebug() << "in is " << c.unicode();
    if(  (c.unicode() >= 0x30 && c.unicode()<= 0x39) ||
         (c.unicode() >= 0x61 && c.unicode()<= 0x7a)  ||  (c.unicode() >= 0x41 && c.unicode()<= 0x5a) )
        return true;
    else
        return false;
}

QString Utilities::getUnicodeForWebPage(QString str, QString delim){

  //  qDebug() << "Getting unicode";
    QStringList wordCodes = Utilities::getStringListFromHexString(str,delim);
    QString output ="";

    foreach(QString character, wordCodes) {
        output.append( QString("&#x") + character + QString(";") );
    }
    return output;
}


QString Utilities::getUnicode(QString str, QString delim){

  //  qDebug() << "Getting unicode";
    return Utilities::createStringFromCodeList(Utilities::split(str, delim));

}

// fields start at 0, 1, 2....
QString Utilities::getField(QString str, QString delim, unsigned int i){

    QStringList fields;
  //  qDebug() << "Getting unicode";
    fields = Utilities::split(str, delim);
    if( fields.size() > (int) i  ) {
        return fields.at(i).trimmed();
    }
    return QString("");
}


QString Utilities::padWithSpaces(QString s,unsigned int n) {
    QString padded =s;
    if( n == 0 ) { return QString(""); }
    for( unsigned int i=0; i < n- s.length(); i++) {
        padded.append(" ");
    }
    return padded;
}


QString Utilities::wordUnderCursor(QTextEdit *textDocument) {

    QTextCursor tc= textDocument->textCursor();

    tc.movePosition(QTextCursor::Left);

    tc.select( QTextCursor::WordUnderCursor );

   // tc.select( QTextCursor::BlockUnderCursor );


    return tc.selectedText();

}


bool Utilities::isAssamese( const QString &str ) {
    return Utilities::isValidCompleterPrefix(str);
}

bool Utilities::isValidCompleterPrefix(QString str) {
    QChar qchar;

    if(str.size() <=0) return false;

    for( int i =0 ; i < str.size(); i++) {
         qchar = str.at(i);
         if(
             !((2433 <= qchar.unicode() && qchar.unicode() <= 2554) ||

                     (qchar.unicode() == 0x27 ) ||
                     (qchar.unicode() == 0x19 ) ||
                     (qchar.unicode() == 0x2010 ) ||
                     (qchar.unicode() == 0x2011 ) ||
                     (qchar.unicode() == 0x2d )
                   )
         ) {
     //        qDebug() << "completer false";
             return false;
         }
    }
   // qDebug() << "completer true";
    return true;
}


QString Utilities::getUnicodeString(QString str) {
    QChar qchar;
    QString unicodeString="";

    if(str.size() <=0) return unicodeString;

    for( int i =0 ; i < str.size(); i++) {
         qchar = str.at(i);
         if( (2433 <= qchar.unicode() && qchar.unicode() <= 2554) ||
              (qchar.unicode() == 0x27 ) ||
              (qchar.unicode() == 0x19 ) ||
              (qchar.unicode() == 0x2010 ) ||
              (qchar.unicode() == 0x2011 ) ||
              (qchar.unicode() == 0x2d )

           ) {
             unicodeString = unicodeString + "0x" + Utilities::convertToHexString(qchar.unicode()).toLower();
         }
    }

    return unicodeString;

}

QString Utilities::getUnicodeStringX(QString str) {
    QChar qchar;
    QString unicodeString="";

    if(str.size() <=0) return unicodeString;

    for( int i =0 ; i < str.size(); i++) {
         qchar = str.at(i);
         unicodeString = unicodeString  + Utilities::convertToHexString(qchar.unicode()).toLower();

    }

    return unicodeString;

}




bool Utilities::isDelinkCharacter(QChar qchar) {
    qchar = qchar.toLower();

    if( Utilities::convertToHexString(qchar.unicode()).toLower() =="9cd") {
     //   qDebug() << "0x" + Utilities::convertToHexString(qchar.unicode()).toLower();
        return true;
    }
    return false;
}

bool Utilities::isValidForComposition(QChar qchar) {
    qchar = qchar.toLower();

  //  qDebug() << "covert " << Utilities::convertToHexString(qchar.unicode()).toLower();
    if( Utilities::consonantMap->contains("0x" + Utilities::convertToHexString(qchar.unicode()).toLower())) {
 //       qDebug() << "  new 0x" + Utilities::convertToHexString(qchar.unicode()).toLower();
        return true;
    }
    return false;
}

bool Utilities::isValidForVowelComposition(QChar qchar) {
    qchar = qchar.toLower();

    if( Utilities::vowelMap->contains("0x" + Utilities::convertToHexString(qchar.unicode()).toLower())) {
        return true;
    }

    if( Utilities::inverseVowelMap->contains("0x" + Utilities::convertToHexString(qchar.unicode()).toLower())) {
        return true;
    }
    return false;
}

QString Utilities::convertToHexString(ushort value) {
    QString str;

    int remainder;

    if(value== 0)  return( QString("0") );

    while( value > 0 ) {
        remainder = value % 16;
        value = (ushort)value/16;

        switch(remainder) {
            case 0:
                    str.append("0");
                    break;
            case 1:
                    str.append("1");
                    break;
            case 2:
                    str.append("2");
                    break;
            case 3:
                    str.append("3");
                    break;
            case 4:
                    str.append("4");
                    break;
            case 5:
                    str.append("5");
                    break;
            case 6:
                    str.append("6");
                    break;
            case 7:
                    str.append("7");
                    break;
            case 8:
                    str.append("8");
                    break;
            case 9:
                    str.append("9");
                    break;
            case 10:
                    str.append("a");
                    break;
            case 11:
                    str.append("b");
                    break;
            case 12:
                    str.append("c");
                    break;
            case 13:
                    str.append("d");
                    break;
            case 14:
                    str.append("e");
                    break;
            case 15:
                    str.append("f");
                    break;
            default:
                    break;

        }

    }

    return Utilities::reverse(str);
}


QString Utilities::readAmbiguousSpellings(QString fileName) {
   QRegExp rx("^>\\d+");
   QFile file(fileName);
   if(!file.open(QIODevice::ReadOnly)) {
     QMessageBox::information(0, "error", file.errorString());
   }
   QString result;
   QString line;
   QTextStream in(&file);
   int count = 1;
   while(!in.atEnd()) {
       line = in.readLine();
       line = line.trimmed();
       if(rx.indexIn(line) == -1) {
         //  qDebug() << line;
           result = result +  Utilities::getUnicode(line,"0x") +"\n";
       }
       else {
           result = result + ">" + QString("%1").arg(count) +"\n";
           count++;
       }
   }

   return(result);

}

bool Utilities::toAsending( QKeyFloat & s1 , QKeyFloat & s2 )
{
    return s1.value < s2.value;

}

bool Utilities::toDesending( QKeyFloat &s1 , QKeyFloat &s2 )
{
    return s1.value > s2.value;

}

void Utilities::sortQKeyFloatList(QList<QKeyFloat> &list, bool Ascending) {
    if(Ascending )
      qSort(list.begin(), list.end(), &Utilities::toAsending);
    else
      qSort(list.begin(), list.end(), &Utilities::toDesending);
}



bool Utilities::compareUnicode(QString &s1, QString &s2) {

    QStringList list1 = Utilities::split(s1,"0x");
    QStringList list2 = Utilities::split(s2, "0x");

  //  qDebug() << "comp " << list1;
    //exit(0);
    unsigned int iLen = list1.size();
    unsigned int jLen = list2.size();
    unsigned i=0, j=0;
   // return true;
    while( i < iLen && j < jLen ) {
        if( !Utilities::alphabetOrder->contains(list1.at(i)) ) {
            i++; continue;
        }
        if( !Utilities::alphabetOrder->contains(list2.at(j)) ) {
            j++; continue;
        }

        if(Utilities::alphabetOrder->value(list1.at(i))  < Utilities::alphabetOrder->value(list2.at(j)) ) return true;
        if(Utilities::alphabetOrder->value(list1.at(i))  > Utilities::alphabetOrder->value(list2.at(j)) ) return false;
        i++;
        j++;
    }

    if( i >= iLen ) return true;
    else return false;

}


bool Utilities::toUnicodeDesending( QString &s1 , QString &s2 )
{
    return (! Utilities::compareUnicode(s1, s2));

}


bool Utilities::toUnicodeAsending( QString &s1 , QString &s2 )
{
    return Utilities::compareUnicode(s1, s2);

}


void Utilities::sortQStringList(QStringList &list, bool Ascending) {
    if(Ascending )
      qSort(list.begin(), list.end(),&Utilities::toUnicodeAsending);
    else
      qSort(list.begin(), list.end(),&Utilities::toUnicodeDesending);
}

bool Utilities::YesNoDialog(QString infoText, QString confimText) {
    QMessageBox msgBox;
    msgBox.setText(infoText);
    msgBox.setInformativeText(confimText);
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Discard | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Cancel);
    int ret = msgBox.exec();

    switch (ret) {
      case QMessageBox::Ok:
       // Save was clicked
          return true;
      case QMessageBox::Discard:
       // Don't Save was clicked
          return false;
      case QMessageBox::Cancel:
       // Cancel was clicked
          return false;
      default:
         // should never be reached
         return false;
    }
    return true;

}


bool Utilities::warningBox(QString infoText) {
    QMessageBox msgBox;
    msgBox.setText(infoText);
//    msgBox.setInformativeText(confimText);
    msgBox.setStandardButtons(QMessageBox::Ok );

    msgBox.exec();

    return true;

}

QString Utilities::getImageFileName(QString str) {
      QRegExp rxleft("<img src=\"file:///");
      QRegExp rxleft1("<img src=\"");


      str.remove(rxleft);
      str.remove(rxleft1);

      QRegExp rx("^([^\".]+)");

      QString fileName="did not match";

      if( rx.indexIn(str) != -1 ) {
          fileName = rx.cap(1);
          fileName = fileName.trimmed();
          //value.remove(rxDoubleQuote);
          return fileName;
      }

      return fileName;
}

/*
fileFormat.insert(".png", "PNG");
fileFormat.insert(".jpg", "JPEG");
fileFormat.insert(".jpeg", "JPEG");
fileFormat.insert(".bmp", "BMP");
fileFormat.insert("svg", "SVG");
*/

QString Utilities::getImageDatainBase64(QTextDocument *doc, QString imageFileName) {

    QRegExp rx("\"");
    imageFileName.remove(rx);
    QImageReader reader(imageFileName);

    QImage image = reader.read();
    if(image.isNull() )
    {
       // qDebug() << "Image name " << imageFileName;
       QVariant image_data=doc->resource(QTextDocument::ImageResource, QUrl(imageFileName));
       image=image_data.value<QImage>();
    }

    QByteArray byteArray;
    QBuffer buffer(&byteArray);

    //qDebug() << "Data" << image.size();
    image.save(&buffer,"PNG" );
    QString iconBase64 = QString::fromLatin1(byteArray.toBase64().data());
  //  qDebug() << iconBase64;
    return iconBase64;

}

 QString Utilities::toLuitPadFormat(QTextDocument *doc, QString htmlCode, QString ignoreWordsString ) {
     int count = 0;
     int pos = 0;

     QString luit_images("\n<LuitPad_images>\n");
     QRegExp rx("<img src=\"[^<]+/>");

     QString strToReplaceWith;

     QHash<QString, QString>  imageNameHash;
     QString imageFileName;


     while ((pos = rx.indexIn(htmlCode, pos)) != -1)
     {
         imageFileName= Utilities::getImageFileName(rx.cap(0));

         QString width = Utilities::getAttribute(rx.cap(0), QString("width"));
         QString height =  Utilities::getAttribute(rx.cap(0), QString("height"));

         QString sizeAttributes ="";
         if( width.length() > 0 ) {
             sizeAttributes += QString(" width=\"") + width  + QString("\"");
         }
         if( height.length() > 0 ) {
             sizeAttributes += QString(" height=\"") + height  + QString("\"");
         }

         if(!imageNameHash.contains(imageFileName)) {
              imageNameHash.insert(imageFileName, QString("\"LuitPad_image_%1\"").arg(count));
              strToReplaceWith = QString("<img src=\"LuitPad_image_%1\" ").arg(count) + sizeAttributes + QString(" />");
              count++;
         }
         else {
             strToReplaceWith = QString("<img src=") + imageNameHash.value(imageFileName) + sizeAttributes + QString(" />");
         }


         htmlCode.replace(pos, rx.matchedLength(), strToReplaceWith);
         pos +=  strToReplaceWith.length();

     }


     QHash<QString, QString>::const_iterator it;
     for( it = imageNameHash.begin(); it!=imageNameHash.end(); ++it) {
        luit_images.append(QString("<LuitPad_image name=%1>\n").arg(it.value()));
        luit_images.append(Utilities::getImageDatainBase64(doc,it.key()));
        luit_images.append("</LuitPad_image>\n");
     }

     luit_images.append("</LuitPad_images>");
     htmlCode.append(luit_images);

     if( ignoreWordsString.size() > 0) htmlCode.append(ignoreWordsString);

     // deal with the ignore list
   //  qDebug() << htmlCode;

     return( htmlCode);

 }



bool Utilities::IsTag(QString str, QString tag){

    QRegExp rx( "<\\s*" + tag);
    if( rx.indexIn(str) != -1 ) {
        return true;

    }
    return false;

}

QString Utilities::getAttribute(QString str, QString attr) {
    QRegExp rx(attr + QString("\\s*=\\s*\"([^\\s][^\".]*)\""));
    QString value;
    QString  rxDoubleQuote("\"");
    QRegExp rxattr(attr + QString("\\s*="));
    if( rx.indexIn(str) != -1 ) {
        value = rx.cap(1);
        value.remove(rxattr);
        //value.remove(rxDoubleQuote);
        return value;
    }
    return QString("");
}



QString Utilities::toPdfConvertibleLuitPadFormat(QTextDocument *doc, QString htmlCode) {

    int pos = 0;

    QRegExp rx("<img src=\"[^<.]+/>");
    QString strToReplaceWith;
    QString imageFileName;


    while ((pos = rx.indexIn(htmlCode, pos)) != -1)
    {
        imageFileName= Utilities::getImageFileName(rx.cap(0));
       // qDebug() << rx.cap(0);
        QString width = Utilities::getAttribute(rx.cap(0), QString("width"));
        QString height =  Utilities::getAttribute(rx.cap(0), QString("height"));

        QString sizeAttributes ="";
        if( width.length() > 0 ) {
            sizeAttributes += QString(" width=\"") + width  + QString("\"");
        }
        if( height.length() > 0 ) {
            sizeAttributes += QString(" height=\"") + height  + QString("\"");
        }

        QString data = Utilities::getImageDatainBase64(doc,imageFileName);
        strToReplaceWith = QString("<img src=\"data:image/png;base64,");
        strToReplaceWith.append(data);
        strToReplaceWith.append("\"");
        strToReplaceWith.append(sizeAttributes);
        strToReplaceWith.append(" />");
   //     qDebug() << strToReplaceWith;
        htmlCode.replace(pos, rx.matchedLength(), strToReplaceWith);
        pos +=  strToReplaceWith.length();

    }
   // qDebug() << htmlCode;
    return htmlCode;
}



void Utilities::writeStringToUnicode(QString fileName, QString output, unsigned int col) {
     QFile outFile(fileName);
     outFile.open(QIODevice::WriteOnly);
     QString outText;

     if(!outFile.isOpen()) {
         qDebug() << "Error, unable to open" << fileName << "for output";
         return;
     }
     QTextStream outStream(&outFile);

     QStringList wordsList = output.split("\n");
     unsigned int count =0;
     QHash<QString, bool> seenWords;
     foreach(QString word, wordsList) {
         QStringList words = word.split("\t");
         if( words.size() > col) {
             QString unicode = words.at(col).trimmed();
             if( !seenWords.contains(unicode) ) {
                 if( outText.size() > 0  )
                     outText.append("\n" + Utilities::getUnicodeString(unicode));
                 else
                     outText.append( Utilities::getUnicodeString(unicode));
                 seenWords.insert(unicode,true);
                 count++;
             }
         }
         if( count%5000 ==0 ) qDebug() << " Words written = " << count;
     }
     //qDebug() << outText;
     outStream <<  outText;
     outFile.flush();
     outFile.close();

 }






///////////////////////////////////
bool Utilities::bisargahAtEnd(QString &str) {

    if( str.size()> 0) {
        QChar c = str.at( str.size()-1);
        if( c.unicode() == 0x983 )
            return true;
        else
            return false;


    }
    return false;
}


bool Utilities::bisargahAnywhere(QString &str) {

    foreach( QChar c, str) {
        if( c.unicode() == 0x983)
            return true;
    }
    return false;
}


bool Utilities::hasCharAnywhere(QString &str, ushort uCode) {

    foreach( QChar c, str) {
        if( c.unicode() == uCode)
            return true;
    }
    return false;
}

 bool Utilities::hyphenPresent(QString &str) {
     foreach( QChar c, str) {
         if( c.unicode() == 0x2d )
             return true;
     }
     return false;


 }

 bool Utilities::hoHoPresent(QString &str) {

     ushort ho = 0x9b8;
     ushort Ho = 0x9b9;
     ushort dlink = 0x9cd;

     QChar a;
     QChar b;

     foreach( QChar c, str) {
         if( a.unicode()==ho && b.unicode()==dlink && c.unicode() == Ho  )
             return true;
        a = b;
        b = c;
     }
     return false;
 }
 bool Utilities::shortWord(QString &str, unsigned int len) {

     if( str.size() <=len )
         return true;
     else
         return false;
 }

#ifdef _WINDOWS
QString Utilities::getSectorNumber(QString driveLetter) {


    LPCTSTR  fileName = L"F:\\serial1.txt";

    // Get the file's extents
    DWORD bytesReturned;

    // Get a handle to the file
    HANDLE fileHandle = CreateFile(
                                       fileName,
                                       GENERIC_READ | GENERIC_WRITE,
                                       FILE_SHARE_READ|FILE_SHARE_WRITE,
                                       NULL,
                                       OPEN_EXISTING,
                                       FILE_ATTRIBUTE_NORMAL,
                                       NULL
                                   );

\
    if (fileHandle == INVALID_HANDLE_VALUE)
    {
        perror("Cannot open file\n"); getchar();
        return QString();
    }



    // Saying we want the begining of the file
    STARTING_VCN_INPUT_BUFFER inBuffer;
    inBuffer.StartingVcn.QuadPart = 0;

    int extentsBufferSize = 1024;

    // Creating a buffer for the extents information
    PRETRIEVAL_POINTERS_BUFFER lpRetrievalPointersBuffer =(PRETRIEVAL_POINTERS_BUFFER) malloc(extentsBufferSize);


    if (
            !DeviceIoControl(
                                fileHandle,
                                FSCTL_GET_RETRIEVAL_POINTERS,
                                &inBuffer,
                                sizeof(STARTING_VCN_INPUT_BUFFER),
                                lpRetrievalPointersBuffer,
                                extentsBufferSize,
                                &bytesReturned,
                                NULL
                             )
      )
    {
        perror("Cannot open Device controller\n"); getchar();
        return  QString();
    }

    // Got the file's extents !
    CloseHandle(fileHandle);


    // Send the first extent's LCN to translation to physical offset from the beginning of the disk
\

    char szTemp[100];
    sprintf(szTemp,"%ld",lpRetrievalPointersBuffer -> Extents [0].Lcn);

    free(lpRetrievalPointersBuffer);

    return QString(szTemp);
}

#endif
