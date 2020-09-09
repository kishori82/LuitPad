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

#include "romanization.h"

QHash<QString, QString> Romanization::UnicodeToRomanOverrideMap;
QHash<QString, QStringList> Romanization::UnicodeToRomanOverrideList;
QHash<QString, QString> Romanization::UnicodeToRomanMap;
QHash<QString, QString> Romanization::UnicodeToRomanMapInit;
QHash<QString, QString> Romanization::UnicodeToDlinkRomanMap;
QHash<QString, QHash<QString, QString> > Romanization::ForwardAlterSound;
QHash<QString, QHash<QString, QString> > Romanization::BackwardAlterSound;

void Romanization::InitializeUnicodeToRomanOverrideMaps(QString fileName){
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly)) {
            QMessageBox::information(0, "error", file.errorString());
    }

    QTextStream in(&file);
    QStringList list ;

    int i =0;
    QString line;
    QString roman;
    QString unicodeString;
    QRegExp rxComment("//");


    while(!in.atEnd()) {
        line = in.readLine();
        //qDebug() << line;
        if( rxComment.indexIn(line,0) > -1 ) { /*qDebug() << line; */ continue;}

        list = line.split("\t");
        if( list.size() < 2 ) {
            continue;
        }

        if( list.size() == 2 ) {
            roman =  list.at(0).trimmed().toLower();
            unicodeString = Utilities::getUnicodeString(list.at(1));

            //qDebug() << roman << " -- " << unicodeString;
            if( roman.size() >0  ) {
              //  if( roman=="kishori") qDebug() << " Read " << roman << "  " << unicodeString;
                UnicodeToRomanOverrideMap[unicodeString] = roman;
                if(! UnicodeToRomanOverrideList.contains(unicodeString)) {
                    QStringList list;
                    UnicodeToRomanOverrideList[unicodeString]=list;
                }
                UnicodeToRomanOverrideList[unicodeString].append(roman);
            }
        }
        else {
               ;// qDebug() << line;
        }
    }
    file.close();

}

void Romanization::InitializeMaps(){
    UnicodeToRomanMap["0x0985"]="o";
    UnicodeToRomanMap["0x0986"]="a";
    UnicodeToRomanMap["0x0987"]="i";
    UnicodeToRomanMap["0x0988"]="i";
    UnicodeToRomanMap["0x0989"]="u";
    UnicodeToRomanMap["0x098a"]="u";
    UnicodeToRomanMap["0x098b"]="ri";
    UnicodeToRomanMap["0x098f"]="e";
    UnicodeToRomanMap["0x0990"]="oi";
    UnicodeToRomanMap["0x0993"]="u";
    UnicodeToRomanMap["0x0994"]="ou";
    UnicodeToRomanMap["0x0995"]="ko";
    UnicodeToRomanMap["0x0996"]="kho";
    UnicodeToRomanMap["0x0997"]="go";
    UnicodeToRomanMap["0x0998"]="gho";
    UnicodeToRomanMap["0x0999"]="ngo";
    UnicodeToRomanMap["0x099a"]="co";
    UnicodeToRomanMap["0x099b"]="co";    // cho
    UnicodeToRomanMap["0x099c"]="jo";
    UnicodeToRomanMap["0x099d"]="jho";
    UnicodeToRomanMap["0x099e"]="yo";     // changed from nyo to yo
    UnicodeToRomanMap["0x099f"]="to";
    UnicodeToRomanMap["0x09a0"]="tho";
    UnicodeToRomanMap["0x09a1"]="do";
    UnicodeToRomanMap["0x09a2"]="dho";
    UnicodeToRomanMap["0x09a3"]="no";
    UnicodeToRomanMap["0x09a4"]="to";
    UnicodeToRomanMap["0x09a5"]="tho";
    UnicodeToRomanMap["0x09a6"]="do";
    UnicodeToRomanMap["0x09a7"]="dho";
    UnicodeToRomanMap["0x09a8"]="no";
    UnicodeToRomanMap["0x09aa"]="po";
    UnicodeToRomanMap["0x09ab"]="fo";
    UnicodeToRomanMap["0x09ac"]="bo";
    UnicodeToRomanMap["0x09ad"]="vo";
    UnicodeToRomanMap["0x09ae"]="mo";
    UnicodeToRomanMap["0x09af"]="jo";
    UnicodeToRomanMap["0x09f0"]="ro";
    UnicodeToRomanMap["0x09b2"]="lo";
    UnicodeToRomanMap["0x09f1"]="wo";
    UnicodeToRomanMap["0x09b6"]="kho";    // talubiya ho
    UnicodeToRomanMap["0x09b7"]="sho";
    UnicodeToRomanMap["0x09b8"]="kho";     // dontiya ho
    UnicodeToRomanMap["0x09b9"]="ho";
    UnicodeToRomanMap["0x09df"]="yo";
    UnicodeToRomanMap["0x09dc"]="ro";
    UnicodeToRomanMap["0x09dd"]="ro";
    UnicodeToRomanMap["0x09ce"]="t";
    UnicodeToRomanMap["0x0982"]="ng";
    UnicodeToRomanMap["0x09be"]="a";
    UnicodeToRomanMap["0x09bf"]="i";
    UnicodeToRomanMap["0x09c0"]="i";
    UnicodeToRomanMap["0x09c1"]="u";
    UnicodeToRomanMap["0x09c2"]="u";
    UnicodeToRomanMap["0x09c3"]="rhi";
    UnicodeToRomanMap["0x09c7"]="e";
    UnicodeToRomanMap["0x09c8"]="oi";
    UnicodeToRomanMap["0x09cb"]="u";
    UnicodeToRomanMap["0x09cc"]="ou";
    UnicodeToRomanMap["0x0981"]="o";

    UnicodeToRomanMap["0x0aaa"]="khya";

    UnicodeToRomanMapInit["0x09b6"]= "ho";
    UnicodeToRomanMapInit["0x09b8"]= "ho";

    UnicodeToDlinkRomanMap["0x0985"]="o" ;
    UnicodeToDlinkRomanMap["0x0986"]="a" ;
    UnicodeToDlinkRomanMap["0x0987"]="i" ;
    UnicodeToDlinkRomanMap["0x0988"]="i";
    UnicodeToDlinkRomanMap["0x0989"]="u" ;
    UnicodeToDlinkRomanMap["0x098a"]="u" ;
    UnicodeToDlinkRomanMap["0x098b"]="ri";
    UnicodeToDlinkRomanMap["0x098f"]="e" ;
    UnicodeToDlinkRomanMap["0x0990"]="oi" ;
    UnicodeToDlinkRomanMap["0x0993"]="u" ;
    UnicodeToDlinkRomanMap["0x0994"]="ou";
    UnicodeToDlinkRomanMap["0x0995"]="ko" ;
    UnicodeToDlinkRomanMap["0x0996"]="kho" ;
    UnicodeToDlinkRomanMap["0x0997"]="go" ;
    UnicodeToDlinkRomanMap["0x0998"]="gho" ;
    UnicodeToDlinkRomanMap["0x0999"]="ngo";
    UnicodeToDlinkRomanMap["0x099a"]="co" ;
    UnicodeToDlinkRomanMap["0x099b"]="cho" ;
    UnicodeToDlinkRomanMap["0x099c"]="jo" ;
    UnicodeToDlinkRomanMap["0x099d"]="jho" ;
    UnicodeToDlinkRomanMap["0x099e"]="yo";    // nyo -> yo
    UnicodeToDlinkRomanMap["0x099f"]="to" ;
    UnicodeToDlinkRomanMap["0x09a0"]="tho" ;
    UnicodeToDlinkRomanMap["0x09a1"]="do" ;
    UnicodeToDlinkRomanMap["0x09a2"]="dho" ;
    UnicodeToDlinkRomanMap["0x09a3"]="no";
    UnicodeToDlinkRomanMap["0x09a4"]="to" ;
    UnicodeToDlinkRomanMap["0x09a5"]="tho" ;
    UnicodeToDlinkRomanMap["0x09a6"]="do" ;
    UnicodeToDlinkRomanMap["0x09a7"]="dho" ;
    UnicodeToDlinkRomanMap["0x09a8"]="no";
    UnicodeToDlinkRomanMap["0x09aa"]="po" ;
    UnicodeToDlinkRomanMap["0x09ab"]="fo" ;
    UnicodeToDlinkRomanMap["0x09ac"]="bo" ;
    UnicodeToDlinkRomanMap["0x09ad"]="vo" ;
    UnicodeToDlinkRomanMap["0x09ae"]="mo";
    UnicodeToDlinkRomanMap["0x09af"]="yo" ;
    UnicodeToDlinkRomanMap["0x09f0"]="ro" ;
    UnicodeToDlinkRomanMap["0x09b2"]="lo" ;
    UnicodeToDlinkRomanMap["0x09f1"]="wo";
    UnicodeToDlinkRomanMap["0x09b6"]="ho" ;
    UnicodeToDlinkRomanMap["0x09b7"]="sho" ;
    UnicodeToDlinkRomanMap["0x09b8"]="ho" ;
    UnicodeToDlinkRomanMap["0x09b9"]="ho";
    UnicodeToDlinkRomanMap["0x09df"]="yo" ;
    UnicodeToDlinkRomanMap["0x09dc"]="ro" ;
    UnicodeToDlinkRomanMap["0x09dd"]="ro";
    UnicodeToDlinkRomanMap["0x09ce"]="t" ;
    UnicodeToDlinkRomanMap["0x0982"]="ng";
    UnicodeToDlinkRomanMap["0x09be"]="a" ;
    UnicodeToDlinkRomanMap["0x09bf"]="i" ;
    UnicodeToDlinkRomanMap["0x09c0"]="i";
    UnicodeToDlinkRomanMap["0x09c1"]="u" ;
    UnicodeToDlinkRomanMap["0x09c2"]="u" ;
    UnicodeToDlinkRomanMap["0x09c3"]="rhi";
    UnicodeToDlinkRomanMap["0x09c7"]="e" ;
    UnicodeToDlinkRomanMap["0x09c8"]="oi" ;
    UnicodeToDlinkRomanMap["0x09cb"]="u" ;
    UnicodeToDlinkRomanMap["0x09cc"]="ou";
    UnicodeToDlinkRomanMap["0x0981"]="o";




    QHash< QString, QString> a;


    // Be Careful that we are not using the 0 after the 0x
    ForwardAlterSound["0x09b8"] = a;
    ForwardAlterSound["0x09b8"]["0x09ae"] = "so";   //dontiya  ho + mo
    ForwardAlterSound["0x09b8"]["0x09ac"] = "so";   //dontiya ho + bo
    ForwardAlterSound["0x09b8"]["0x0995"] = "so";   //dontiya ho + ko
    ForwardAlterSound["0x09b8"]["0x09a4"] = "so";   //dontiya ho + dontiya to
    ForwardAlterSound["0x09b8"]["0x099f"] = "so";   //dontiya ho + murdhanya to
    ForwardAlterSound["0x09b8"]["0x09a5"] = "so";   //dontiya ho + dontiya tho
    ForwardAlterSound["0x09b8"]["0x09b2"] = "so";   //dontiya ho + lo
    ForwardAlterSound["0x09b8"]["0x09f0"] = "so";   //dontiya ho + wo
    ForwardAlterSound["0x09b8"]["0x09aa"] = "so";   //dontiya ho + po

    a.clear();
    ForwardAlterSound["0x09b6"] = a;
    ForwardAlterSound["0x09b6"]["0x09f0"] = "so";     // talu ho + ro
    ForwardAlterSound["0x09b6"]["0x09ac"] = "so";  // talubiya  ho + bo
    ForwardAlterSound["0x09b6"]["0x09b2"] = "so";  // taluiya  ho + lo
    ForwardAlterSound["0x09b6"]["0x099a"] = "so";  // talubiya  ho + prothom cho

    a.clear();
    ForwardAlterSound["0x09b7"] = a;
    ForwardAlterSound["0x09b7"]["0x099f"] = "so";  // murdhanya  kho + murdhanhya to
    ForwardAlterSound["0x09b7"]["0x09a0"] = "so";  // murdhanya  kho + murdhanhya tho
    ForwardAlterSound["0x09b7"]["0x099e"] = "so";  // murdhanya  kho + murdhanhya sno

    a.clear();
    ForwardAlterSound["0x099e"] = a;
    ForwardAlterSound["0x099e"]["0x099a"] = "no";  // nyo + prothom co
    ForwardAlterSound["0x099e"]["0x099b"] = "no";  // nyo + prothom co

    a.clear();
    ForwardAlterSound["0x099c"]=a;
    ForwardAlterSound["0x099c"]["0x099e"] = "g";   // borgiya go + nya



    a.clear();
    BackwardAlterSound["0x09ac"] = a;
    BackwardAlterSound["0x09ac"]["0x09b8"] = "wo";   //dontiya  ho + bo
    BackwardAlterSound["0x09ac"]["0x09a8"] = "wo";    //  dontiya no + bo
    BackwardAlterSound["0x09ac"]["0x09b6"] = "wo";    // talu ho + bo
    BackwardAlterSound["0x09ac"]["0x09a4"] = "wo";    // dontiya to  + bo
    BackwardAlterSound["0x09ac"]["0x099f"] = "wo";    // murdhanya to  + bo


    //dontiya ho + re kar
    //MatraAlterSound["0x09b8"]["0x9c3"]="so";

   // qDebug() << "sound " << ForwardAp time (including calls, data transfer etc.) - 15 hourslterSound["0x09b8"]["0x09ae"];

}

QStringList Romanization::vowels =
        QStringList() << "0x0985" <<  "0x0986" << "0x0987"  << "0x0988" << "0x0989"
                      << "0x098a" << "0x098b" << "0x098f" << "0x0990"<< "0x0993"<< "0x0994";


QStringList Romanization::vMod = QStringList()
  << "0x09be" << "0x09bf" << "0x09c0"
  << "0x09c1" << "0x09c2" << "0x09c3"
  << "0x09c7" << "0x09c8" << "0x09cb" << "0x09cc"
  << "0x0981";


Romanization::Romanization()
{
/*
    qDebug() << "Size of vMod " << vMod.size();
    qDebug() << "Size of dlink_Rom " << dlink_Rom.size();
    qDebug() << "Size of  Rom " << Rom.size();
    qDebug() << "Size of Unik " << Unik.size();
    */
}

bool compareUnicode(const QWordUnicode &a, const QWordUnicode &b) {

    if( a.unicode < b.unicode)
        return true;
    else
        return false;

}

void Romanization::Romanize(QString inputFile, QHash<QString, QStringList> &roman2UnicodeMap) {
    QFile infile(inputFile);
     if(!infile.open(QIODevice::ReadOnly)) {
             QMessageBox::information(0, "error", infile.errorString());
     }
     QTextStream inStream(&infile);

//#define PRINT_ROMAN
#ifdef   PRINT_ROMAN
     bool html = true;
     QFile outfile("files/romanized.html");
     if(!outfile.open(QIODevice::WriteOnly)) {
             QMessageBox::information(0, "error", outfile.errorString());
     }
     QTextStream outStream(&outfile);
#endif
     QString out_str;
     QString unicode_str;

#ifdef   PRINT_ROMAN
     out_str.append("<html><head> \n");
     out_str.append("<style type =\"text/css\">\n");
     out_str.append("@font-face { \n");
     out_str.append("   font-family: cool_font;\n");
     out_str.append("   src: url('Siyamrupali_1_01.ttf');\n");
     out_str.append("}\n");
     out_str.append("span {\n");
     out_str.append("   font-family: cool_font;\n");
     out_str.append("} \n");
     out_str.append("</style>\n");
     out_str.append("</head>");

     if(html) {
         outStream << out_str;
     }

     int count=1;
#endif

     QList<QWordUnicode> webPrintableWords;
     QWordUnicode k;
     QStringList list;
     QHash<QString, bool>  already_seen;

     QString str; //Utilities::getUnicodeString(list.at(1).trimmed());

     QString roman="";
     int i = 0;
     while (!inStream.atEnd()) {
         list = inStream.readLine().split("\t");
         str = list.at(1).trimmed(); //Utilities::getUnicodeString(list.at(1).trimmed());

         if( already_seen.contains(str)) continue;
         already_seen.insert(str, true);


        out_str.clear();
        if( !UnicodeToRomanOverrideMap.contains(str)) {
            roman = convert2Roman(str);
        }

        i = i + 1;
        if( str.size() > 0 && roman.size() > 0 ) {
            roman2UnicodeMap[roman].append(str);
            k.word = roman;
            k.unicode = Utilities::getUnicodeForWebPage(str,"0x");
            webPrintableWords.append(k);
        }
     }
    // qDebug() << "total words " << i;
     return;

     QHash<QString, QStringList>::const_iterator hit;

     for(hit = UnicodeToRomanOverrideList.begin(); hit!= UnicodeToRomanOverrideList.end(); ++hit) {
        out_str.clear();
        str = hit.key();
        foreach( QString roman, hit.value()) {
            if( roman.size() > 0 ) {
                roman2UnicodeMap[roman].append(str);
                k.word = roman;
                k.unicode = Utilities::getUnicodeForWebPage(str,"0x");
                webPrintableWords.append(k);
            }
        }
     }

     qSort(webPrintableWords.begin(), webPrintableWords.end(), &compareUnicode);

 //    qDebug() << "Number of words " << webPrintableWords.size();
#ifdef   PRINT_ROMAN
     foreach(QWordUnicode q, webPrintableWords ) {
        if( html ) {

            outStream << "&nbsp;&nbsp;&nbsp;" << count;
            outStream << "&nbsp;&nbsp;&nbsp;" << q.word;
            outStream << "&nbsp;&nbsp;&nbsp;" << "<span> " << q.unicode << " </span>";
            outStream << "<br>\n";
        }
        else {
            outStream << "\t" << q.word;;
            outStream << "\t" << q.word;
            outStream << "\n";
        }
        count++;
     }
#endif

#ifdef   PRINT_ROMAN
      if(html) {
           outStream << "</html>";
      }
#endif


      infile.close();

      /* outfile.close(); */
 }


bool Romanization::isEndWithVowel(QString token) {
      bool flag = false;
      for (int i = 0; i < vowels.length(); i++) {
          if (token.endsWith(vowels[i])) {
              flag = true;
          }
      }
      return flag;
  }

QString Romanization::convert2Roman(QString  strC) {
      QString roman ;
      QRegExp re("o$");
      QString prevtoken="";
      QString token;

      QString hex_str;
      hex_str.clear();

      QString str = strC;
      str.replace(QRegExp("0x9950x9cd0x9b7"),"0x0aaa");
      QStringList tokens = Utilities::getStringListFromHexString(str, "0x");


      QString tok;
      unsigned int numTokens = tokens.size();
      for(unsigned int i=0; i< numTokens; i++) {
          tok = tokens.at(i);

          if( QString(QString("0x") + tok).size() < 6)
              token = QString("0x0") + tok;
          else
              token = QString("0x") + tok;

         // qDebug() << "Token " << token;


          if (isVowelModifier(token)|| isConjunctChar(token) ) {
              roman.remove(re);
              if( UnicodeToDlinkRomanMap.contains(token)){
                  roman.append(UnicodeToDlinkRomanMap.value(token));
              }
          }
          else {
              if(prevtoken.length()>1 && isConjunctChar(prevtoken) ) {
                  if( UnicodeToDlinkRomanMap.contains(token)){
                      roman.append(UnicodeToDlinkRomanMap.value(token));
                  }
              }
              else if( numTokens - i > 2  &&  ForwardAlterSound.contains(token)
                       && isConjunctChar(QString("0x0")+tokens.at(i+1))
                       && ForwardAlterSound[token].contains(QString("0x0") + tokens.at(i+2)) ) {
                  roman.append(ForwardAlterSound[token].value(QString("0x0") + tokens.at(i+2)));
                 // qDebug() << "Token " << token << " Far token " << tokens.at(i+2) << " Sound " << ForwardAlterSound[token].value(QString("0x0") + tokens.at(i+2));
              }
              else if(  i > 1  &&  BackwardAlterSound.contains(token)
                       && isConjunctChar(QString("0x0")+tokens.at(i-1))
                       && BackwardAlterSound[token].contains(QString("0x0") + tokens.at(i-2)) ) {
                  roman.append(BackwardAlterSound[token].value(QString("0x0") + tokens.at(i-2)));
                 // qDebug() << "Token " << token << " Far token " << tokens.at(i+2) << " Sound " << ForwardAlterSound[token].value(QString("0x0") + tokens.at(i+2));
              }
              else {
                  if( UnicodeToRomanMap.contains(token)){
                      //if( i== 0 && token == "0x09b6" ) qDebug() << token;
                      if( i == 0 && UnicodeToRomanMapInit.contains(token)) {
                          roman.append(UnicodeToRomanMapInit.value(token));
                         // qDebug() << UnicodeToRomanMapInit.value(token);
                      }
                      else
                          roman.append(UnicodeToRomanMap.value(token));
                  }
              }
          }
          prevtoken = token;

      }


      if (tokens.length()> 1  ) {
          roman.remove(re);
      }

      if( tokens.length() > 2) {
         if (isConjunctChar(tokens.at(tokens.length()-2))) {
            roman.append("o");
         }
      }
      return roman;
  }

 bool Romanization::isVowelModifier(QString ch) {
      bool flag = false;
      for (int i = 0; i < vMod.length(); i++) {
          if (ch==vMod.at(i) ) {
              flag = true;
          }
      }
      return flag;
  }


bool Romanization::isConjunctChar(QString ch) {
      bool flag = false;
      if (ch == QString("0x09cd")) {
          flag = true;
      }
      return flag;
  }


/*
bool Romanization::insertWord(QString &word) {
    qDebug()  << "Romanized " << Romanization::convert2Roman(  Utilities::getUnicodeString( word)  );



    QStringList charList;
    charList.clear();
    foreach(QChar c, it.key())charList.append( Phonetic::phoneticEquivString(QString(c).toLower()));

       // qDebug() << charList << "  " << unicodeWord;
    Phonetic::insertWord(roman2UnicodeTree, charList, unicodeWord);


    return true;

}
*/
/*
QString Romanization::removeLastVowel(QString token) {
      return token.endsWith("o.") ? token.substring(0, token.length() - 2) : token.substring(0, token.length() - 1);
  }
  */
/*
for( QString s, Unik) {
     out_str.setLength(0);
     unicode_str.setLength(0);
     String hex= "";
     for(int i=0 ; i < s.length(); i++) {
         hex = Integer.toHexString(s.charAt(i) & 0xFFFF);
        // System.out.print("&#x" + hex +";");
         out_str.append("&#x"+hex +";");
     }
     output_writer.write(out_str.toString() );
     output_writer.write("<br>\n");
     unicode_str.append("\\u");

     for(int j=0;j< 4-hex.length(); j++) {
        unicode_str.append("0");
     }
     unicode_str.append(hex);
    // output_writer.write(unicode_str.toString() );
    // output_writer.write("<br>\n");
 }
 output_writer.write("</html>");
 output_writer.close();
 System.exit(0);
*/
