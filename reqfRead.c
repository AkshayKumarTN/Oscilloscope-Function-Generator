/*
===============================================================================
 File Name   : reqfRead.c
 Description : Handles FTDI read device operations for Morse Code project
 Author      : Akshay Kumar Talur Narasimmulu
===============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ftd2xx.h"
#include "reqfRead.h"
#include "reqfWrite.h"  // for morse_code reference

void initReadDevice(reqfRead *dev, int deviceIndex) {
    dev->status = FT_Open(deviceIndex, &dev->handle);
    if (dev->status != FT_OK) {
        fprintf(stderr, "Error: Unable to open FTDI read device %d\n", deviceIndex);
        exit(EXIT_FAILURE);
    }
    FT_ResetDevice(dev->handle);
    FT_Purge(dev->handle, FT_PURGE_RX | FT_PURGE_TX);
    FT_SetUSBParameters(dev->handle, 64, 0);
    FT_SetBitMode(dev->handle, 0xFF, 0x01);
    printf("FTDI Read Device %d initialized successfully.\n", deviceIndex);
}

// Convert Morse code string to text
void morseToText(const char *morse, char *output) {
    char temp[10];
    int outIndex = 0, tempIndex = 0;

    for (int i = 0; i <= strlen(morse); ++i) {
        if (morse[i] == ' ' || morse[i] == '\0') {
            temp[tempIndex] = '\0';
            for (int j = 0; j < 36; j++) {
                if (strcmp(temp, morse_code[j]) == 0) {
                    output[outIndex++] = (j < 26) ? ('A' + j) : ('0' + j - 26);
                    break;
                }
            }
            tempIndex = 0;
            if (morse[i + 1] == ' ') {
                output[outIndex++] = ' ';
                i++;
            }
        } else {
            temp[tempIndex++] = morse[i];
        }
    }
    output[outIndex] = '\0';
}
