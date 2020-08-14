#include "qtextedittooltip.h"
QTextEditToolTip *QTextEditToolTip::object=NULL;

QTextEditToolTip::QTextEditToolTip()
{

}


QTextEditToolTip * QTextEditToolTip::getQTextEditToolTip() {
   if( object == 0) {
       object = new QTextEditToolTip();
       object->setFont( QFont("Kalpurush",10,0,false));
   }
   return object;
}

void QTextEditToolTip::showTextEditToolTip(QString html, const QPoint pos) {
    this->setHtml(html);
    this->setReadOnly(true);

    this->move(pos);
    this->show();
}


/*
 *

        QTextEdit *tooltip = new QTextEdit;

        tooltip->setFont( QFont("Kalpurush",10,0,false));
        tooltip->setHtml(idioms->toolTip() );
        tooltip->setReadOnly(true);
        tooltip->anchorAt(event->pos());
        tooltip->show();


*/
