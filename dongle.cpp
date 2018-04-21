
#ifndef __DONGLE_H_
#define  __DONGLE_H_

#include "dongle.h"

#include <string>

#ifdef _WINDOWS
QString diskID(QChar driveLetter) {

    wchar_t drive[4] = L"C:\\";

    drive[0] = QString(driveLetter).toStdString().c_str()[0];

    DWORD dwVolSerial;
    BOOL bIsRetrieved;
    bIsRetrieved = GetVolumeInformation(L"C:\\", NULL, NULL, &dwVolSerial, NULL, NULL, NULL, NULL);
    QString hardDiskSerial = QString::number(dwVolSerial, 16).toUpper();
    QString hashID = QCryptographicHash::hash(hardDiskSerial.toAscii().constData()  ,QCryptographicHash::Sha1 ).toBase64();
    return hashID;
}


QString createSignature() {

      QChar drive = getInstallableDriveName();
      if( drive== QChar(' ')) return QString("");

      writeFileNames(drive);

      QString fingerPrint = computeSignature();
      return fingerPrint;
}


QString computeSignature() {

    QChar drive = getInstallableDriveName();
    if( drive== QChar(' ')) return QString("");

    QString diskdriveID = diskID(drive);
    QString concatenated =  readFiles(drive);
  //  concatenated.append(diskdriveID);
    QString fingerPrint = QCryptographicHash::hash(concatenated.toAscii().constData()  ,QCryptographicHash::Sha1 ).toBase64();
    return fingerPrint;
}

QChar getInstallableDriveName() {


    DWORD dwVolSerial;
    BOOL bIsRetrieved;

    wchar_t drive[4] = L"C:\\";

    for(int i = 0; i < 10; i++ ) {
        drive[0] = drive[0] + 1;


        dwVolSerial=0;
        bIsRetrieved = GetVolumeInformation(drive, NULL, NULL, &dwVolSerial, NULL, NULL, NULL, NULL);
        if( dwVolSerial != 0 ) {
          // qDebug() << QString::fromWCharArray(drive) <<"  " <<  hardDiskSerial;
           QFile file( QString::fromWCharArray(drive) + "\\LuitPad.txt");
           if (file.open(QIODevice::ReadOnly | QIODevice::Text))
               return( QChar(drive[0]));

        }
    }

    return(QChar(' '));

}



void loadFileNames(QHash<QString, unsigned int> &FileName) {

    FileName[QString("wmvdmoe2.dll")] = 101472;
    FileName[QString("vxblock.dll")] = 100848;
    FileName[QString("modex.dll")] = 10112;
    FileName[QString("txflog.dll")] = 101376;
    FileName[QString("cscdll.dll")] = 101888;
    FileName[QString("gpkcsp.dll")] = 101888;
    FileName[QString("nvwimg.dll")] = 109904;
    FileName[QString("hccutils.dll")] = 102400;
    FileName[QString("rcbdyctl.dll")] = 102400;
    FileName[QString("win32spl.dll")] = 102400;
    FileName[QString("wmpshell.dll")] = 102400;
    FileName[QString("lprhelp.dll")] = 10240;
    FileName[QString("mcd32.dll")] = 10240;
    FileName[QString("panmap.dll")] = 10240;
    FileName[QString("scriptpw.dll")] = 10240;
    FileName[QString("usrv42a.dll")] = 102457;
    FileName[QString("dpcdll.dll")] = 102912;
    FileName[QString("msaatext.dll")] = 102912;
    FileName[QString("dmsynth.dll")] = 103424;
    FileName[QString("mfcm100d.dll")] = 103760;
    FileName[QString("ncscolib.dll")] = 1043288;
    FileName[QString("dmusic.dll")] = 104448;
    FileName[QString("mfc71u.dll")] = 1047552;
    FileName[QString("mcdsrv32.dll")] = 10496;
    FileName[QString("PresentationCFFRasterizerNative_v0300.dll")] = 105016;
    FileName[QString("mfcm100ud.dll")] = 105296;
    FileName[QString("WMNetmgr.dll")] = 1053696;
    FileName[QString("danim.dll")] = 1054208;
    FileName[QString("p2pgasvc.dll")] = 105472;
    FileName[QString("polstore.dll")] = 105472;
    FileName[QString("dmstyle.dll")] = 105984;
    FileName[QString("msoert2.dll")] = 105984;
    FileName[QString("mfc71.dll")] = 1060864;
    FileName[QString("mmcfxcommon.dll")] = 106496;
    FileName[QString("odbccp32.dll")] = 106496;
    FileName[QString("vpncategories.dll")] = 10680;
    FileName[QString("oleprn.dll")] = 107008;
    FileName[QString("GEARAspi.dll")] = 107368;
    FileName[QString("rend.dll")] = 107520;
    FileName[QString("c_iscii.dll")] = 10752;
    FileName[QString("mqcertui.dll")] = 10752;
    FileName[QString("pschdprf.dll")] = 10752;
    FileName[QString("smtpapi.dll")] = 10752;
    FileName[QString("FontInstaller2.dll")] = 108504;
    FileName[QString("wshbth.dll")] = 108032;
    FileName[QString("esent.dll")] = 1082368;
    FileName[QString("netapi.dll")] = 108464;
    FileName[QString("PICEntry.dll")] = 108704;
    FileName[QString("nvcuda.dll")] = 109536;
    FileName[QString("dbnetlib.dll")] = 110592;
    FileName[QString("EEBDSCVR.dll")] = 110592;
    FileName[QString("inetcplc.dll")] = 110592;
    FileName[QString("dgnet.dll")] = 111104;
    FileName[QString("wiavideo.dll")] = 111104;
    FileName[QString("esent97.dll")] = 114896;
    FileName[QString("wmsdmoe2.dll")] = 119744;
    FileName[QString("mapi32.dll")] = 112128;
    FileName[QString("mapistub.dll")] = 112128;
    FileName[QString("icaapi.dll")] = 11264;
    FileName[QString("icardres.dll")] = 11264;
    FileName[QString("msrle32.dll")] = 11264;
    FileName[QString("wshrm.dll")] = 11264;
    FileName[QString("dfshim.dll")] = 110824;
    FileName[QString("dsuiext.dll")] = 113152;

}



void writeFileNames(QChar drive ) {

    QHash<QString, unsigned int> FileNames;
    loadFileNames(FileNames);

    QStringList fileNames =  FileNames.keys();

    QString dir = QString(drive) + QString(":\\");


    QTime midnight(0, 0, 0);
    qsrand(midnight.secsTo(QTime::currentTime()));

    unsigned int fileSize = 0;
    unsigned int data ;
    using namespace std ;
    random_shuffle(FileNames.begin(),FileNames.end());


    foreach(QString fileName, fileNames) {
       QFile file( dir + fileName);
       if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
           QDataStream out(&file);
           out.setVersion(QDataStream::Qt_4_1);
         //  out << quint32(0x12345678) << quint64("hello");
           fileSize = 0;
           while( FileNames.value(fileName) > fileSize ) {
               data =  quint64(rand());
               fileSize += sizeof(data );
               out << data;
           }
           //out << fileContent;

           file.close();
       }

    }
}


QString readFiles(QChar drive) {

    QHash<QString, unsigned int> FileNames;
    loadFileNames(FileNames);

    QStringList fileNames =  FileNames.keys();

    QString dir = QString(drive) + QString(":\\luitpad_libs\\");
    QDir qDir;
    //qDir.setPath(dir);
    if( !qDir.exists(dir) ) {
         dir = QString(drive) + QString(":\\");
    }


    QTime midnight(0, 0, 0);
    qsrand(midnight.secsTo(QTime::currentTime()));

    QString concatenate;

    foreach(QString fileName, fileNames) {
        concatenate.append(getNumberForFile(dir + fileName));
    }
    return concatenate;
}



QString getNumberForFile(QString filename) {

    using namespace std;

    wstring fileName = filename.toStdWString();

    // Get the file's extents
    DWORD bytesReturned;
    // Get a handle to the file
    HANDLE fileHandle = CreateFile(
                                       fileName.c_str(),
                                       GENERIC_READ | GENERIC_WRITE,
                                       FILE_SHARE_READ|FILE_SHARE_WRITE,
                                       NULL,
                                       OPEN_EXISTING,
                                       FILE_ATTRIBUTE_NORMAL,
                                       NULL
                                   );

\
    if (fileHandle == INVALID_HANDLE_VALUE)
    {
        return QString("x");
    }



    // Saying we want the begining of the file
    STARTING_VCN_INPUT_BUFFER inBuffer;
    inBuffer.StartingVcn.QuadPart = 0;

    int extentsBufferSize = 1024;

    // Creating a buffer for the extents information
    PRETRIEVAL_POINTERS_BUFFER lpRetrievalPointersBuffer =(PRETRIEVAL_POINTERS_BUFFER) malloc(extentsBufferSize);


    if (
            !DeviceIoControl(
                                fileHandle,
                                FSCTL_GET_RETRIEVAL_POINTERS,
                                &inBuffer,
                                sizeof(STARTING_VCN_INPUT_BUFFER),
                                lpRetrievalPointersBuffer,
                                extentsBufferSize,
                                &bytesReturned,
                                NULL
                             )
      )
    {
        return QString("x");
    }

    // Got the file's extents !
    CloseHandle(fileHandle);


    // Send the first extent's LCN to translation to physical offset from the beginning of the disk
\

    char szTemp[100];
    sprintf(szTemp,"%ld",lpRetrievalPointersBuffer -> Extents [0].Lcn);

    free(lpRetrievalPointersBuffer);

    return QString(szTemp);
}
#endif

#endif
