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

#include "inflexTrie.h"
#include <iostream>
#include <QList>


InflexTrie *InflexTrie::inflexMapTree=NULL;


InflexTrie::InflexTrie()
{
    inflexTreeNode = new TreeNode();
}



InflexTrie *InflexTrie::getInflexTrie() {
    if( inflexMapTree == NULL) {
        inflexMapTree = new InflexTrie();

    }

    return inflexMapTree;
}

/* insert a character code */

bool InflexTrie::insertWord(TreeNode * curNode, QList<QString> & charList, QString unicodeChar)
{

    //   TreeNode *curNode = charTreeRoot;
       QString  newChar = charList.at(0);

   //    qDebug() << "Size of hash " << curNode->links.size();


       if( ! curNode->links.contains(newChar)) {
           curNode->links.insert(newChar, new TreeNode(QString(""),false));
       }

       charList.removeFirst();

       if(  charList.size() > 0 ) {
          curNode->children++;

          insertWord(curNode->links.value(newChar), charList, unicodeChar );
       }
       else {
           curNode->links[newChar]->unicode = unicodeChar;
           curNode->links[newChar]->fullWord = true;
       }
       return true;
 }


   /* get the choices from the sub-tree */




QString InflexTrie::hasInflection(QString word) {

     QStringList chars;

     chars = Utilities::split(Utilities::reverseX(word),"");

    // qDebug() << "Searching in WordsTrie 121" << chars;
     return  _hasInflection(inflexTreeNode, chars);
 }



QString InflexTrie::_hasInflection(TreeNode *root,  QStringList & chars) {
     QString key;

     bool fullWord = false;
     QString inflection;
     while( root!=NULL &&  chars.size() !=0 && root->links.contains(chars.at(0)) ) {
        key = chars.at(0);
        fullWord= root->links.value(key)->fullWord;
        inflection = root->links.value(key)->unicode;
        root = root->links.value(key);

        chars.removeFirst();

     }
     if(fullWord)
       return inflection;
     else
         return QString("");

 }


 bool InflexTrie::deleteWord(const QString &word) {
     QStringList chars;

     chars = Utilities::getStringListFromHexString(word, "0x");
 //    qDebug() << "WordTrie.cpp 156 going to delete" << word << "  " << chars;
     _deleteWord(inflexTreeNode, chars);
     return true;
 }

 bool InflexTrie::_deleteWord(TreeNode *root,  QStringList & chars) {
     QString key = chars.at(0);
     chars.removeFirst();
 //    qDebug() << "WordTrie.cpp 160 ..." << key;

     while(root->links.contains(key) ) {

        root= root->links[key];
        if( chars.size() == 0 ) {
 //            qDebug() << "Deleting the key " << key;
            root->fullWord=false;
            break;
        }
        key = chars.at(0);
        chars.removeFirst();
     }

     if( chars.size() == 0 ) {
         return true;
     }
     return false;
 }


void InflexTrie::printData() {
   QStack<QString>  partword;
   printTree(inflexTreeNode,  partword);

}

void InflexTrie::printTree( TreeNode *root, QStack<QString> & partword)
   {
       if (root == NULL) {
        //   qDebug() << "Empty";
           return;

       }
     //  qDebug() << "Would like to print the tree";
       QHash<QString, TreeNode *>::const_iterator it= root->links.constBegin();
       QStack<QString>::const_iterator pit;

       while(it != root->links.constEnd() ) {
           QString key = it.key();
           partword.push(key);

           printTree(root->links[key],partword);
           if(root->links[key]->fullWord == true ) {
               pit=partword.constBegin();
               while( pit!=partword.constEnd()) {
                  //qDebug() << Utilities::getUnicodeString(*pit);
                   ++pit;
               }

               //qDebug() << root->links[key]->unicode << "//";
              //  qDebug() << root->links[key]->unicode << "//";
           }
           partword.pop();
           ++it;
       }

   }


   /* the main function */

void InflexTrie::delete_Tree(TreeNode *tree ) {

    QHash<QString, TreeNode *>::const_iterator it;


    for(it= tree->links.constBegin(); it != tree->links.constEnd(); ++it ) {
        delete_Tree(it.value());
    }

    for(it= tree->links.constBegin(); it != tree->links.constEnd(); ++it ) {
        delete it.value();
   //     qDebug() << "Hello";
    }
}


void InflexTrie::addWords(QHash<QString,QString> charMap) {
     if(inflexTreeNode == NULL ) {
         inflexTreeNode = new TreeNode();
     }

     QList<QString> char_list;
     QStringList list ;

     QHash<QString, QString>::const_iterator it= charMap.constBegin();
     QStringList::const_iterator it_list;

     while(it != charMap.constEnd() ) {
         list = it.key().split("0x");
         foreach(QString str, list) {
              if( str.trimmed().length() !=0) {
                 char_list.append(str);
              }
         }

         insertWord(inflexTreeNode, char_list, it.value());
         ++it;
      }

  }


 // loads a mapping from the file
void InflexTrie:: LoadInflections(QHash<QString,QString> *charMap) {
     if(inflexTreeNode != NULL ) {
         delete_Tree(inflexTreeNode);
     }
     inflexTreeNode = new TreeNode();
     delete_Tree(inflexTreeNode);

     QList<QString> char_list;

     QHash<QString, QString>::const_iterator it= charMap->constBegin();

     while(it != charMap->constEnd() ) {
       //  qDebug() << Utilities::getUnicodeString(it.key());
         char_list.clear();
         foreach( QChar kChar, Utilities::reverseX(it.key()) ) {
             char_list.append(QString(kChar));
             //qDebug() << Utilities::getUnicodeString(QString(kChar));

         }  
         insertWord(inflexTreeNode, char_list, it.key());
         ++it;
      }

  }

