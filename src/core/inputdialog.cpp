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

#include "inputdialog.h"

InputDialog::InputDialog() {

    imageNum = 1;

    QGridLayout *layout = new QGridLayout(this);

    width = new QLineEdit(this);
    height = new QLineEdit(this);

    imageName = new QLabel(tr("Enter the desired size for image %1").arg(imageNum));

    width_lbl = new QLabel(tr("Width "));
    height_lbl = new QLabel(tr("Height"));

    QPushButton *button = new QPushButton("Ok", this);

    layout->addWidget(imageName , 0, 0, 1, 2);

    layout->addWidget(width_lbl , 1, 0);
    layout->addWidget(height_lbl, 2, 0);

    layout->addWidget(width, 1, 1);
    layout->addWidget(height, 2, 1);

    width->setText("200");
    height->setText("200");

    layout->addWidget(button,3,0);
    setLayout(layout);

    connect(button, SIGNAL(clicked()), SLOT(accept()));
}


QPair<unsigned int, unsigned int>  InputDialog::getSize() const {
    QPair<unsigned int, unsigned int>  p;


    if( width->text().toInt() == 0 ||  height->text().toInt() == 0 ) {
        Utilities::warningBox(QString("Values should be positive integers only\n Defaulting to size 200 x 200"));
        p = qMakePair<unsigned int, unsigned int>((int)200, (int)200);
    }
    else {
        p = qMakePair< unsigned int, unsigned int>(width->text().toInt(),height->text().toInt());
    }
    return p;
}

void InputDialog::incImageNum() {
    imageNum++;
    imageName->setText(tr("Enter the desired size for image %1").arg(imageNum));
    width->setText(QString("200"));
    height->setText(QString("200"));

    return;
}

void InputDialog::setSize(unsigned int w, unsigned int h) {
    imageNum++;
    imageName->setText(tr("Enter the desired size for image %1").arg(imageNum));
    if( w==0 || h==0) {
        width->setText(QString("200"));
        height->setText(QString("200"));
    }
    else {
        width->setText(QString("%1").arg(w));
        height->setText(QString("%1").arg(h));
    }
    return;
}
