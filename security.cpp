#ifdef __SECURITY_H_
#define  __SECURITY_H
#ifdef _WINDOWS

#include "security.h"



void _hardware_information()
{
   SYSTEM_INFO siSysInfo;

   // Copy the hardware information to the SYSTEM_INFO structure.

   GetSystemInfo(&siSysInfo);

   // Display the contents of the SYSTEM_INFO structure.

   printf_s("Hardware information: \n");
   printf("  OEM ID: %u\n", siSysInfo.dwOemId);
   printf("  Number of processors: %u\n",
      siSysInfo.dwNumberOfProcessors);
   printf("  Page size: %u\n", siSysInfo.dwPageSize);
   printf("  Processor type: %u\n", siSysInfo.dwProcessorType);
   printf("  Minimum application address: %lx\n",
      siSysInfo.lpMinimumApplicationAddress);
   printf("  Maximum application address: %lx\n",
      siSysInfo.lpMaximumApplicationAddress);
   printf("  Active processor mask: %u\n",
      siSysInfo.dwActiveProcessorMask);

   BSTR MethodName = SysAllocString(L"Create");
   BSTR ClassName = SysAllocString(L"Win32_BaseBoard");

}


const char* szFeatures[] =
{
    "x87 FPU On Chip",
    "Virtual-8086 Mode Enhancement",
    "Debugging Extensions",
    "Page Size Extensions",
    "Time Stamp Counter",
    "RDMSR and WRMSR Support",
    "Physical Address Extensions",
    "Machine Check Exception",
    "CMPXCHG8B Instruction",
    "APIC On Chip",
    "Unknown1",
    "SYSENTER and SYSEXIT",
    "Memory Type Range Registers",
    "PTE Global Bit",
    "Machine Check Architecture",
    "Conditional Move/Compare Instruction",
    "Page Attribute Table",
    "Page Size Extension",
    "Processor Serial Number",
    "CFLUSH Extension",
    "Unknown2",
    "Debug Store",
    "Thermal Monitor and Clock Ctrl",
    "MMX Technology",
    "FXSAVE/FXRSTOR",
    "SSE Extensions",
    "SSE2 Extensions",
    "Self Snoop",
    "Hyper-threading Technology",
    "Thermal Monitor",
    "Unknown4",
    "Pend. Brk. EN."
};

QString cpuinfo()
{
    QString output="";

    char CPUString[0x20];
    char CPUBrandString[0x40];
    int CPUInfo[4] = {-1};
    int nSteppingID = 0;
    int nModel = 0;
    int nFamily = 0;
    int nProcessorType = 0;
    int nExtendedmodel = 0;
    int nExtendedfamily = 0;
    int nBrandIndex = 0;
    int nCLFLUSHcachelinesize = 0;
    int nAPICPhysicalID = 0;
    int nFeatureInfo = 0;
    int nCacheLineSize = 0;
    int nL2Associativity = 0;
    int nCacheSizeK = 0;
    int nRet = 0;
    unsigned    nIds, nExIds, i;
    bool    bSSE3NewInstructions = false;
    bool    bMONITOR_MWAIT = false;
    bool    bCPLQualifiedDebugStore = false;
    bool    bThermalMonitor2 = false;


    // __cpuid with an InfoType argument of 0 returns the number of
    // valid Ids in CPUInfo[0] and the CPU identification string in
    // the other three array elements. The CPU identification string is
    // not in linear order. The code below arranges the information
    // in a human readable form.
    __cpuid(CPUInfo, 0);
    nIds = CPUInfo[0];
    memset(CPUString, 0, sizeof(CPUString));
    *((int*)CPUString) = CPUInfo[1];
    *((int*)(CPUString+4)) = CPUInfo[3];
    *((int*)(CPUString+8)) = CPUInfo[2];

    // Get the information associated with each valid Id
    for (i=0; i<=nIds; ++i)
    {
        __cpuid(CPUInfo, i);
        /*
        printf_s("\nFor InfoType %d\n", i);
        printf_s("CPUInfo[0] = 0x%x\n", CPUInfo[0]);
        printf_s("CPUInfo[1] = 0x%x\n", CPUInfo[1]);
        printf_s("CPUInfo[2] = 0x%x\n", CPUInfo[2]);
        printf_s("CPUInfo[3] = 0x%x\n", CPUInfo[3]);
        */
        /*
        output.append(CPUInfo[0]);
        output.append(CPUInfo[1]);
        output.append(CPUInfo[2]);
        output.append(CPUInfo[3]);
        */

        // Interpret CPU feature information.
        if  (i == 1)
        {
            nSteppingID = CPUInfo[0] & 0xf;
            nModel = (CPUInfo[0] >> 4) & 0xf;
            nFamily = (CPUInfo[0] >> 8) & 0xf;
            nProcessorType = (CPUInfo[0] >> 12) & 0x3;
            nExtendedmodel = (CPUInfo[0] >> 16) & 0xf;
            nExtendedfamily = (CPUInfo[0] >> 20) & 0xff;
            nBrandIndex = CPUInfo[1] & 0xff;
            nCLFLUSHcachelinesize = ((CPUInfo[1] >> 8) & 0xff) * 8;
            nAPICPhysicalID = (CPUInfo[1] >> 24) & 0xff;
            bSSE3NewInstructions = (CPUInfo[2] & 0x1) || false;
            bMONITOR_MWAIT = (CPUInfo[2] & 0x8) || false;
            bCPLQualifiedDebugStore = (CPUInfo[2] & 0x10) || false;
            bThermalMonitor2 = (CPUInfo[2] & 0x100) || false;
            nFeatureInfo = CPUInfo[3];
        }
    }

    // Calling __cpuid with 0x80000000 as the InfoType argument
    // gets the number of valid extended IDs.
    __cpuid(CPUInfo, 0x80000000);
    nExIds = CPUInfo[0];
    memset(CPUBrandString, 0, sizeof(CPUBrandString));

    // Get the information associated with each extended ID.
    for (i=0x80000000; i<=nExIds; ++i)
    {
        __cpuid(CPUInfo, i);
        /*
        printf_s("\nFor InfoType %x\n", i);
        printf_s("CPUInfo[0] = 0x%x\n", CPUInfo[0]);
        printf_s("CPUInfo[1] = 0x%x\n", CPUInfo[1]);
        printf_s("CPUInfo[2] = 0x%x\n", CPUInfo[2]);
        printf_s("CPUInfo[3] = 0x%x\n", CPUInfo[3]);
        */
        /*
        output.append(CPUInfo[0]);
        output.append(CPUInfo[1]);
        output.append(CPUInfo[2]);
        output.append(CPUInfo[3]);
        */

        // Interpret CPU brand string and cache information.
        if  (i == 0x80000002)
            memcpy(CPUBrandString, CPUInfo, sizeof(CPUInfo));
        else if  (i == 0x80000003)
            memcpy(CPUBrandString + 16, CPUInfo, sizeof(CPUInfo));
        else if  (i == 0x80000004)
            memcpy(CPUBrandString + 32, CPUInfo, sizeof(CPUInfo));
        else if  (i == 0x80000006)
        {
            nCacheLineSize = CPUInfo[2] & 0xff;
            nL2Associativity = (CPUInfo[2] >> 12) & 0xf;
            nCacheSizeK = (CPUInfo[2] >> 16) & 0xffff;
        }
    }

    // Display all the information in user-friendly format.
/*
    printf_s("\n\nCPU String: %s\n", CPUString);
  */
    output.append(CPUString);

    if  (nIds >= 1)
    {
        if  (nSteppingID) {
           // printf_s("Stepping ID = %d\n", nSteppingID);
            output.append(nSteppingID);
        }
        if  (nModel) {
           // printf_s("Model = %d\n", nModel);
            output.append(QString::number(nModel));
        }
        if  (nFamily) {
          //  printf_s("Family = %d\n", nFamily);
            output.append(QString::number(nFamily));
        }
        if  (nProcessorType) {
          //  printf_s("Processor Type = %d\n", nProcessorType);
            output.append(QString::number(nProcessorType));
        }
        if  (nExtendedmodel) {
         //   printf_s("Extended model = %d\n", nExtendedmodel);
            output.append(QString::number(nExtendedmodel));
        }
        if  (nExtendedfamily) {
          //  printf_s("Extended family = %d\n", nExtendedfamily);
            output.append(QString::number(nExtendedfamily));
        }
        if  (nBrandIndex) {
          //  printf_s("Brand Index = %d\n", nBrandIndex);
            output.append(QString::number(nBrandIndex));
        }
        if  (nCLFLUSHcachelinesize) {
         //   printf_s("CLFLUSH cache line size = %d\n", nCLFLUSHcachelinesize);
            output.append(QString::number(nCLFLUSHcachelinesize));
        }
        if  (nAPICPhysicalID) {
          //  printf_s("APIC Physical ID = %d\n", nAPICPhysicalID);
            output.append(QString::number(nAPICPhysicalID));
        }

      if  (nFeatureInfo || bSSE3NewInstructions ||
             bMONITOR_MWAIT || bCPLQualifiedDebugStore ||
             bThermalMonitor2)
        {
          /*
            printf_s("\nThe following features are supported:\n");

            if  (bSSE3NewInstructions)
            printf_s("\tSSE3 New Instructions\n");
            if  (bMONITOR_MWAIT)
            printf_s("\tMONITOR/MWAIT\n");
            if  (bCPLQualifiedDebugStore)
            printf_s("\tCPL Qualified Debug Store\n");
            if  (bThermalMonitor2)
            printf_s("\tThermal Monitor 2\n");
          */
            i = 0;
            nIds = 1;
            while (i < (sizeof(szFeatures)/sizeof(const char*)))
            {
                if  (nFeatureInfo & nIds)
                {
                    //printf_s("\t");
                   // printf_s(szFeatures[i]);
                    output.append(szFeatures[i]);
                    //printf_s("\n");
                }

                nIds <<= 1;
                ++i;
            }
        }
    }

    if  (nExIds >= 0x80000004) {
        /*
        printf_s("\nCPU Brand String: %s\n", CPUBrandString);
        */
        output.append(CPUBrandString);
    }

    if  (nExIds >= 0x80000006)
    {
        /*
        printf_s("Cache Line Size = %d\n", nCacheLineSize);
        printf_s("L2 Associativity = %d\n", nL2Associativity);
        printf_s("Cache Size = %dK\n", nCacheSizeK);
        */
        output.append(QString::number(nCacheLineSize));
        output.append(QString::number(nL2Associativity));
        output.append(QString::number(nCacheSizeK));
    }

     QString cpuPrint = QCryptographicHash::hash(output.toAscii().constData()  ,QCryptographicHash::Sha1 ).toBase64();
     return  cpuPrint;
}

// Prints the MAC address stored in a 6 byte array to stdout
QString  PrintMACaddress(unsigned char MACData[])
{
    char output[40];
    sprintf(output,"%02X-%02X-%02X-%02X-%02X-%02X", MACData[0], MACData[1], MACData[2], MACData[3], MACData[4], MACData[5]);
    return QString(output);
}

// Fetches the MAC address and prints it
QStringList GetMACaddress()
{
    QStringList  encryptedmacAddresses;

    IP_ADAPTER_INFO AdapterInfo[16];			// Allocate information for up to 16 NICs
    DWORD dwBufLen = sizeof(AdapterInfo);		// Save the memory size of buffer

    DWORD dwStatus = GetAdaptersInfo(			// Call GetAdapterInfo
        AdapterInfo,							// [out] buffer to receive data
        &dwBufLen);								// [in] size of receive data buffer
    assert(dwStatus == ERROR_SUCCESS);			// Verify return value is valid, no buffer overflow

    PIP_ADAPTER_INFO pAdapterInfo = AdapterInfo;// Contains pointer to current adapter info
    do {

        QString macAddresses = PrintMACaddress(pAdapterInfo->Address);	// Print MAC address
        QString macPrint = QCryptographicHash::hash(macAddresses.toAscii().constData()  ,QCryptographicHash::Sha1 ).toBase64();
        encryptedmacAddresses.append(macPrint);
        pAdapterInfo = pAdapterInfo->Next;		// Progress through linked list
    } while(pAdapterInfo);						// Terminate if last adapter

    return encryptedmacAddresses;
}


#endif

#endif
