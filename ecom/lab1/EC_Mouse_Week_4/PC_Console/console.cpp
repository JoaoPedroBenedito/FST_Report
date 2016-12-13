/*********************************************************************
 *
 *                Example 02 - Run-time Linking
 *
 *********************************************************************
 * FileName:        console.cpp
 * Dependencies:    None
 * Compiler:        Borland C++ Builder 6
 * Company:         Copyright (C) 2004 by Microchip Technology, Inc.
 *
 * Software License Agreement
 *
 * The software supplied herewith by Microchip Technology Incorporated
 * (the �Company�) for its PICmicro� Microcontroller is intended and
 * supplied to you, the Company�s customer, for use solely and
 * exclusively on Microchip PICmicro Microcontroller products. The
 * software is owned by the Company and/or its supplier, and is
 * protected under applicable copyright laws. All rights are reserved.
 * Any use in violation of the foregoing restrictions may subject the
 * user to criminal sanctions under applicable laws, as well as to
 * civil liability for the breach of the terms and conditions of this
 * license.
 *
 * THIS SOFTWARE IS PROVIDED IN AN �AS IS� CONDITION. NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
 * TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
 * IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
 * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 * Author               Date        Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Rawin Rojvanit       11/19/04
 ********************************************************************/

//---------------------------------------------------------------------------

#pragma hdrstop

#include <stdio.h>
#include "windows.h"
#include "mpusbapi.h"                   // MPUSBAPI Header File

//---------------------------------------------------------------------------
#pragma argsused

//#include "stdafx.h"
//#include "GDI_CapturingAnImage.h"

// Global Vars
char vid_pid[]= "vid_04d8&pid_000c";    // Default Demo Application Firmware
char out_pipe[]= "\\MCHP_EP1";
char in_pipe[]= "\\MCHP_EP1";

DWORD temp;

HINSTANCE libHandle;
HANDLE myOutPipe;
HANDLE myInPipe;

//---------------------------------------------------------------------------
// Prototypes
void GetSummary(void);
void LoadDLL(void);
void GetDataFromPIC(void);
DWORD SendReceivePacket(BYTE *SendData, DWORD SendLength, BYTE *ReceiveData,
                    DWORD *ReceiveLength, UINT SendDelay, UINT ReceiveDelay);
void CheckInvalidHandle(void);
void ReadSPI(char spi_cmd, char spi_value);
void WriteRegister();
void GetDeltaXY(POINT * delta);
void MoveMouse();

//---------------------------------------------------------------------------

int main(int argc, char* argv[])
{
    BOOLEAN bQuit;
    DWORD selection;
    bQuit = false;
    POINT cursorPos, delta;

    // Load DLL when it is necessary, i.e. on start-up!
    LoadDLL();

    // Always a good idea to initialize the handles
    myOutPipe = myInPipe = INVALID_HANDLE_VALUE;

    printf("Microchip Technology Inc., 2004\r\n");
    printf("===============================\r\n");
    while(!bQuit)
    {
        printf("Select an option\r\n");
        printf("[1] Get this application version\r\n");
        printf("[2] List boards present\r\n");
        printf("[3] Read the registry 0x00\r\n");
        printf("[4] Quit\r\n");
        printf("[5] Change resolution\r\n>>");
        scanf("%d",&selection);

        switch(selection)
        {
            case 1:
                temp = MPUSBGetDLLVersion();
                printf("MPUSBAPI Version: %d.%d\r\n",HIWORD(temp),LOWORD(temp));
                break;
            case 2:
                GetSummary();
                break;
            case 3:
                GetDataFromPIC();
                break;
            case 4:
                bQuit = true;
                break;
            case 5:
                WriteRegister();
                break;
            case 6:
                GetCursorPos(&cursorPos);
                printf("%d, %d", cursorPos.x, cursorPos.y);
                break;
            case 7:
                GetDeltaXY(&delta);
                break;
            case 8:
                MoveMouse();
                break;
            default:
                break;
        }// end switch

        fflush(stdin);printf("\r\n");
    }//end while

    // Always check to close all handles before exiting!
    if (myOutPipe != INVALID_HANDLE_VALUE) MPUSBClose(myOutPipe);
    if (myInPipe != INVALID_HANDLE_VALUE) MPUSBClose(myInPipe);
    myOutPipe = myInPipe = INVALID_HANDLE_VALUE;

    // Always check to close the library too.
    if (libHandle != NULL) FreeLibrary(libHandle);

    return 0;
}//end main

//---------------------------------------------------------------------------

void GetSummary(void)
{
    HANDLE tempPipe = INVALID_HANDLE_VALUE;
    DWORD count = 0;
    DWORD max_count;

    max_count = MPUSBGetDeviceCount(vid_pid);

    printf("\r\n%d device(s) with %s currently attached\r\n",max_count,vid_pid);

    // Note:
    // The total number of devices using the generic driver could be
    // bigger than max_count. They could have different vid & pid numbers.
    // This means if max_count is 2, the valid instance index do not
    // necessary have to be '0' and '1'.
    //
    // Below is a sample code for searching for all valid instance indexes.
    // MAX_NUM_MPUSB_DEV is defined in _mpusbapi.h

    count = 0;
    for(int i = 0; i < MAX_NUM_MPUSB_DEV; i++)
    {
        tempPipe = MPUSBOpen(i,vid_pid,NULL,MP_READ,0);
        if(tempPipe != INVALID_HANDLE_VALUE)
        {
            printf("Instance Index # %d\r\n",i);
            MPUSBClose(tempPipe);
            count++;
        }
        if(count == max_count) break;
    }//end for
    printf("\r\n");
}//end GetSummary

//---------------------------------------------------------------------------

void LoadDLL(void)
{
    libHandle = NULL;
    libHandle = LoadLibrary("mpusbapi");
    if(libHandle == NULL)
    {
        printf("Error loading mpusbapi.dll\r\n");
    }
    else
    {
        MPUSBGetDLLVersion=(DWORD(*)(void))\
                    GetProcAddress(libHandle,"_MPUSBGetDLLVersion");
        MPUSBGetDeviceCount=(DWORD(*)(PCHAR))\
                    GetProcAddress(libHandle,"_MPUSBGetDeviceCount");
        MPUSBOpen=(HANDLE(*)(DWORD,PCHAR,PCHAR,DWORD,DWORD))\
                    GetProcAddress(libHandle,"_MPUSBOpen");
        MPUSBWrite=(DWORD(*)(HANDLE,PVOID,DWORD,PDWORD,DWORD))\
                    GetProcAddress(libHandle,"_MPUSBWrite");
        MPUSBRead=(DWORD(*)(HANDLE,PVOID,DWORD,PDWORD,DWORD))\
                    GetProcAddress(libHandle,"_MPUSBRead");
        MPUSBReadInt=(DWORD(*)(HANDLE,PVOID,DWORD,PDWORD,DWORD))\
                    GetProcAddress(libHandle,"_MPUSBReadInt");
        MPUSBClose=(BOOL(*)(HANDLE))GetProcAddress(libHandle,"_MPUSBClose");
        MPUSBSetConfiguration=(DWORD(*)(HANDLE,USHORT))\
                    GetProcAddress(libHandle,"_MPUSBSetConfiguration");
        MPUSBGetStringDescriptor = \
                    (DWORD(*)(HANDLE,UCHAR,USHORT,PVOID,DWORD,PDWORD))\
                    GetProcAddress(libHandle,"_MPUSBGetStringDescriptor");
        MPUSBGetConfigurationDescriptor = \
                   (DWORD(*)(HANDLE,UCHAR,PVOID,DWORD,PDWORD))\
                   GetProcAddress(libHandle,"_MPUSBGetConfigurationDescriptor");
        MPUSBGetDeviceDescriptor = (DWORD(*)(HANDLE,PVOID,DWORD,PDWORD))\
                   GetProcAddress(libHandle,"_MPUSBGetDeviceDescriptor");

        if((MPUSBGetDeviceCount == NULL) || (MPUSBOpen == NULL) ||
            (MPUSBWrite == NULL) || (MPUSBRead == NULL) ||
            (MPUSBClose == NULL) || (MPUSBGetDLLVersion == NULL) ||
            (MPUSBReadInt == NULL) || (MPUSBSetConfiguration == NULL) ||
            (MPUSBGetStringDescriptor == NULL) ||
            (MPUSBGetConfigurationDescriptor == NULL) ||
            (MPUSBGetDeviceDescriptor == NULL))
            printf("GetProcAddress Error\r\n");
    }//end if else
}//end LoadDLL

//---------------------------------------------------------------------------

void GetDataFromPIC(void)
{
    // First we need to open data pipes...
    DWORD selection;
    selection = 0; // Assumes only one board is connected to PC through USB and it has index 0
    fflush(stdin);

    myOutPipe = MPUSBOpen(selection,vid_pid,out_pipe,MP_WRITE,0);
    myInPipe = MPUSBOpen(selection,vid_pid,out_pipe,MP_READ,0);
    if(myOutPipe == INVALID_HANDLE_VALUE || myInPipe == INVALID_HANDLE_VALUE)
    {
        printf("Failed to open data pipes.\r\n");
        return;
    }//end if


    // This Computer Electronics Course Demo has a simple communication protocol.
    // To read the status of switches S2 and S3 one has to send the MY_COMMAND command
    // which is defined as 0x00, follows by the length of the
    // expected result, in this case is 2 bytes, S2 status, and S3 status.
    // i.e. <MY_COMMAND><0x02>
    //
    // The response packet from the board has the following format:
    // <MY_COMMAND><0x02><S2><S3>

    // The receive buffer size must be equal to or larger than the maximum
    // endpoint size it is communicating with. In this case, 64 bytes.

    BYTE send_buf[64],receive_buf[64];
    DWORD RecvLength=4;

    #define MY_COMMAND 0
    send_buf[0] = MY_COMMAND;      // CommWriteWithSPIHalfDuplex(char address, char data)and
    send_buf[1] = 0x02;            // Expected length of the result

    if(SendReceivePacket(send_buf,2,receive_buf,&RecvLength,1000,1000) == 1)
    {
        if(RecvLength == 4 && receive_buf[0] == MY_COMMAND &&
            receive_buf[1] == 0x02)
        {
            printf("%d", receive_buf[2]);
        }
    }
    else
        printf("USB Operation Failed\r\n");

    // Let's close the data pipes since we have nothing left to do..
    MPUSBClose(myOutPipe);
    MPUSBClose(myInPipe);
    myOutPipe = myInPipe = INVALID_HANDLE_VALUE;

}//end GetUSBDemoFWVersion

void GetDeltaXY(POINT * delta)
{
    // First we need to open data pipes...
    DWORD selection;
    selection = 0; // Assumes only one board is connected to PC through USB and it has index 0
    fflush(stdin);

    myOutPipe = MPUSBOpen(selection,vid_pid,out_pipe,MP_WRITE,0);
    myInPipe = MPUSBOpen(selection,vid_pid,out_pipe,MP_READ,0);
    if(myOutPipe == INVALID_HANDLE_VALUE || myInPipe == INVALID_HANDLE_VALUE)
    {
        printf("Failed to open data pipes.\r\n");
        return;
    }//end if


    // This Computer Electronics Course Demo has a simple communication protocol.
    // To read the status of switches S2 and S3 one has to send the MY_COMMAND command
    // which is defined as 0x00, follows by the length of the
    // expected result, in this case is 2 bytes, S2 status, and S3 status.
    // i.e. <MY_COMMAND><0x02>
    //
    // The response packet from the board has the following format:
    // <MY_COMMAND><0x02><S2><S3>

    // The receive buffer size must be equal to or larger than the maximum
    // endpoint size it is communicating with. In this case, 64 bytes.

    BYTE send_buf[64],receive_buf[64];
    DWORD RecvLength=4;

    #define MY_COMMAND 0
    send_buf[0] = MY_COMMAND;      // CommWriteWithSPIHalfDuplex(char address, char data)and
    send_buf[1] = 0x02;            // Expected length of the result

    if(SendReceivePacket(send_buf,2,receive_buf,&RecvLength,1000,1000) == 1)
    {
        if(RecvLength == 4 && receive_buf[0] == MY_COMMAND &&
            receive_buf[1] == 0x02)
        {
            delta->x = (signed char) receive_buf[2];
            delta->y = (signed char) receive_buf[3];

            printf("dX: %d,\t ", delta->x);
            printf("dY: %d\n", delta->y);
        }
    }
    else
        printf("USB Operation Failed\r\n");

    // Let's close the data pipes since we have nothing left to do..
    MPUSBClose(myOutPipe);
    MPUSBClose(myInPipe);
    myOutPipe = myInPipe = INVALID_HANDLE_VALUE;

}//end GetUSBDemoFWVersion

void WriteRegister(void)
{
    // First we need to open data pipes...
    DWORD selection;
    selection = 0; // Assumes only one board is connected to PC through USB and it has index 0
    fflush(stdin);

    myOutPipe = MPUSBOpen(selection,vid_pid,out_pipe,MP_WRITE,0);
    myInPipe = MPUSBOpen(selection,vid_pid,out_pipe,MP_READ,0);
    if(myOutPipe == INVALID_HANDLE_VALUE || myInPipe == INVALID_HANDLE_VALUE)
    {
        printf("Failed to open data pipes.\r\n");
        return;
    }//end if


    // This Computer Electronics Course Demo has a simple communication protocol.
    // To read the status of switches S2 and S3 one has to send the MY_COMMAND command
    // which is defined as 0x00, follows by the length of the
    // expected result, in this case is 2 bytes, S2 status, and S3 status.
    // i.e. <MY_COMMAND><0x02>
    //
    // The response packet from the board has the following format:
    // <MY_COMMAND><0x02><S2><S3>

    // The receive buffer size must be equal to or larger than the maximum
    // endpoint size it is communicating with. In this case, 64 bytes.

    BYTE send_buf[64],receive_buf[64];
    DWORD RecvLength=4;

    #define MY_COMMAND1 1
    send_buf[0] = MY_COMMAND1;      // CommWriteWithSPIHalfDuplex(char address, char data)and
    send_buf[1] = 0x02;            // Expected length of the result

    if(SendReceivePacket(send_buf,2,receive_buf,&RecvLength,1000,1000) == 1)
    {
        if(RecvLength == 4 && receive_buf[0] == MY_COMMAND1 &&
            receive_buf[1] == 0x02)
        {
            printf("done\n");
        }
    }
    else
        printf("USB Operation Failed\r\n");

    // Let's close the data pipes since we have nothing left to do..
    MPUSBClose(myOutPipe);
    MPUSBClose(myInPipe);
    myOutPipe = myInPipe = INVALID_HANDLE_VALUE;

}//end GetUSBDemoFWVersion
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////////////////
//
// A typical application would send a command to the target device and expect
// a response.
// SendReceivePacket is a wrapper function that facilitates the
// send command / read response paradigm
//
// SendData - pointer to data to be sent
// SendLength - length of data to be sent
// ReceiveData - Points to the buffer that receives the data read from the call
// ReceiveLength - Points to the number of bytes read
// SendDelay - time-out value for MPUSBWrite operation in milliseconds
// ReceiveDelay - time-out value for MPUSBRead operation in milliseconds
//

DWORD SendReceivePacket(BYTE *SendData, DWORD SendLength, BYTE *ReceiveData,
                    DWORD *ReceiveLength, UINT SendDelay, UINT ReceiveDelay)
{
    DWORD SentDataLength;
    DWORD ExpectedReceiveLength = *ReceiveLength;

    if(myOutPipe != INVALID_HANDLE_VALUE && myInPipe != INVALID_HANDLE_VALUE)
    {
        if(MPUSBWrite(myOutPipe,SendData,SendLength,&SentDataLength,SendDelay))
        {

            if(MPUSBRead(myInPipe,ReceiveData, ExpectedReceiveLength,
                        ReceiveLength,ReceiveDelay))
            {
                if(*ReceiveLength == ExpectedReceiveLength)
                {
                    return 1;   // Success!
                }
                else if(*ReceiveLength < ExpectedReceiveLength)
                {
                    return 2;   // Partially failed, incorrect receive length
                }//end if else
            }
            else
                CheckInvalidHandle();
        }
        else
            CheckInvalidHandle();
    }//end if

    return 0;  // Operation Failed
}//end SendReceivePacket

//---------------------------------------------------------------------------

void CheckInvalidHandle(void)
{
    if(GetLastError() == ERROR_INVALID_HANDLE)
    {
        // Most likely cause of the error is the board was disconnected.
        MPUSBClose(myOutPipe);
        MPUSBClose(myInPipe);
        myOutPipe = myInPipe = INVALID_HANDLE_VALUE;
    }//end if
    else
        printf("Error Code \r\n",GetLastError());
}//end CheckInvalidHandle

void MoveMouse(void)
{
    POINT cursorPos, delta;

    for(;;)
    {
        GetCursorPos(&cursorPos);
        GetDeltaXY(&delta);
        SetCursorPos(cursorPos.x - delta.x, cursorPos.y + delta.y);
    }
}

/*
void DrawCapturedImage(void)
{
    int d = 10;
    int cor = 10;
    int x = 0;
    int y = 0;
    HDC hdcScreen = GetDC(NULL);
    HDC MemDCExercising = CreateCompatibleDC(hdcScreen);
    HBITMAP bm = CreateCompatibleBitmap( hdcScreen, 15*d,15*d);
    SelectObject(MemDCExercising, bm);
    HBRUSH hBrush = CreateSolidBrush(RGB(cor,cor,cor));
    SelectObject(MemDCExercising, hBrush);
    HPEN hPen = CreatePen(PS_SOLID ,1,RGB(cor,cor,cor));
    SelectObject(MemDCExercising, hPen);
    Rectangle(MemDCExercising, x*d,y*d,(x+1)*d,(y+1)*d);
    BitBlt(hdcScreen, 0, 0, 15*d, 15*d, MemDCExercising, 0, 0, SRCCOPY);
    DeleteObject(hBrush);
    DeleteObject(hPen);
    DeleteObject(bm);
    DeleteDC(MemDCExercising);
}*/
//---------------------------------------------------------------------------
