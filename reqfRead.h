#ifndef REQFREAD_H
#define REQFREAD_H

#include "ftd2xx.h"

typedef struct {
    FT_HANDLE handle;
    FT_STATUS status;
} reqfRead;

void initReadDevice(reqfRead *dev, int deviceIndex);
void morseToText(const char *morse, char *output);

#endif
