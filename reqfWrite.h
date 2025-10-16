#ifndef REQFWRITE_H
#define REQFWRITE_H

#include "ftd2xx.h"

typedef struct {
    FT_HANDLE handle;
    FT_STATUS status;
} reqfWrite;

extern const char *morse_code[];

void initWriteDevice(reqfWrite *dev, int deviceIndex);
void sendMorseCodeCharacter(reqfWrite *dev, char c);
void sendMorseCode(reqfWrite *dev, const char *message);

#endif
