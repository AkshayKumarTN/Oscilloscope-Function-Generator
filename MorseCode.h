#ifndef MORSECODE_H
#define MORSECODE_H

#include <string>
#include <map>
#include <vector>
#include <fstream>
#include <iostream>
#include "ftd2xx.h"
#include "ReqWrite.h"

#ifdef _WIN32
#include <windows.h>
#define sleep_ms(x) Sleep(x)
#else
#include <unistd.h>
#define sleep_ms(x) usleep((x) * 1000)
#endif

class MorseCode {
private:
    // Morse code mapping
    std::map<char, std::string> morseMap;
    
    // Log file path
    std::string logFilePath;
    
    // Initialize morse code map
    void initializeMorseMap();
    
    // Convert character to morse code
    std::string charToMorse(char c);

public:
    // Constructor
    MorseCode();
    MorseCode(const std::string& logFile);
    
    // Convert text to morse code string
    std::string textToMorse(const std::string& text);
    
    // Send morse code via FTDI and save to file
    void sendAndSaveMorse(FT_HANDLE ftHandle, const std::string& text);
    
    // Send morse character via FTDI
    void sendMorseChar(FT_HANDLE ftHandle, char c, std::ofstream& logFile);
    
    // Save morse code to file only (no transmission)
    void saveMorseToFile(const std::string& text, const std::string& filename);
    
    // Set log file path
    void setLogFile(const std::string& filename) { logFilePath = filename; }
};

#endif // MORSECODE_H