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
