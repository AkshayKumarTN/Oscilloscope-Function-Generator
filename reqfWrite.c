/*
===============================================================================
 File Name   : reqfWrite.c
 Description : Handles FTDI write device operations for Morse Code project
 Author      : Akshay Kumar Talur Narasimmulu
===============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
#include <windows.h>
#define usleep(x) Sleep((x) / 1000)
#else
#include <unistd.h>
#endif
#include "ftd2xx.h"
#include "reqfWrite.h"

// Morse code table
const char *morse_code[] = {
    ".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---",
    "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.", "...", "-",
    "..-", "...-", ".--", "-..-", "-.--", "--..",
    "-----", ".----", "..---", "...--", "....-", ".....",
    "-....", "--...", "---..", "----."
};

// Initialize FTDI write device
void initWriteDevice(reqfWrite *dev, int deviceIndex) {
    dev->status = FT_Open(deviceIndex, &dev->handle);
    if (dev->status != FT_OK) {
        fprintf(stderr, "Error: Unable to open FTDI write device %d\n", deviceIndex);
        exit(EXIT_FAILURE);
    }
    FT_ResetDevice(dev->handle);
    FT_Purge(dev->handle, FT_PURGE_RX | FT_PURGE_TX);
    FT_SetUSBParameters(dev->handle, 64, 0);
    FT_SetBitMode(dev->handle, 0xFF, 0x01); // Synchronous bit-bang
    printf("FTDI Write Device %d initialized successfully.\n", deviceIndex);
}

// Send a single Morse code character
void sendMorseCodeCharacter(reqfWrite *dev, char c) {
    FT_STATUS status;
    DWORD bytesWritten;
    BYTE outputBuffer = 0x01;
    const char *morse;

    if (c >= 'A' && c <= 'Z') morse = morse_code[c - 'A'];
    else if (c >= 'a' && c <= 'z') morse = morse_code[c - 'a'];
    else if (c >= '0' && c <= '9') morse = morse_code[c - '0' + 26];
    else return;

    printf("Sending Morse for '%c': %s\n", c, morse);

    while (*morse) {
        if (*morse == '.') {
            outputBuffer = 0x01;
            status = FT_Write(dev->handle, &outputBuffer, 1, &bytesWritten);
            usleep(100000);
        } else if (*morse == '-') {
            outputBuffer = 0x01;
            status = FT_Write(dev->handle, &outputBuffer, 1, &bytesWritten);
            usleep(300000);
        }
        outputBuffer = 0x00;
        status = FT_Write(dev->handle, &outputBuffer, 1, &bytesWritten);
        usleep(100000);
        morse++;
    }
    usleep(200000);
}

// Send an entire message in Morse
void sendMorseCode(reqfWrite *dev, const char *message) {
    for (int i = 0; message[i] != '\0'; ++i) {
        if (message[i] != ' ')
            sendMorseCodeCharacter(dev, message[i]);
        else
            usleep(400000); // space between words
    }
    printf("Message '%s' sent in Morse Code.\n", message);
}
