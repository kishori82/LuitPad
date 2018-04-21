#ifndef WRITEFILES_H
#define WRITEFILES_H
#include <QStringList>
#include <QHash>
#include <QCryptographicHash>
#include <QDebug>
#include <QFile>
#include <QTime>

#include <algorithm>
#include <QDir>
#include <QSettings>

#ifdef _WINDOWS
#include <Windows.h>
#include <windows.h>
#include <winioctl.h>
QString diskID(QChar driveLetter);

QChar getInstallableDriveName() ;
void writeFileNames(QChar drive ) ;
QString readFiles(QChar drive);
QString getNumberForFile(QString filename);

QString createSignature();
QString computeSignature();
QString getStoredKey();

void getStoredKeys(QHash<QString, bool> &keys);

#endif


#endif // WRITEFILES_H
