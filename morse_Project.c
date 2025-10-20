#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#ifdef _WIN32
#include <windows.h>
#define usleep(x) Sleep((x) / 1000)
#else
#include <unistd.h>
#endif
#include "ftd2xx.h"

// Define Morse code for each letter and number
const char *morse_code[] = {
    ".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---", "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.", "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--..",
    "-----", ".----", "..---", "...--", "....-", ".....", "-....", "--...", "---..", "----."
};

// Function to get Morse code for a character
const char* getMorseCode(char c) {
    if (c >= 'A' && c <= 'Z') {
        return morse_code[c - 'A'];
    } else if (c >= 'a' && c <= 'z') {
        return morse_code[c - 'a'];
    } else if (c >= '0' && c <= '9') {
        return morse_code[c - '0' + 26];
    } else if (c == ' ') {
        return " ";  // Space between words
    }
    return NULL;  // Unsupported character
}

// Function to send a Morse code character and write to file
void sendMorseCodeCharacter(FT_HANDLE ftHandle, char c, FILE *outputFile) {
    FT_STATUS ftStatus;
    DWORD bytesWritten;
    BYTE outputBuffer = 0x01;
    const char *morse;

    morse = getMorseCode(c);
    
    if (morse == NULL) {
        return; // Unsupported character
    }

    // Handle space between words
    if (c == ' ') {
        printf("   ");  // Visual space in console
        if (outputFile != NULL) {
            fprintf(outputFile, "   ");  // 7 units space between words (represented as 3 spaces)
        }
        usleep(700000);  // 7 units pause for word space
        return;
    }

    // Print Morse code for the character
    printf("%s ", morse);
    
    // Write to file if file pointer is valid
    if (outputFile != NULL) {
        fprintf(outputFile, "%c: %s | ", c, morse);
    }

    // Send Morse code to the FTDI chip
    while (*morse) {
        if (*morse == '.') {
            outputBuffer = 0x01;
            ftStatus = FT_Write(ftHandle, &outputBuffer, sizeof(outputBuffer), &bytesWritten);
            usleep(100000); // 1 unit for dot
        } else if (*morse == '-') {
            outputBuffer = 0x01;
            ftStatus = FT_Write(ftHandle, &outputBuffer, sizeof(outputBuffer), &bytesWritten);
            usleep(300000); // 3 units for dash
        }
        outputBuffer = 0x00;
        ftStatus = FT_Write(ftHandle, &outputBuffer, sizeof(outputBuffer), &bytesWritten);
        usleep(100000); // 1 unit space between parts of the same letter
        morse++;
    }
    usleep(200000); // 3 units space between letters
}

// Function to generate filename with timestamp
void generateFilename(char *filename, size_t size) {
    time_t now;
    struct tm *timeinfo;
    
    time(&now);
    timeinfo = localtime(&now);
    
    snprintf(filename, size, "morse_code_%04d%02d%02d_%02d%02d%02d.txt",
             timeinfo->tm_year + 1900,
             timeinfo->tm_mon + 1,
             timeinfo->tm_mday,
             timeinfo->tm_hour,
             timeinfo->tm_min,
             timeinfo->tm_sec);
}

// Function to send a Morse code message
void sendMorseCode(FT_HANDLE ftHandle) {
    char input[256];
    char filename[100];
    FILE *outputFile = NULL;
    int saveToFile;
    
    printf("Do you want to save the Morse code to a file? (1 = Yes, 0 = No): ");
    scanf("%d", &saveToFile);
    getchar(); // Consume newline
    
    if (saveToFile) {
        generateFilename(filename, sizeof(filename));
        outputFile = fopen(filename, "w");
        
        if (outputFile == NULL) {
            fprintf(stderr, "Error: Could not create output file %s\n", filename);
            printf("Continuing without saving to file...\n");
        } else {
            printf("Morse code will be saved to: %s\n", filename);
            fprintf(outputFile, "Morse Code Translation\n");
            fprintf(outputFile, "======================\n\n");
        }
    }
    
    printf("Enter your message (type 'E0' to finish): ");

    while (1) {
        fgets(input, sizeof(input), stdin);
        
        if (strncmp(input, "E0", 2) == 0) {
            printf("\nExiting Morse code sender...\n");
            break;
        }

        if (outputFile != NULL) {
            fprintf(outputFile, "Original text: %s", input);
            fprintf(outputFile, "Morse code:    ");
        }
        
        printf("Morse code: ");

        for (int i = 0; input[i] != '\0' && input[i] != '\n'; ++i) {
            if (input[i] == 'E' && input[i+1] == '0' && (input[i+2] == '\0' || input[i+2] == '\n')) {
                break;
            }
            sendMorseCodeCharacter(ftHandle, input[i], outputFile);
        }
        
        printf("\n");
        if (outputFile != NULL) {
            fprintf(outputFile, "\n\n");
            fflush(outputFile);  // Ensure data is written to file
        }
    }

    if (outputFile != NULL) {
        fprintf(outputFile, "\n======================\n");
        fprintf(outputFile, "End of Morse Code Translation\n");
        fclose(outputFile);
        printf("Morse code saved to file: %s\n", filename);
    }
    
    printf("\nMorse code message sent.\n");
}

void sendtestCode(FT_HANDLE ftHandle) {
    printf("Your message is being sent in Morse code...\n");
}