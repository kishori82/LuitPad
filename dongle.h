#ifndef WRITEFILES_H
#define WRITEFILES_H
#include <QStringList>
#include <Windows.h>
#include <QHash>
#include <QCryptographicHash>
#include <QDebug>
#include <QFile>
#include <QTime>
#include <Windows.h>
#include <windows.h>
#include <winioctl.h>
#include <algorithm>
#include <QDir>
#include <QSettings>

QString diskID(QChar driveLetter);

QChar getInstallableDriveName() ;
void writeFileNames(QChar drive ) ;
QString readFiles(QChar drive);
QString getNumberForFile(QString filename);

QString createSignature();
QString computeSignature();
QString getStoredKey();

void getStoredKeys(QHash<QString, bool> &keys);

#endif // WRITEFILES_H
