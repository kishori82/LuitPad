#ifndef SECURITY_H
#define SECURITY_H
#include <stdio.h>
#include <string.h>
#include <intrin.h>

#define _WIN32_DCOM

#include <iostream>
using namespace std;
#include <comdef.h>
#include <Wbemidl.h>
#include <QString>
#include <QCryptographicHash>
#include <QStringList>

#include <Windows.h>
#include <Iphlpapi.h>
#include <Assert.h>
#pragma comment(lib, "iphlpapi.lib")

QString cpuinfo();
QStringList GetMACaddress();
#endif // SECURITY_H
