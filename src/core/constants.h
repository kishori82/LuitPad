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
***************************************************************************/

#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <QString>
#include <QHash>

#undef  ASSAMESE_ENGLISH_WORDS

const QString toolTipPrefix = "<html><head><meta charset=utf-8\"></head><body bgcolor=\"#8AB8E6\" font size = \"10\">";
const QString toolTipSuffix = "</body></html>";
extern const int CRYPTOSEED;

typedef struct keyfloat {
    QString key;
    float value;
} QKeyFloat;


typedef struct keyvalue {
    QString key;
    QString value;
} QKeyValue;

typedef struct wordUnicode {
    QString word;
    QString unicode;
} QWordUnicode;


typedef struct wordRank {
    QString word;
    QString unicode;
    float rank;
    float len;
} QWordRank;

typedef struct keyDistance {
    QString word;
    float dist;
} QKeyDistance;

typedef struct stringInt {
    QString key;
    unsigned int size;
} QKeyInt;


enum APP_STATES { F2, F3, F4};


enum CONFIG_STATES{ C0, C1, C2, C3, C4 };


extern APP_STATES _state;


extern bool _auto_matra_setting;
extern bool _auto_character_fill_setting;
extern bool  _single_character_mode_setting;
extern bool _hide_tooltip_mode_setting;

#endif // CONSTANTS_H
