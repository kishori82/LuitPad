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

#include "src/characters/chartrie.h"
#include "src/utils/utilities.h"

CharTrie *CharTrie::charMapTree = NULL;

CharTrie::CharTrie() {
  // charTree = new TreeNode();
}

CharTrie *CharTrie::getCharTrie() {
  if (charMapTree == NULL) {
    charMapTree = new CharTrie();
    charMapTree->LoadCharTreeMap(":/files/user_keymap.txt");
    // qDebug() << "CharTrie::getCharTrie";
  }

  return charMapTree;
}

/* insert a character code */

bool CharTrie::insertWord(TreeNode *charTreeRoot, QList<QString> &charList,
                          QString unicodeChar) {

  TreeNode *curNode = charTreeRoot;
  QString newChar = charList.at(0);

  if (curNode->links[newChar] == NULL)
    curNode->links.insert(newChar, new TreeNode(NULL, false));

  charList.removeFirst();

  if (charList.size() > 0) {
    curNode->children++;
    insertWord(curNode->links[newChar], charList, unicodeChar);
  } else {
    //     qDebug() << newChar;
    curNode->links[newChar]->unicode = unicodeChar;
    curNode->links[newChar]->fullWord = true;
  }

  return true;
}

QString CharTrie::whatPrefixMatch(QString prefix) {

  //   Utilities::warningBox("hello");

  QList<QString> prefix_list;
  for (int i = 0; i < prefix.length(); i++)
    prefix_list.append(prefix.data()[i]);

  if (prefix_list.size() == 0) {
    return "";
  }

  QString whatMatch = "";

  _whatPrefixMatch(charTree, prefix_list, whatMatch);
  return whatMatch;
}

void CharTrie::_whatPrefixMatch(TreeNode *root,
                                QList<QString> &prefix_chars_list,
                                QString &whatMatch) {

  if (root->fullWord) {
    return;
  }

  if (prefix_chars_list.size() == 0)
    return;
  QString key = prefix_chars_list.at(0);
  prefix_chars_list.removeFirst();

  if (root->links[key] != NULL) {
    whatMatch.append(key);
    _whatPrefixMatch(root->links[key], prefix_chars_list, whatMatch);
  }
}

QString CharTrie::getPrefixMatch(QString prefix) {

  //   Utilities::warningBox("hello");

  QList<QString> prefix_list;
  for (int i = 0; i < prefix.length(); i++)
    prefix_list.append(prefix.data()[i]);

  if (prefix_list.size() == 0) {
    return "";
  }

  return _getPrefixMatch(charTree, prefix_list);
}

QString CharTrie::_getPrefixMatch(TreeNode *root,
                                  QList<QString> &prefix_chars_list) {
  QString result;

  if (root->fullWord) {
    return root->unicode;
  }

  if (prefix_chars_list.size() == 0)
    return result;
  QString key = prefix_chars_list.at(0);
  prefix_chars_list.removeFirst();

  if (root->links[key] != NULL) {
    result = _getPrefixMatch(root->links[key], prefix_chars_list);
  }

  return result;
}

// this is the function  the editor calls to predict the choices
bool CharTrie::doesPrefixMatch(QString prefix) {

  //   Utilities::warningBox("hello");

  QList<QString> prefix_list;
  for (int i = 0; i < prefix.length(); i++)
    prefix_list.append(prefix.data()[i]);

  if (prefix_list.size() == 0) {
    return false;
  }

  // qDebug() << prefix_list;

  return foundPrefixMatch(charTree, prefix_list);
}

bool CharTrie::foundPrefixMatch(TreeNode *root,
                                QList<QString> &prefix_chars_list) {
  bool found = false;

  if (root->fullWord) {
    return true;
  }
  if (prefix_chars_list.size() == 0)
    return false;
  QString key = prefix_chars_list.at(0);
  prefix_chars_list.removeFirst();

  if (root->links[key] != NULL) {
    found = foundPrefixMatch(root->links[key], prefix_chars_list);
  }

  return found;
}

/* get the choices from the sub-tree */

// this is the function  the editor calls to predict the choices
void CharTrie::get_choice(QString prefix, int choicesize, bool autoselect,
                          QList<QKeyValue> &choices) {

  //   Utilities::warningBox("hello");
  choices.clear();

  QList<QString> prefix_list;

  QStack<QString> wordstack;
  for (int i = 0; i < prefix.length(); i++)
    prefix_list.append(prefix.data()[i]);

  if (prefix.length() == 0) {
    // choices.add("");
    return;
  }
  // Utilities::warningBox("hello 1");
  get_choice_words(charTree, choices, prefix_list, wordstack, choicesize,
                   autoselect);
  //   Utilities::warningBox("hello again");
  return;
}

// the second function to be called.
void CharTrie::get_choice_words(TreeNode *root, QList<QKeyValue> &choices,
                                QList<QString> &words,
                                QStack<QString> wordstack, int choicesize,
                                bool autoselect) {
  //     Utilities::warningBox("hello 2");
  QKeyValue pickChar;
  if (words.size() == 0) {
    get_choices_from_subtree(root, choices, wordstack, choicesize);
  } else {
    QString key = words.at(0);
    words.removeFirst();
    wordstack.push(key);
    // qDebug() << " links " << root->links.size();
    //  Utilities::warningBox("hello 3" + key);
    if (root->links[key] != 0)
      get_choice_words(root->links[key], choices, words, wordstack, choicesize,
                       autoselect);
    else if (autoselect) {
      words.clear();
      if (root->fullWord) {
        pickChar.value = root->unicode;
        choices.append(pickChar);
      }
    }
    //  Utilities::warningBox("hello 3");
  }
}
/* get the choices from the sub-tree */

void CharTrie::get_choices_from_subtree(TreeNode *root,
                                        QList<QKeyValue> &choices,
                                        QStack<QString> &wordstack,
                                        int max_choices) {
  if (root == NULL)
    return;
  QHash<QString, TreeNode *>::const_iterator it = root->links.constBegin();

  QKeyValue pickChar;
  pickChar.key = "";

  if (root->fullWord) {
    foreach (QString c, wordstack)
      pickChar.key += c;

    pickChar.value = root->unicode;
    choices.append(pickChar);
  }

  QString key;
  while (it != root->links.constEnd()) {
    key = it.key();
    wordstack.push(key);
    get_choices_from_subtree(root->links[key], choices, wordstack, max_choices);
    wordstack.pop();
    ++it;
  }
}
void CharTrie::printTree(TreeNode *root, QStack<QString> &partword) {
  if (root == NULL)
    return;

  QHash<QString, TreeNode *>::const_iterator it = root->links.constBegin();
  QStack<QString>::const_iterator pit;

  while (it != root->links.constEnd()) {
    QString key = it.key();
    partword.push(key);
    // System.out.print(key);
    printTree(root->links[key], partword);
    if (root->links[key]->fullWord == true) {
      pit = partword.constBegin();
      while (pit != partword.constEnd()) {
        //  qDebug() << *pit;
        ++pit;
      }
    }
    //          qDebug() << root->links[key]->unicode << "//";
    partword.pop();
    ++it;
  }
}

/* the main function */

void CharTrie::load_saved_maps(QString fileName,
                               QHash<QString, QString> &charMap) {

  QFile file(fileName);
  if (!file.open(QIODevice::ReadOnly)) {
    QMessageBox::information(0, "error", file.errorString());
  }

  QTextStream in(&file);
  QStringList list;

  QString line;
  while (!in.atEnd()) {
    line = in.readLine();
    list = line.split("\t");
    charMap[list.at(1).trimmed()] = list.at(0).trimmed();
    // Kishori    qDebug() << line ;
  }
}

void CharTrie::LoadCharTreeFromProfile(QHash<QString, QString> &charMap) {
  charTree = new TreeNode();
  QList<QString> char_list;
  QHash<QString, QString>::const_iterator it = charMap.constBegin();
  while (it != charMap.constEnd()) {
    for (int i = 0; i < it.value().length(); i++) {
      // qDebug() << it.value().data()[i] <<"++" << it.key();
      char_list.append(it.value().data()[i]);
    }

    //  System.out.println(mapkey +  "  -----");
    /// String unicode = charMap.get(mapkey);
    //   qDebug() << char_list <<"++" << it.key();

    insertWord(charTree, char_list, it.key());
    ++it;
  }

  QStack<QString> partword;
  // printTree(charTree, partword);
}

/*
 *  Loads the  Char Tree map
 */
void CharTrie::LoadCharTreeMap(QString fileName) {
  QHash<QString, QString> charMap;

  charTree = new TreeNode();

  // qDebug() << "in CharTrie::LoadCharTreeMap 0";
  // load the map from a file
  load_saved_maps(fileName, charMap);

  QList<QString> char_list;
  // qDebug() << "in CharTrie::LoadCharTreeMap 1";

  QHash<QString, QString>::const_iterator it = charMap.constBegin();
  while (it != charMap.constEnd()) {
    for (int i = 0; i < it.key().length(); i++) {
      // Kishori  qDebug() << it.key().data()[i] <<"++" << it.key();
      char_list.append(it.key().data()[i]);
    }

    //  System.out.println(mapkey +  "  -----");
    /// String unicode = charMap.get(mapkey);

    insertWord(charTree, char_list, it.value());
    ++it;
  }
  // qDebug() << "in CharTrie::LoadCharTreeMap 4";

  QStack<QString> partword;
  printTree(charTree, partword);
  // qDebug() << "in CharTrie::LoadCharTreeMap 5";
}

/*
static void CharTrie::ReLoadPrefixTreeMap(String fileName){
    Singleton_PrefixTree = new PrefixTreeMap( fileName);
}

static PrefixTreeMap CharTrie::GetPrefixTreeMap(String fileName){
    if( Singleton_PrefixTree == null ) {
        Singleton_PrefixTree = new PrefixTreeMap( fileName);
    }
    return( Singleton_PrefixTree);
}
*/
