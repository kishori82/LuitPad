#include "asciiTrie.h"
#include <iostream>
#include <QList>

AsciiTrie *AsciiTrie::wordMapTree=NULL;
AsciiTrie *AsciiTrie::unicodeTree=NULL;


AsciiTrie::AsciiTrie()
{
    asciiTree = new TreeNode();
}


AsciiTrie *AsciiTrie::getAsciiTrie() {
    if( wordMapTree == NULL) {
        wordMapTree = new AsciiTrie();
    }
    return wordMapTree;
}


AsciiTrie *AsciiTrie::getUnicodeTrie() {
    if( unicodeTree == NULL) {
        unicodeTree = new AsciiTrie();
    }
    return unicodeTree;
}

/* insert a character code */

bool AsciiTrie::insertWord(TreeNode * curNode, QList<QString> & charList, QString unicodeChar)
{

    //   TreeNode *curNode = asciiTreeRoot;
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
void AsciiTrie::get_choices_from_subtree(TreeNode *root, QList<QString> & choices, QStack<QString> & wordstack, int max_choices){
       if (root == NULL)
            return;
       QHash<QString, TreeNode *>::const_iterator it = root->links.constBegin();


       if( root->fullWord) choices.append(root->unicode);

       QString key;
       while(it !=  root->links.constEnd()) {
           key = it.key();
           get_choices_from_subtree(root->links[key], choices, wordstack, max_choices);
           ++it;
       }
   }

   // this is the function  the editor calls to predict the choices
void AsciiTrie::get_choice( QStringList prefix, int choicesize, bool autoselect, QList<QString> & choices ) {

       choices.clear();

       QList<QString> prefix_list;

       for(int i = 0; i < prefix.length(); i++ )
           prefix_list.append( prefix.at(i));

       if( prefix.length() ==0 ) {
           //choices.add("");
           return;
       }

       get_choice_words( asciiTree, choices,  prefix_list, choicesize, autoselect);
       return;
   }

   // the second function to be called.
 void AsciiTrie::get_choice_words(TreeNode *root,  QList<QString> & choices, QList<QString> & words, int choicesize, bool autoselect)
   {
       if( words.size() ==0 ) {
           QStack<QString> wordstack;
           get_choices_from_subtree(root, choices, wordstack, choicesize);
       }
       else {
         QString key = words.at(0);
         words.removeFirst();

         if( root->links.contains(key))
            get_choice_words(root->links[key],  choices, words, choicesize, autoselect);
         else if( autoselect ) {
             words.clear();
             if( root->fullWord ) {
                 choices.append(root->unicode);
             }
         }
       }
   }

 QString AsciiTrie::findPrefix(QStringList &chars) {

     QString result;

    // chars = Utilities::getStringListFromUnicodeString(word);

     int pos = _findPrefix(asciiTree, chars);


     for( int i = 0; i <= pos; i++) {
        result = result + QString("0x") + chars.at(i);
     }

     for(int i=0; i <=pos; i++)
         chars.removeFirst();

    // qDebug() << " Position " << pos << " result " << result;
     return result;
 }


 int AsciiTrie::_findPrefix(TreeNode *root,  QStringList chars) {
     QString key;


     QStack<TreeNode *> mstack;

     bool fullWord = false;
     int i = -1;

     while( root!=NULL &&  chars.size() !=0 && root->links.contains(chars.at(0)) ) {
        key = chars.at(0);
        fullWord= root->links.value(key)->fullWord;
        i = i + 1;
        //mstack.push(root);
        root = root->links.value(key);
        mstack.push(root);
        chars.removeFirst();
        //qDebug() << "key " << key;
     }

     while( mstack.size() > 0) {
         root = mstack.pop();
         //qDebug() << "Full word " << root->fullWord;
         if( root->fullWord ) return i;
         --i;
     }


     return i;
 }


void AsciiTrie::printData() {
   QStack<QString>  partword;
   printTree(asciiTree,  partword);

}

void AsciiTrie::printTree( TreeNode *root, QStack<QString> & partword)
   {
       if (root == NULL) {
        //   qDebug() << "Empty";
           return;

       }

       QHash<QString, TreeNode *>::const_iterator it= root->links.constBegin();
       QStack<QString>::const_iterator pit;

       while(it != root->links.constEnd() ) {
           QString key = it.key();
           partword.push(key);

           printTree(root->links[key],partword);
           if(root->links[key]->fullWord == true ) {
               pit=partword.constBegin();
               while( pit!=partword.constEnd()) {
                  qDebug() << *pit;
                   ++pit;
               }

               //qDebug() << root->links[key]->unicode << "//";
              //  qDebug() << root->links[key]->unicode << "//";
           }
           partword.pop();
           ++it;
       }

   }



void AsciiTrie::delete_Tree(TreeNode *tree ) {

    QHash<QString, TreeNode *>::const_iterator it;


    for(it= tree->links.constBegin(); it != tree->links.constEnd(); ++it ) {
        delete_Tree(it.value());
    }

    for(it= tree->links.constBegin(); it != tree->links.constEnd(); ++it ) {
        delete it.value();
   //     qDebug() << "Hello";
    }
}


void AsciiTrie::addWords(QHash<QString,QString> charMap) {
     if(asciiTree == NULL ) {
         asciiTree = new TreeNode();
     }

     QList<QString> char_list;
     QStringList list ;

     QHash<QString, QString>::const_iterator it= charMap.constBegin();

     while(it != charMap.constEnd() ) {
         list = it.key().split("0x");
         foreach(QString str, list) {
              if( str.trimmed().length() !=0) {
                 char_list.append(str);
              }
         }
       //  qDebug() << "Add words " << char_list;

         insertWord(asciiTree, char_list, it.value());
         ++it;
      }
   //   qDebug() << "Adding words = " << charMap.size();

  }




