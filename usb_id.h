#ifndef USB_ID_H
#define USB_ID_H
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <windows.h>
#include <winioctl.h>
#define PRINTING_TO_CONSOLE_ALLOWED


    //  special include from the MS DDK
//#include "c:\win2kddk\inc\ddk\ntddk.h"
//#include "c:\win2kddk\inc\ntddstor.h"


#define  TITLE   L"DiskId32"
//  Required to ensure correct PhysicalDrive IOCTL structure setup
#pragma pack(1)


#define  IDENTIFY_BUFFER_SIZE  512


//  IOCTL commands
#define  DFP_GET_VERSION          0x00074080
#define  DFP_SEND_DRIVE_COMMAND   0x0007c084
#define  DFP_RECEIVE_DRIVE_DATA   0x0007c088

#define  FILE_DEVICE_SCSI              0x0000001b
#define  IOCTL_SCSI_MINIPORT_IDENTIFY  ((FILE_DEVICE_SCSI << 16) + 0x0501)
#define  IOCTL_SCSI_MINIPORT 0x0004D008  //  see NTDDSCSI.H for definition

#define SMART_GET_VERSION               CTL_CODE(IOCTL_DISK_BASE, 0x0020, METHOD_BUFFERED, FILE_READ_ACCESS)
#define SMART_SEND_DRIVE_COMMAND        CTL_CODE(IOCTL_DISK_BASE, 0x0021, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)
#define SMART_RCV_DRIVE_DATA            CTL_CODE(IOCTL_DISK_BASE, 0x0022, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)

/*
typedef struct _GETVERSIONINPARAMS {
     UCHAR    bVersion;               // Binary driver version.
     UCHAR    bRevision;              // Binary driver revision.
     UCHAR    bReserved;              // Not used.
     UCHAR    bIDEDeviceMap;          // Bit map of IDE devices.
     ULONG   fCapabilities;          // Bit mask of driver capabilities.
     ULONG   dwReserved[4];          // For future use.
} GETVERSIONINPARAMS, *PGETVERSIONINPARAMS, *LPGETVERSIONINPARAMS;

*/
//  GETVERSIONOUTPARAMS contains the data returned from the
//  Get Driver Version function.
typedef struct _GETVERSIONOUTPARAMS
{
BYTE bVersion;      // Binary driver version.
BYTE bRevision;     // Binary driver revision.
BYTE bReserved;     // Not used.
BYTE bIDEDeviceMap; // Bit map of IDE devices.
DWORD fCapabilities; // Bit mask of driver capabilities.
DWORD dwReserved[4]; // For future use.
} GETVERSIONOUTPARAMS, *PGETVERSIONOUTPARAMS, *LPGETVERSIONOUTPARAMS;


//  Bits returned in the fCapabilities member of GETVERSIONOUTPARAMS
#define  CAP_IDE_ID_FUNCTION             1  // ATA ID command supported
#define  CAP_IDE_ATAPI_ID                2  // ATAPI ID command supported
#define  CAP_IDE_EXECUTE_SMART_FUNCTION  4  // SMART commannds supported

/*
//  IDE registers
typedef struct _IDEREGS
{
BYTE bFeaturesReg;       // Used for specifying SMART "commands".
BYTE bSectorCountReg;    // IDE sector count register
BYTE bSectorNumberReg;   // IDE sector number register
BYTE bCylLowReg;         // IDE low order cylinder value
BYTE bCylHighReg;        // IDE high order cylinder value
BYTE bDriveHeadReg;      // IDE drive/head register
BYTE bCommandReg;        // Actual IDE command.
BYTE bReserved;          // reserved for future use.  Must be zero.
} IDEREGS, *PIDEREGS, *LPIDEREGS;


//  SENDCMDINPARAMS contains the input parameters for the
//  Send Command to Drive function.
typedef struct _SENDCMDINPARAMS
{
DWORD     cBufferSize;   //  Buffer size in bytes
IDEREGS   irDriveRegs;   //  Structure with drive register values.
BYTE bDriveNumber;       //  Physical drive number to send
                         //  command to (0,1,2,3).
BYTE bReserved[3];       //  Reserved for future expansion.
DWORD     dwReserved[4]; //  For future use.
BYTE      bBuffer[1];    //  Input buffer.
} SENDCMDINPARAMS, *PSENDCMDINPARAMS, *LPSENDCMDINPARAMS;
*/

//  Valid values for the bCommandReg member of IDEREGS.
#define  IDE_ATAPI_IDENTIFY  0xA1  //  Returns ID sector for ATAPI.
#define  IDE_ATA_IDENTIFY    0xEC  //  Returns ID sector for ATA.

/*

// Status returned from driver
typedef struct _DRIVERSTATUS
{
BYTE  bDriverError;  //  Error code from driver, or 0 if no error.
BYTE  bIDEStatus;    //  Contents of IDE Error register.
                     //  Only valid when bDriverError is SMART_IDE_ERROR.
BYTE  bReserved[2];  //  Reserved for future expansion.
DWORD  dwReserved[2];  //  Reserved for future expansion.
} DRIVERSTATUS, *PDRIVERSTATUS, *LPDRIVERSTATUS;


// Structure returned by PhysicalDrive IOCTL for several commands
typedef struct _SENDCMDOUTPARAMS
{
DWORD         cBufferSize;   //  Size of bBuffer in bytes
DRIVERSTATUS  DriverStatus;  //  Driver status structure.
BYTE          bBuffer[1];    //  Buffer of arbitrary length in which to store the data read from the                                                       // drive.
} SENDCMDOUTPARAMS, *PSENDCMDOUTPARAMS, *LPSENDCMDOUTPARAMS;

*/
// The following struct defines the interesting part of the IDENTIFY
// buffer:
typedef struct _IDSECTOR
{
USHORT  wGenConfig;
USHORT  wNumCyls;
USHORT  wReserved;
USHORT  wNumHeads;
USHORT  wBytesPerTrack;
USHORT  wBytesPerSector;
USHORT  wSectorsPerTrack;
USHORT  wVendorUnique[3];
CHAR    sSerialNumber[20];
USHORT  wBufferType;
USHORT  wBufferSize;
USHORT  wECCSize;
CHAR    sFirmwareRev[8];
CHAR    sModelNumber[40];
USHORT  wMoreVendorUnique;
USHORT  wDoubleWordIO;
USHORT  wCapabilities;
USHORT  wReserved1;
USHORT  wPIOTiming;
USHORT  wDMATiming;
USHORT  wBS;
USHORT  wNumCurrentCyls;
USHORT  wNumCurrentHeads;
USHORT  wNumCurrentSectorsPerTrack;
ULONG   ulCurrentSectorCapacity;
USHORT  wMultSectorStuff;
ULONG   ulTotalAddressableSectors;
USHORT  wSingleWordDMA;
USHORT  wMultiWordDMA;
BYTE    bReserved[128];
} IDSECTOR, *PIDSECTOR;


typedef struct _SRB_IO_CONTROL
{
ULONG HeaderLength;
UCHAR Signature[8];
ULONG Timeout;
ULONG ControlCode;
ULONG ReturnCode;
ULONG Length;
} SRB_IO_CONTROL, *PSRB_IO_CONTROL;


// Define global buffers.


void PrintIdeInfo (int drive, DWORD diskdata [256]);
BOOL DoIDENTIFY(HANDLE, PSENDCMDINPARAMS, PSENDCMDOUTPARAMS, BYTE, BYTE,
                 PDWORD);


   //  Max number of drives assuming primary/secondary, master/slave topology
#define  MAX_IDE_DRIVES  16

char *ConvertToString (DWORD diskdata [256],
            int firstIndex,
            int lastIndex,
            char* buf);
int ReadPhysicalDriveInNTWithAdminRights (void);
DWORD GetMACaddress(void);

//
// IDENTIFY data (from ATAPI driver source)
//

#pragma pack(1)

typedef struct _IDENTIFY_DATA {
    USHORT GeneralConfiguration;            // 00 00
    USHORT NumberOfCylinders;               // 02  1
    USHORT Reserved1;                       // 04  2
    USHORT NumberOfHeads;                   // 06  3
    USHORT UnformattedBytesPerTrack;        // 08  4
    USHORT UnformattedBytesPerSector;       // 0A  5
    USHORT SectorsPerTrack;                 // 0C  6
    USHORT VendorUnique1[3];                // 0E  7-9
    USHORT SerialNumber[10];                // 14  10-19
    USHORT BufferType;                      // 28  20
    USHORT BufferSectorSize;                // 2A  21
    USHORT NumberOfEccBytes;                // 2C  22
    USHORT FirmwareRevision[4];             // 2E  23-26
    USHORT ModelNumber[20];                 // 36  27-46
    UCHAR  MaximumBlockTransfer;            // 5E  47
    UCHAR  VendorUnique2;                   // 5F
    USHORT DoubleWordIo;                    // 60  48
    USHORT Capabilities;                    // 62  49
    USHORT Reserved2;                       // 64  50
    UCHAR  VendorUnique3;                   // 66  51
    UCHAR  PioCycleTimingMode;              // 67
    UCHAR  VendorUnique4;                   // 68  52
    UCHAR  DmaCycleTimingMode;              // 69
    USHORT TranslationFieldsValid:1;        // 6A  53
    USHORT Reserved3:15;
    USHORT NumberOfCurrentCylinders;        // 6C  54
    USHORT NumberOfCurrentHeads;            // 6E  55
    USHORT CurrentSectorsPerTrack;          // 70  56
    ULONG  CurrentSectorCapacity;           // 72  57-58
    USHORT CurrentMultiSectorSetting;       //     59
    ULONG  UserAddressableSectors;          //     60-61
    USHORT SingleWordDMASupport : 8;        //     62
    USHORT SingleWordDMAActive : 8;
    USHORT MultiWordDMASupport : 8;         //     63
    USHORT MultiWordDMAActive : 8;
    USHORT AdvancedPIOModes : 8;            //     64
    USHORT Reserved4 : 8;
    USHORT MinimumMWXferCycleTime;          //     65
    USHORT RecommendedMWXferCycleTime;      //     66
    USHORT MinimumPIOCycleTime;             //     67
    USHORT MinimumPIOCycleTimeIORDY;        //     68
    USHORT Reserved5[2];                    //     69-70
    USHORT ReleaseTimeOverlapped;           //     71
    USHORT ReleaseTimeServiceCommand;       //     72
    USHORT MajorRevision;                   //     73
    USHORT MinorRevision;                   //     74
    USHORT Reserved6[50];                   //     75-126
    USHORT SpecialFunctionsEnabled;         //     127
    USHORT Reserved7[128];                  //     128-255
} IDENTIFY_DATA, *PIDENTIFY_DATA;

#pragma pack()



int ReadPhysicalDriveInNTUsingSmart (void);

//  ---------------------------------------------------

   // (* Output Bbuffer for the VxD (rt_IdeDinfo record) *)
typedef struct _rt_IdeDInfo_
{
    BYTE IDEExists[4];
    BYTE DiskExists[8];
    WORD DisksRawInfo[8*256];
} rt_IdeDInfo, *pt_IdeDInfo;


   // (* IdeDinfo "data fields" *)
typedef struct _rt_DiskInfo_
{
   BOOL DiskExists;
   BOOL ATAdevice;
   BOOL RemovableDevice;
   WORD TotLogCyl;
   WORD TotLogHeads;
   WORD TotLogSPT;
   char SerialNumber[20];
   char FirmwareRevision[8];
   char ModelNumber[40];
   WORD CurLogCyl;
   WORD CurLogHeads;
   WORD CurLogSPT;
} rt_DiskInfo;


#define  m_cVxDFunctionIdesDInfo  1
int ReadDrivePortsInWin9X (void);
long getHardDriveComputerID ();
//  ---------------------------------------------------
#endif // USB_ID_H
