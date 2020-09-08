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

#include "loaddatathread.h"
LoadDataThread::LoadDataThread(QObject *parent) :
    QThread(parent)
{
}


void LoadDataThread::LoadDictionary(
        QString IdeasFile,
        QString engWrdIdFile,
        QString asmWrdWrdId,
        QString examplesFile,
        QString idiomsFile,
        QString poribhashaFile) {


    Dictionaries *dictionaries = Dictionaries::getDictionaries();

    dictionaries->loadAssameseEnglishDictionaries(
                IdeasFile,
                engWrdIdFile,
                asmWrdWrdId,
                examplesFile,
                idiomsFile,
                poribhashaFile
             );




    //dictionaries->setAssameseEnglishDictionaries(dictionaryFile);
    //dictionaries->setPoriBhashaDictionaries(poribhashaFile);
}


