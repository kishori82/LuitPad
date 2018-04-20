#include "dictionary.h"

Dictionaries *Dictionaries::dictionaries = NULL;

Dictionaries *Dictionaries::getDictionaries() {

    if( dictionaries == NULL )
        dictionaries = new Dictionaries();

    return dictionaries;

}

Dictionaries::Dictionaries() {
    assamese = new Dictionary;
    english = new Dictionary();
    assamesePoribhasha = new Dictionary();
    englishPoribhasha = new Dictionary();

}

void Dictionaries::setAssameseEnglishDictionaries(QString dictionaryFile) {
    QFile file(dictionaryFile);
    if(!file.open(QIODevice::ReadOnly)) {
            QMessageBox::information(0, "error", file.errorString());
    }

    QTextStream in(&file);

    unsigned int i =0;
    QString line;
    while(!in.atEnd()) {
        line = in.readLine();
      //  qDebug() << line;
        assamese->set_New_Entry(line);
        english->set_New_Entry_Reversed(line);
        i++;
    }
    qDebug() << "Number of words inserted to assamese-english dictionay " << QString::number(i);
    file.close();



}



void Dictionaries::setPoriBhashaDictionaries(QString poribhashaFile) {
    QFile file(poribhashaFile);
    if(!file.open(QIODevice::ReadOnly)) {
            QMessageBox::information(0, "error", file.errorString());
    }

    QTextStream in(&file);


    QString line;
    while(!in.atEnd()) {
        line = in.readLine();
        englishPoribhasha->set_New_Entry(line);

    }

    file.close();

}

Dictionary *Dictionaries::getDictionary(unsigned int i) {
    switch(i) {
       case 0:
             return assamese;
             break;
       case 1:
             return english;
             break;
       case 2:
             return assamesePoribhasha;
             break;
       case 3:
             return englishPoribhasha;
             break;
       default:
             return assamese;
             break;
    }

}


Dictionary::Dictionary()
{

}

Dictionary::Dictionary(QString dictionaryFile, QString poribhashaFile) {


}


void Dictionary::set_New_Entry(const QString str) {
    QStringList fields = str.split("\t");
    if( fields.size()<2) return;
    QString word = fields.at(0).trimmed();
    if( word.size() <1) return;
    QString trimmedField;

    unsigned int index = meanings.size();

    for(unsigned int i = 1; i< fields.size(); i++) {
        trimmedField = fields.at(i).trimmed();
        if(trimmedField.size()< 1) continue;
/*
        if( !tempHash.contains(trimmedField)) {
            index = meanings.size();
            meanings.push_back(trimmedField);
            tempHash.insert(trimmedField,index);
        } else {
            index = tempHash[trimmedField];
        }
*/

        if( !words.contains(word)) {
            words.insert(word, localTempHash);
        }

        if( !words[word].contains(trimmedField)) {
            words[word].insert(trimmedField, index);
        }
    }
}

void Dictionary::set_New_Entry_Reversed(const QString str) {
    QStringList fields = str.split("\t");
    if( fields.size()<2) return;
    QString word = fields.at(0).trimmed();
    if( word.size() <1) return;
    QString trimmedField;

    unsigned int index = meanings.size();

    for(unsigned int i = 1; i< fields.size(); i++) {
        trimmedField = fields.at(i).trimmed();
        if(trimmedField.size()< 1) continue;
        if( trimmedField.split(" ").size() > 1 ) continue;

        if( !words.contains(trimmedField)) {
            words.insert(trimmedField, localTempHash);
        }

        if( !words[trimmedField].contains(word)) {
            words[trimmedField].insert(word, index);
        }

    }
}


QStringList Dictionary::getWords() {
    QStringList results = words.keys();

    return results;
}

bool  Dictionary::hasWord(QString word) {
    if( words.contains(word))
        return true;
    else
        return false;
}


QStringList Dictionary::getMeanings(const QString str) {
    QStringList results;
    if( words.contains(str) ) {
        results = words[str].keys();
    }

    return results;
}


