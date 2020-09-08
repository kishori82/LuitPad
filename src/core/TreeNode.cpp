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

#include "TreeNode.h"

TreeNode::TreeNode(QString code, bool fullWord)
{
    children =0;
    this->unicode = code;
//    links = new QHash<QString, TreeNode *>();
    this->fullWord = fullWord;
    this->used = false;
}


TreeNode::TreeNode()
{
    children =0;
    this->unicode = QString("");
    this->fullWord = false;
    this->used = false;
}

void TreeNode::printData(TreeNode *asciiTree) {
   QStack<QString>  partword;
   printTree(asciiTree,  partword);

}

void TreeNode::printTree( TreeNode *root, QStack<QString> & partword)
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
                 // qDebug() << *pit;
                   ++pit;
               }

               //qDebug() << root->links[key]->unicode << "//";
              //  qDebug() << root->links[key]->unicode << "//";
           }
           partword.pop();
           ++it;
       }

   }

void TreeNode::delete_Tree(TreeNode *tree ) {

    QHash<QString, TreeNode *>::const_iterator it;


    for(it= tree->links.begin(); it != tree->links.end(); ++it ) {
        TreeNode::delete_Tree(it.value());
    }

    for(it= tree->links.begin(); it != tree->links.end(); ++it ) {
        delete it.value();
   //     qDebug() << "Hello";
    }
}
