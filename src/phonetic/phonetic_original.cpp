#include "float.h"
#include "phonetic.h"
#include "utilities.h"

QStringList *Phonetic::inflexCombinations = NULL;
QHash<QString, QString> *Phonetic::charPhoneticMap = NULL;
QHash<QString, QList<QKeyValue> *> *Phonetic::inflexTypes = NULL;
QHash<QString, QString> *Phonetic::allInflexions = NULL;
QHash<QString, QString> *Phonetic::singleInflexions = NULL;

QHash<QString, QString> *Phonetic::allWords = NULL;

TreeNode *Phonetic::roman2UnicodeTree = NULL;
QList<TreeNode *> Phonetic::treeNodeList;

QHash<QString, QString> *Phonetic::phoneticMap = NULL;
#define ASSAMESE_ENGLISH_WORD 1

/*

float min(float a, float b, float c) {
  float mi;

  mi = a;
  if (b < mi) {
    mi = b;
  }
  if (c < mi) {
    mi = c;
  }
  return mi;

}


float min(float a, float b) {

  if( a < b)
      return a;
  else
      return b;

}


*/

Phonetic::Phonetic() {}

bool Phonetic::insertWord(TreeNode *curNode, QStringList &charList,
                          QString unicodeWord) {
  QString newChar = charList.at(0);
  //  qDebug() << unicodeWord;
  if (curNode->links[newChar] == NULL)
    curNode->links.insert(newChar, new TreeNode(NULL, false));

  charList.removeFirst();

  if (charList.size() > 0) {
    insertWord(curNode->links[newChar], charList, unicodeWord);
  } else {
    curNode->links[newChar]->roman2UnicodeList.append(unicodeWord);
    curNode->links[newChar]->fullWord = true;
  }

  return true;
}
void Phonetic::createPhoneticTree(QString fileName) {
  QHash<QString, QStringList> roman2UnicodeMap;
  QStringList charList;

  Romanization *rom = new Romanization();
  rom->Romanize(fileName, roman2UnicodeMap);
  QHash<QString, QStringList>::const_iterator it;
  roman2UnicodeTree = new TreeNode();
  for (it = roman2UnicodeMap.begin(); it != roman2UnicodeMap.end(); ++it) {
    foreach (QString unicodeWord, it.value()) {

      charList.clear();
      foreach (QChar c, it.key())
        charList.append(Phonetic::phoneticEquivString(QString(c).toLower()));

      // qDebug() << charList << "  " << unicodeWord;
      insertWord(roman2UnicodeTree, charList, unicodeWord);
    }
  }

  QStack<QString> partword;
  //   printTree(roman2UnicodeTree, partword);
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

void Phonetic::rankWords(QList<QWordUnicode> &words, QString word) {

  QList<QWordRank> wordRanks;

  QWordRank wr;
  unsigned int n = word.length();
  foreach (QWordUnicode wd, words) {
    wr.word = wd.word;
    wr.unicode = wd.unicode;
    wr.len = wd.word.size();
    wr.rank = Phonetic::LD(wd.word.left(n), word);
    wordRanks.append(wr);
  }

  qSort(wordRanks.begin(), wordRanks.end(), &Phonetic::toAsending);
  qDebug() << "A = " << word;

  /*foreach(QWordRank w, wordRanks) {
      qDebug() << "a = " << w.word;
  }*/

  words.clear();
  QWordUnicode wu;
  foreach (QWordRank w, wordRanks) {
    //    qDebug() << " a= " << w.word << " unicode " << w.unicode << " b = " <<
    //    word << "match =" <<  w.word.left(n) << " rank = " << w.rank;
    if ((double)w.rank / (double)w.word.size() < 0.1) {
      wu.word = w.word;
      wu.unicode = w.unicode;
      words.append(wu);
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
    if (!(preWord.size() < 2 && depth < 2))
      childrenOfTree(root->links[key], partword, words, preWord, depth + 1);

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
void Phonetic::searchRoman2UnicodeTree(TreeNode *curNode, QStack<QString> &seen,
                                       QStack<QString> &notSeen,
                                       QStack<QString> &word,
                                       QList<QWordUnicode> &words, int depth,
                                       int traversal_depth) {
  QStack<QString> partword;
  QHash<QString, TreeNode *>::const_iterator it;
  int count;
  QString w;

  //   qDebug() << "Seen " << seen.size() << " Not seen " <<  notSeen.size() <<
  //   "trav depth " << traversal_depth << " depth " << depth;
  if (traversal_depth == depth) {
    QStack<QString>::const_iterator it;
    QString preWord;
    for (it = word.begin(); it != word.end(); ++it) {
      preWord.append(it);
    }

    childrenOfTree(curNode, partword, words, preWord, depth);
  }

  if (notSeen.isEmpty())
    return;
  QString c = notSeen.pop();
  seen.push(c);

  count = 0;
  for (it = curNode->links.begin(); it != curNode->links.end(); ++it) {
    if (curNode->links.value(it.key())->used == true) {
      qDebug() << " Going into key " << it.key();
      word.push(it.key());
      searchRoman2UnicodeTree(curNode->links.value(it.key()), seen, notSeen,
                              word, words, depth, traversal_depth + 1);
      word.pop();
    }
  }

  c = seen.pop();
  notSeen.push(c);

  return;
}
void Phonetic::clearForNewWord(QString newWord) {

  foreach (TreeNode *node, treeNodeList) { node->used = false; }

  treeNodeList.clear();
  //   qDebug() << " after the foreach and after to set it true " <<
  //   roman2UnicodeTree;
  roman2UnicodeTree->used = true;

  QHash<QString, TreeNode *>::const_iterator it;
  for (it = roman2UnicodeTree->links.begin();
       it != roman2UnicodeTree->links.end(); ++it) {
    QString first = Phonetic::phoneticEquivString(QString(newWord.at(0)));
    QString second = Phonetic::phoneticEquivString(QString(it.key().at(0)));
    // qDebug() << "setting keys " << it.key() << " " <<
    // roman2UnicodeTree->links.value(it.key())->used;
    if (first == second) {
      qDebug() << " first second " << first;
      roman2UnicodeTree->links.value(it.key())->used = true;
      treeNodeList.append(roman2UnicodeTree->links.value(it.key()));
    }
  }
}

bool Phonetic::markUsedWord(TreeNode *curNode, QStringList &charList, bool used,
                            unsigned int depth) {
  QString newChar = charList.at(0);

  charList.removeFirst();

  if (charList.size() > 0 && depth > 0) {
    markUsedWord(curNode->links[newChar], charList, used, depth - 1);
  } else {
    curNode->links[newChar]->used = used;
  }

  return used;
}

void Phonetic::phoneticWordChoices(QString newWord, QStringList &wordList,
                                   float factor) {

  int depth = newWord.length() * factor;
  //   qDebug() << " choice words";

  QStack<QString> seen;
  QStack<QString> notSeen;
  QStack<QString> word;
  foreach (QChar s, newWord) { notSeen.prepend(s); }

  if (newWord.length() == 1)
    clearForNewWord(newWord);

  // qDebug( ) << "PhoneticWordChoices" << newWord;
  QList<QWordUnicode> words;
  if (newWord == QString("pur")) {

    newWord = newWord;
  }
  if (notSeen.size() > 0)
    searchRoman2UnicodeTree(roman2UnicodeTree, seen, notSeen, word, words,
                            newWord.size(), 1);

  rankWords(words, newWord);

  /*
  int count=0;
  for(int i=0; i < 5000; i++) {
      QStringList charList;
      foreach( QChar c, words.at(i)) charList.append(
  Phonetic::phoneticEquivString(QString(c).toLower())); qDebug() << " word = "
  << charList; markUsedWord(roman2UnicodeTree, charList,true, newWord.size() );
  }
*/

  foreach (QWordUnicode word, words) {
    QStringList charList;
    foreach (QChar c, word.word)
      charList.append(Phonetic::phoneticEquivString(QString(c).toLower()));
    markUsedWord(roman2UnicodeTree, charList, true, newWord.size());
  }

  int maxLenWord = 0;
  foreach (QWordUnicode word, words) {
    if (Utilities::getUnicode(word.unicode, "0x").length() > maxLenWord)
      maxLenWord = Utilities::getUnicode(word.unicode, "0x").length();
  }

#ifdef ASSAMESE_ENGLISH_WORDS
  foreach (QWordUnicode word, words) {
    wordList.append(Utilities::padWithSpaces(
                        Utilities::getUnicode(word.unicode, "0x"), maxLenWord) +
                    "\t" + word.word);
  }
#else
  foreach (QWordUnicode word, words) {
    wordList.append(Utilities::padWithSpaces(
                        Utilities::getUnicode(word.unicode, "0x"), maxLenWord) +
                    "\t" + word.word);
    //    wordList.append(
    //    Utilities::padWithSpaces(Utilities::getUnicode(word.unicode,"0x"),
    //    maxLenWord) );
  }
#endif

  if (newWord.size() < 2) {
    wordList.clear();
    return;
  }
  /*
  QHash<QString, QString>::const_iterator it;
  wordList.clear();
  int maxLenWord = 0;
  int count =0;
  for(it = allWords->begin(); it != allWords->end() && count < 400 ; ++it) {
      if( Utilities::getUnicode(it.key(),"0x").length() > maxLenWord)
          maxLenWord = Utilities::getUnicode(it.key(),"0x").length();
      count++;
  }
  count=0;
  for(it = allWords->begin(); it != allWords->end() && count < 400; ++it) {
      QString first = Phonetic::phoneticEquivString(QString(newWord.at(0)));
      QString second = Phonetic::phoneticEquivString(QString(it.value().at(0)));

     if( first == second && wordList.size() < 10) {
        //  qDebug() <<  it.key() << "    "  << it.value() << " LD " <<
  Phonetic::LD(it.value(),newWord) << "<" <<first <<"," << second << ">";
         wordList.append(
  Utilities::padWithSpaces(Utilities::getUnicode(it.key(),"0x"), maxLenWord) +
  "\t" + it.value() );
     }
     count++;

  }

  */
}

void Phonetic::phoneticInflexChoices(QString newWord, QStringList &wordList) {
  QHash<QString, QString>::const_iterator it;
  QList<QKeyDistance> inflexList;
  QKeyDistance k;
  for (it = singleInflexions->begin(); it != singleInflexions->end(); ++it) {
    k.word = it.key();
    k.dist = Phonetic::LD(newWord, k.word);
    if (k.dist < 10)
      inflexList.append(k);
  }

  qSort(inflexList.begin(), inflexList.end(), &Phonetic::toAsendingInflex);
  foreach (QKeyDistance k, inflexList)
    wordList.append(singleInflexions->value(k.word) + "\t" + k.word);

  qDebug() << "Number of inflex choices " << inflexList.size();
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
      if (count % 1000 == 0) {
        qDebug() << count;
      }
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
        //   qDebug() << "Match found";
        //  qDebug() << "Before Match" << dynStr;
        // qDebug() << "Inflex Match" << inflex;
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

void Phonetic::setInflexTypes(QString fileName) {

  if (inflexTypes != NULL)
    return;

  inflexTypes = new QHash<QString, QList<QKeyValue> *>();

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

  for (sit = inflexTypes->value("extra")->begin();
       sit != inflexTypes->value("extra")->end(); ++sit) {
    singleInflexions->insert(sit->key, sit->value);
  }

  for (sit = inflexTypes->value("action")->begin();
       sit != inflexTypes->value("action")->end(); ++sit) {
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
  QHash<QString, QString>::const_iterator it;
  for (it = singleInflexions->begin(); it != singleInflexions->end(); ++it)
    allInflexions->insert(it.key(), it.value());
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
    if (list.size() == 1) {
      allWords->insert(list.at(0).trimmed(),
                       Romanization::convert2Roman(list.at(0).trimmed()));
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
  //  charPhoneticMap->insert( "v","b");

  charPhoneticMap->insert("j", "j");
  charPhoneticMap->insert("z", "j");
}

QString Phonetic::replaceChars(QString a) {

  QHash<QString, QString>::const_iterator it;

  QString temp = a;

  for (it = charPhoneticMap->begin(); it != charPhoneticMap->end(); ++it) {
    temp = temp.replace(QRegExp(it.key()), it.value());
  }

  return temp;
}

int Phonetic::LD(QString s, QString t) {

  /*
    The difference between this impl. and the previous is that, rather
     than creating and retaining a matrix of size s.length()+1 by t.length()+1,
     we maintain two single-dimensional arrays of length s.length()+1.  The
    first, d, is the 'current working' distance array that maintains the newest
    distance cost counts as we iterate through the characters of String s.  Each
    time we increment the index of String t we are comparing, d is copied to p,
    the second int[].  Doing so allows us to retain the previous cost counts as
    required by the algorithm (taking the minimum of the cost count to the left,
    up one, and diagonally up and to the left of the current cost count being
    calculated).  (Note that the arrays aren't really copied anymore, just
    switched...this is clearly much better than cloning an array or doing a
    System.arraycopy() each time  through the outer loop.)

     Effectively, the difference between the two implementations is this one
    does not cause an out of memory condition when calculating the LD over two
    very large strings.
  */

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
      d[i] = min(min(d[i - 1] + 1, p[i] + 1), p[i - 1] + cost);
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

QString Phonetic::phoneticEquivString(QString orig) {
  QString modified = "";
  foreach (QChar c, orig) {
    if (charPhoneticMap->contains(QString(c)))
      modified.append(charPhoneticMap->value(QString(c)));
    else
      modified.append(QString(c));
  }
  //  qDebug() << "modified " << modified;
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
