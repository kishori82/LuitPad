#include "tooltipsingleton.h"

ToolTipSingleton *ToolTipSingleton::object=NULL;
QHash<QString, int> *ToolTipSingleton::charUsageFrequency = NULL;

ToolTipSingleton::ToolTipSingleton()
{
    state = 0;

    this->resetRowCol();
    rowSizes[0] = 3;
    rowSizes[1] = 3;
    rowSizes[2] = 4;


    vowelList[0].append(QString("0x9be"));
    vowelList[0].append(QString("0x9bf"));
    vowelList[0].append(QString("0x9c0"));

    vowelList[1].append(QString("0x9c1"));
    vowelList[1].append(QString("0x9c2"));
    vowelList[1].append(QString("0x9c3"));

    vowelList[2].append(QString("0x9c7"));
    vowelList[2].append(QString("0x9c8"));
    vowelList[2].append(QString("0x9cb"));
    vowelList[2].append(QString("0x9cc"));


}

int ToolTipSingleton::SetCharacterList(QList<QString> list) {

//    charlist = list;
    return charlist.size();

}

 ToolTipSingleton * ToolTipSingleton::GetToolTipSingleton(QString char_freq ) {
    if( object == 0) {
        object = new ToolTipSingleton();
        Utilities::initializeVowelMap();
        if(char_freq.length()>0) {
           initializeCharFrequency(char_freq);
        }
    }

    return object;

}

void ToolTipSingleton::initializeCharFrequency(QString char_freq_fileName) {
    QFile file(char_freq_fileName);
    if(!file.open(QIODevice::ReadOnly)) {
            QMessageBox::information(0, "error", file.errorString());
    }

    charUsageFrequency = new QHash<QString, int>();

    QTextStream in(&file);
    QStringList list ;


    QString line;
    while(!in.atEnd()) {
        line = in.readLine();
        list = line.split("\t");
      //  qDebug() << list;
        if( list.size() >= 1 ) {
            if( list.at(0).trimmed().size() !=0 )
               charUsageFrequency->insert(list.at(0).trimmed().toLower(), list.at(1).trimmed().toInt());
        }
    }


    file.close();

}


 void ToolTipSingleton::incrementIndex() {
     index++;
     index = index % keyvaluelist.size();
 }

 void ToolTipSingleton::decrementIndex() {

     index = index + keyvaluelist.size();
     index--;
     index = index % keyvaluelist.size();
 }


 void ToolTipSingleton::incrementRow() {
     row++;
     row = row % 3;
 }


 void ToolTipSingleton::decrementRow() {
     row = row + 3;
     row--;
     row = row % 3;
 }

 void ToolTipSingleton::incrementCol() {
     col++;
     col = col % rowSizes[row];
 }


 void ToolTipSingleton::decrementCol() {
     col = col + rowSizes[row];
     col--;
     col = col % rowSizes[row];
 }


unsigned int ToolTipSingleton::getSize() {
     return keyvaluelist.size();
}

 QString ToolTipSingleton::getAtIndex() {
     if( index > -1 && keyvaluelist.size() > 0)
         return( keyvaluelist.at(index).value);
     else
         return("");
 }


 QString ToolTipSingleton::getAtRowCol() {
  //   qDebug() << "vowel " << vowelList[row].at(col);
    return( vowelList[row].at(col));

 }


bool ToolTipSingleton::hasAssamesePrefix(TextEdit *textDocument) {
     QTextCursor tc= textDocument->textCursor();

     tc.movePosition(QTextCursor::Left);
     tc.select( QTextCursor::WordUnderCursor );
     QString word =Utilities::reverse(tc.selectedText());
     unsigned int romanCount =0;
     for(int i = 0;  i < word.size() && word.at(i).isLetter();  ++i) {
         romanCount++;
     }
     return(romanCount < word.size() ? true : false);
 }


QString ToolTipSingleton::trimRomanSuffix(TextEdit *textDocument) {

    QTextCursor tc= textDocument->textCursor();

    tc.movePosition(QTextCursor::Left);

    tc.select( QTextCursor::WordUnderCursor );

    QString word =tc.selectedText();
    //QChar *data= tc.selectedText().data() ;


    unsigned int j = 0;
    for(int i = word.size() - 1;  (i >=0  && Utilities::isRomanAlphabet(word.at(i))) ; i--) {
      //  qDebug() << QChar(word.at(i)).unicode();
        j++;
    }
    //qDebug() << "Number of suffix to trim" << j;
    return word.left(word.size() - j);
}


 QString ToolTipSingleton::charPrefix(TextEdit *textDocument) {

     QTextCursor tc= textDocument->textCursor();

     tc.movePosition(QTextCursor::Left);

     tc.select( QTextCursor::WordUnderCursor );

     QString word =Utilities::reverse(tc.selectedText());
     //QChar *data= tc.selectedText().data() ;

     QString keyChar = "";

     for(int i = 0;  i < word.size() && word.at(i).isLetter();  ++i) {
         keyChar.append(word.at(i));
     }
     return Utilities::reverse(keyChar);
 }

 QString ToolTipSingleton::consonantPrefix(TextEdit *textDocument) {

     QTextCursor tc= textDocument->textCursor();

     tc.movePosition(QTextCursor::Left);

     tc.select( QTextCursor::WordUnderCursor );

     QString word =Utilities::reverse(tc.selectedText());
     //QChar *data= tc.selectedText().data() ;

     QString keyChar = "";

     for(int i = 0;  i < word.size() && !word.at(i).isLetter();  ++i) {
         keyChar.append(word.at(i));
     }
     return Utilities::reverse(keyChar);
 }


 QString ToolTipSingleton::charMidPrefix(TextEdit *textDocument) {

     QTextCursor tc= textDocument->textCursor();

     tc.movePosition(QTextCursor::Left);

     tc.select( QTextCursor::WordUnderCursor );

     QString word =Utilities::reverse(tc.selectedText());
     //QChar *data= tc.selectedText().data() ;

     QString keyChar = "";

     for(int i = 0;  i < word.size() ;  ++i) {
         if( word.at(i).isLetter() || word.at(i).isDigit() )
            keyChar.append(word.at(i));
     }

     return Utilities::reverse(keyChar);
 }

 void ToolTipSingleton::clearCharList() {
     charlist.clear();
     index = -1;
 }

 bool ToolTipSingleton::isEmptyCharList() {
     if( keyvaluelist.size() > 0 )
         return false;
     else
         return true;

 }

 void ToolTipSingleton::clearKeyValueList() {
     keyvaluelist.clear();
     index = -1;
 }

void ToolTipSingleton::addChar( QString c ) {
    charlist.append(c);
}

void ToolTipSingleton::addQKeyValue(QKeyValue kv) {
    keyvaluelist.append(kv);
}

bool ToolTipSingleton::toAssending( QKeyValue & s1 , QKeyValue & s2 )
{
      return (ToolTipSingleton::charUsageFrequency->value(s1.value) <  ToolTipSingleton::charUsageFrequency->value(s2.value));
}

bool ToolTipSingleton::toDesending( QKeyValue & s1 , QKeyValue & s2 )
{

    return (ToolTipSingleton::charUsageFrequency->value(s1.value) >  ToolTipSingleton::charUsageFrequency->value(s2.value));
}
void ToolTipSingleton::sortByUsage() {

    qSort(keyvaluelist.begin(), keyvaluelist.end(),&ToolTipSingleton::toDesending);
}

void ToolTipSingleton::resetIndex() {
    if( keyvaluelist.size()>0)
      index =0;
    else
      index = -1;
}

void ToolTipSingleton::resetRowCol() {
    row=1;
    col=1;
}

QString ToolTipSingleton::createToolTipText() {

    QString text;

    text = toolTipPrefix;
    text += "<table border=\"0\" align=\"center\">";

    text +=  "<tr>";
    for(int i=0; i < keyvaluelist.size(); i++) {
       text += "<td>";
       text += keyvaluelist.at(i).key;
       text += " ";
       text += "</td>";
    }
    text +=  "</tr>";


    text +=  "<tr>";

    for(int i=0; i < keyvaluelist.size(); i++) {
        text += "<td>";
        if(index==i)
           text += "<span style=\"background-color: #99FF66\">";
        text += Utilities::getUnicode( keyvaluelist.at(i).value, "0x");
        if(index==i)
            text +=  "</span>";
        text += "</td>";
    }

    text +=  "</tr>";
    text += "</table>";
    text += toolTipSuffix;

    return text;
}



QString ToolTipSingleton::createVowelModToolTipText() {

    QString text;

    text = toolTipPrefix;
    text += "<table border=\"0\" align=\"center\">";

    for(int i=0; i < 3; i++){
        text +=  "<tr>";
        for(int j=0; j <  vowelList[i].size(); j++) {
            text += "<td>";
            if(row==i && col == j) text += "<span style=\"background-color: #99FF66\">";
            text += Utilities::getUnicode(vowelList[i].at(j),"0x");
            if(row==i && col == j)  text +=  "</span>";
            text += " ";
            text += "</td>";
        }
        text +=  "</tr>";
    }

    text += "</table>";
    text += toolTipSuffix;

    return text;
}
