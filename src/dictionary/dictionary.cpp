/************************************************************************
# MIT License

Copyright (c) 2020 Kishori M. Konwar

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
******************************************************************************/

#include "dictionary.h"

Dictionaries *Dictionaries::dictionaries = NULL;

Dictionaries *Dictionaries::getDictionaries() {
  if (dictionaries == NULL)
    dictionaries = new Dictionaries();

  return dictionaries;
}

Dictionaries::Dictionaries() {
  assamese = new Dictionary;
  english = new Dictionary();
  assamesePoribhasha = new Dictionary();
  englishPoribhasha = new Dictionary();
}

void Dictionaries::loadAssameseEnglishDictionaries(
    QString IdeasFile, QString engWrdIdFile, QString asmWrdIdFile,
    QString examplesFile, QString idiomsFile, QString poribhashaFile) {

  QFile file(IdeasFile);
  if (!file.open(QIODevice::ReadOnly)) {
    QMessageBox::information(0, "error", file.errorString());
  }

  QTextStream in(&file);
  unsigned int i = 0;
  QStringList fields;
  while (!in.atEnd()) {
    fields = in.readLine().split('\t');
    if (fields.size() >= 3) {
      assamese->asmIdeaIdIdea.insert(fields[0], fields[2]);
      assamese->engIdeaIdIdea.insert(fields[0].toLower(), fields[1]);
      i++;
    }
  }
  qDebug() << "Number of lines inserted from " << IdeasFile << " : "
           << QString::number(i);
  file.close();

  /// engWrdIdFile
  file.setFileName(engWrdIdFile);
  if (!file.open(QIODevice::ReadOnly)) {
    QMessageBox::information(0, "error", file.errorString());
  }

  QTextStream in1(&file);
  i = 0;
  while (!in1.atEnd()) {
    fields = in1.readLine().split('\t');
    if (fields.size() >= 3) {
      assamese->engWrdIdWrd.insert(fields[0], fields[1]);
      assamese->engWrdWrdId.insert(fields[1], fields[0]);
      assamese->engWrdIdIdeaId.insert(fields[0], fields[2]);
      i++;
    }
  }

  qDebug() << "Number of lines inserted from " << engWrdIdFile << " : "
           << QString::number(i);
  file.close();

  /// asmWrdIdFile
  file.setFileName(asmWrdIdFile);
  if (!file.open(QIODevice::ReadOnly)) {
    QMessageBox::information(0, "error", file.errorString());
  }

  QTextStream in2(&file);
  i = 0;
  while (!in2.atEnd()) {
    fields = in2.readLine().split('\t');
    if (fields.size() >= 3) {
      /*
     if(i < 10 ) {
          qDebug() << fields;
          for(int j=0; j < fields[1].size(); j++) {
              QChar chr = fields[1].at(j);
              qDebug() << chr.unicode();
          }
     }*/
      assamese->asmWrdIdWrd.insert(fields[0], fields[1]);
      // qDebug() <<fields[1];
      assamese->asmWrdWrdId.insert(fields[1], fields[0]);
      assamese->asmWrdIdIdeaId.insert(fields[0], fields[2]);
      i++;
    }
  }
  qDebug() << "Number of lines inserted from " << asmWrdIdFile << " : "
           << QString::number(i);
  file.close();

  /// examplesFile
  ////////// CREATE COMPRESSED FILE ////////////

  /*
  file.setFileName(examplesFile);
  if(!file.open(QIODevice::ReadOnly)) {
          QMessageBox::information(0, "error", file.errorString());
  }


  QFile filex("T_WrdExamples.tsv.compressed");
  if(!filex.open(QIODevice::WriteOnly)) {
          QMessageBox::information(0, "error", file.errorString());
  }
  filex.write(qCompress(file.readAll(), 9));
  filex.close();
  file.close();
*/
  //////////// END /////////

  file.setFileName(examplesFile);
  //    QByteArray  examples;
  i = 0;
  if (!file.open(QIODevice::ReadOnly)) {
    QMessageBox::information(0, "error", file.errorString());
  }
  // QTextStream in3(&file);

  QStringList lines = QString(qUncompress(file.readAll())).split('\n');
  // QStringList lines =  file.readAll()  //
  // QString(file.readAll()).split('\n'); qDebug() << lines;
  // qDebug() << " Reading " << examplesFile;
  // while( !in3.atEnd()) {
  foreach (QString line, lines) {
    // examples.append(line)
    // QString line = in3.readLine();
    fields = line.split('\t');
    if (fields.size() >= 2) {
      QString unicodefield0 = fields[0];
      if (assamese->asmWrdWrdId.contains(unicodefield0)) {
        QString wordId = assamese->asmWrdWrdId.value(unicodefield0);
        if (!assamese->examples.contains(wordId))
          assamese->examples.insert(wordId, QVector<QString>());
        assamese->examples[wordId].append(fields[1]);
        i++;
      }
    }
  }

  qDebug() << "Number of lines inserted from " << examplesFile << " : "
           << QString::number(i);
  file.close();

  /*
      QByteArray compressed = qCompress(examples);
      qDebug() << "uncompressed" << examples.length() << "  " <<
     compressed.length() << " " << qUncompress(compressed).length();
      file.close();
  */

  // idiomsFile
  file.setFileName(idiomsFile);
  if (!file.open(QIODevice::ReadOnly)) {
    QMessageBox::information(0, "error", file.errorString());
  }
  QTextStream in4(&file);

  i = 0;
  while (!in4.atEnd()) {
    fields = in4.readLine().split('\t');
    if (fields.size() >= 3) {
      QString unicodefield0 = fields[0];

      if (!assamese->idioms.contains(unicodefield0))
        assamese->idioms.insert(unicodefield0, QVector<QString>());
      assamese->idioms[unicodefield0].append(fields[1]);
      assamese->idioms[unicodefield0].append(fields[2]);
      i++;
    }
  }
  qDebug() << "Number of lines inserted from " << idiomsFile << " : "
           << QString::number(i);
  file.close();

  /// poribhashaFile
  file.setFileName(poribhashaFile);
  if (!file.open(QIODevice::ReadOnly)) {
    QMessageBox::information(0, "error", file.errorString());
  }
  QTextStream in5(&file);

  QHash<QString, bool> added_already;
  i = 0;
  while (!in5.atEnd()) {
    fields = in5.readLine().split('\t');
    if (fields.size() >= 2) {
      if (!added_already.contains(fields[0])) {
        assamese->officialWrd.append(
            QPair<QString, QString>(fields[0], fields[1]));
        added_already[fields[0]] = true;
      }
    }
    i++;
  }
  added_already.clear();
  qDebug() << "Number of lines inserted from " << poribhashaFile << " : "
           << QString::number(i);
  file.close();
}

void Dictionaries::setAssameseEnglishDictionaries(QString dictionaryFile) {
  QFile file(dictionaryFile);
  if (!file.open(QIODevice::ReadOnly)) {
    QMessageBox::information(0, "error", file.errorString());
  }

  QTextStream in(&file);
  unsigned int i = 0;
  QString line;
  while (!in.atEnd()) {
    line = in.readLine();
    //  qDebug() << line;
    assamese->set_New_Entry(line);
    english->set_New_Entry_Reversed(line);
    i++;
  }
  qDebug() << "Number of words inserted to assamese-english dictionay "
           << QString::number(i);
  file.close();
}

void Dictionaries::setPoriBhashaDictionaries(QString poribhashaFile) {
  QFile file(poribhashaFile);
  if (!file.open(QIODevice::ReadOnly)) {
    QMessageBox::information(0, "error", file.errorString());
  }

  QTextStream in(&file);
  QString line;
  while (!in.atEnd()) {
    line = in.readLine();
    englishPoribhasha->set_New_Entry(line);
  }
  file.close();
}

Dictionary *Dictionaries::getDictionary(unsigned int i) {
  switch (i) {
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

Dictionary::Dictionary() {}

Dictionary::Dictionary(QString dictionaryFile, QString poribhashaFile) {}

void Dictionary::set_New_Entry(const QString str) {
  QStringList fields = str.split("\t");
  if (fields.size() < 2)
    return;
  QString word = fields.at(0).trimmed();
  if (word.size() < 1)
    return;
  QString trimmedField;

  unsigned int index = meanings.size();

  for (int i = 1; i < fields.size(); i++) {
    trimmedField = fields.at(i).trimmed();
    if (trimmedField.size() < 1)
      continue;
    /*
            if( !tempHash.contains(trimmedField)) {
                index = meanings.size();
                meanings.push_back(trimmedField);
                tempHash.insert(trimmedField,index);
            } else {
                index = tempHash[trimmedField];
            }
    */
    if (!words.contains(word)) {
      words.insert(word, localTempHash);
    }

    if (!words[word].contains(trimmedField)) {
      words[word].insert(trimmedField, index);
    }
  }
}

void Dictionary::set_New_Entry_Reversed(const QString str) {
  QStringList fields = str.split("\t");
  if (fields.size() < 2)
    return;
  QString word = fields.at(0).trimmed();
  if (word.size() < 1)
    return;
  QString trimmedField;

  unsigned int index = meanings.size();

  for (int i = 1; i < fields.size(); i++) {
    trimmedField = fields.at(i).trimmed();
    if (trimmedField.size() < 1)
      continue;
    if (trimmedField.split(" ").size() > 1)
      continue;

    if (!words.contains(trimmedField)) {
      words.insert(trimmedField, localTempHash);
    }

    if (!words[trimmedField].contains(word)) {
      words[trimmedField].insert(word, index);
    }
  }
}

QStringList Dictionary::getWords() {
  QStringList results = words.keys();
  return results;
}

/*
bool  Dictionary::hasWord(QString word) {
    if( words.contains(word))
        return true;
    else
        return false;
}*/

bool Dictionary::hasWord(QString word) {
  if (asmWrdWrdId.contains(word) || engWrdWrdId.contains(word.toLower()))
    return true;
  else
    return false;
}

/*
QStringList Dictionary::getMeanings(const QString str) {
    QStringList results;
    if( words.contains(str) ) {
        results = words[str].keys();
    }

    return results;
}
*/

QStringList Dictionary::getMeanings(const QString str) {
  QStringList results;

  if (str.trimmed().size() == 0)
    return results;

  // is this work in the Word table
  QString wrdId, IdeaId = "";
  if (asmWrdWrdId.contains(str)) {
    // if it is, then get the wordid
    wrdId = asmWrdWrdId[str];
    IdeaId = asmWrdIdIdeaId[wrdId];
  }

  // now for the english
  // is this work in the Word table
  if (engWrdWrdId.contains(str.toLower())) {
    wrdId = engWrdWrdId[str];
    IdeaId = engWrdIdIdeaId[wrdId];
  }

  if (IdeaId.size() == 0)
    return results;

  // is this ideaid in the idea table
  if (asmIdeaIdIdea.contains(IdeaId)) {
    // if so get the string
    QString asmMeaning = asmIdeaIdIdea[IdeaId];
    results.append(Utilities::getHTMLStringFromMixedHexString(asmMeaning));
  }
  // is this ideaid in the idea table
  if (engIdeaIdIdea.contains(IdeaId)) {
    // if so get the string
    QString engMeaning = engIdeaIdIdea[IdeaId];
    results.append(engMeaning);
  }
  return results;
}

QStringList Dictionary::getIdioms(const QString str) {
  QStringList results;
  // qDebug() << "idioms in " << str;
  if (str.trimmed().size() == 0)
    return results;

  foreach (QString idiom, idioms.keys()) {
    if (idiom.contains(str)) {
      results.append(Utilities::getHTMLStringFromMixedHexString(idiom));
      foreach (const QString a, idioms[idiom])
        results.append(Utilities::getHTMLStringFromMixedHexString(a));
    }
  }
  return results;
}

QStringList Dictionary::getExamples(const QString str) {
  QStringList results;
  if (str.trimmed().size() == 0)
    return results;

  // is this work in the Word table
  QString wrdId, IdeaId = "";
  // qDebug() << "example in " << str;
  if (asmWrdWrdId.contains(str)) {
    wrdId = asmWrdWrdId[str];
    if (examples.contains(wrdId)) {
      foreach (const QString a, examples[wrdId])
        results.append(Utilities::getHTMLStringFromMixedHexString(a));
    }
  }
  return results;
}

QStringList Dictionary::getOfficialUse(const QString str) {
  QStringList results;
  QPair<QString, QString> pair;

  if (str.size() == 0)
    return results;

  foreach (pair, officialWrd) {
    if (pair.first.contains(str, Qt::CaseInsensitive) ||
        pair.second.contains(str, Qt::CaseInsensitive)) {
      results.append(pair.first + " " +
                     Utilities::getHTMLStringFromMixedHexString(pair.second));
      // results.append(pair.second);
    }
  }
  return results;
}

QStringList Dictionary::getSynonyms(const QString str) {
  QStringList results;
  QStringList resultseng, resultsasm;
  if (str.trimmed().size() == 0)
    return results;

  QString wrdId, ideaId;

  if (asmWrdWrdId.contains(str)) {
    wrdId = asmWrdWrdId[str];
    if (asmWrdIdIdeaId.contains(wrdId)) {
      ideaId = asmWrdIdIdeaId[wrdId];
    }
  }
  if (engWrdWrdId.contains(str)) {
    wrdId = engWrdWrdId[str];
    if (engWrdIdIdeaId.contains(wrdId)) {
      ideaId = engWrdIdIdeaId[wrdId];
    }
  }

  QHash<QString, bool> added_already;
  foreach (QString asmID, asmWrdIdIdeaId) {
    if (ideaId == asmWrdIdIdeaId[asmID]) {
      if (asmID != wrdId && asmWrdIdWrd.contains(asmID))
        if (!added_already.contains(asmWrdIdWrd[asmID])) {
          resultsasm.append(
              Utilities::getHTMLStringFromMixedHexString(asmWrdIdWrd[asmID]));
          added_already[asmWrdIdWrd[asmID]] = true;
        }
    }
  }

  foreach (QString engID, engWrdIdIdeaId) {
    if (ideaId == engWrdIdIdeaId[engID]) {
      if (engID != wrdId && engWrdIdWrd.contains(engID))
        if (!added_already.contains(engWrdIdWrd[engID])) {
          resultseng.append(engWrdIdWrd[engID]);
          added_already[engWrdIdWrd[engID]] = true;
        }
    }
  }

  if (resultsasm.size() != 0)
    results.append(resultsasm.join(","));
  if (resultseng.size() != 0)
    results.append(resultseng.join(","));

  return results;
}
