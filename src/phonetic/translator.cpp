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

#include <algorithm>
#include "src/phonetic/translator.h"


using namespace std;

QHash<QString, QString> *Translator::vowelModifierMap=NULL;

QHash<QString, QString> *Translator::unicode2AsciiMap=NULL;
QHash<QString, QString> *Translator::unicode2AsciiMapExt=NULL;

QHash<QString, QString> *Translator::unicode2AsciiMatraLeftMap=NULL;
QHash<QString, QString> *Translator::ascii2UnicodeMatraLeftMap=NULL;
QHash<QString, QString> *Translator::unicode2AsciiMatraRightMap=NULL;
QHash<QString, QString> *Translator::ascii2UnicodeMatraRightMap=NULL;

QHash<QString, QString> *Translator::unicode2AsciiMatraRefMap=NULL;
QHash<QString, QString> *Translator::ascii2UnicodeMatraRefMap=NULL;
QHash<QString, QString> *Translator::ascii2UnicodeExoticMatraMap = NULL;
QHash<QString, QString> *Translator::unicode2AsciiExoticMatraMap=NULL;
QHash<QString, QString> *Translator::numericUnicode2AsciiMap;
QHash<QString, QString> *Translator::numericAscii2UnicodeMap;

QHash<QString, QString> *Translator::ascii2UnicodeMap=NULL;
QList<QKeyValuePair> *Translator::ascii2UnicodeList=NULL;
QList<QKeyValuePair> *Translator::unicode2AsciiList=NULL;

QHash<QString, QString> *Translator::romanAsciiMap = NULL;
QHash<QString, QLeftRightPair> *Translator::unicode2MatraCombMap =NULL;
QHash<QString, QLeftRightPair> *Translator::unicode2PrefixMatraCombMap =NULL;
QHash<QString, QLeftRightPair> *Translator::unicode2SuffixMatraCombMap =NULL;

Translator * Translator::translator =NULL;

bool compareQKeyValuePair(const QKeyValuePair  &s1, const QKeyValuePair  &s2){
     return s1.len >= s2.len;
 }
void Translator::initializeMatraCombinations() {

    if( unicode2MatraCombMap != NULL) { return; }

    unicode2MatraCombMap = new QHash<QString, QLeftRightPair>();


    unicode2MatraCombMap->insert("0x9be",QLeftRightPair("","0xb1"));

    unicode2MatraCombMap->insert("0x9bf",QLeftRightPair("0xbf",""));


    unicode2MatraCombMap->insert("0x9c0",QLeftRightPair("","0xcf"));

    unicode2MatraCombMap->insert("0x9c1",QLeftRightPair("","0xb3"));   //unicode2AsciiMap->insert("0x9c1","0xc5");

    unicode2MatraCombMap->insert("0x9c2",QLeftRightPair("","0xd3"));
    unicode2MatraCombMap->insert("0x9c3",QLeftRightPair("","0xd4"));

    unicode2MatraCombMap->insert("0x9c7",QLeftRightPair("0xcb",""));   //unicode2AsciiMap->insert("0x9c7","0xcb");
    unicode2MatraCombMap->insert("0x9c8",QLeftRightPair("0xcd",""));
    unicode2MatraCombMap->insert("0x9cb",QLeftRightPair("0xcb","0xb1"));

    /*
    unicode2MatraCombMap->insert("0x9cd0x9af","0xc9");
    */
    unicode2MatraCombMap->insert("0x9cc",QLeftRightPair("0xcb","0xcc"));


}


void Translator::initializePrefixMatraCombinations() {

    if( unicode2PrefixMatraCombMap != NULL) { return; }
    unicode2PrefixMatraCombMap = new QHash<QString, QLeftRightPair>();

    unicode2PrefixMatraCombMap->insert("0x9f00x9cd", QLeftRightPair("","0xc7"));

}

void Translator::initializeSuffixMatraCombinations() {

    if( unicode2SuffixMatraCombMap != NULL) { return; }
    unicode2SuffixMatraCombMap = new QHash<QString, QLeftRightPair>();

    unicode2SuffixMatraCombMap->insert("0x9cd0x9af",QLeftRightPair("","0xc9"));

}

void Translator::initializeAsciiToUnicodeMap() {
    if( ascii2UnicodeMap != NULL) { return; }
    ascii2UnicodeMap=  new QHash<QString,QString>();
    QHash<QString, QString>::const_iterator it;


    for(it = unicode2AsciiMap->begin(); it!=unicode2AsciiMap->end(); ++it) {
        ascii2UnicodeMap->insert(it.value(), it.key());
//        ascii2UnicodeList->append(QKeyValuePair(it.value(), it.value().size()));
    }
    ascii2UnicodeMap->insert("0xcb","0x9c7");
    ascii2UnicodeMap->insert("0xc6","0x9c8");
    ascii2UnicodeMap->insert("0xe5","0x99b");
    ascii2UnicodeMap->insert("0xc5","0x9c1");
    ascii2UnicodeMap->insert("0xe4","0x99a");
    ascii2UnicodeMap->insert("0xcb0xb1","0x9cb");
    ascii2UnicodeMap->insert("0x17d","0x9950x9cd0x9b7");
    ascii2UnicodeMap->insert("0xf08e","0x9950x9cd0x9b7");
    ascii2UnicodeMap->insert("0xcb0xcc","0x9cc");
    ascii2UnicodeMap->insert("0x310x2b","0x9f00x9c1");
    ascii2UnicodeMap->insert("0xa60xa8" ,"0x9b80x9cd0x995");

    ascii2UnicodeList = new QList<QKeyValuePair>();
    for(it = ascii2UnicodeMap->begin(); it!=ascii2UnicodeMap->end(); ++it) {
        //qDebug() << it.key();
        ascii2UnicodeList->append(QKeyValuePair(it.key(), it.key().size()));
    }
    qSort( ascii2UnicodeList->begin(), ascii2UnicodeList->end(), compareQKeyValuePair);



    unicode2AsciiList = new QList<QKeyValuePair>();
    for(it = unicode2AsciiMap->begin(); it!=unicode2AsciiMap->end(); ++it) {
     //   qDebug() << it.key();
        unicode2AsciiList->append(QKeyValuePair(it.key(), it.key().size()));
    }
    qSort( unicode2AsciiList->begin(), unicode2AsciiList->end(), compareQKeyValuePair);


  //  qDebug() << " here it is " << ascii2UnicodeMap->value("0x310x2b");


    QList<QKeyValuePair>::const_iterator itList;

  /*  for(itList = ascii2UnicodeList->begin(); itList!= ascii2UnicodeList->end(); ++itList, ++i) {
        qDebug() <<  i << "   " << itList->ascii << "   " << itList->len;

    }*/

    ascii2UnicodeMatraLeftMap= new QHash<QString, QString>();
    for(it = unicode2AsciiMatraLeftMap->begin(); it!=unicode2AsciiMatraLeftMap->end(); ++it) {
        ascii2UnicodeMatraLeftMap->insert(it.value(), it.key());
    }
    ascii2UnicodeMatraLeftMap->insert("0xcb","0x9c7");
    ascii2UnicodeMatraLeftMap->insert("0xc6","0x9c8");


    ascii2UnicodeMatraRightMap= new QHash<QString, QString>();
    for(it = unicode2AsciiMatraRightMap->begin(); it!=unicode2AsciiMatraRightMap->end(); ++it) {
        ascii2UnicodeMatraRightMap->insert(it.value(), it.key());
    }

    ascii2UnicodeMatraRefMap= new QHash<QString, QString>();
    for(it = unicode2AsciiMatraRefMap->begin(); it!=unicode2AsciiMatraRefMap->end(); ++it) {
        ascii2UnicodeMatraRefMap->insert(it.value(), it.key());
    }


    ascii2UnicodeExoticMatraMap = new QHash<QString, QString>();
    for(it = unicode2AsciiExoticMatraMap->begin(); it!=unicode2AsciiExoticMatraMap->end(); ++it) {
        ascii2UnicodeExoticMatraMap->insert(it.value(), it.key());
    }

    qDebug() << "Characters loaded " << ascii2UnicodeList->size();



    numericAscii2UnicodeMap = new QHash<QString, QString>();
    for( it = numericUnicode2AsciiMap->begin(); it != numericUnicode2AsciiMap->end(); ++it) {
        numericAscii2UnicodeMap->insert(it.value(), it.key());
    }


}

void Translator::initializeUnicodeToAsciiMap() {
    if( unicode2AsciiMap != NULL) { return; }
    unicode2AsciiMap = new QHash<QString,QString>();
    unicode2AsciiMatraLeftMap = new QHash<QString,QString>();
    unicode2AsciiMatraRightMap = new QHash<QString,QString>();
    unicode2AsciiMatraRefMap = new QHash<QString, QString>();

    unicode2AsciiExoticMatraMap = new QHash<QString, QString>();

    unicode2AsciiMap->insert("0x20","0x20");
    unicode2AsciiMap->insert("0x21","0x2f");


    unicode2AsciiMap->insert("0x985","0xd5");
    unicode2AsciiMap->insert("0x986","0xd50xb1");
    unicode2AsciiMap->insert("0x987","0xfd0xd7");
    unicode2AsciiMap->insert("0x988","0xd6");

    unicode2AsciiMap->insert("0x989","0xeb0xd7");
    unicode2AsciiMap->insert("0x98a","0xd8");
    unicode2AsciiMap->insert("0x98b","0xd90xd4");
    unicode2AsciiMap->insert("0x98b","0xd9");

    unicode2AsciiMap->insert("0x98f","0xdb");
    unicode2AsciiMap->insert("0x990","0xdc");
    unicode2AsciiMap->insert("0x993","0xdd");
    unicode2AsciiMap->insert("0x994","0xde");



    //ka kha
    unicode2AsciiMap->insert("0x995","0xdf");
    unicode2AsciiMap->insert("0x996","0xe0");
    unicode2AsciiMap->insert("0x997","0xe1");
    unicode2AsciiMap->insert("0x998","0xe2");
    unicode2AsciiMap->insert("0x999","0xe3");

    //cha chaa
    unicode2AsciiMap->insert("0x99a","0x32");  // unicode2AsciiMap->insert("0x99a","0xe4");
    unicode2AsciiMap->insert("0x99b","0x36");  // unicode2AsciiMap->insert("0x99b","0x35");
    unicode2AsciiMap->insert("0x99c","0xe6");
    unicode2AsciiMap->insert("0x99d","0xe7");
    unicode2AsciiMap->insert("0x99e","0xdb0x7");

    //ta tha da
    unicode2AsciiMap->insert("0x99f","0xe9");
    unicode2AsciiMap->insert("0x9a0","0xea");
    unicode2AsciiMap->insert("0x9a1","0xeb");
    unicode2AsciiMap->insert("0x9a2","0xec");
    unicode2AsciiMap->insert("0x9a3","0xed");

    //Ta Tha Da
    unicode2AsciiMap->insert("0x9a4","0xee");
    unicode2AsciiMap->insert("0x9a5","0xef");
    unicode2AsciiMap->insert("0x9a6","0xf0");
    unicode2AsciiMap->insert("0x9a7","0xf1");
    unicode2AsciiMap->insert("0x9a8","0xf2");

    // pa pha bo bho mo
    unicode2AsciiMap->insert("0x9aa","0xf3");
    unicode2AsciiMap->insert("0x9ab","0xf4");
    unicode2AsciiMap->insert("0x9ac","0xf5");
    unicode2AsciiMap->insert("0x9ad","0xf6");
    unicode2AsciiMap->insert("0x9ae","0xf7");

    // ja ra la wab
    unicode2AsciiMap->insert("0x9af","0xfb");
    unicode2AsciiMap->insert("0x9f0","0x31");
    unicode2AsciiMap->insert("0x9b2","0xf9");
      //unicode2AsciiMap->insert("0x9f1","0xf50xc4");
       unicode2AsciiMap->insert("0x9f1","0xbb");

    // xa xha dxha ha
    unicode2AsciiMap->insert("0x9b6","0xfa");
    unicode2AsciiMap->insert("0x9b7","0xf8");   //0xb8 removed
    unicode2AsciiMap->insert("0x9b8","0xfc");
    unicode2AsciiMap->insert("0x9b9","0xfd");

    unicode2AsciiMap->insert("0x9950x9cd0x9b7","0xf08e"); unicode2AsciiMap->insert("0x9950x9cd0x9b7","0x17d"); unicode2AsciiMap->insert("0x9950x9cd0x9b7","0xf08e");



    // ya r dhra dhra
    unicode2AsciiMap->insert("0x9950x9b7","0x17d");
    unicode2AsciiMap->insert("0x9df","0xfb0xfe");
    unicode2AsciiMap->insert("0x9dc","0xeb0xc20xff");
    unicode2AsciiMap->insert("0x9dd","0xec0xc20xff");



    // hasantiya bisorgo

    unicode2AsciiMap->insert("0x9ce","0xc8");
    unicode2AsciiMap->insert("0x981","0xd2");
    unicode2AsciiMap->insert("0x982","0xd1");
    unicode2AsciiMap->insert("0x983","0xd0");


    // all the kars/matra
    unicode2AsciiMap->insert("0x9be","0xb1");
    unicode2AsciiMap->insert("0x9bf","0xbf");
    unicode2AsciiMap->insert("0x9c0","0xcf");
    unicode2AsciiMap->insert("0x9c1","0xb3");   //unicode2AsciiMap->insert("0x9c1","0xc5");
    unicode2AsciiMap->insert("0x9c2","0xd3");
    unicode2AsciiMap->insert("0x9c3","0xd4");
    unicode2AsciiMap->insert("0x9c7","0xce");   //unicode2AsciiMap->insert("0x9c7","0xcb");
    unicode2AsciiMap->insert("0x9c8","0xcd");
    unicode2AsciiMap->insert("0x9cd0x9af","0xc9");
    unicode2AsciiMap->insert("0xcc","0xcc");
   // unicode2AsciiMap->insert("0x9af")


    unicode2AsciiMatraRightMap->insert("0x9be","0xb1");
    unicode2AsciiMatraLeftMap->insert("0x9bf","0xbf");
    unicode2AsciiMatraRightMap->insert("0x9c0","0xcf");
    unicode2AsciiMatraRightMap->insert("0x9c1","0xb3");
    unicode2AsciiMatraRightMap->insert("0x9c2","0xd3");
    unicode2AsciiMatraRightMap->insert("0x9c3","0xd4");
    unicode2AsciiMatraLeftMap->insert("0x9c7","0xce"); // unicode2AsciiMatraLeftMap->insert("0x9c7","0xcb");
    unicode2AsciiMatraLeftMap->insert("0x9c8","0xcd");
    unicode2AsciiMatraRightMap->insert("0xcc","0xcc");


    // ref and dirgha ref

    unicode2AsciiMap->insert("0x9f00x9cdM", "0xc7");
    unicode2AsciiMatraRefMap->insert("0x9f00x9cdM", "0xc7");
    unicode2AsciiMap->insert("0x9f00x9cdM0x9c0", "0xb9");
    unicode2AsciiMatraRefMap->insert("0x9f00x9cdM0x9c0", "0xb9");


    unicode2AsciiMap->insert("0x981","0xd2");
 //   unicode2AsciiExoticMatraMap->insert("0x9cd0x9af","0xc9");
    unicode2AsciiExoticMatraMap->insert("0x981","0xd2");

    unicode2AsciiMap->insert("0x9cb","0xce0xb1");   // unicode2AsciiMap->insert("0x9cb","0xcb0xb1"); unicode2AsciiMap->insert("0x9cb","0xcd");
    unicode2AsciiMap->insert("0x9cc", "0xce0xcc");  // unicode2AsciiMap->insert("0x9cc", "0xcb0xcc");

    unicode2AsciiMap->insert("0x964", "0xbc");


    //small symbols punctuation
    unicode2AsciiMap->insert("0x2d","0x2d");
    unicode2AsciiMap->insert("0x2e","0x2e");
    unicode2AsciiMap->insert("0x2f","0x2f");
    unicode2AsciiMap->insert("0x2c", "0x2c");
    unicode2AsciiMap->insert("0x2018", "0x2018");
    unicode2AsciiMap->insert("0x2019", "0x2019");
    unicode2AsciiMap->insert("0x2013", "0x2013");
    unicode2AsciiMap->insert("0x5b", "0x5b");
    unicode2AsciiMap->insert("0x5d", "0x5d");

    unicode2AsciiMap->insert("0x3b","0xca0x2c");
    unicode2AsciiMap->insert("0x3f", "0x2022");

    //jya kars

    // composite letters
    unicode2AsciiMap->insert("0x9950x9cd0x995","0x21");
    unicode2AsciiMap->insert("0x9950x9cd0x99f","0x22");
    unicode2AsciiMap->insert("0x9950x9cd0x9a8","0x23");
    unicode2AsciiMap->insert("0x9950x9cd0x9ac","0x24");

    unicode2AsciiMap->insert("0x9950x9cd0x9ae","0x25");
    unicode2AsciiMap->insert("0x9970x9c1","0x26");
    unicode2AsciiMap->insert("0x9950x9cd0x9b8","0x27");



    unicode2AsciiMap->insert("0x9b60x9cd0x99a","0x28");
    unicode2AsciiMap->insert("0x9970x9cd0x9aa","0x29");
    unicode2AsciiMap->insert("0x99a0x9cd0x99b0x9cd0x9f0","0x34");

    //composite chars
    unicode2AsciiMap->insert("0x9aa0x9cd0x9a4","0x35");
    unicode2AsciiMap->insert("0x99c0x9cd0x99c","0x37");
    unicode2AsciiMap->insert("0x99c0x9cd0x99c0x9cd0x9ac","0x38");
    unicode2AsciiMap->insert("0x99c0x9cd0x99c","0x39");
    unicode2AsciiMap->insert("0x99c0x9cd0x99e","0x3a");
    unicode2AsciiMap->insert("0x99c0x9cd0x9ac","0x3b");
    unicode2AsciiMap->insert("0x99c0x9cd0x9f0","0x3c");
    unicode2AsciiMap->insert("0x99e0x9cd0x99a","0x3d");
    unicode2AsciiMap->insert("0x99e0x9cd0x9a5","0x3e");
    unicode2AsciiMap->insert("0x99e0x9cd0x99c","0x3f");
    unicode2AsciiMap->insert("0x99e0x9cd0x99d","0x40");
    unicode2AsciiMap->insert("0x99f0x9cd0x9a4","0x41");
    unicode2AsciiMap->insert("0x99f0x9cd0x9ac","0x42");
    unicode2AsciiMap->insert("0x99f0x9cd0x9f0","0x43");
    unicode2AsciiMap->insert("0x9a10x9cd0x9a1","0x44");
    unicode2AsciiMap->insert("0x9a10x9cd0x9f0","0x45");
    unicode2AsciiMap->insert("0x9a30x9cd0x9a0","0x46");
    unicode2AsciiMap->insert("0x9a30x9cd0x9a1","0x47");
    unicode2AsciiMap->insert("0x9a30x9cd0x9a10x9cd0x9f0","0x48");
    unicode2AsciiMap->insert("0x9a30x9cd0x9a3","0x4A");

    unicode2AsciiMap->insert("0x9a40x9cd0x9a4","0x4d");
    unicode2AsciiMap->insert("0x9a40x9cd0x9ac","0x4e");
    unicode2AsciiMap->insert("0x9b20x9cd0x9ac","0x4f");
    unicode2AsciiMap->insert("0x9a40x9cd0x9a8","0x50");
    unicode2AsciiMap->insert("0x9a40x9cd0x9ac","0x51");
    unicode2AsciiMap->insert("0x9a40x9cd0x9ae","0x52");
    unicode2AsciiMap->insert("0x9a40x9cd0x9f0","0x53");

    unicode2AsciiMap->insert("0x9a50x9cd0x9ae","0x54");
    unicode2AsciiMap->insert("0x9b90x9c1","0x55");

    unicode2AsciiMap->insert("0x9a60x9cd0x9a6","0x56");
    unicode2AsciiMap->insert("0x9a60x9cd0x9a60x9cd0x9ac","0x57");

    unicode2AsciiMap->insert("0x9a60x9cd0x9a7","0x58");
    unicode2AsciiMap->insert("0x9a60x9cd0x9ac0x9cd0x9a7","0x59");
    unicode2AsciiMap->insert("0x9a60x9cd0x9ac","0x5a");
    unicode2AsciiMap->insert("0x9a60x9cd0x9ad0x9cd0x9f0","0x5c");
    unicode2AsciiMap->insert("0x9a60x9cd0x9f0","0x5e");

    unicode2AsciiMap->insert("0x9a80x9cd0x9a0","0x5f");
    unicode2AsciiMap->insert("0x9a80x9cd0x9a1","0x60");
    unicode2AsciiMap->insert("0x9a80x9cd0x9a40x9c1", "0x63");
    unicode2AsciiMap->insert("0x9b80x9cd0x9a40x9c1", "0x64");
    unicode2AsciiMap->insert("0x9a80x9cd0x9a60x9cd0x9f0","0x66");
    unicode2AsciiMap->insert("0x9b80x9cd0x9a4", "0x21220xa60xbb");
    unicode2AsciiMap->insert("0x9b60x9c1", "0x71");

    unicode2AsciiMap->insert("0x9ac0x9cd0x9a6", "0x73");
    unicode2AsciiMap->insert("0x9ac0x9cd0x9a7", "0x74");
     // bhra  pra
    unicode2AsciiMap->insert("0x9ad0x9cd0x9f0","0x77");
    unicode2AsciiMap->insert("0x9aa0x9cd0x9f0","0x78");
    unicode2AsciiMap->insert("0x9ae0x9cd0x9ad","0x79");

    unicode2AsciiMap->insert("0x9ae0x9cd0x9ad0x9cd0x9f0","0x7a");
    unicode2AsciiMap->insert("0x9b60x9cd0x9f0","0x7c");
    unicode2AsciiMap->insert("0x9b20x9cd0x9b2","0x7e");

    unicode2AsciiMap->insert("0x9b80x9cd0x9b2","0x80");
    unicode2AsciiMap->insert("0x9b90x9cd0x9a8","0x81");

    unicode2AsciiMap->insert("0x9a80x9cd0x9a6","0xb5");
    unicode2AsciiMap->insert("0x9a60x9cd0x9ad","0xbe");

    unicode2AsciiMap->insert("0x9b20x9cd0x9aa","0x160");

    unicode2AsciiMap->insert("0x9aa0x9cd0x9f0", "0x203a0xb6");
    unicode2AsciiMap->insert("0x9f00x9c1", "0x310x6e");   //0xb8 removed

     unicode2AsciiMap->insert("0x9950x9cd0x9a4", "0x4d0x90");
     unicode2AsciiMap->insert("0x9a80x9cd0x9a4", "0x21220x4c");
     unicode2AsciiMap->insert("0x9b60x9cd0x9f1", "0x9d0xab");

     unicode2AsciiMap->insert("0x9b60x9cd0x9a8", "0x9d0x178");
     unicode2AsciiMap->insert("0x9a80x9cd0x9a8","0xf9");

     unicode2AsciiMap->insert("0x989", "0xeb0xc20xd7");
     unicode2AsciiMap->insert("0x9a80x9cd0x9ae", "0x690x6a");
     unicode2AsciiMap->insert("0x9970x9cd0x9f0", "0xa20xb6");
     unicode2AsciiMap->insert("0x9b60x9cd0x9b2", "0x9d0xad");
     unicode2AsciiMap->insert("0x9b70x9cd0x99f" ,   "0xa90x2020");
     unicode2AsciiMap->insert("0x9b70x9cd0x99f0x9cd0x9f0" ,   "0xa90x20200xaa");

     unicode2AsciiMap->insert("0x9b80x9cd0x9f1", "0xa60xa4");

     unicode2AsciiMap->insert("0x9b90x9cd0x9b2", "0xaf");


     unicode2AsciiMap->insert("0x9ae0x9cd0x9aa", "0xa50xf3");

     unicode2AsciiMap->insert("0x9b80x9cd0x9ae", "0xa60x153");
     unicode2AsciiMap->insert("0x9a80x9cd0x9a5","0x4c0x161");
     unicode2AsciiMap->insert("0x9b70x9cd0x9a0", "0x20210xc2");
     unicode2AsciiMap->insert("0x9a80x9cd0x9a8", "0x690xa7");
     unicode2AsciiMap->insert("0x9a80x9cd0x9a7","0x670xc5");
     unicode2AsciiMap->insert("0x9b80x9cd0x9a5","0xa60x161");
     unicode2AsciiMap->insert("0x9ac0x9cd0x9f0","0xe80x70");
     unicode2AsciiMap->insert("0x9b80x9cd0x995","0xa60xa8" ); // unicode2AsciiMap->insert("0x9b80x9cd0x995","0xa60xa8" );
     unicode2AsciiMap->insert("0x9a80x9cd0x9a7","0x67");
  //   unicode2AsciiMap->insert("0x9a70x9cd0x9f00x9c1", "0xf10xe80x6e");  //0xb8 matra removed
     unicode2AsciiMap->insert("0x9cd0x9f0", "0xe8");
      unicode2AsciiMap->insert("0x9c1", "0x6e");
      unicode2AsciiMap->insert("0x9ae0x9cd0x9ac","0x690xa4");
      unicode2AsciiMap->insert("0x9b20x9cd0x9a1","0x2039");
      unicode2AsciiMap->insert("0x9b80x9cd0x99f","0x2c6");
      unicode2AsciiMap->insert("0x9b80x9cd0x9950x9c1","0xa60xc50xa8");
      unicode2AsciiMap->insert("0x9990x9cd0x995","0x201a");
      unicode2AsciiMap->insert("0x9b80x9cd0x995", "0xa90xa8");
      unicode2AsciiMap->insert("0x9970x9cd0x9a8", "0xa20x178");
      unicode2AsciiMap->insert("0x9b80x9cd0x9ab", "0xa60xa3");
      unicode2AsciiMap->insert("0x9b70x9cd0x9ae","0x192");
      unicode2AsciiMap->insert("0x9ac0x9cd0x9b2","0xf50x76");
      unicode2AsciiMap->insert("0x9a30x9cd0x99f", "0xb00x490xd7");
      unicode2AsciiMap->insert("0x9b90x9cd0x9f0", "0xfd0x7d");
      unicode2AsciiMap->insert("0x9b70x9cd0x9aa","0xa90xf3");
      unicode2AsciiMap->insert("0x9950x9cd0x9b2", "0xdf0x76");
      unicode2AsciiMap->insert("0x9cd", "0xc4");
      unicode2AsciiMap->insert("0x9ac0x9cd0x9ac", "0xf50x33");
      unicode2AsciiMap->insert("0x9aa0x9cd0x9b2","0x203a0xad");  // po + lo
      unicode2AsciiMap->insert("0x99e","0xdb0x17e");
      unicode2AsciiMap->insert("0x9b80x9cd0x9aa", "0xa60xf3");
      unicode2AsciiMap->insert("0x9b80x9cd0x9a4", "0x21220xa6");
      unicode2AsciiMap->insert("0x9a30x9cd0x99f", "0x4b0x490xd7");
      unicode2AsciiMap->insert("0x9ae0x9cd0x9ac", "0xa50xa4");
      unicode2AsciiMap->insert("0x9b20x9cd0x99f", "0x1520x490xd7");
      unicode2AsciiMap->insert("0x9970x9cd0x9b2", "0xa20xad");
      unicode2AsciiMap->insert("0x9b60x9cd0x9f00x9c0", "0xc0");
      unicode2AsciiMap->insert("0x9b90x9cd0x9ae", "0x70");
      unicode2AsciiMap->insert("0x99c0x9cd0x99c", "0x37");
      unicode2AsciiMap->insert("0x9a80x9cd0x9a40x9cd0x9f0", "0x4c0x61");

      unicode2AsciiMap->insert("0x9950x9cd0x9a4", "0x4d0x90");

      unicode2AsciiMap->insert("0x9950x9cd0x9f0", "0x530x90");

      unicode2AsciiMap->insert("0x9a80x9cd0x9b8", "0x6b");
      unicode2AsciiMap->insert("0x9a30x9cd0x99f", "0xb00x490xd7");
      unicode2AsciiMap->insert("0x9a80x9cd0x99f", "0x4b0x490xd7");

      unicode2AsciiMap->insert("0x9b70x9cd0x9a3", "0xf80x17e");
      unicode2AsciiMap->insert("0x9990x9cd0x997", "0x65");
      unicode2AsciiMap->insert("0x9b80x9cd0x9a8","0xa60xa7");

      unicode2AsciiMap->insert("0x9b90x9cd0x9a3","0xf081");
      unicode2AsciiMap->insert("0x9b90x9cd0x9a8","0xfd0x90");
      unicode2AsciiMap->insert("0x9ab0x9cd0x9b2", "0xf40x76");


     numericUnicode2AsciiMap = new QHash<QString, QString>();

     numericUnicode2AsciiMap->insert("0x9e6", "0x30");
     numericUnicode2AsciiMap->insert("0x9e7", "0x31");
     numericUnicode2AsciiMap->insert("0x9e8", "0x32");
     numericUnicode2AsciiMap->insert("0x9e9", "0x33");
     numericUnicode2AsciiMap->insert("0x9ea","0x34");
     numericUnicode2AsciiMap->insert("0x9eb", "0x35");
     numericUnicode2AsciiMap->insert("0x9ec","0x36");
     numericUnicode2AsciiMap->insert("0x9ed","0x37");
     numericUnicode2AsciiMap->insert("0x9ee","0x38");
     numericUnicode2AsciiMap->insert("0x9ef","0x39");

     numericUnicode2AsciiMap->insert("0x2c","0x2c");
   //  numericUnicode2AsciiMap->insert("0xfe","0xfe");
     numericUnicode2AsciiMap->insert("0xed","0xed");
     numericUnicode2AsciiMap->insert("0x2d","0x2d");
     numericUnicode2AsciiMap->insert("0x2e","0x2e");

//     numericUnicode2AsciiMap->insert("0xf7","0xf7");
  //   numericUnicode2AsciiMap->insert("0xe2","0xe2");
  //   numericUnicode2AsciiMap->insert("0xd0","0xd0");

    // numericUnicode2AsciiMap->insert("0xb7","0xb7");


}

void Translator::initializeUnicodeToAsciiMapExt() {
   if ( unicode2AsciiMapExt != NULL) { return; }

    unicode2AsciiMapExt = new QHash<QString, QString>();

    unicode2AsciiMapExt->insert("0x20","0x20");
    unicode2AsciiMapExt->insert("0x21","0x2f");


    unicode2AsciiMapExt->insert("0x985","0xd5");
    unicode2AsciiMapExt->insert("0x986","0xd50xb1");
    unicode2AsciiMapExt->insert("0x987","0xfd0xd7");
    unicode2AsciiMapExt->insert("0x988","0xd6");

    unicode2AsciiMapExt->insert("0x989","0xeb0xd70xc2");
    unicode2AsciiMapExt->insert("0x98a","0xd8");
    unicode2AsciiMapExt->insert("0x98b","0xd90xd4");
    unicode2AsciiMapExt->insert("0x98b","0xd9");

    unicode2AsciiMapExt->insert("0x98f","0xdb");
    unicode2AsciiMapExt->insert("0x990","0xdc");
    unicode2AsciiMapExt->insert("0x993","0xdd");
    unicode2AsciiMapExt->insert("0x994","0xde");



    //ka kha
    unicode2AsciiMapExt->insert("0x995","0xdf0xc2");
    unicode2AsciiMapExt->insert("0x996","0xe0");
    unicode2AsciiMapExt->insert("0x997","0xe1");
    unicode2AsciiMapExt->insert("0x998","0xe2");
    unicode2AsciiMapExt->insert("0x999","0xe3");

    //cha chaa
    unicode2AsciiMapExt->insert("0x99a","0x320xc2");  // unicode2AsciiMapExt->insert("0x99a","0xe4");
    unicode2AsciiMapExt->insert("0x99b","0x36");  // unicode2AsciiMapExt->insert("0x99b","0x35");
    unicode2AsciiMapExt->insert("0x99c","0xe6");
    unicode2AsciiMapExt->insert("0x99d","0xe7");
    unicode2AsciiMapExt->insert("0x99e","0xdb0x7");

    //ta tha da
    unicode2AsciiMapExt->insert("0x99f","0xe90xc2");
    unicode2AsciiMapExt->insert("0x9a0","0xea0xc2");
    unicode2AsciiMapExt->insert("0x9a1","0xeb0xc2");
    unicode2AsciiMapExt->insert("0x9a2","0xec0xc2");
    unicode2AsciiMapExt->insert("0x9a3","0xed");

    //Ta Tha Da
    unicode2AsciiMapExt->insert("0x9a4","0xee0xc2");
    unicode2AsciiMapExt->insert("0x9a5","0xef");
    unicode2AsciiMapExt->insert("0x9a6","0xf0");
    unicode2AsciiMapExt->insert("0x9a7","0xf1");
    unicode2AsciiMapExt->insert("0x9a8","0xf2");

    // pa pha bo bho mo
    unicode2AsciiMapExt->insert("0x9aa","0xc20xf3");
    unicode2AsciiMapExt->insert("0x9ab","0xf40xc2");
    unicode2AsciiMapExt->insert("0x9ac","0xc20xf5");
    unicode2AsciiMapExt->insert("0x9ad","0xf60xc2");
    unicode2AsciiMapExt->insert("0x9ae","0xf7");

    // ja ra la wab
    unicode2AsciiMapExt->insert("0x9af","0xfb");
    unicode2AsciiMapExt->insert("0x9f0","0x31");
    unicode2AsciiMapExt->insert("0x9b2","0xf9");
      //unicode2AsciiMapExt->insert("0x9f1","0xf50xc4");
    unicode2AsciiMapExt->insert("0x9f1","0xbb");

    // xa xha dxha ha
    unicode2AsciiMapExt->insert("0x9b6","0xfa");
    unicode2AsciiMapExt->insert("0x9b7","0xf8");   //0xb8 removed
    unicode2AsciiMapExt->insert("0x9b8","0xfc");
    unicode2AsciiMapExt->insert("0x9b9","0xfd");

    unicode2AsciiMapExt->insert("0x9950x9cd0x9b7","0xf08e"); unicode2AsciiMapExt->insert("0x9950x9cd0x9b7","0x17d"); unicode2AsciiMapExt->insert("0x9950x9cd0x9b7","0xf08e");



    // ya r dhra dhra
    unicode2AsciiMapExt->insert("0x9950x9b7","0x17d");
    unicode2AsciiMapExt->insert("0x9df","0xfb0xfe");
    unicode2AsciiMapExt->insert("0x9dc","0xeb0xc20xff");
    unicode2AsciiMapExt->insert("0x9dd","0xec0xc20xff");



    // hasantiya bisorgo

    unicode2AsciiMapExt->insert("0x9ce","0xc8");
    unicode2AsciiMapExt->insert("0x981","0xd2");
    unicode2AsciiMapExt->insert("0x982","0xd1");
    unicode2AsciiMapExt->insert("0x983","0xd0");


    // all the kars/matra
    unicode2AsciiMapExt->insert("0x9be","0xb1");
    unicode2AsciiMapExt->insert("0x9bf","0xbf");
    unicode2AsciiMapExt->insert("0x9c0","0xcf");
    unicode2AsciiMapExt->insert("0x9c1","0xb3");   //unicode2AsciiMapExt->insert("0x9c1","0xc5");
    unicode2AsciiMapExt->insert("0x9c2","0xd3");
    unicode2AsciiMapExt->insert("0x9c3","0xd4");
    unicode2AsciiMapExt->insert("0x9c7","0xce");   //unicode2AsciiMapExt->insert("0x9c7","0xcb");
    unicode2AsciiMapExt->insert("0x9c8","0xcd");
    unicode2AsciiMapExt->insert("0x9cd0x9af","0xc9");
    unicode2AsciiMapExt->insert("0xcc","0xcc");
   // unicode2AsciiMapExt->insert("0x9af")

    // ref and dirgha ref

  //  unicode2AsciiMapExt->insert("0x9f00x9cd", "0xc7");
    unicode2AsciiMapExt->insert("0x9f00x9cdM0x9c0", "0xb9");

    unicode2AsciiMapExt->insert("0x981","0xd2");

    unicode2AsciiMapExt->insert("0x9cb","0xce0xb1");   // unicode2AsciiMapExt->insert("0x9cb","0xcb0xb1"); unicode2AsciiMapExt->insert("0x9cb","0xcd");
    unicode2AsciiMapExt->insert("0x9cc", "0xce0xcc");  // unicode2AsciiMapExt->insert("0x9cc", "0xcb0xcc");

    unicode2AsciiMapExt->insert("0x964", "0xbc");


    //small symbols punctuation
    unicode2AsciiMapExt->insert("0x2d","0x2d");
    unicode2AsciiMapExt->insert("0x2e","0x2e");
    unicode2AsciiMapExt->insert("0x2f","0x2f");
    unicode2AsciiMapExt->insert("0x2c", "0x2c");
    unicode2AsciiMapExt->insert("0x2018", "0x2018");
    unicode2AsciiMapExt->insert("0x2019", "0x2019");
    unicode2AsciiMapExt->insert("0x2013", "0x2013");
    unicode2AsciiMapExt->insert("0x5b", "0x5b");
    unicode2AsciiMapExt->insert("0x5d", "0x5d");

    unicode2AsciiMapExt->insert("0x3b","0xca0x2c");
    unicode2AsciiMapExt->insert("0x3f", "0x2022");

    //jya kars

    // composite letters
    unicode2AsciiMapExt->insert("0x9950x9cd0x995","0x21");
    unicode2AsciiMapExt->insert("0x9950x9cd0x99f","0x22");
    unicode2AsciiMapExt->insert("0x9950x9cd0x9a8","0x23");
    unicode2AsciiMapExt->insert("0x9950x9cd0x9ac","0x24");

    unicode2AsciiMapExt->insert("0x9950x9cd0x9ae","0x25");
    unicode2AsciiMapExt->insert("0x9970x9c1","0x26");
    unicode2AsciiMapExt->insert("0x9950x9cd0x9b8","0x27");



    unicode2AsciiMapExt->insert("0x9b60x9cd0x99a","0x28");
    unicode2AsciiMapExt->insert("0x9970x9cd0x9aa","0x29");
    unicode2AsciiMapExt->insert("0x99a0x9cd0x99b0x9cd0x9f0","0x34");

    //composite chars
    unicode2AsciiMapExt->insert("0x9aa0x9cd0x9a4","0x35");
    unicode2AsciiMapExt->insert("0x99c0x9cd0x99c","0x37");
    unicode2AsciiMapExt->insert("0x99c0x9cd0x99c0x9cd0x9ac","0x38");
    unicode2AsciiMapExt->insert("0x99c0x9cd0x99c","0x39");
    unicode2AsciiMapExt->insert("0x99c0x9cd0x99e","0x3a");
    unicode2AsciiMapExt->insert("0x99c0x9cd0x9ac","0x3b");
    unicode2AsciiMapExt->insert("0x99c0x9cd0x9f0","0x3c");
    unicode2AsciiMapExt->insert("0x99e0x9cd0x99a","0x3d");
    unicode2AsciiMapExt->insert("0x99e0x9cd0x9a5","0x3e");
    unicode2AsciiMapExt->insert("0x99e0x9cd0x99c","0x3f");
    unicode2AsciiMapExt->insert("0x99e0x9cd0x99d","0x40");
    unicode2AsciiMapExt->insert("0x99f0x9cd0x9a4","0x41");
    unicode2AsciiMapExt->insert("0x99f0x9cd0x9ac","0x42");
    unicode2AsciiMapExt->insert("0x99f0x9cd0x9f0","0x43");
    unicode2AsciiMapExt->insert("0x9a10x9cd0x9a1","0x44");
    unicode2AsciiMapExt->insert("0x9a10x9cd0x9f0","0x45");
    unicode2AsciiMapExt->insert("0x9a30x9cd0x9a0","0x46");
    unicode2AsciiMapExt->insert("0x9a30x9cd0x9a1","0x47");
    unicode2AsciiMapExt->insert("0x9a30x9cd0x9a10x9cd0x9f0","0x48");
    unicode2AsciiMapExt->insert("0x9a30x9cd0x9a3","0x4A");

    unicode2AsciiMapExt->insert("0x9a40x9cd0x9a4","0x4d");
    unicode2AsciiMapExt->insert("0x9a40x9cd0x9ac","0x4e");
    unicode2AsciiMapExt->insert("0x9b20x9cd0x9ac","0x4f");
    unicode2AsciiMapExt->insert("0x9a40x9cd0x9a8","0x50");
    unicode2AsciiMapExt->insert("0x9a40x9cd0x9ac","0x51");
    unicode2AsciiMapExt->insert("0x9a40x9cd0x9ae","0x52");
    unicode2AsciiMapExt->insert("0x9a40x9cd0x9f0","0x53");

    unicode2AsciiMapExt->insert("0x9a50x9cd0x9ae","0x54");
    unicode2AsciiMapExt->insert("0x9b90x9c1","0x55");

    unicode2AsciiMapExt->insert("0x9a60x9cd0x9a6","0x56");
    unicode2AsciiMapExt->insert("0x9a60x9cd0x9a60x9cd0x9ac","0x57");

    unicode2AsciiMapExt->insert("0x9a60x9cd0x9a7","0x58");
    unicode2AsciiMapExt->insert("0x9a60x9cd0x9ac0x9cd0x9a7","0x59");
    unicode2AsciiMapExt->insert("0x9a60x9cd0x9ac","0x5a");
    unicode2AsciiMapExt->insert("0x9a60x9cd0x9ad0x9cd0x9f0","0x5c");
    unicode2AsciiMapExt->insert("0x9a60x9cd0x9f0","0x5e");

    unicode2AsciiMapExt->insert("0x9a80x9cd0x9a0","0x5f");
    unicode2AsciiMapExt->insert("0x9a80x9cd0x9a1","0x60");
    unicode2AsciiMapExt->insert("0x9a80x9cd0x9a40x9c1", "0x63");
    unicode2AsciiMapExt->insert("0x9b80x9cd0x9a40x9c1", "0x64");
    unicode2AsciiMapExt->insert("0x9a80x9cd0x9a60x9cd0x9f0","0x66");
    unicode2AsciiMapExt->insert("0x9b80x9cd0x9a4", "0x21220xa60xbb");
    unicode2AsciiMapExt->insert("0x9b60x9c1", "0x71");

    unicode2AsciiMapExt->insert("0x9ac0x9cd0x9a6", "0x73");
    unicode2AsciiMapExt->insert("0x9ac0x9cd0x9a7", "0x74");
     // bhra  pra
    unicode2AsciiMapExt->insert("0x9ad0x9cd0x9f0","0x77");
    unicode2AsciiMapExt->insert("0x9aa0x9cd0x9f0","0x78");
    unicode2AsciiMapExt->insert("0x9ae0x9cd0x9ad","0x79");

    unicode2AsciiMapExt->insert("0x9ae0x9cd0x9ad0x9cd0x9f0","0x7a");
    unicode2AsciiMapExt->insert("0x9b60x9cd0x9f0","0x7c");
    unicode2AsciiMapExt->insert("0x9b20x9cd0x9b2","0x7e");

    unicode2AsciiMapExt->insert("0x9b80x9cd0x9b2","0x80");
    unicode2AsciiMapExt->insert("0x9b90x9cd0x9a8","0x81");

    unicode2AsciiMapExt->insert("0x9a80x9cd0x9a6","0xb5");
    unicode2AsciiMapExt->insert("0x9a60x9cd0x9ad","0xbe");

    unicode2AsciiMapExt->insert("0x9b20x9cd0x9aa","0x160");

    unicode2AsciiMapExt->insert("0x9aa0x9cd0x9f0", "0x203a0xb6");
    unicode2AsciiMapExt->insert("0x9f00x9c1", "0x310x6e");   //0xb8 removed

     unicode2AsciiMapExt->insert("0x9950x9cd0x9a4", "0x4d0x90");
     unicode2AsciiMapExt->insert("0x9a80x9cd0x9a4", "0x21220x4c");
     unicode2AsciiMapExt->insert("0x9b60x9cd0x9f1", "0x9d0xab");

     unicode2AsciiMapExt->insert("0x9b60x9cd0x9a8", "0x9d0x178");
     unicode2AsciiMapExt->insert("0x9a80x9cd0x9a8","0xf9");

     unicode2AsciiMapExt->insert("0x989", "0xeb0xc20xd7");
     unicode2AsciiMapExt->insert("0x9a80x9cd0x9ae", "0x690x6a");
     unicode2AsciiMapExt->insert("0x9970x9cd0x9f0", "0xa20xb6");
     unicode2AsciiMapExt->insert("0x9b60x9cd0x9b2", "0x9d0xad");
     unicode2AsciiMapExt->insert("0x9b70x9cd0x99f" ,   "0xa90x2020");
     unicode2AsciiMapExt->insert("0x9b70x9cd0x99f0x9cd0x9f0" ,   "0xa90x20200xaa");

     unicode2AsciiMapExt->insert("0x9b80x9cd0x9f1", "0xa60xa4");

     unicode2AsciiMapExt->insert("0x9b90x9cd0x9b2", "0xaf");


     unicode2AsciiMapExt->insert("0x9ae0x9cd0x9aa", "0xa50xf3");

     unicode2AsciiMapExt->insert("0x9b80x9cd0x9ae", "0xa60x153");
     unicode2AsciiMapExt->insert("0x9a80x9cd0x9a5","0x4c0x161");
     unicode2AsciiMapExt->insert("0x9b70x9cd0x9a0", "0x20210xc2");
     unicode2AsciiMapExt->insert("0x9a80x9cd0x9a8", "0x690xa7");
     unicode2AsciiMapExt->insert("0x9a80x9cd0x9a7","0x670xc5");
     unicode2AsciiMapExt->insert("0x9b80x9cd0x9a5","0xa60x161");
     unicode2AsciiMapExt->insert("0x9ac0x9cd0x9f0","0xf50x70");
     unicode2AsciiMapExt->insert("0x9b80x9cd0x995","0xa60xa8" ); // unicode2AsciiMapExt->insert("0x9b80x9cd0x995","0xa60xa8" );
     unicode2AsciiMapExt->insert("0x9a80x9cd0x9a7","0x67");
  //   unicode2AsciiMapExt->insert("0x9a70x9cd0x9f00x9c1", "0xf10xe80x6e");  //0xb8 matra removed
     unicode2AsciiMapExt->insert("0x9cd0x9f0", "0xe8");
      unicode2AsciiMapExt->insert("0x9c1", "0x6e");
      unicode2AsciiMapExt->insert("0x9ae0x9cd0x9ac","0x690xa4");
      unicode2AsciiMapExt->insert("0x9b20x9cd0x9a1","0x2039");
      unicode2AsciiMapExt->insert("0x9b80x9cd0x99f","0x2c6");
      unicode2AsciiMapExt->insert("0x9b80x9cd0x9950x9c1","0xa60xc50xa8");
      unicode2AsciiMapExt->insert("0x9990x9cd0x995","0x201a");
      unicode2AsciiMapExt->insert("0x9b80x9cd0x995", "0xa90xa8");
      unicode2AsciiMapExt->insert("0x9970x9cd0x9a8", "0xa20x178");
      unicode2AsciiMapExt->insert("0x9b80x9cd0x9ab", "0xa60xa3");
      unicode2AsciiMapExt->insert("0x9b70x9cd0x9ae","0x192");
      unicode2AsciiMapExt->insert("0x9ac0x9cd0x9b2","0xf50x76");
      unicode2AsciiMapExt->insert("0x9a30x9cd0x99f", "0xb00x490xd7");
      unicode2AsciiMapExt->insert("0x9b90x9cd0x9f0", "0xfd0x7d");
      unicode2AsciiMapExt->insert("0x9b70x9cd0x9aa","0xa90xf3");
      unicode2AsciiMapExt->insert("0x9950x9cd0x9b2", "0xdf0x76");
      unicode2AsciiMapExt->insert("0x9cd", "0xc4");
      unicode2AsciiMapExt->insert("0x9ac0x9cd0x9ac", "0xf50x33");
      unicode2AsciiMapExt->insert("0x9aa0x9cd0x9b2","0x203a0xad");  // po + lo
      unicode2AsciiMapExt->insert("0x99e","0xdb0x17e");
      unicode2AsciiMapExt->insert("0x9b80x9cd0x9aa", "0xa60xf3");
      unicode2AsciiMapExt->insert("0x9b80x9cd0x9a4", "0x21220xa6");
      unicode2AsciiMapExt->insert("0x9a30x9cd0x99f", "0x4b0x490xd7");
      unicode2AsciiMapExt->insert("0x9ae0x9cd0x9ac", "0xa50xa4");
      unicode2AsciiMapExt->insert("0x9b20x9cd0x99f", "0x1520x490xd7");
      unicode2AsciiMapExt->insert("0x9970x9cd0x9b2", "0xa20xad");
      unicode2AsciiMapExt->insert("0x9b60x9cd0x9f00x9c0", "0xc0");
      unicode2AsciiMapExt->insert("0x9b90x9cd0x9ae", "0x70");
      unicode2AsciiMapExt->insert("0x99c0x9cd0x99c", "0x37");
      unicode2AsciiMapExt->insert("0x9a80x9cd0x9a40x9cd0x9f0", "0x4c0x61");

      unicode2AsciiMapExt->insert("0x9950x9cd0x9a4", "0x4d0x90");

      unicode2AsciiMapExt->insert("0x9950x9cd0x9f0", "0x530x90");

      unicode2AsciiMapExt->insert("0x9a80x9cd0x9b8", "0x6b");
      unicode2AsciiMapExt->insert("0x9a30x9cd0x99f", "0xb00x490xd7");
      unicode2AsciiMapExt->insert("0x9a80x9cd0x99f", "0x4b0x490xd7");

      unicode2AsciiMapExt->insert("0x9b70x9cd0x9a3", "0xf80x17e");
      unicode2AsciiMapExt->insert("0x9990x9cd0x997", "0x65");
      unicode2AsciiMapExt->insert("0x9b80x9cd0x9a8","0xa60xa7");

      unicode2AsciiMapExt->insert("0x9b90x9cd0x9a3","0xf081");
      unicode2AsciiMapExt->insert("0x9b90x9cd0x9a8","0xfd0x90");
      unicode2AsciiMapExt->insert("0x9ab0x9cd0x9b2", "0xf40x76");


}


void Translator::initializeRomanAsciiMap() {
    romanAsciiMap =new QHash<QString, QString>();

      romanAsciiMap->insert("0x27","0x27");

    romanAsciiMap->insert("0x41","0x41");
    romanAsciiMap->insert("0x42","0x42");
    romanAsciiMap->insert("0x43","0x43");
    romanAsciiMap->insert("0x44","0x44");
    romanAsciiMap->insert("0x45","0x45");
    romanAsciiMap->insert("0x46","0x46");
    romanAsciiMap->insert("0x47","0x47");
    romanAsciiMap->insert("0x48","0x48");
    romanAsciiMap->insert("0x49","0x49");
    romanAsciiMap->insert("0x4a","0x4a");
    romanAsciiMap->insert("0x4b","0x4b");
    romanAsciiMap->insert("0x4c","0x4c");
    romanAsciiMap->insert("0x4d","0x4d");
    romanAsciiMap->insert("0x4e","0x4e");
    romanAsciiMap->insert("0x4f","0x4f");

    romanAsciiMap->insert("0x50","0x50");
    romanAsciiMap->insert("0x51","0x51");
    romanAsciiMap->insert("0x52","0x52");
    romanAsciiMap->insert("0x53","0x53");
    romanAsciiMap->insert("0x54","0x54");
    romanAsciiMap->insert("0x55","0x55");
    romanAsciiMap->insert("0x56","0x56");
    romanAsciiMap->insert("0x57","0x57");
    romanAsciiMap->insert("0x58","0x58");
    romanAsciiMap->insert("0x59","0x59");
    romanAsciiMap->insert("0x5a","0x5a");


    romanAsciiMap->insert("0x61","0x61");
    romanAsciiMap->insert("0x62","0x62");
    romanAsciiMap->insert("0x63","0x63");
    romanAsciiMap->insert("0x64","0x64");
    romanAsciiMap->insert("0x65","0x65");
    romanAsciiMap->insert("0x66","0x66");
    romanAsciiMap->insert("0x67","0x67");
    romanAsciiMap->insert("0x68","0x68");
    romanAsciiMap->insert("0x69","0x69");
    romanAsciiMap->insert("0x6a","0x6a");
    romanAsciiMap->insert("0x6b","0x6b");
    romanAsciiMap->insert("0x6c","0x6c");
    romanAsciiMap->insert("0x6d","0x6d");
    romanAsciiMap->insert("0x6e","0x6e");
    romanAsciiMap->insert("0x6f","0x6f");

    romanAsciiMap->insert("0x70","0x70");
    romanAsciiMap->insert("0x71","0x71");
    romanAsciiMap->insert("0x72","0x72");
    romanAsciiMap->insert("0x73","0x73");
    romanAsciiMap->insert("0x74","0x74");
    romanAsciiMap->insert("0x75","0x75");
    romanAsciiMap->insert("0x76","0x76");
    romanAsciiMap->insert("0x77","0x77");
    romanAsciiMap->insert("0x78","0x78");
    romanAsciiMap->insert("0x79","0x79");
    romanAsciiMap->insert("0x7a","0x7a");

    romanAsciiMap->insert("0x7b","0x7b");
    romanAsciiMap->insert("0x7c","0x7c");
    romanAsciiMap->insert("0x7d","0x7d");
    romanAsciiMap->insert("0x7e","0x7e");
    romanAsciiMap->insert("0x7f","0x7f");

    romanAsciiMap->insert("0xb7","0xb7");
    romanAsciiMap->insert("0x2013","0x2013");
    romanAsciiMap->insert("0x2014","0x2014");
    romanAsciiMap->insert("0x201c","0x201c");
    romanAsciiMap->insert("0x201d","0x201d");
    romanAsciiMap->insert("0x2018","0x2018");
    romanAsciiMap->insert("0x2019","0x2019");

    romanAsciiMap->insert("0xff","0xff");
    romanAsciiMap->insert("0xeb","0xeb");
    romanAsciiMap->insert("0x2c","0x2c");
    romanAsciiMap->insert("0x2f","0x2f");
    romanAsciiMap->insert("0x3a","0x3a");
    romanAsciiMap->insert("0x5b","0x5b");
    romanAsciiMap->insert("0x5d","0x5d");

    romanAsciiMap->insert("0xe0","0xe0");
    romanAsciiMap->insert("0xe2","0xe2");
    romanAsciiMap->insert("0xe4","0xe4");
    romanAsciiMap->insert("0xe5","0xe5");

    romanAsciiMap->insert("0xf2","0xf2");
    romanAsciiMap->insert("0xfb","0xfb");
    romanAsciiMap->insert("0xe4","0xe4");
    romanAsciiMap->insert("0xe5","0xe5");

    romanAsciiMap->insert("0f75","0xf7");
    romanAsciiMap->insert("0xd4","0xd4");
    romanAsciiMap->insert("0xd9","0xd9");


    romanAsciiMap->insert("0xfe","0xfe");
   // romanAsciiMap->insert("0xbc","0xbc");
    romanAsciiMap->insert("0x2d","0x2d");

    romanAsciiMap->insert("0x2e","0x2e");
    romanAsciiMap->insert("0xbc", "0x964");
   // romanAsciiMap->insert( "0x964", "0xbc");
}

/*
void Translator::initializeAssameseCharMap() {
     if( assameseCharMap != NULL) return;
     assameseCharMap = new QHash<QString,QString>();
     assameseCharMap->insert("0x995","0x995");
     assameseCharMap->insert("0x996","0x996");
     assameseCharMap->insert("0x997","0x997");
     assameseCharMap->insert("0x998","0x998");
     assameseCharMap->insert("0x999","0x999");
     assameseCharMap->insert("0x99a","0x99a");
     assameseCharMap->insert("0x99b","0x99b");
     assameseCharMap->insert("0x99c","0x99c");
     assameseCharMap->insert("0x99d","0x99d");
     assameseCharMap->insert("0x99e","0x99e");
     assameseCharMap->insert("0x99f","0x99f");
     assameseCharMap->insert("0x9a0","0x9a0");
     assameseCharMap->insert("0x9a1","0x9a1");
     assameseCharMap->insert("0x9a2","0x9a2");
     assameseCharMap->insert("0x9a3","0x9a3");
     assameseCharMap->insert("0x9a4","0x9a4");
     assameseCharMap->insert("0x9a5","0x9a5");
     assameseCharMap->insert("0x9a6","0x9a6");
     assameseCharMap->insert("0x9a7","0x9a7");
     assameseCharMap->insert("0x9a8","0x9a8");
     assameseCharMap->insert("0x9aa","0x9aa");
     assameseCharMap->insert("0x9ab","0x9ab");
     assameseCharMap->insert("0x9ac","0x9ac");
     assameseCharMap->insert("0x9ad","0x9ad");
     assameseCharMap->insert("0x9ae","0x9ae");
     assameseCharMap->insert("0x9af","0x9af");
     assameseCharMap->insert("0x9f0","0x9f0");
     assameseCharMap->insert("0x9b2","0x9b2");
     assameseCharMap->insert("0x9f1","0x9f1");
     assameseCharMap->insert("0x9b6","0x9b6");
     assameseCharMap->insert("0x9b7","0x9b7");
     assameseCharMap->insert("0x9b8","0x9b8");
     assameseCharMap->insert("0x9b9","0x9b9");
     assameseCharMap->insert("0x9df","0x9df");
     assameseCharMap->insert("0x9dc","0x9dc");
     assameseCharMap->insert("0x9dd","0x9dd");
     assameseCharMap->insert("0x9ce","0x9ce");
     assameseCharMap->insert("0x982","0x982");
     assameseCharMap->insert("0x983","0x983");
     assameseCharMap->insert("0x985","0x985");
     assameseCharMap->insert("0x986","0x986");
     assameseCharMap->insert("0x987","0x987");
     assameseCharMap->insert("0x988","0x988");
     assameseCharMap->insert("0x989","0x989");
     assameseCharMap->insert("0x98a","0x98a");
     assameseCharMap->insert("0x98b","0x98b");
     assameseCharMap->insert("0x98f","0x98f");
     assameseCharMap->insert("0x990","0x990");
     assameseCharMap->insert("0x993","0x993");
     assameseCharMap->insert("0x994","0x994");
     assameseCharMap->insert("0x9be","0x9be");
     assameseCharMap->insert("0x9bf","0x9bf");
     assameseCharMap->insert("0x9c0","0x9c0");
     assameseCharMap->insert("0x9c1","0x9c1");
     assameseCharMap->insert("0x9c2","0x9c2");
     assameseCharMap->insert("0x9c3","0x9c3");
     assameseCharMap->insert("0x9c7","0x9c7");
     assameseCharMap->insert("0x9c8","0x9c8");
     assameseCharMap->insert("0x9cb","0x9cb");
     assameseCharMap->insert("0x9cc","0x9cc");
     assameseCharMap->insert("0x9cd","0x9cd");

     assameseCharMap->insert("0x19","0x19");
     assameseCharMap->insert("0x27","0x27");


     // punctuation characters
     assameseCharMap->insert("0x2010","0x2010");  // hyphen
     assameseCharMap->insert("0x2011","0x2011");
     assameseCharMap->insert("0x2019","0x2019");  // right quotation mark
     assameseCharMap->insert("0x2d","0x2d");
     assameseCharMap->insert("0x2e","0x2e");
     assameseCharMap->insert("0x2f","0x2f");
}

*/

void Translator::initializeVowelModifierMap() {

  if( vowelModifierMap != NULL) return;

  vowelModifierMap = new QHash<QString,QString>();
  vowelModifierMap->insert( "0x9be", "0x986");

  vowelModifierMap->insert( "0x9bf", "0x987");
  vowelModifierMap->insert( "0x9c0", "0x988");
  vowelModifierMap->insert( "0x9c1", "0x989");

  vowelModifierMap->insert( "0x9c2", "0x98A");
  vowelModifierMap->insert( "0x9c3", "0x98B");

  vowelModifierMap->insert( "0x9c7", "0x98F");
  vowelModifierMap->insert( "0x9c8", "0x990");

  vowelModifierMap->insert( "0x9cb", "0x993");
  vowelModifierMap->insert( "0x9cc", "0x994");

}


Translator *Translator::getTranslator() {
    if( translator != NULL ) return translator;
    translator = new Translator();
    return translator;
}

Translator::Translator(){

    Translator::initializeUnicodeToAsciiMap();
    Translator::initializeUnicodeToAsciiMapExt();
    Translator::initializeAsciiToUnicodeMap();
    Translator::initializeVowelModifierMap();
    Translator::intializeAsciiTree();
 //   AsciiTrie *unicodeTree = AsciiTrie::getUnicodeTrie();
  //  unicodeTree->printData();

    Translator::initializeMatraCombinations();
    Translator::initializePrefixMatraCombinations();
    Translator::initializeSuffixMatraCombinations();
    Translator::intializeUnicodeTree();
    Translator::initializeRomanAsciiMap() ;

}

QString Translator::reverse(QString str) {

    if( str.size() <= 0) { return (QString("")); }

    QByteArray ba = str.toLatin1();   // str.toAscii();


    char *d = ba.data();
    std::reverse(d, d+str.length());
    str = QString(d);
    return str;

}

 QList<QString> Translator::split(QString str, QString delim) {

    QList<QString> str_list;

    QStringList list ;

    QStringList::const_iterator it_list;

    if( delim.size() == 0) {
       for( int i =0; i < str.size(); i++) {
           str_list.append(QString( str[i]));
       }
       qDebug() << "split " << str_list.size();
       return str_list;
    }

    list = str.split(delim);
    for(it_list = list.constBegin(); it_list != list.constEnd(); ++it_list ) {
      if( QString::compare(QString(""), *it_list)!=0) {
          str_list.append(*it_list);
      }
    }
    return str_list;

}


QString Translator::createStringFromCodeList( QList<QString> list) {

    QString text("");
    bool ok;

    QList<QString>::const_iterator it_list;
    for(it_list = list.constBegin(); it_list != list.constEnd(); ++it_list ) {
        text += QChar(it_list->toUInt(&ok, 16));
    }
   // qDebug() << "List size " << list.size();
    return text;

};



QStringList  Translator::getStringListFromHexString(QString word, QString delim, int len) {
    QStringList firstList = word.split(delim);
    QStringList resultList ;

    // currently we are not using the len
    foreach(QString code, firstList) {
        if( code.trimmed().length() !=0 ) {
     //       qDebug() << code.trimmed().length() << "<<" <<  code.trimmed()<< ">>";
           resultList.append( code.trimmed());
        }
    }

    return resultList;

}


QStringList  Translator::getStringListFromUnicodeString(QString unicodeString) {
    QStringList stringlist;

    foreach(QChar str, unicodeString) {
        stringlist.append( Translator::convertToHexString( str.unicode()));
     //   qDebug() << "Code : " << Translator::convertToHexString( str.unicode()) ;
    }
    return stringlist;


}

QString Translator::createStringFromCodeList( QList<QString> list, int base) {

    QString text("");
    bool ok;

    QList<QString>::const_iterator it_list;
    for(it_list = list.constBegin(); it_list != list.constEnd(); ++it_list ) {
        text += QChar(it_list->toUInt(&ok, base));
    }

    return text;

};



QString Translator::getCode(QString line, QString delim) {

     QString code = "";

     foreach( QChar c, line) {
          code = code + delim + QString::number(c.unicode(), 16);


                  //QString("%1").arg(c.unicode(16));
     }
     return code;
}


QString Translator::getUnicode(QString str, QString delim){

  //  qDebug() << "Getting unicode";
    return Translator::createStringFromCodeList(Translator::split(str, delim));

}


QString Translator::getUnicodeString(QString str) {
    QChar qchar;
    QString unicodeString="";

    if(str.size() <=0) return unicodeString;

    for( int i =0 ; i < str.size(); i++) {
         qchar = str.at(i);
         if( (2433 <= qchar.unicode() && qchar.unicode() <= 2554) ||
              (qchar.unicode() == 0x27 ) ||
              (qchar.unicode() == 0x19 ) ||
              (qchar.unicode() == 0x2010 ) ||
              (qchar.unicode() == 0x2011 ) ||
              (qchar.unicode() == 0x2019 ) ||
              (qchar.unicode() == 0x2d )

           ) {
             unicodeString = unicodeString + "0x" + Translator::convertToHexString(qchar.unicode()).toLower();
         }
    }
    return unicodeString;

}



QString Translator::convertToHexString(ushort value) {
    QString str;

    int remainder;

    if(value== 0)  return( QString("0") );

    while( value > 0 ) {
        remainder = value % 16;
        value = (ushort)value/16;

        switch(remainder) {
            case 0:
                    str.append("0");
                    break;
            case 1:
                    str.append("1");
                    break;
            case 2:
                    str.append("2");
                    break;
            case 3:
                    str.append("3");
                    break;
            case 4:
                    str.append("4");
                    break;
            case 5:
                    str.append("5");
                    break;
            case 6:
                    str.append("6");
                    break;
            case 7:
                    str.append("7");
                    break;
            case 8:
                    str.append("8");
                    break;
            case 9:
                    str.append("9");
                    break;
            case 10:
                    str.append("a");
                    break;
            case 11:
                    str.append("b");
                    break;
            case 12:
                    str.append("c");
                    break;
            case 13:
                    str.append("d");
                    break;
            case 14:
                    str.append("e");
                    break;
            case 15:
                    str.append("f");
                    break;
            default:
                    break;

        }

    }

    return reverse(str);
}



void Translator::createUnicodeWords(QString string, QList<QString> &words ) {

    QList<QString> splitLines =  Translator::split(string,"\n");

    bool prevSpace = true;
    QString tempWord;
    foreach( QString line, splitLines) {
        QString word="";
        prevSpace = true;
        foreach( QChar c, line) {

            //qDebug() << Translator::getCode(QString(c),"0x");
            if( Translator::getCode(QString(c),"0x") == QString("0x20") ) {
             //   qDebug() << word;
                tempWord = word;
                if( tempWord.remove(QRegExp("0x20")).size() > 0)words.append(word);
                word.clear();
                prevSpace = true;

            }
            else if( Translator::getCode(QString(c),"0x") != QString("0x20") && prevSpace ==true ) {
             //   qDebug() << word;
                tempWord = word;
                if( tempWord.remove(QRegExp("0x20")).size() > 0)words.append(word);
                word.clear();
                prevSpace = false;
            }
            word.append(Translator::getCode(QString(c),"0x"));
        }
        words.append(word);
        words.append( QString("0xghij") );
    }


}

void Translator::createAsciiWords(QString inputString, QList<QString> &words) {
    QString word="";
    QList<QString> splitLines =  Translator::split(inputString,"\n");
    bool prevSpace = true;
    QString tempWord;
    foreach( QString line, splitLines) {
        QString word="";

        prevSpace = true;
        foreach( QChar c, line) {
            if( Translator::getCode(QString(c),"0x") == QString("0x20") ) {
         //   qDebug() << word;
               tempWord = word;
               if( tempWord.remove(QRegExp("0x20")).size() > 0) words.append(word);
               word.clear();
               prevSpace = true;

            }
            else if( Translator::getCode(QString(c),"0x") != QString("0x20") && prevSpace ==true ) {
         //   qDebug() << word;
                tempWord = word;
                if( tempWord.remove(QRegExp("0x20")).size() > 0 ) words.append(word);
                word.clear();
                prevSpace = false;
            }
            word.append(Translator::getCode(QString(c),"0x"));
         }
        words.append(word);
        words.append( QString("0xghij") );
    }
    return;

}

QString Translator::readGeetanjaliFile(QString fileName) {

  QFile file(fileName);
  if(!file.open(QIODevice::ReadOnly)) {
          QMessageBox::information(0, "error", file.errorString());
  }

  QTextStream in(&file);

  QString line;
  QString output="";


  while(!in.atEnd()) {
      line = in.readLine() + "\n";
      output = output + line;
  }
  return output;
 // qDebug() << words.size();
}


bool Translator::isNumeric(QString word){

    QHash<QString, QString>::const_iterator it;

    for(it = numericAscii2UnicodeMap->begin(); it!= numericAscii2UnicodeMap->end(); ++it  ) {
        word.replace( QRegExp(it.key()),"");
    }

    return ( word.size() == 0 );

}


bool Translator::isRoman(QString word){

    QStringList::const_iterator it;


    QStringList charList = word.split("0x");
    charList.removeFirst();

    float totChars = (float)charList.size();

    float numRoman = 0;
   //  qDebug() << "<" << charList.at(0) <<">";
    for(it = charList.begin(); it!= charList.end(); ++it  ) {

        if( romanAsciiMap->contains( QString("0x") + *it)) {
            numRoman++;
        }
    }

    if( totChars > 0 &&  numRoman/totChars > 0.85 ) {
        return(true);
    }

    return false;

}

QString Translator::translateNumericAndAssamese(QString &word) {

    QHash<QString, QString>::const_iterator it;
    QString newWord ="";
    QList<QString> list = Translator::getStringListFromHexString(word, "0x");

    while( list.size() > 0 &&  Translator::numericAscii2UnicodeMap->contains(QString("0x")+list.at(0))) {
       newWord = newWord + Translator::numericAscii2UnicodeMap->value(QString("0x")+list.at(0));
       list.removeFirst();
    }

    word.clear();
    while( list.size()> 0 ) {
        word.append( QString("0x") + list.at(0));
        list.removeFirst();
    }

    newWord = Translator::getUnicode(newWord,"0x") + this->translateAssamese(word);

    return newWord;

}

QString Translator::translateAssamese(QString &word){

    AsciiTrie *asciitree = AsciiTrie::getAsciiTrie() ;
    QStack<QString> matraStack;
    QStack<QString> chandraStack;
    QString tempChar;
    QString output="";
    bool trimmed = true;
    QStringList chars = Translator::split(word, "0x");


    while( chars.size() > 0 && trimmed == true) {
         word = word.replace(QRegExp("^0xc3"),"");
         word = word.replace(QRegExp("^0xc2"),"");
         word = word.replace(QRegExp("^0xc1"),"");
         word = word.replace(QRegExp("^0xb8"),"");

         if( chars.at(0) == "c1"  || chars.at(0) == "c2" || chars.at(0) == "c3"  || chars.at(0) == "b8"  ) { chars.removeFirst(); trimmed= true; continue;}


         QString asciiString = asciitree->findPrefix(chars);

         trimmed = false;
         if( asciiString.length() == 0) break;
         trimmed= true;

         if( word.indexOf(asciiString) == 0 ) {

             /*

             if( i == 645) {
                  qDebug() << "Word number = " << i << " Value = " << asciiString
                           << " Length = " << (asciiString).size() << "MStack " << matraStack.size() << " Word " << word;
              }
              */

              word = word.replace(0, asciiString.size(),"");

              tempChar.clear();
              QString tempQChar;
              QString tempQString;


              if( ascii2UnicodeMatraRefMap->contains(asciiString) ) {
              //    qDebug() << "Ref Matra";
                  tempQChar = output.at(output.size() - 1);
                  output.remove(output.size()-1, 1);
                  tempQString = ascii2UnicodeMatraRefMap->value(asciiString);
                  tempQString.replace("M", Translator::getUnicodeString(QString(tempQChar)));

                  output = output + Translator::getUnicode(tempQString,"0x");
               //   qDebug() << "Reif found;" << tempQString;
              }
              else if(ascii2UnicodeMatraLeftMap->contains(asciiString)) {
               //    qDebug() << "Left Matra";
                  if(matraStack.size() == 0) {
                      matraStack.push(asciiString);
    //                  qDebug() << "Pushing in " << itQKeyVal->ascii;
                  }
                  else if(matraStack.size() == 2) {
                      tempChar = matraStack.pop();
                      output = output + Translator::getUnicode(ascii2UnicodeMap->value(tempChar), "0x" );
                      tempChar = matraStack.pop();
                      output = output + Translator::getUnicode(ascii2UnicodeMap->value(tempChar), "0x" );
                      matraStack.push(asciiString);
                      if( chandraStack.size() > 0) {
                          tempChar = matraStack.pop();
                          output = output + Translator::getUnicode(ascii2UnicodeMap->value(tempChar), "0x" );
                      }

                  }

              }
              else if(ascii2UnicodeMatraRightMap->contains(asciiString)) {
                 //  qDebug() << "Right Matra";
                  if( matraStack.size()==0 ) {
                       output = output + Translator::getUnicode(ascii2UnicodeMap->value(asciiString), "0x" );
                  }
                  else if( matraStack.size()==2) {
                      tempChar = matraStack.pop();

                   /*   QRegExp chandra("(0x981)");
                      QString chandraCode ="";

                      if( chandra.indexIn(tempChar) != -1) {
                          chandraCode = chandra.cap(1);
                     //     qDebug() << "tempchar " << tempChar;
                          tempChar.remove(chandra);
                    //      qDebug() << " chandra code" << chandraCode << " tempQChar " << tempChar;
                      }
                      */
                      output = output + Translator::getUnicode(ascii2UnicodeMap->value(tempChar), "0x" );
                      tempChar = matraStack.pop();
                      tempChar.append(asciiString);
                     // qDebug() << "added unicode matra " << tempChar << "  " << ascii2UnicodeMap->value(tempChar);
                      output = output + Translator::getUnicode(ascii2UnicodeMap->value(tempChar), "0x" );

                      if( chandraStack.size() > 0 ) {
                          output = output + Translator::getUnicode(QString("0x981"), "0x" );
                          chandraStack.pop();
                      }
                  }

              }// chandrabindu
              else if( asciiString == QString("0xd2") ) {
                  chandraStack.push(QString("0x981"));
              }
              // jya kar
              else if( asciiString == QString("0xc9") ) {
                  output = output +  handle0x9cd(asciiString, matraStack,output);

              }
              else {
              //    qDebug() << "No Matra";
                  if(matraStack.size() == 0) {

                      output = output + Translator::getUnicode(ascii2UnicodeMap->value(asciiString), "0x" );

                  }
                  else if(matraStack.size() == 1) {
                      matraStack.push(asciiString);

                  }
                  else if(matraStack.size() == 2) {
                      if( ascii2UnicodeExoticMatraMap->contains(asciiString)) {
                   //       qDebug() << "Found an exotic symbol " ;
                          tempChar = matraStack.pop();
                          tempChar = tempChar + ascii2UnicodeExoticMatraMap->value(asciiString);
                          matraStack.push(tempChar);
                      }
                      else {
                         tempChar = matraStack.pop();
                         output = output + Translator::getUnicode(ascii2UnicodeMap->value(tempChar), "0x" );
                         tempChar = matraStack.pop();
                         output = output + Translator::getUnicode(ascii2UnicodeMap->value(tempChar), "0x" );
                         if( chandraStack.size() > 0 ) {
                             output = output + Translator::getUnicode(QString("0x981"), "0x" );
                             chandraStack.pop();
                         }
                         output = output + Translator::getUnicode(ascii2UnicodeMap->value(asciiString), "0x" );
                      }

                  }

             } // end of character type if



         }


    }  // iterate over the word

    if( matraStack.size() == 2) {
    //    qDebug() << "End of word";
        tempChar = matraStack.pop();
        output = output + Translator::getUnicode(ascii2UnicodeMap->value(tempChar), "0x" );
        tempChar = matraStack.pop();
        output = output + Translator::getUnicode(ascii2UnicodeMap->value(tempChar), "0x" );
    }

    return output;



}


QString Translator::translateRoman(QString word){

    //return Translator::getUnicode(word,"0x");

    QHash<QString, QString>::const_iterator it;
    QString newWord ="";
    QList<QString> list = Translator::getStringListFromHexString(word, "0x");

    for(unsigned int i = 0; i < list.length(); i++) {

       newWord = newWord + romanAsciiMap->value(QString("0x")+list.at(i));
    }


    return Translator::getUnicode(newWord,"0x");

}

QString Translator::translateNumeric(QString word){

    QHash<QString, QString>::const_iterator it;
    QString newWord ="";
    QList<QString> list = Translator::getStringListFromHexString(word, "0x");

    for(unsigned int i = 0; i < list.length(); i++) {

       newWord = newWord + numericAscii2UnicodeMap->value(QString("0x")+list.at(i));

    }


    return Translator::getUnicode(newWord,"0x");

}

QString Translator::handle0x9cd(QString unicodeChar, QStack<QString>& matraStack, QString &output) {
    QString tempQChar;
    if( matraStack.size() == 2 ) {

         tempQChar = matraStack.pop();
         tempQChar = Translator::ascii2UnicodeMap->value(tempQChar) + QString("0x9cd0x9af");
         ascii2UnicodeMap->insert(tempQChar, tempQChar);
         matraStack.push(tempQChar);

         return( QString(""));
    }
    else if( matraStack.size() == 0) {
        // qDebug() << " stack size 0 ";
        tempQChar = Translator::getUnicodeString(output.at(output.size() - 1));
      //   qDebug() << " stack size 0 " << tempQChar;
        if( Translator::vowelModifierMap->contains(tempQChar) ) {
          //  qDebug() << " stack size 0 " << tempQChar;
            output.remove(output.size()-1, 1);
            tempQChar = QString("0x9cd0x9af") + tempQChar ;

        }
        else {
            tempQChar = QString("0x9cd0x9af");
        }
        return(Translator::getUnicode(tempQChar,"0x"));
    }
    qDebug() << " nothing found " << unicodeChar;
}

void Translator::intializeAsciiTree() {
    AsciiTrie *asciiTree = AsciiTrie::getAsciiTrie() ;
    QList<QKeyValuePair>::const_iterator itQKeyVal;
    QHash<QString,QString> charMap;
    for( itQKeyVal = ascii2UnicodeList->begin(); itQKeyVal != ascii2UnicodeList->end(); ++itQKeyVal ) {
        charMap.insert(itQKeyVal->ascii, itQKeyVal->ascii);
   //     qDebug() << itQKeyVal->ascii;
    }
    asciiTree->addWords(charMap);
  //  asciiTree->printData();

}


void Translator::intializeUnicodeTree() {
    AsciiTrie *unicodeTree = AsciiTrie::getUnicodeTrie() ;
    QHash<QString, QString>::const_iterator it;

    QList<QKeyValuePair> *unicode2AsciiExtList = new QList<QKeyValuePair>();
    for(it = unicode2AsciiMapExt->begin(); it!=unicode2AsciiMapExt->end(); ++it) {
     //   qDebug() << it.key();
        unicode2AsciiExtList->append(QKeyValuePair(it.key(), it.key().size()));
    }

    QHash<QString, QLeftRightPair>::const_iterator it2;

    for(it2 = unicode2MatraCombMap->begin(); it2!=unicode2MatraCombMap->end(); ++it2) {
//        qDebug() << it2.key();

        unicode2AsciiExtList->append(QKeyValuePair(it2.key(), it2.key().size()));
    }
    for(it2 = unicode2PrefixMatraCombMap->begin(); it2!=unicode2PrefixMatraCombMap->end(); ++it2) {
     //   qDebug() << it2.key();

        unicode2AsciiExtList->append(QKeyValuePair(it2.key(), it2.key().size()));
    }

    for(it2 = unicode2SuffixMatraCombMap->begin(); it2!=unicode2SuffixMatraCombMap->end(); ++it2) {
       // qDebug() << it2.key();
        unicode2AsciiExtList->append(QKeyValuePair(it2.key(), it2.key().size()));
    }

    qSort( unicode2AsciiExtList->begin(), unicode2AsciiExtList->end(), compareQKeyValuePair);

    QList<QKeyValuePair>::const_iterator itQKeyVal;

    QHash<QString,QString> charMap;
    for( itQKeyVal = unicode2AsciiExtList->begin(); itQKeyVal != unicode2AsciiExtList->end(); ++itQKeyVal ) {
        charMap.insert(itQKeyVal->ascii, itQKeyVal->ascii);
    //    qDebug() << itQKeyVal->ascii;
    }


    unicodeTree->addWords(charMap);
    unicode2AsciiExtList->clear();

  //  unicodeTree->printData();


}



QString Translator::translateToUnicode(QString word) {

    QString output="";

    QString origword = word;
    static unsigned int i = 0;
    static int jerr = 0;

    if( false && (  i <0 || i> 180 ) )  return QString("");

    i++;
    if( i == 706) {
        qDebug() << "Word number = " << i << "  Word " << word;
     }

    static bool  prevWasNumeric = false;

    static bool  prevWasRoman = true;
    if( isNumeric(word) && prevWasRoman==false ) {
        return(translateNumeric(word) + " ");
    }

    if( isRoman(word)  ) {
       prevWasRoman = true;
    //   return(translateRoman(word) + "((" + QString::number(i) +") ");
       return ( translateRoman(word)  +" ");
    }


    output = this->translateAssamese(word);

    if( word.size() > 0) {
       word = origword;
       output = translateNumericAndAssamese(word) ;
    }

    if( word.size() > 0 ) {
        jerr++;
        qDebug() << jerr << " Failed on word " + QString::number(i)  << "  as " << word << " original " << origword;
    }
    prevWasRoman = false;
    return output + " ";

  //  return output + "(" + QString::number(i) +") ";

}

bool Translator::isUnicodeEnglish(QString word) {

    QStringList chars = Translator::split(word, "0x");
    foreach( QString Char, chars) {
        if( !romanAsciiMap->contains(QString("0x") + Char) ) {
            return false;
        }
    }
    return true;
}

bool Translator::isUnicodeNumeric(QString word) {

    QStringList chars = Translator::split(word, "0x");
    foreach( QString Char, chars) {
        if( ! numericUnicode2AsciiMap->contains(QString("0x") + Char) ) {
            return false;
        }
    }
    return true;
}


QString Translator::translateTextToAscii(QString text) {

    QList< QString> unicodeWords;
    translator->createUnicodeWords(text, unicodeWords);
    QList<QString>::const_iterator itWord;
    QString output="";
     //    output ="";

    for( itWord = unicodeWords.begin();   itWord != unicodeWords.end(); ++itWord  ) {
             if( *itWord == "0xghij")
                 output = output + QString("\n");
             else
                // output = output + Translator::translateToAscii(*itWord) + "(" + QString::number(i) +")" + " ";
                 output = output + translator->translateToAscii(*itWord ) + " ";

    }

    return output;

}

QString Translator::translateTextToUnicode(QString text) {
    QList< QString> geetanjaliWords;
    translator->createAsciiWords(text, geetanjaliWords);

    QList<QString>::const_iterator itWord;
    QString output="";
         unsigned int i=1;
         for( itWord = geetanjaliWords.begin();   itWord != geetanjaliWords.end(); ++itWord  ) {
             if( *itWord == "0xghij") {
                 output = output + QString("\n");
               //  shortOutput = shortOutput + QString("\n");
             }
             else {
                // output = output + translator->translateToUnicode(*itWord) + "(" + QString::number(i) +")" + " ";
                 output = output + translator->translateToUnicode(*itWord);
                 //shortOutput = shortOutput + translator->translateToUnicode(*itWord ) + " ";
                 i++;
             }

         }
    return output;

}




QString Translator::translateToAscii(QString word) {

    if( isUnicodeEnglish(word))
        return translateToEnglishAscii(word);
    else if( isUnicodeNumeric(word))
        return translateToNumericAscii(word);
    else
        return translateToAssameseAscii(word);
}



QString Translator::translateToNumericAscii(QString word) {
    QString output="";

    QStringList chars = Translator::split(word, "0x");

    foreach( QString Char, chars) {
        output = output + Translator::getUnicode(numericUnicode2AsciiMap->value(QString("0x") + Char),"0x" );
    }
    return output;
}


QString Translator::translateToEnglishAscii(QString word) {
    QString output="";

    QStringList chars = Translator::split(word, "0x");

    foreach( QString Char, chars) {
        output = output + Translator::getUnicode(romanAsciiMap->value(QString("0x") + Char),"0x" );
    }
    return output;
}




QString Translator::translateToAssameseAscii(QString word) {
    AsciiTrie *unicodetree = AsciiTrie::getUnicodeTrie() ;
    QString output="";

    QString origword = word;
    static unsigned int i = 0;

    bool trimmed = true;
    QStringList chars = Translator::split(word, "0x");
    QString prevWord="";
    QString prefixMatra="";
    QString suffixMatra="";

    QRegExp chandra("0x9aa0x9cd0x9b2");
    if( chandra.indexIn(word) != -1 )
        qDebug() << "word= " << word ;

    while( chars.size() > 0 && trimmed == true) {
     //   word = word.replace(QRegExp("^0x20"),"");
        //if( chars.at(0) == "20" ) { chars.removeFirst(); trimmed= true; continue;}

        QString unicodeString = unicodetree->findPrefix(chars);
        if( unicodeString == QString("0x20") ) {
            trimmed= true;
            continue;
        }
      //  qDebug() << word << "   " << unicodeString;

        trimmed = false;
        if(  unicodeString.length() == 0) break;
        trimmed= true;

        if( word.indexOf(unicodeString) == 0 ) {
             word = word.replace(0, unicodeString.size(),"");
       //      qDebug() << " word 1" << word << " chars size " << chars.size();
             if( unicode2MatraCombMap->contains( unicodeString)) {
                 if( unicode2MatraCombMap->value(unicodeString).left != QString("") ) {
                     output = output + Translator::getUnicode(unicode2MatraCombMap->value(unicodeString).left, "0x");
                 }
                 output = output + prevWord;
                 if( unicode2MatraCombMap->value(unicodeString).right != QString("") ) {
                     output = output + Translator::getUnicode(unicode2MatraCombMap->value(unicodeString).right, "0x");
                 }
                 prevWord=QString("");
             }
             else if( unicode2PrefixMatraCombMap->contains( unicodeString)) {
                 prefixMatra = unicodeString;
             }
             else if( unicode2SuffixMatraCombMap->contains( unicodeString)) {
                 suffixMatra = unicodeString;

                 if( suffixMatra.length() > 0 ) {

                     if( unicode2SuffixMatraCombMap->value(suffixMatra).left != QString("") ) {
                         prevWord = Translator::getUnicode(unicode2SuffixMatraCombMap->value(suffixMatra).left, "0x") + prevWord;
                     }
                     if( unicode2SuffixMatraCombMap->value(suffixMatra).right != QString("") ) {
                         prevWord = prevWord + Translator::getUnicode(unicode2SuffixMatraCombMap->value(suffixMatra).right, "0x");
                     }
                     suffixMatra = "";
                 }

             }
             else if( unicode2AsciiMapExt->contains( unicodeString ) ) {
                 if( chandra.indexIn(origword) != -1 )
                     qDebug() << "output word = " << unicodeString << "  " << unicode2AsciiMapExt->value(unicodeString);
                output = output + prevWord;
                prevWord = Translator::getUnicode(unicode2AsciiMapExt->value(unicodeString), "0x");
  //              if( chandra.indexIn(origword) != -1 )
//                    qDebug() << "output word = " << unicodeString << "  " << unicode2AsciiMapExt->value(unicodeString) << " " << prevWord;

                if( prefixMatra.length() > 0 ) {

                    if( unicode2PrefixMatraCombMap->value(prefixMatra).left != QString("") ) {
                        prevWord = Translator::getUnicode(unicode2PrefixMatraCombMap->value(prefixMatra).left, "0x") + prevWord;
                    }
                    if( unicode2PrefixMatraCombMap->value(prefixMatra).right != QString("") ) {
                        prevWord = prevWord + Translator::getUnicode(unicode2PrefixMatraCombMap->value(prefixMatra).right, "0x");
                    }
                    prefixMatra = "";
                }



             }
        }
    }
    if( prevWord.length() > 0 )   output = output + prevWord;
    if( chandra.indexIn(origword) != -1 )
        qDebug() << "output word = " << output ;

  //  return output + "(" + QString::number(i) +") ";
 //   qDebug() << "Output " << output;
    return output;
}
