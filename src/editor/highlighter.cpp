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

#include <QtGui>
#include <iostream>

#include "src/editor/highlighter.h"
#include "src/utils/utilities.h"
#include "src/phonetic/romanization.h"

WordsTrie *Highlighter::profileWords = NULL;
WordsTrie *Highlighter::dictionaryWords = NULL;

Highlighter::Highlighter(QTextDocument *parent, bool spellCheckState):
	QSyntaxHighlighter(parent) {
	HighlightingRule rule;

	spellCheckFormat.setUnderlineColor(QColor(Qt::red));
	spellCheckFormat.setUnderlineStyle(QTextCharFormat::SpellCheckUnderline);

	//Settings for online spellchecking
    spellCheckActive=true;
	spellerError=!spellCheckActive;
	spellCheckActive=spellCheckActive && spellCheckState;
}

Highlighter::~Highlighter() {

}

void Highlighter::highlightBlock(const QString &text) {
     spellCheck(text);
}

void Highlighter::enableSpellChecking(const bool state) {
    bool old = spellCheckActive;
    if (!spellerError) spellCheckActive = state;
    if (old != spellCheckActive) rehighlight();
}



void Highlighter::spellCheck(const QString &text) {
	if (spellCheckActive) {
		// split text into words
		QString str = text.simplified();
		if (!str.isEmpty()) {
			QStringList Checkliste = str.split(QRegExp("([^\\w,^\\\\]|(?=\\\\))+"),
					QString::SkipEmptyParts);
			int l,number;
         //   qDebug() << "number of words " << Checkliste.size();
			// check all words
            for (int i = 0; i < Checkliste.size(); ++i) {
				str = Checkliste.at(i);

                if (str.length() > 1 && !str.startsWith('\\'))
				{
                  //  qDebug() << "> " << i << " " << str;

                    if (checkWord(str)) {
						number = text.count(QRegExp("\\b" + str + "\\b"));
						l=-1;
						// underline all incorrect occurences of misspelled word
                        for (int j = 0; j < number; ++j) {
							l = text.indexOf(QRegExp("\\b" + str + "\\b"),l+1);
                            if (l >= 0)
								setFormat(l, str.length(), spellCheckFormat);
						} // for j
					} // if spell chek error
				} // if str.length > 1
			} // for
		} // if str.isEmpty
	} // initial check

 //   qDebug() << "Done spell check " << text;
}

bool Highlighter::checkWord(QString word) {

//    qDebug() << Utilities::getUnicodeString(word);
//	check = pChecker->spell(encodedString.data());
    if (!Utilities::isAssamese(word)) {
        return false;
    }
    //qDebug() << word;

    //if( Romanization::UnicodeToRomanOverrideMap.contains( Utilities::getUnicodeString(word)) ) return false;
   // qDebug() << " ignore list size " << ignoreDictionary->size();
    if (ignoreDictionary != NULL && ignoreDictionary->contains(word)) return false;

    if (profileWords == NULL) profileWords = WordsTrie::getProfileWordsTrie();
    if (dictionaryWords == NULL) dictionaryWords = WordsTrie::getWordsTrie();

    QStringList candidateWordsList= Phonetic::getInflectionalFormsX(word);


    foreach( QString str, candidateWordsList) {
       // qDebug() << "word " << str;
        if( Romanization::UnicodeToRomanOverrideMap.contains( Utilities::getUnicodeString(str)) ) return false;
        if( dictionaryWords->hasWord(str) || profileWords->hasWord(str)) {
           return false;
        }
    }
    return true;
}

void Highlighter::setIgnoreWords( QHash<QString, bool> *ignoreDictionaryRef) {
    ignoreDictionary = ignoreDictionaryRef;
}

bool Highlighter::setDict(const QString SpellDic) {
    bool spell;
    if (SpellDic != ""){
		//mWords.clear();
		spell_dic=SpellDic.left(SpellDic.length()-4);
//		delete pChecker;
//		pChecker = new Hunspell(spell_dic.toLatin1()+".aff",spell_dic.toLatin1()+".dic");
//		spell_encoding=QString(pChecker->get_dic_encoding());
		codec = QTextCodec::codecForName(spell_encoding.toLatin1());

		QFileInfo fi(SpellDic);

		if (fi.exists() && fi.isReadable()) spell=true;
		else spell=false;

		// get user config dictionary
		QSettings setting;
		QString filePath=QFileInfo(setting.fileName()).absoluteFilePath();
		filePath=filePath+"/User_"+QFileInfo(spell_dic.toLatin1()+".dic").fileName();
		std::cout << qPrintable(filePath) << std::endl;
		fi=QFileInfo(filePath);
		if (fi.exists() && fi.isReadable()){
//			pChecker->add_dic(filePath.toLatin1());
		}
		else filePath="";

		spellCheckFormat.setForeground(Qt::red);//faster Cursoroperation ...
		//spellCheckFormat.setUnderlineColor(QColor(Qt::red));
		//spellCheckFormat.setUnderlineStyle(QTextCharFormat::SpellCheckUnderline);
	}
	else spell=false;
	spellerError=!spell;
	spellCheckActive=spellCheckActive && spell;
	rehighlight();
	return spell;
}

void Highlighter::slot_addWord(QString word) {
    std::cout << qPrintable(word) << std::endl;
    QByteArray encodedString;
//    QString spell_encoding=QString(pChecker->get_dic_encoding());
    QTextCodec *codec = QTextCodec::codecForName(spell_encoding.toLatin1());
    encodedString = codec->fromUnicode(word);
//    pChecker->add(encodedString.data());
    rehighlight();
}
