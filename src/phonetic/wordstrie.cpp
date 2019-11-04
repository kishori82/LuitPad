#include "wordstrie.h"
#include <iostream>
#include <QList>

WordsTrie *WordsTrie::wordMapTree=NULL;
WordsTrie *WordsTrie::profileWordMapTree=NULL;
QHash<QChar, QHash<QChar, float> * > *WordsTrie::distances = NULL;
QString WordsTrie::dictionaryFile = QString(":/files/composite_dictionary.txt");

WordsTrie::WordsTrie()
{
    charTree = new TreeNode();
    initializeDistances();
}

void WordsTrie::initializeDistances() {
    if( distances!=NULL ) return;
    distances = new QHash<QChar, QHash<QChar, float> * >();

    //hosro ee kar and dirgho ee kars
    distances->insert( QChar(0x9bf),NULL);  distances->insert(QChar(0x9bf), new QHash<QChar, float >());
    distances->insert( QChar(0x9c0),NULL);  distances->insert(QChar(0x9c0), new QHash<QChar, float >());

    //hosro ee and dirgho ee kars equivalence
    distances->value(QChar(0x9bf))->insert(QChar(0x9c0), 0.2);  distances->value(QChar(0x9c0))->insert(QChar(0x9bf), 0.2);

    //prothom cho and dutiya cho
    distances->insert( QChar(0x99a),NULL);  distances->insert(QChar(0x99a), new QHash<QChar, float >());
    distances->insert( QChar(0x99b),NULL);  distances->insert(QChar(0x99b), new QHash<QChar, float >());
    distances->value(QChar(0x99a))->insert(QChar(0x99b), 0.2);
    distances->value(QChar(0x99b))->insert(QChar(0x99a), 0.2);

    //ho and dontiya ho
    distances->insert( QChar(0x9b8),NULL);  distances->insert(QChar(0x9b8), new QHash<QChar, float >());
    distances->insert( QChar(0x9b9),NULL);  distances->insert(QChar(0x9b9), new QHash<QChar, float >());
    distances->value(QChar(0x9b8))->insert(QChar(0x9b9), 0.2);
    distances->value(QChar(0x9b9))->insert(QChar(0x9b8), 0.2);

    //hosro u, dirgho u and u
    distances->insert( QChar(0x9c1),NULL);  distances->insert(QChar(0x9c1), new QHash<QChar, float >());
    distances->insert( QChar(0x9c2),NULL);  distances->insert(QChar(0x9c2), new QHash<QChar, float >());
    distances->value(QChar(0x9c1))->insert(QChar(0x9c2), 0.2);
    distances->value(QChar(0x9c2))->insert(QChar(0x9c1), 0.2);

    //borgiya go and ontosto jo
    distances->insert( QChar(0x99c),NULL);  distances->insert(QChar(0x99c), new QHash<QChar, float >());
    distances->insert( QChar(0x9af),NULL);  distances->insert(QChar(0x9af), new QHash<QChar, float >());
    distances->value(QChar(0x99c))->insert(QChar(0x9af), 0.2);
    distances->value(QChar(0x9af))->insert(QChar(0x99c), 0.2);

    // dontiya to and murdhyaniya to
    distances->insert(QChar(0x99f),NULL);  distances->insert(QChar(0x99f), new QHash<QChar, float >());
    distances->insert(QChar(0x9a4),NULL);  distances->insert(QChar(0x9a4), new QHash<QChar, float >());
    distances->value(QChar(0x99f))->insert(QChar(0x9a4), 0.2);
    distances->value(QChar(0x9a4))->insert(QChar(0x99f), 0.2);

    // dontiya tho and murdhyaniya tho
    distances->insert(QChar(0x9a0),NULL);  distances->insert(QChar(0x9a0), new QHash<QChar, float >());
    distances->insert(QChar(0x9a5),NULL);  distances->insert(QChar(0x9a5), new QHash<QChar, float >());
    distances->value(QChar(0x9a0))->insert(QChar(0x9a5), 0.2);
    distances->value(QChar(0x9a5))->insert(QChar(0x9a0), 0.2);

    // dontiya do and murdhyaniya do
    distances->insert(QChar(0x9a1),NULL);  distances->insert(QChar(0x9a1), new QHash<QChar, float >());
    distances->insert(QChar(0x9a6),NULL);  distances->insert(QChar(0x9a6), new QHash<QChar, float >());
    distances->value(QChar(0x9a1))->insert(QChar(0x9a6), 0.2);
    distances->value(QChar(0x9a6))->insert(QChar(0x9a1), 0.2);

    // dontiya dho and murdhyaniya dho
    distances->insert(QChar(0x9a2),NULL);  distances->insert(QChar(0x9a2), new QHash<QChar, float >());
    distances->insert(QChar(0x9a7),NULL);  distances->insert(QChar(0x9a7), new QHash<QChar, float >());
    distances->value(QChar(0x9a2))->insert(QChar(0x9a7), 0.2);
    distances->value(QChar(0x9a7))->insert(QChar(0x9a2), 0.2);

    // dontiya no and murdhyaniya no
    distances->insert(QChar(0x9a8),NULL);  distances->insert(QChar(0x9a8), new QHash<QChar, float >());
    distances->insert(QChar(0x9a3),NULL);  distances->insert(QChar(0x9a3), new QHash<QChar, float >());
    distances->value(QChar(0x9a8))->insert(QChar(0x9a3), 0.2);
    distances->value(QChar(0x9a3))->insert(QChar(0x9a8), 0.2);

    // talubiya kho and murdhanya kho
    distances->insert(QChar(0x9b6),NULL);  distances->insert(QChar(0x9b6), new QHash<QChar, float >());
    distances->insert(QChar(0x9b7),NULL);  distances->insert(QChar(0x9b7), new QHash<QChar, float >());
    distances->value(QChar(0x9b6))->insert(QChar(0x9b7), 0.2);
    distances->value(QChar(0x9b7))->insert(QChar(0x9b6), 0.2);
}

WordsTrie *WordsTrie::getProfileWordsTrie() {
    if( profileWordMapTree == NULL) {
        profileWordMapTree = new WordsTrie();
    }
    return profileWordMapTree;
}

WordsTrie *WordsTrie::getWordsTrie() {
    if( wordMapTree == NULL) {
        wordMapTree = new WordsTrie();
    }
    return wordMapTree;
}

/* insert a character code */
bool WordsTrie::insertWord(TreeNode * curNode, QList<QString> & charList, QString unicodeChar)
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
 void WordsTrie::get_choices_from_subtree(TreeNode *root, QList<QString> & choices, QStack<QString> & wordstack, int max_choices){
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
void WordsTrie::get_choice( QStringList prefix, int choicesize, bool autoselect, QList<QString> & choices ) {

       choices.clear();

       QList<QString> prefix_list;

       for(int i = 0; i < prefix.length(); i++ )
           prefix_list.append( prefix.at(i));

       if( prefix.length() ==0 ) {
           //choices.add("");
           return;
       }

       get_choice_words( charTree, choices,  prefix_list, choicesize, autoselect);
       return;
   }

   // the second function to be called.
 void WordsTrie::get_choice_words(TreeNode *root,  QList<QString> & choices, QList<QString> & words, int choicesize, bool autoselect)
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

 bool WordsTrie::hasWord(QString word) {

     QStringList chars;

     chars = Utilities::getStringListFromUnicodeString(word);
    // qDebug() << "Searching in WordsTrie 121" << chars;
     return  _hasWord(charTree, chars);
 }



 bool WordsTrie::_hasWord(TreeNode *root,  QStringList & chars) {
     QString key;

     bool fullWord = false;

     while( root!=NULL &&  chars.size() !=0 && root->links.contains(chars.at(0)) ) {
        key = chars.at(0);
        fullWord= root->links.value(key)->fullWord;

        root = root->links.value(key);

        chars.removeFirst();
        //qDebug() << "key " << key;
     }
     if(  chars.size() ==0) {
         //qDebug() << "Quitting true";
         return fullWord;
     }
     return false;
 }


 bool WordsTrie::deleteWord(const QString &word) {
     QStringList chars;

     chars = Utilities::getStringListFromHexString(word, "0x");
 //    qDebug() << "WordTrie.cpp 156 going to delete" << word << "  " << chars;
     _deleteWord(charTree, chars);
     return true;
 }

 bool WordsTrie::_deleteWord(TreeNode *root,  QStringList & chars) {
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


 void WordsTrie::getWordList(QStringList &wordList) {

     QStack<QString>  partword;
     getWordsFromTree(charTree,  partword, wordList);

     qDebug() << "Going to sort dictionary ";
     Utilities::sortQStringList(wordList, true);
     qDebug() << "Finished sorting dictionary";
 }


void WordsTrie::getWordsFromTree( TreeNode *root, QStack<QString> & partword, QStringList& wordList)
   {
       if (root == NULL) {
        //   qDebug() << "Empty";
           return;

       }
  //     qDebug() << "Would like to print the tree";
       QHash<QString, TreeNode *>::const_iterator it= root->links.constBegin();
       QStack<QString>::const_iterator pit;

       while(it != root->links.constEnd() ) {
           QString key = it.key();
           partword.push(key);

           getWordsFromTree(root->links[key],partword, wordList);
           if(root->links[key]->fullWord == true ) {
               pit=partword.constBegin();
               QString word;
               while( pit!=partword.constEnd()) {
                   word.append(QString("0x") + (*pit) );
                   ++pit;
               }
               wordList << word ;
           }
           partword.pop();
           ++it;
       }

   }

void WordsTrie::getPossibleWords(QString word, QList<QKeyFloat> &possibleList, unsigned int numMax) {
    QStack<QString>  leftStack;
    QStack<QString>  rightStack;
    QStack<QString> partWord;
    QStringList candidateList;

  //  qDebug() << "Word " << Utilities::getUnicodeString(word);
    foreach(QChar c, Utilities::reverseX(word)) leftStack.push(Utilities::getUnicodeStringX(c));


    getPossibleCandidates( wordMapTree->charTree, word,leftStack, rightStack, partWord, candidateList );
   // qDebug() << " Candidates found " << candidateList.size();
   // qDebug() << " Word " << word;
    QList<QKeyFloat> candidateRankList;
    QKeyFloat wr;
    foreach( QString candidate, candidateList ) {
        wr.key = Utilities::getUnicode(candidate,"0x");
        wr.value = WordsTrie::wordDistance(wr.key, word );
      //  qDebug() << " Word " << word << " Candidate " << candidate << " distance " << wr.value;
        candidateRankList.append(wr);
    }
    Utilities::sortQKeyFloatList(candidateRankList,true);
    for( unsigned int i=0; i < numMax && i < candidateRankList.size(); i++ )
        possibleList.append(candidateRankList.at(i));


    //wordMapTree->printData();
  //  profileWordMapTree->printData();
}

void WordsTrie::getPossibleCandidates( TreeNode *root, QString uniWord, QStack<QString> & leftStack, QStack<QString> &rightStack, QStack<QString> &partWord, QStringList &candidateList)
   {
       if (root == NULL || leftStack.size()==0) {
           return;

       }
       bool stackoper = false;
       QString c;
       QStack<QString>::const_iterator sit;

       if( leftStack.size()!=0 ) {
          c= leftStack.pop();
        //  qDebug() << " -- " << c;
          rightStack.push(c);
          stackoper = true;
       }
       QString searchWord="";
       QString targetWord="";
       for(sit = rightStack.constBegin(); sit!=rightStack.constEnd(); ++sit) searchWord.append(QString("0x") + (*sit) );
       searchWord = Utilities::getUnicode(searchWord,"0x");
       QHash<QString, TreeNode *>::const_iterator it= root->links.constBegin();

       while(it != root->links.constEnd() ) {
           QString key = it.key();

        //   qDebug() << "++ " <<it.key();
           partWord.push(key);

           targetWord.clear();
           for(sit = partWord.constBegin(); sit!=partWord.constEnd(); ++sit) targetWord.append(QString("0x")+ (*sit));
           targetWord = Utilities::getUnicode(targetWord,"0x");


           if( targetWord.size() < searchWord.size() + 3 && WordsTrie::wordDistance(targetWord, searchWord) < 4 ) {
           //    qDebug() << "target and search word uniword key" << Utilities::getUnicodeString(targetWord) <<\
               //            "  " << Utilities::getUnicodeString(searchWord) << "  " << Utilities::getUnicodeString(uniWord)<<\
                 //          "  " << key;
               getPossibleCandidates(root->links[key],uniWord, leftStack,rightStack, partWord, candidateList);
              // if(root->links[key]->fullWord == true && WordsTrie::wordDistance(targetWord, uniWord) < 4 && targetWord.size() +3 > uniWord.size()  ) {
                  // qDebug() << "it is not healthy";
               if(root->links[key]->fullWord == true && WordsTrie::wordDistance(targetWord, uniWord) < 4 && \
                       root->links[key]->unicode.size() > 3 && targetWord.size() +3 > uniWord.size()  ) {
                  // qDebug() << "it is not healthy";
                 //  qDebug() << root->links[key]->unicode;
                   candidateList.append(root->links[key]->unicode);
               }
           }
           partWord.pop();
           ++it;
       }

       if( stackoper ){
          c = rightStack.pop();
          leftStack.push(c);
       }
   }


float WordsTrie::wordDistance(QString s, QString t) {

    int n = s.length(); // length of s
    int m = t.length(); // length of t

    if (n == 0) {
      return m;
    } else if (m == 0) {
      return n;
    }

    float *p = new float[n+1]; //'previous' cost array, horizontally
    float *d = new float[n+1]; // cost array, horizontally
    float * _d; //placeholder to assist in swapping p and d

    // indexes into strings s and t
    int i; // iterates through s
    int j; // iterates through t

    QChar t_j; // jth character of t

    int cost; // cost

    for (i = 0; i<=n; i++) {
       p[i] = i;
    }

    for (j = 1; j<=m; j++) {
       t_j = t.at(j-1);
       d[0] = j;

       for (i=1; i<=n; i++) {
          cost = (s.at(i-1)==t_j ) ? 0 : 1;

          if(  distances->contains(s.at(i-1))  && distances->value(s.at(i-1))->contains(t_j)  ) {
              cost = distances->value(s.at(i-1))->value(t_j);
          }/*
          else if( s.at(i-1)==QChar(0x9cd) || t_j==QChar(0x9cd) ) {
              cost = 0;
          } */
          else if (s.at(i-1)==t_j )
              cost = 0;
          else
              cost = 1;


          // minimum of cell to the left+1, to the top+1, diagonally left and up +cost
          d[i] = Min(Min(d[i-1]+1, p[i]+1),  p[i-1]+cost);
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



   /* the main function */

   void WordsTrie::load_dictionary(QString fileName, QHash<QString,QString> & charMap) {

     QFile file(fileName);
     if(!file.open(QIODevice::ReadOnly)) {
             QMessageBox::information(0, "error", file.errorString());
     }

     QTextStream in(&file);
     QStringList list ;

     int i =0;
     QString line;
     while(!in.atEnd()) {
         line = in.readLine();
         list = line.split("\t");
         //qDebug() << list;
         if( list.size() >= 1 ) {
             if( list.at(0).trimmed().size() !=0 )
                charMap[list.at(0).trimmed()] = list.at(0).trimmed();
              // qDebug() << i << "   " << list.at(0).trimmed();
          //  i++;
         }
     }
   }

// loads a mapping from the file
   void WordsTrie::LoadDictionaryWords(QString  fileName) {
           QHash<QString,QString> charMap;

      //     charTree = new TreeNode();

           //load the map from a file

           load_dictionary(fileName, charMap);

           QList<QString> char_list;
           QStringList list ;

           QHash<QString, QString>::const_iterator it= charMap.constBegin();
           QStringList::const_iterator it_list;

           while(it != charMap.constEnd() ) {
            //  qDebug() << it.key() <<"++" << it.value();
              list = it.key().split("0x");
              for(it_list = list.constBegin(); it_list != list.constEnd(); ++it_list ) {
                  if( QString::compare(QString(""), *it_list)!=0) {
                     char_list.append(*it_list);
                  }
              }

              insertWord(charTree, char_list, it.value());
              ++it;
           }

           qDebug()<< "Loaded dictionary of size " << charMap.size();
   }

void WordsTrie::delete_Tree(TreeNode *tree ) {

    QHash<QString, TreeNode *>::const_iterator it;


    for(it= tree->links.constBegin(); it != tree->links.constEnd(); ++it ) {
        delete_Tree(it.value());
    }

    for(it= tree->links.constBegin(); it != tree->links.constEnd(); ++it ) {
        delete it.value();
   //     qDebug() << "Hello";
    }
}


void WordsTrie::addWords(QHash<QString,QString> charMap) {
     if(charTree == NULL ) {
         charTree = new TreeNode();
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

         insertWord(charTree, char_list, it.value());
         ++it;
      }

  }


 // loads a mapping from the file
void WordsTrie::LoadProfileDictionaryWords(QHash<QString,QString> charMap) {
     if(charTree != NULL ) {
         delete_Tree(charTree);
     }
     charTree = new TreeNode();
     delete_Tree(charTree);

     QList<QString> char_list;
     QStringList list ;

     QHash<QString, QString>::const_iterator it= charMap.constBegin();
     QStringList::const_iterator it_list;

     while(it != charMap.constEnd() ) {

         list = it.key().split("0x");
         for(it_list = list.constBegin(); it_list != list.constEnd(); ++it_list ) {
              if( QString::compare(QString(""), *it_list)!=0) {
                 char_list.append(*it_list);
              }
         }
         insertWord(charTree, char_list, it.value());
         ++it;
      }


  }

