#ifndef SECURITY_H
#define SECURITY_H
#include <stdio.h>
#include <string.h>
#include <QString>
#include <QCryptographicHash>
#include <QStringList>
#include <iostream>
using namespace std;


#ifdef _WIN32_DCOM
#include <intrin.h>
#include <comdef.h>
#include <Wbemidl.h>
#include <Windows.h>
#include <Iphlpapi.h>
#include <Assert.h>

#pragma comment(lib, "iphlpapi.lib")

QString cpuinfo();
QStringList GetMACaddress();
#endif

#endif // SECURITY_H
