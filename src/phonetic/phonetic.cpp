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

#include "src/phonetic/phonetic.h"
#include "src/profile/profile.h"
#include "float.h"
#include "src/utils/utilities.h"

QStringList *Phonetic::inflexCombinations = NULL;
QHash<QString, QString> *Phonetic::charPhoneticMap = NULL;
QHash<QString, QList<QKeyValue> *> *Phonetic::inflexTypes = NULL;
QHash<QString, QString> *Phonetic::allInflexions = NULL;
QHash<QString, QString> *Phonetic::singleInflexions = NULL;
QHash<QString, QString> *Phonetic::singleInflexionsReverse = NULL;

QHash<QString, QString> *Phonetic::allWords = NULL;

TreeNode *Phonetic::roman2UnicodeTreeDefault = NULL;
TreeNode *Phonetic::roman2UnicodeTreeProfile = NULL;
QList<TreeNode *> Phonetic::treeNodeList;
QList<TreeNode *> Phonetic::treeNodeListProfile;

QHash<QString, QString> *Phonetic::phoneticMap = NULL;
QHash<QString, QString> *Phonetic::deleteCharMap = NULL;

QHash<QString, QString> *Phonetic::userwords_phonetic_unicodexstr = NULL;
QHash<QString, QString> *Phonetic::userwords_unicodexstr_phonetic = NULL;

QHash<QChar, QHash<QChar, float> *> *Phonetic::distances = NULL;

Phonetic::Phonetic() {

}

void Phonetic::initUserWordHashes() {
   if (userwords_phonetic_unicodexstr == NULL) {
       userwords_phonetic_unicodexstr = new QHash<QString, QString>;
       userwords_unicodexstr_phonetic = new QHash<QString, QString>;
   }
}

bool Phonetic::insertWordFromOutside(QString unicodeWord) {
  QStringList charList;

  QString roman = Romanization::convert2Roman(
    Utilities::getUnicodeString(unicodeWord));

  foreach (QChar c, roman) {
    charList.append(Phonetic::phoneticEquivString(QString(c).toLower()));
    //  qDebug() << Phonetic::phoneticEquivString(QString(c).toLower());
  }
  insertWord(roman2UnicodeTreeDefault, charList, Utilities::getUnicodeString(unicodeWord));
  return true;
}

void Phonetic::addUserWord(const QString &roman, const QString &unicode) {
    if (userwords_phonetic_unicodexstr == NULL) {
       userwords_phonetic_unicodexstr = new QHash<QString, QString>;
       userwords_unicodexstr_phonetic = new QHash<QString, QString>;
    }
    userwords_phonetic_unicodexstr->insert(roman, unicode);
    userwords_unicodexstr_phonetic->insert(unicode, roman);
}

void Phonetic::insertWord(TreeNode *curNode,
                          QStringList &charList,
                          QString unicodeWord) {
    
  QString newChar = charList.at(0);
  //  qDebug() << unicodeWord;

  if (curNode->links[newChar] == NULL)
    curNode->links.insert(newChar, new TreeNode(NULL, false));

  if (!charList.isEmpty()) charList.removeFirst();

  if (charList.size() > 0) {
     insertWord(curNode->links[newChar], charList, unicodeWord);
  } else {
     // perhaps the same phonetic sound has multiple words
      if (!curNode->links[newChar]->roman2UnicodeList.contains(unicodeWord)) {
         curNode->links[newChar]->roman2UnicodeList.append(unicodeWord);
         curNode->links[newChar]->fullWord = true;
      }
  }

}

/**
bool Phonetic::insertWord(TreeNode *curNode, QStringList &charList,
                          QString unicodeWord) {
  QString newChar = charList.at(0);
  //  qDebug() << unicodeWord;
  if (curNode->links[newChar] == NULL)
    curNode->links.insert(newChar, new TreeNode(NULL, false));
  if (!charList.isEmpty()) charList.removeFirst();

  if (charList.size() > 0) {
     insertWord(curNode->links[newChar], charList, unicodeWord);
  } else {
     curNode->links[newChar]->roman2UnicodeList.append(unicodeWord);
    //   qDebug() << "full word";
     curNode->links[newChar]->fullWord = true;
  }
  return true;
}
*/

void Phonetic::createPhoneticTreeProfile(QStringList &wordList) {

  QHash<QString, QStringList> roman2UnicodeMap;
  QStringList charList;

  QString roman;

  foreach (QString word, wordList) {
    roman = Romanization::convert2Roman(word);
    if (!roman2UnicodeMap.contains(roman)) {
      QStringList list;
      list.append(word);
      roman2UnicodeMap[roman] = list;
    } else {
      roman2UnicodeMap[roman].append(word);
    }
    // qDebug() << roman;
  }

  QHash<QString, QStringList>::const_iterator it;
  // TreeNode::delete_Tree(roman2UnicodeTreeProfile);
  roman2UnicodeTreeProfile = new TreeNode();

  for (it = roman2UnicodeMap.begin(); it != roman2UnicodeMap.end(); ++it) {
    foreach (QString unicodeWord, it.value()) {

      charList.clear();
      foreach (QChar c, it.key())
        charList.append(Phonetic::phoneticEquivString(QString(c).toLower()));

      // qDebug() << charList << "  " << unicodeWord;
      insertWord(roman2UnicodeTreeProfile, charList, unicodeWord);
    }
  }
  QStack<QString> partword;
  if (wordList.size() == 0)
    return;
  printTree(roman2UnicodeTreeProfile, partword);
  //  qDebug() << "unicode maps" << roman2UnicodeMap.size();
}

void Phonetic::addUserWordsToPhoneticTree(const QString fileName) {
  qDebug() << "Adding from the external users profile words" << fileName;
  QStringList charList;

  QFile file(fileName);
  if (!file.open(QIODevice::ReadOnly)) {
    QMessageBox::information(0, "error", file.errorString());
  }
  QTextStream in(&file);

  QHash<QString, bool> already_inserted;
  QStringList fields;
  Romanization rom;

  while (!in.atEnd()) {
    fields = in.readLine().trimmed().split('\t');
    if (fields.size() >= 2 && !already_inserted.contains(fields.at(1))) {
      // add a word on <word>
      if (fields.at(0).trimmed() == QString("<word>")) {
        charList.clear();
        foreach (QChar c, rom.convert2Roman(fields.at(1))) {
           charList.append(Phonetic::phoneticEquivString(QString(c).toLower()));
        }
        insertWord(roman2UnicodeTreeDefault, charList, fields.at(1));
        //already_inserted.insert(fields.at(1), true);
      }
    }
  }
}

void Phonetic::createPhoneticTree(QString fileName) {
  QHash<QString, QStringList> roman2UnicodeMap;
  QStringList charList;

  Romanization *rom = new Romanization();
  rom->Romanize(fileName, roman2UnicodeMap);

  QHash<QString, QStringList>::const_iterator it;
  roman2UnicodeTreeDefault = new TreeNode();
  QHash<QString, bool> already_inserted;

  for (it = roman2UnicodeMap.begin(); it != roman2UnicodeMap.end(); ++it) {
    //   if( i++ < 2 )
    // qDebug() << "phoneticload" << it.key() << "  "<< it.value()  <<" " <<
    // it.value().size();
    //  qDebug() << charList << "  " << unicodeWord;
    if (already_inserted.contains(it.value()[0]))
      continue;
    already_inserted.insert(it.value()[0], true);

    if (it.value()[0].contains("0x2d0"))
      continue;

    foreach (QString unicodeWord, it.value()) {
      charList.clear();
      foreach (QChar c, it.key())
        charList.append(Phonetic::phoneticEquivString(QString(c).toLower()));
      // qDebug() <<  it.key() << " " << charList << "  " << unicodeWord;
      insertWord(roman2UnicodeTreeDefault, charList, unicodeWord);
    }
  }

  // QStack<QString> partword;
  //   printTree(roman2UnicodeTreeDefault, partword);
  //  qDebug() << "unicode maps" << roman2UnicodeMap.size();
}

bool Phonetic::toAsending(QWordRank &s1, QWordRank &s2) {
  if (s1.rank < s2.rank)
    return true;
  else if (s1.rank == s2.rank) {
    return (s1.len < s2.len);
  } else {
    return false;
  }
}

bool Phonetic::toAsendingInflex(QKeyDistance &s1, QKeyDistance &s2) {
  if (s1.dist < s2.dist)
    return true;
  else {
    return false;
  }
}

void Phonetic::rankWords(QList<QWordUnicode> &words, QString word, int max) {

  QList<QWordRank> wordRanks;

  QWordRank wr;
  unsigned int n = word.length();
  foreach (QWordUnicode wd, words) {
    wr.word = wd.word;
    wr.unicode = wd.unicode;
    //   qDebug() << " a= " << wd.left(n) << " b = " << word;
    wr.len = wd.word.size();
    wr.rank = Phonetic::wordDistance(wd.word.left(n), word);
    wordRanks.append(wr);
  }

  qSort(wordRanks.begin(), wordRanks.end(), &Phonetic::toAsending);
  words.clear();
  QWordUnicode wu;

  int i = 0;
  foreach (QWordRank w, wordRanks) {
    //   qDebug() << " a= " << w.word << " unicode " << w.unicode << " b = " <<
    //   word << "match =" <<  w.word.left(n) << " rank = " << w.rank;
    if ((double)w.rank / (double)word.size() < 0.5) {
      //     qDebug() << "ratio = " << (double) w.rank/(double)w.word.size() <<
      //     "  a= " << w.word << " word = " << w.word << "match =" <<
      //     w.word.left(n) << " rank = " << w.rank;
      wu.word = w.word;
      wu.unicode = w.unicode;
      words.append(wu);
      if (i++ > max)
        break;
    }
  }
}

void Phonetic::childrenOfTree(TreeNode *root, QStack<QString> &partword,
                              QList<QWordUnicode> &words, QString preWord,
                              int depth) {
  if (root == NULL) {
    //   qDebug() << "Empty";
    return;
  }
  //    qDebug() << "Pre word" << preWord;
  QHash<QString, TreeNode *>::const_iterator it = root->links.constBegin();
  QStack<QString>::const_iterator pit;

  QString printRoman;
  QWordUnicode wordWithUnicode;

  while (it != root->links.constEnd()) {
    QString key = it.key();
    partword.push(key);
    //           if( ! (preWord.size() < 2 && depth < 2 )  )
    if (preWord.size() >= 1 && depth < preWord.size() + 5) {
      //  qDebug() << "preword 1" << preWord << "num words" << words.size();

      childrenOfTree(root->links[key], partword, words, preWord, depth + 1);
      //  qDebug() << "preword 2" << preWord << "num words" << words.size();
    }
    if (root->links[key]->fullWord == true) {
      pit = partword.constBegin();
      printRoman.clear();
      while (pit != partword.constEnd()) {
        printRoman.append(*pit);
        ++pit;
      }
      foreach (QString wUnicode, root->links[key]->roman2UnicodeList) {
        wordWithUnicode.word = preWord + printRoman;
        wordWithUnicode.unicode = wUnicode;
        words.append(wordWithUnicode);
      }
    }

    partword.pop();
    ++it;
  }
}
/**
 * @brief Phonetic::searchRoman2UnicodeTree: this searches the roman to unicode tre
 *   by using a recursive metod in the TreeNode structure
 * @param curNode the current node being searched
 * @param seen
 * @param notSeen
 * @param word
 * @param words
 * @param depth
 * @param traversal_depth
*/



void Phonetic::searchRoman2UnicodeTree(TreeNode *curNode, QStack<QString> &seen,
                                       QStack<QString> &notSeen,
                                       QStack<QString> &wordStack,
                                       QList<QWordUnicode> &words, int depth,
                                       int traversal_depth) {
  QStack<QString> partword;
  QHash<QString, TreeNode *>::const_iterator it;
  int count;

  //   qDebug() << "Seen " << seen.size() << " Not seen " <<  notSeen.size() <<
  //   "trav depth " << traversal_depth << " depth " << depth;

  // when the search is at the first character of the phonetic string then
  // the algorithm picks the right branch of the tree
  if (depth == traversal_depth) {
    QStack<QString>::const_iterator it;
    QString preWord;
    for (it = wordStack.begin(); it != wordStack.end(); ++it) {
      preWord.append(it);
    }
    childrenOfTree(curNode, partword, words, preWord, depth);
    // qDebug() << "Phonetic::searchroman2UnicodeTree preword" << preWord <<
    // "words" << words.size();
  }

  if (notSeen.isEmpty())
    return;

  QString c = notSeen.pop();
  seen.push(c);

  count = 0;
  for (it = curNode->links.begin(); it != curNode->links.end(); ++it) {
    //  if( it.key() == "h" ) {
    //      qDebug() <<  "Char = " << c << "  H = " <<   curNode->links.value(
    //      it.key())->used;
    //  }

    // travel down the path that has the right phonetic character match
    if (curNode->links.value(it.key())->used == true) {
      wordStack.push(it.key());
      searchRoman2UnicodeTree(curNode->links.value(it.key()), seen, notSeen,
                              wordStack, words, depth, traversal_depth + 1);
      wordStack.pop();
    }
  }

  c = seen.pop();
  notSeen.push(c);
  //   qDebug() << "words" << words.size();
  return;
}

/**
 * @brief Phonetic::clearForNewWord
 * @param newWord
 * @param visitedNodeList
 * @param roman2UnicodeTreeGeneric
 */
void Phonetic::clearForNewWord(QString newWord,
                               QList<TreeNode *> *visitedNodeList,
                               TreeNode *roman2UnicodeTreeGeneric) {
  // clean the previous nodes, this is a new word starting with a
  // single character, i.e., newWord.size() == 1
  Q_ASSERT(newWord.size() == 1);
  foreach (TreeNode *node, *visitedNodeList) { node->used = false; }
  visitedNodeList->clear();

  roman2UnicodeTreeGeneric->used = true;

  QHash<QString, TreeNode *>::const_iterator it;
  for (it = roman2UnicodeTreeGeneric->links.begin();
       it != roman2UnicodeTreeGeneric->links.end(); ++it) {
    QString first = Phonetic::phoneticEquivString(QString(newWord.at(0)));
    QString second = Phonetic::phoneticEquivString(QString(it.key().at(0)));

    // need to remember the visited notes as we need them
    if (first == second) {
      roman2UnicodeTreeGeneric->links.value(it.key())->used = true;
      visitedNodeList->append(roman2UnicodeTreeGeneric->links.value(it.key()));
    }
  }
}

void Phonetic::markUsedWord(TreeNode *curNode, QStringList &charList, bool used,
                            unsigned int depth) {
  QString newChar = charList.at(0);

  charList.removeFirst();

  if (charList.size() > 0 && depth > 0) {
    curNode->links[newChar]->used = used;
    markUsedWord(curNode->links[newChar], charList, used, depth - 1);
  } else {
    curNode->links[newChar]->used = used;
  }

}

void Phonetic::arrangeWordChoices(QList<QWordUnicode> &words,
                                  QStringList &wordList, QString newWord) {

  rankWords(words, newWord);
  int maxLenWord = 0;
  foreach (QWordUnicode word, words) {
    if (Utilities::getUnicode(word.unicode, "0x").length() > maxLenWord)
      maxLenWord = Utilities::getUnicode(word.unicode, "0x").length();
  }
  //    qDebug() << "maxlegth " << maxLenWord;
#ifdef ASSAMESE_ENGLISH_WORDS
  foreach (QWordUnicode word, words) {
    wordList.append(Utilities::padWithSpaces(
                        Utilities::getUnicode(word.unicode, "0x"), maxLenWord) +
                    "\t" + word.word);
  }
#else
  foreach (QWordUnicode word, words) {
    wordList.append(Utilities::padWithSpaces(
        Utilities::getUnicode(word.unicode, "0x"), maxLenWord));
  }
#endif
  //  qDebug() << "Word list " << words.size();
  if (newWord.size() < 2) {
    wordList.clear();
    return;
  }
}

void Phonetic::phoneticWordChoicesLengthBased(QString rawnewWord,
                                              QList<QWordUnicode> &words,
                                              bool ProfileTree) {
  QString newWord = Phonetic::phoneticEquivString(rawnewWord);

  qDebug() << "Phonetic::phoneticWordChoicesLengthBased" << newWord;

  int i = 0;
  QHash<QString, QString>::const_iterator it;
  QWordUnicode wu;
  QList<QWordUnicode> _words;
  // it.value().size() - 2 <= newWord.size() && newWord.size() <=
  // it.value().size() - 2 &&
  for (it = allWords->begin(); it != allWords->end(); ++it) {
    if (newWord.size() > 0 && it.value().size() > 0 &&
        it.value().at(0) == newWord.at(0)) {
      wu.word = it.key();
      wu.unicode = it.value();
      _words.append(QWordUnicode(wu));
      i++;
    }
  }


  // qDebug() << "i" << i << words.size();

  rankWords(_words, newWord);


  foreach (QWordUnicode word, _words) {
    QStringList charList;
    foreach (QChar c, word.word)
      charList.append(Phonetic::phoneticEquivString(QString(c).toLower()));

    if (ProfileTree == false)
      markUsedWord(roman2UnicodeTreeDefault, charList, true, newWord.size());
    else
      markUsedWord(roman2UnicodeTreeProfile, charList, true, newWord.size());
  }

  foreach (QWordUnicode word, _words)
    words.append(word);
}

void Phonetic::phoneticWordChoices(QString rawnewWord,
                                   QList<QWordUnicode> &words,
                                   bool ProfileTree) {

  QStack<QString> seen;
  QStack<QString> notSeen;
  QStack<QString> word;
  TreeNode *roman2UnicodeTree;
  QList<TreeNode *> *nodeList;

  if (ProfileTree == true) {
      roman2UnicodeTree = roman2UnicodeTreeProfile;
      nodeList = &treeNodeListProfile;
  } else {
     roman2UnicodeTree = roman2UnicodeTreeDefault;
     nodeList = &treeNodeList;
  }
  // qDebug() << "phonetic.cpp 494 " << words.size();

  // get the phonetic string for searching in the tree
  QString newWord = Phonetic::phoneticEquivString(rawnewWord);
  Phonetic::processPhoneticInput(newWord);

  //qDebug() << "phonetic.cpp 499 " << newWord << " " << rawnewWord;

  // store the full roman word for sequentially searching in the
  // romanto unicode tree
  foreach (QChar s, newWord) {
    notSeen.prepend(s.toLower());
    //          notSeen.push(s.toLower());
  }

  //qDebug() << "phonetic.cpp 509 " << "profiletree " << ProfileTree
  //  << " treenodeList size" << nodeList->size();

  if (roman2UnicodeTree == NULL || roman2UnicodeTree->links.size() == 0)
      return;

  // while searching a word with phonetic spelling
  // mark the TreeNode "used" that is rooted at a character
  // this helps us avoid searching the tree to subtree that is not used
  // and leads to speed gain
  if (newWord.length() == 1) {
      clearForNewWord(newWord, nodeList, roman2UnicodeTree);
  }

  QList<QWordUnicode> localWords;
  if (notSeen.size() > 0) {
      searchRoman2UnicodeTree(roman2UnicodeTree, seen, notSeen, word,
                              localWords, newWord.size(),
                              /* the sub-tree rooted at the first character of the
                                 phonetic word is already marked */
                              1);
  // qDebug() << "Candidates list size before ranking" << words.size();
  }


 rankWords(localWords, newWord, 15);
 int i =0;

 foreach (QWordUnicode word, localWords) {
   i = i + 1;
    QStringList charList;
    foreach (QChar c, word.word) {
      charList.append(Phonetic::phoneticEquivString(QString(c).toLower()));
    }
    markUsedWord(roman2UnicodeTree, charList, true, newWord.size());
  }
  foreach (QWordUnicode word, localWords) {
    words.append(word);
   }
  //   qDebug() << " choices " << words.size();
}

/**
 * @brief Phonetic::getUserWordsFromPrefix
 * @param newWordPrefix prefix of the word in the format "0x995..."
 * @param choices reference to a list choices to add to
 */

/**
 * @brief Phonetic::getUserWordsFromPrefix Returns a set of words in the
 *      user defined dictionary that begins with the prefix.
 * @param newWordPrefix prefix of the word in the format "0x995..."
 * @return list of workds in the format "0x995...."
 */

QStringList Phonetic::getUserWordsFromPrefix(const QString &newWordPrefix) {
    if (userwords_phonetic_unicodexstr == NULL) {
       userwords_phonetic_unicodexstr = new QHash<QString, QString>;
       userwords_unicodexstr_phonetic = new QHash<QString, QString>;
    }
    QStringList wordList;
    for (QHash<QString, QString>::iterator it = Phonetic::userwords_unicodexstr_phonetic->begin(); \
            it != Phonetic::userwords_unicodexstr_phonetic->end(); it++) {
        if(it.key().left(newWordPrefix.size()) == newWordPrefix) {
            wordList.append(it.key());
        }
    }

    return wordList;
}

/**
 * @brief Phonetic::profileWordChoices
 * @param newWord
 * @param wordList
 */
void Phonetic::profileWordChoices(QString newWord, QList<QWordUnicode> &wordList) {
    if (userwords_phonetic_unicodexstr == NULL) {
       userwords_phonetic_unicodexstr = new QHash<QString, QString>;
       userwords_unicodexstr_phonetic = new QHash<QString, QString>;
    }
    for (QHash<QString, QString>::iterator it = Phonetic::userwords_phonetic_unicodexstr->begin(); \
            it != Phonetic::userwords_phonetic_unicodexstr->end(); it++) {
        if(it.key().left(newWord.size()) == newWord) {
            wordList.append( QWordUnicode( it.key(), it.value()) );
        }
    }
}


void Phonetic::phoneticInflexChoices(QString newWord, QStringList &wordList) {
  QHash<QString, QString>::const_iterator it;

  QList<QKeyDistance> inflexList;
  QKeyDistance k;
  for (it = singleInflexionsReverse->begin();
       it != singleInflexionsReverse->end(); ++it) {
    k.word = it.value();
    k.dist = Phonetic::wordDistance(newWord, k.word);
    if (k.dist < 5) {
      //   qDebug() << "<<" << k.word<< ">> " << "<<" << newWord <<">>" << " "
      //   <<  k.dist;
      k.word = it.key();
      inflexList.append(k);
    }
  }

  qSort(inflexList.begin(), inflexList.end(), &Phonetic::toAsendingInflex);
  foreach (QKeyDistance k, inflexList) {
    wordList.append(
        k.word + "\t" +
        singleInflexionsReverse->value(k.word)); // + QString::number(k.dist));
  }

  //   qDebug() << "Number of inflex choices " << inflexList.size();
}

void Phonetic::printTree(TreeNode *root, QStack<QString> &partword) {
  static int count = 0;
  if (root == NULL) {
    //   qDebug() << "Empty";
    return;
  }
  //#define  PRINT_ROMAN_WORDS

#ifdef PRINT_ROMAN_WORDS
  QFile outfile("files/romanized.txt");
  if (!outfile.open(QIODevice::WriteOnly)) {
    QMessageBox::information(0, "error", outfile.errorString());
  }
  QTextStream outStream(&outfile);
#endif

  QHash<QString, TreeNode *>::const_iterator it = root->links.constBegin();
  QStack<QString>::const_iterator pit;

  QString printRoman;

  while (it != root->links.constEnd()) {
    QString key = it.key();
    partword.push(key);

    printTree(root->links[key], partword);
    if (root->links[key]->fullWord == true) {
      pit = partword.constBegin();
      printRoman.clear();
      while (pit != partword.constEnd()) {
        printRoman.append(*pit);
        ++pit;
      }
      foreach (QString uni, root->links[key]->roman2UnicodeList) {
        printRoman.append("\t");
        printRoman.append(uni);
        count++;
      }
#ifdef PRINT_ROMAN_WORDS
      outStream << printRoman << "\n";
#endif
      /*  if( count % 1000 == 0 ) {
           qDebug() << count;
         }
         */
      // qDebug() << root->links[key]->unicode << "//";
      //  qDebug() << root->links[key]->unicode << "//";
    }

    partword.pop();
    ++it;
  }
#ifdef PRINT_ROMAN_WORDS
  outfile.close();
#endif
}

QStringList Phonetic::getInflectionalForms(QString newWord) {
  QStringList s;
  QHash<QString, QString>::const_iterator sit;
  QRegExp rx("");
  if (newWord.size() == 0) {
    return s;
  }
  rx.setPatternSyntax(QRegExp::Wildcard);
  //   rx.exactMatch("README.txt");        // returns true
  //  rx.exactMatch("welcome.txt.bak");   //
  // qDebug() << newWord;
  QString dynStr = Utilities::getUnicodeString(newWord);
  bool changed = true;
  s.append(newWord);

  QString inflex, inflex1;

  //   qDebug() << "Code " << dynStr;

  while (changed) {
    changed = false;
    for (sit = singleInflexions->begin(); sit != singleInflexions->end();
         ++sit) {
      // qDebug() << "Phonetic " << sit.key() <<  sit.value();

      // qDebug() << " Patthen " << Utilities::getUnicodeString(sit.value());
      inflex = QString("*") + Utilities::getUnicodeString(sit.value());
      inflex1 = Utilities::getUnicodeString(sit.value());
      rx.setPattern(inflex);
      if (rx.exactMatch(dynStr)) {
        dynStr.remove(QRegExp(inflex1));
        changed = true;
        // qDebug() << "After Match" << dynStr;
        if (dynStr.trimmed().length() > 0) {
          s.append(Utilities::getUnicode(dynStr, "0x"));
        }
      }
    }
  }
  return s;
}

QStringList Phonetic::getInflectionalFormsX(QString newWord) {
  QStringList s;
  QHash<QString, QString>::const_iterator sit;
  QRegExp rx("");
  if (newWord.size() == 0) {
    return s;
  }

  QString dynStr = newWord;
  bool changed = true;
  s.append(newWord);

  QString inflex = " ";

  //   qDebug() << "Code " << dynStr;
  unsigned int count = 0;
  InflexTrie *inflexTree = InflexTrie::getInflexTrie();
  while (inflex.length() > 0 && count < 6) {
    changed = false;
    inflex = inflexTree->hasInflection(dynStr);
    rx.setPattern(inflex);
    //    qDebug() << "iflex " << Utilities::getUnicodeString(inflex) << "   "
    //    << Utilities::getUnicodeString(dynStr);
    dynStr.remove(QRegExp(inflex + "$"));
    if (inflex.length() > 0 && dynStr.trimmed().length() > 0) {
      s.append(dynStr);
    }
    count++;
  }
  //   qDebug() << " size " << s.size();
  return s;
}

// this function also returns the root as well
QString Phonetic::getRootWord(QString newWord) {
  QRegExp rx("");
  if (newWord.size() == 0) {
    return newWord;
  }

  QString dynStr = newWord;

  QString inflex;

  unsigned int count = 0;
  InflexTrie *inflexTree = InflexTrie::getInflexTrie();
  while (count == 0 || count < 6) {
    inflex = inflexTree->hasInflection(dynStr);
    rx.setPattern(inflex);
    dynStr.remove(QRegExp(inflex + "$"));
    if (inflex.length() == 0 && dynStr.trimmed().length() > 0) {
      //       qDebug() << " matched " << count;
      return dynStr;
    }
    count++;
  }
  // qDebug() << " size " << s.size();
  return dynStr;
}

QHash<QString, QString> Phonetic::getInflectionalFormsPairs(QString newWord) {
  QHash<QString, QString> s;
  QHash<QString, QString>::const_iterator sit;
  QRegExp rx("");
  if (newWord.size() == 0) {
    return s;
  }

  QString dynStr = newWord;
  bool changed = true;
  s.insert(dynStr, "");

  QString inflex = " ";

  unsigned int count = 0;
  InflexTrie *inflexTree = InflexTrie::getInflexTrie();
  while (inflex.length() > 0 && count < 6) {
    changed = false;
    inflex = inflexTree->hasInflection(dynStr);
    rx.setPattern(inflex);
    //    qDebug() << "iflex " << Utilities::getUnicodeString(inflex) << "   "
    //    << Utilities::getUnicodeString(dynStr);
    dynStr.remove(QRegExp(inflex + "$"));
    if (inflex.length() > 0 && dynStr.trimmed().length() > 0) {
      //    qDebug() << " matched " << count;
      s.insert(dynStr, inflex);
    }
    count++;
  }
  // qDebug() << " size " << s.size();
  return s;
}

void Phonetic::setInflexTypes(QString fileName) {

  if (inflexTypes != NULL)
    return;

  inflexTypes = new QHash<QString, QList<QKeyValue> *>();

  singleInflexionsReverse = new QHash<QString, QString>();

  QFile file(fileName);
  if (!file.open(QIODevice::ReadOnly)) {
    QMessageBox::information(0, "error", file.errorString());
  }

  QTextStream in(&file);
  QStringList list;
  QString category = "";

  QString line;
  while (!in.atEnd()) {
    line = in.readLine();

    if (line.contains(QRegExp("#")) || line.trimmed().length() < 1) {
      continue;
    }

    if (line.contains(QRegExp("//"))) {
      category = line.replace(QRegExp("//"), "").trimmed();
      continue;
    }

    list = line.split("\t");

    if (!inflexTypes->value(category))
      inflexTypes->insert(category, new QList<QKeyValue>());

    QKeyValue k;

    k.key = list.at(0).trimmed();
    k.value = list.at(1).trimmed();

    singleInflexionsReverse->insert(k.value, k.key);
    // qDebug() << k.key;
    inflexTypes->value(category)->append(k);

    // inflexTypes->value(category);
    //     charMap[list.at(1).trimmed()] = list.at(0).trimmed();
    // Kishori    qDebug() << line ;
  }

  file.close();
}

void Phonetic::createSingleInflections() {
  QList<QKeyValue>::const_iterator sit;
  singleInflexions = new QHash<QString, QString>();

  for (sit = inflexTypes->value("plurals")->begin();
       sit != inflexTypes->value("plurals")->end(); ++sit) {
    singleInflexions->insert(sit->key, sit->value);
  }
  for (sit = inflexTypes->value("case")->begin();
       sit != inflexTypes->value("case")->end(); ++sit) {
    singleInflexions->insert(sit->key, sit->value);
  }
  for (sit = inflexTypes->value("pleo")->begin();
       sit != inflexTypes->value("pleo")->end(); ++sit) {
    singleInflexions->insert(sit->key, sit->value);
  }
  for (sit = inflexTypes->value("definitive")->begin();
       sit != inflexTypes->value("definitive")->end(); ++sit) {
    singleInflexions->insert(sit->key, sit->value);
  }
}

void Phonetic::createInflexCombinations() {

  QList<QKeyValue>::const_iterator fit, sit;
  allInflexions = new QHash<QString, QString>();

  QKeyValue k;

  for (fit = inflexTypes->value("plurals")->begin();
       fit != inflexTypes->value("plurals")->end(); ++fit) {
    for (sit = inflexTypes->value("case")->begin();
         sit != inflexTypes->value("case")->end(); ++sit) {
      k.key = fit->key + sit->key;
      k.value = fit->value + sit->value;
      allInflexions->insert(k.key, k.value);
    }
  }

  for (fit = inflexTypes->value("plurals")->begin();
       fit != inflexTypes->value("plurals")->end(); ++fit) {
    for (sit = inflexTypes->value("pleo")->begin();
         sit != inflexTypes->value("pleo")->end(); ++sit) {
      k.key = fit->key + sit->key;
      k.value = fit->value + sit->value;
      allInflexions->insert(k.key, k.value);
    }
  }

  for (fit = inflexTypes->value("definitive")->begin();
       fit != inflexTypes->value("definitive")->end(); ++fit) {
    for (sit = inflexTypes->value("case")->begin();
         sit != inflexTypes->value("case")->end(); ++sit) {
      k.key = fit->key + sit->key;
      k.value = fit->value + sit->value;
      allInflexions->insert(k.key, k.value);
    }
  }

  for (fit = inflexTypes->value("definitive")->begin();
       fit != inflexTypes->value("definitive")->end(); ++fit) {
    for (sit = inflexTypes->value("pleo")->begin();
         sit != inflexTypes->value("pleo")->end(); ++sit) {
      k.key = fit->key + sit->key;
      k.value = fit->value + sit->value;
      allInflexions->insert(k.key, k.value);
    }
  }

  for (fit = inflexTypes->value("case")->begin();
       fit != inflexTypes->value("case")->end(); ++fit) {
    for (sit = inflexTypes->value("pleo")->begin();
         sit != inflexTypes->value("pleo")->end(); ++sit) {
      k.key = fit->key + sit->key;
      k.value = fit->value + sit->value;
      allInflexions->insert(k.key, k.value);
    }
  }
  //   QHash<QString, QString>::const_iterator it;
  //   for(it= allInflexions->begin();  it!= allInflexions->end(); ++it)
  //     qDebug() << it.key();
}

void Phonetic::loadAllWords(QString fileName) {

  if (allWords != NULL)
    return;

  allWords = new QHash<QString, QString>();

  QFile file(fileName);
  if (!file.open(QIODevice::ReadOnly)) {
    QMessageBox::information(0, "error", file.errorString());
  }

  QTextStream in(&file);
  QStringList list;

  QString line;
  //    QString binary;
  while (!in.atEnd()) {
    line = in.readLine();
    // binary.append(line);
    list = line.split("\t");
    QString unicodeString =
        list.at(1)
            .trimmed(); // Utilities::getUnicodeString(list.at(1).trimmed());

    if (list.size() >= 2) {
      // qDebug() << unicodeString <<
      // Romanization::convert2Roman(unicodeString);
      allWords->insert(unicodeString,
                       Romanization::convert2Roman(unicodeString));
    }
  }

  QHash<QString, QString>::const_iterator it;
  /* for(it= allWords->begin();  it!= allWords->end(); ++it)
       qDebug() << it.key();

       */
  //  qDebug() << "Size : " << allWords->size();

  /*
        QFile bfile("composite_dictionary.dat");
        bfile.open(QIODevice::WriteOnly);
        QDataStream out(&bfile);
        out << binary;
        bfile.close();
  */

  file.close();
}

void Phonetic::initializeCharPhoneticMap() {

  if (charPhoneticMap != NULL)
    return;

  charPhoneticMap = new QHash<QString, QString>();

  charPhoneticMap->insert("x", "s");
  charPhoneticMap->insert("s", "s");
  charPhoneticMap->insert("c", "s");
  //    charPhoneticMap->insert( "h","s");

  //  charPhoneticMap->insert( "a","e");
  charPhoneticMap->insert("e", "e");
  charPhoneticMap->insert("i", "e");

  //  charPhoneticMap->insert( "p","p");
  //  charPhoneticMap->insert( "f","p");

  charPhoneticMap->insert("w", "b");
  charPhoneticMap->insert("b", "b");

  //    charPhoneticMap->insert( "v","b");
}

QString Phonetic::replaceChars(QString a) {

  QHash<QString, QString>::const_iterator it;

  QString temp = a;

  for (it = charPhoneticMap->begin(); it != charPhoneticMap->end(); ++it) {
    temp = temp.replace(QRegExp(it.key()), it.value());
  }

  return temp;
}

/**
 * @brief Phonetic::LD computes the distance between two strings  [NOT USED]
 * @param s first string
 * @param t second string
 * @return the Leveinstein distance between s and t
 */

int Phonetic::LD(QString s, QString t) {

  int n = s.length(); // length of s
  int m = t.length(); // length of t

  if (n == 0) {
    return m;
  } else if (m == 0) {
    return n;
  }

  float *p = new float[n + 1]; //'previous' cost array, horizontally
  float *d = new float[n + 1]; // cost array, horizontally
  float *_d;                   // placeholder to assist in swapping p and d

  // indexes into strings s and t
  int i; // iterates through s
  int j; // iterates through t

  QChar t_j; // jth character of t

  int cost; // cost

  for (i = 0; i <= n; i++) {
    p[i] = i;
  }

  for (j = 1; j <= m; j++) {
    t_j = t.at(j - 1);
    d[0] = j;

    for (i = 1; i <= n; i++) {
      cost = (s.at(i - 1) == t_j) ? 0 : 1;
      // minimum of cell to the left+1, to the top+1, diagonally left and up
      // +cost
      d[i] = Min(Min(d[i - 1] + 1, p[i] + 1), p[i - 1] + cost);
    }

    // copy current distance counts to 'previous row' distance counts
    _d = p;
    p = d;
    d = _d;
  }

  // our last action in the above loop was to switch d and p, so p now
  // actually has the most recent cost counts
  return p[n];
}
/**
 * @brief Phonetic::processPhoneticInput normalizes the roman strings to
 *   a standard form to make matching easier
 * @param orig string in roman to normalize
 * @return the normalized phonetic string
 */

QString Phonetic::processPhoneticInput(QString orig) {
  QRegExp rx("h");
  QHash<QString, QString>::const_iterator it = deleteCharMap->constBegin();
  while (it != deleteCharMap->constEnd()) {
    rx.setPattern(it.key());
    if (rx.indexIn(orig, 0) != -1) {
      orig.replace(it.key(), it.value());
    }
    ++it;
  }
  return (orig);
}

QString Phonetic::phoneticEquivString(QString orig) {
  QString modified = "";
  foreach (QChar c, orig) {
    if (charPhoneticMap->contains(QString(c)))
      modified.append(charPhoneticMap->value(QString(c)));
    else
      modified.append(QString(c));
  }
  // qDebug() << "modified " << modified;

  return modified;
}

/*
void Phonetic::initializePhoneticMap() {

  if( phoneticMap != NULL) return;

  phoneticMap = new QHash<QString,QString>();
  phoneticMap->insert( "w", "b");
  phoneticMap->insert( "v", "b");


  phoneticMap->insert( "e", "a");
  phoneticMap->insert( "i", "a");


  phoneticMap->insert( "p", "f");

  phoneticMap->insert( "j", "i");
  phoneticMap->insert( "y", "i");

  phoneticMap->insert( "k", "c");

  phoneticMap->insert( "x", "c");
  phoneticMap->insert( "s", "c");
//  phoneticMap->insert( "h", "c");


}
*/

/**
 * @brief Phonetic::initializeDeleteCharMap replace with equivalent
 *   phonetic string to normalize them
 */

void Phonetic::initializeDeleteCharMap() {
  if (deleteCharMap != NULL)
    return;

  deleteCharMap = new QHash<QString, QString>();
  deleteCharMap->insert("sh", "s");
  deleteCharMap->insert("ph", "f");
  deleteCharMap->insert("bh", "v");
}

void Phonetic::initializeDistances() {

  if (distances != NULL)
    return;

  distances = new QHash<QChar, QHash<QChar, float> *>();

  // hosro ee kar and dirgho ee kars
  distances->insert(QChar(0x9bf), NULL);
  distances->insert(QChar(0x9bf), new QHash<QChar, float>());
  distances->insert(QChar(0x9c0), NULL);
  distances->insert(QChar(0x9c0), new QHash<QChar, float>());

  // hosro ee and dirgho ee kars equivalence
  distances->value(QChar(0x9bf))->insert(QChar(0x9c0), 0.2);
  distances->value(QChar(0x9c0))->insert(QChar(0x9bf), 0.2);
}

float Phonetic::wordDistance(QString s, QString t) {

  int n = s.length(); // length of s
  int m = t.length(); // length of t

  if (n == 0) {
    return m;
  } else if (m == 0) {
    return n;
  }

  float *p = new float[n + 1]; //'previous' cost array, horizontally
  float *d = new float[n + 1]; // cost array, horizontally
  float *_d;                   // placeholder to assist in swapping p and d

  // indexes into strings s and t
  int i; // iterates through s
  int j; // iterates through t

  QChar t_j; // jth character of t

  int cost; // cost

  for (i = 0; i <= n; i++) {
    p[i] = i;
  }

  for (j = 1; j <= m; j++) {
    t_j = t.at(j - 1);
    d[0] = j;

    for (i = 1; i <= n; i++) {
      cost = (s.at(i - 1) == t_j) ? 0 : 1;

      if (distances->contains(s.at(i - 1)) &&
          distances->value(s.at(i - 1))->contains(t_j)) {
        cost = distances->value(s.at(i - 1))->value(t_j);
      } else if (s.at(i - 1) == t_j)
        cost = 0;
      else
        cost = 1;

      // minimum of cell to the left+1, to the top+1, diagonally left and up
      // +cost
      d[i] = Min(Min(d[i - 1] + 1, p[i] + 1), p[i - 1] + cost);
    }

    // copy current distance counts to 'previous row' distance counts
    _d = p;
    p = d;
    d = _d;
  }

  // our last action in the above loop was to switch d and p, so p now
  // actually has the most recent cost counts
  return p[n];
}
