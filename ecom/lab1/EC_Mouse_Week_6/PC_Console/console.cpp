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
 * (the “Company”) for its PICmicro® Microcontroller is intended and
 * supplied to you, the Company’s customer, for use solely and
 * exclusively on Microchip PICmicro Microcontroller products. The
 * software is owned by the Company and/or its supplier, and is
 * protected under applicable copyright laws. All rights are reserved.
 * Any use in violation of the foregoing restrictions may subject the
 * user to criminal sanctions under applicable laws, as well as to
 * civil liability for the breach of the terms and conditions of this
 * license.
 *
 * THIS SOFTWARE IS PROVIDED IN AN “AS IS” CONDITION. NO WARRANTIES,
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
#include "Windows.h"
#include "mpusbapi.h"                   // MPUSBAPI Header File
#include <conio.h>

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
// Defines
#define MOUSE_INFO 0x00
#define MOUSE_DETECT_MOTION 0x02
#define MOUSE_DELTA_X 0x03
#define MOUSE_DELTA_Y 0x04
#define MOUSE_SENSOR_IO 0x0b
#define MOUSE_CONTROL_REG 0x0d

#define READ 0
#define WRITE 1

//---------------------------------------------------------------------------
// Prototypes
void GetSummary(void);
void LoadDLL(void);
DWORD SendReceivePacket(BYTE *SendData, DWORD SendLength, BYTE *ReceiveData,
                    DWORD *ReceiveLength, UINT SendDelay, UINT ReceiveDelay);
void CheckInvalidHandle(void);
void SendReceiveData(int send_or_receive, int size_of_answer,
    BYTE destination_register, BYTE * receive_buf, BYTE data_to_write);
void FuncMouse();

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
        printf("[3] Use mouse\r\n");
        printf("[4] Quit\r\n>>");
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
                FuncMouse();
                break;
            case 4:
                bQuit = true;
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

void SendReceiveData(int send_or_receive, int size_of_answer,
    BYTE destination_register, BYTE * receive_buf, BYTE data_to_write = 0x00)
{
    DWORD selection = 0;
    fflush(stdin);
    int size_of_message = (send_or_receive == READ) ? 3 : 4;

    myOutPipe = MPUSBOpen(selection, vid_pid, out_pipe, MP_WRITE, 0);
    myInPipe = MPUSBOpen(selection, vid_pid, out_pipe, MP_READ, 0);
    if(myOutPipe == INVALID_HANDLE_VALUE || myInPipe == INVALID_HANDLE_VALUE)
    {
        printf("Failed to open data pipes.\n");
        return;
    }

    BYTE send_buf[64];
    DWORD RecvLength = size_of_answer;

    send_buf[0] = send_or_receive;
    send_buf[1] = size_of_answer;
    send_buf[2] = destination_register;

    if(send_or_receive == WRITE)
        send_buf[3] = data_to_write;

    if(SendReceivePacket(send_buf, size_of_message, receive_buf, &RecvLength, 1000, 1000) == 1
        && !(RecvLength == size_of_answer && receive_buf[0] == send_or_receive &&
            receive_buf[1] == size_of_answer && receive_buf[2] == destination_register))
    {
            printf("[ERROR] USB Operation Failed!\n");
    }

    MPUSBClose(myOutPipe);
    MPUSBClose(myInPipe);
    myOutPipe = myInPipe = INVALID_HANDLE_VALUE;
}

void FuncMouse(void)
{
    POINT cursorPos, delta;
    BYTE receive_buf[64];
    int image[225], aux;
    int teste[15][15];

    #define SCALE 15

    HDC hdcScreen = GetDC(NULL);
    HDC MemDCExercising = CreateCompatibleDC(hdcScreen);
    HBITMAP bm = CreateCompatibleBitmap(hdcScreen, 15 * SCALE, 15 * SCALE);
    SelectObject(MemDCExercising, bm);

    HBRUSH hBrush;
    HPEN hPen;

    SendReceiveData(WRITE, 4, MOUSE_CONTROL_REG, receive_buf, 1);

    while(!kbhit())
    {
        for(int i = 0; i < 4; i++)
        {
            if(i != 3)
                SendReceiveData(READ, 64, MOUSE_SENSOR_IO, receive_buf);
            else
                SendReceiveData(2, 47, MOUSE_SENSOR_IO, receive_buf);

            for(int j = 3; j < 64; j++)
            {
                //image[aux] = (receive_buf[j] & 0x80) * 2;
                image[aux] = 2 * receive_buf[j];
                aux++;
                if(aux == 225) break;
            }
        }

        GetCursorPos(&cursorPos);

        delta.x = (signed char) receive_buf[45];
        delta.y = (signed char) receive_buf[46];

        SetCursorPos(cursorPos.x - delta.x, cursorPos.y + delta.y);

        aux = 0;
        SendReceiveData(WRITE, 4, MOUSE_SENSOR_IO, receive_buf);

        // draw image on the screen
        for(int i = 0; i < 225; i++)
        {
            int x = i / 15;
            int y = 14 - (i % 15);
            int color = image[i];

            hBrush = CreateSolidBrush(RGB(color, color, color));
            SelectObject(MemDCExercising, hBrush);
            hPen = CreatePen(PS_SOLID, 1, RGB(color, color, color));
            SelectObject(MemDCExercising, hPen);
            Rectangle(MemDCExercising, x * SCALE, y * SCALE, (x + 1) * SCALE, (y + 1) * SCALE);
            DeleteObject(hBrush);
            DeleteObject(hPen);
        }

        BitBlt(hdcScreen, 0, 0, 15 * SCALE, 15 * SCALE, MemDCExercising, 0, 0, SRCCOPY);

    }

    DeleteObject(bm);
    DeleteDC(MemDCExercising);

}


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
        // Most likely cause of the error is //the board was disconnected.
        MPUSBClose(myOutPipe);
        MPUSBClose(myInPipe);
        myOutPipe = myInPipe = INVALID_HANDLE_VALUE;
    }//end if
    else
        printf("Error Code \r\n",GetLastError());
}


/*-------------------------------------------------------------------------*/
