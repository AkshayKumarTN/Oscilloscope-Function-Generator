#include "MorseCode.h"
#include <cctype>
#include <sstream>
#include <ctime>

// Constructor
MorseCode::MorseCode() : logFilePath("morse_output.txt") {
    initializeMorseMap();
}

MorseCode::MorseCode(const std::string& logFile) : logFilePath(logFile) {
    initializeMorseMap();
}

// Initialize morse code mapping
void MorseCode::initializeMorseMap() {
    // Letters A-Z
    morseMap['A'] = ".-";    morseMap['B'] = "-...";  morseMap['C'] = "-.-.";
    morseMap['D'] = "-..";   morseMap['E'] = ".";     morseMap['F'] = "..-.";
    morseMap['G'] = "--.";   morseMap['H'] = "....";  morseMap['I'] = "..";
    morseMap['J'] = ".---";  morseMap['K'] = "-.-";   morseMap['L'] = ".-..";
    morseMap['M'] = "--";    morseMap['N'] = "-.";    morseMap['O'] = "---";
    morseMap['P'] = ".--.";  morseMap['Q'] = "--.-";  morseMap['R'] = ".-.";
    morseMap['S'] = "...";   morseMap['T'] = "-";     morseMap['U'] = "..-";
    morseMap['V'] = "...-";  morseMap['W'] = ".--";   morseMap['X'] = "-..-";
    morseMap['Y'] = "-.--";  morseMap['Z'] = "--..";
    
    // Numbers 0-9
    morseMap['0'] = "-----"; morseMap['1'] = ".----"; morseMap['2'] = "..---";
    morseMap['3'] = "...--"; morseMap['4'] = "....-"; morseMap['5'] = ".....";
    morseMap['6'] = "-...."; morseMap['7'] = "--..."; morseMap['8'] = "---..";
    morseMap['9'] = "----.";
    
    // Special characters
    morseMap[' '] = "/";  // Word separator
}

// Convert character to morse code
std::string MorseCode::charToMorse(char c) {
    char upperC = std::toupper(c);
    
    if (morseMap.find(upperC) != morseMap.end()) {
        return morseMap[upperC];
    }
    
    return "";  // Unsupported character
}

// Convert entire text to morse code string
std::string MorseCode::textToMorse(const std::string& text) {
    std::string morseText;
    
    for (size_t i = 0; i < text.length(); i++) {
        std::string morse = charToMorse(text[i]);
        
        if (!morse.empty()) {
            morseText += morse;
            if (i < text.length() - 1 && text[i] != ' ') {
                morseText += " ";  // Space between letters
            }
        }
    }
    
    return morseText;
}

// Send morse character via FTDI with logging
void MorseCode::sendMorseChar(FT_HANDLE ftHandle, char c, std::ofstream& logFile) {
    std::string morse = charToMorse(c);
    
    if (morse.empty()) {
        return;
    }
    
    char upperC = std::toupper(c);
    
    // Log character and its morse code
    if (c == ' ') {
        logFile << "   ";  // Word space
        std::cout << "   ";
        sleep_ms(700);  // 7 units for word space
        return;
    }
    
    logFile << upperC << ": " << morse << " | ";
    std::cout << morse << " ";
    
    // Send each dot/dash
    for (char symbol : morse) {
        ReqWrite writeReq;
        
        if (symbol == '.') {
            // Dot - LED on for 100ms
            writeReq.init(0x01);
            writeReq.execute(ftHandle);
            sleep_ms(100);
            
            // LED off
            writeReq.init(0x00);
            writeReq.execute(ftHandle);
            sleep_ms(100);
            
        } else if (symbol == '-') {
            // Dash - LED on for 300ms
            writeReq.init(0x01);
            writeReq.execute(ftHandle);
            sleep_ms(300);
            
            // LED off
            writeReq.init(0x00);
            writeReq.execute(ftHandle);
            sleep_ms(100);
        }
    }
    
    sleep_ms(200);  // Space between letters
}

// Send morse code and save to file
void MorseCode::sendAndSaveMorse(FT_HANDLE ftHandle, const std::string& text) {
    // Generate filename with timestamp
    time_t now = time(nullptr);
    struct tm* timeinfo = localtime(&now);
    char timestamp[64];
    strftime(timestamp, sizeof(timestamp), "%Y%m%d_%H%M%S", timeinfo);
    
    std::string filename = "morse_" + std::string(timestamp) + ".txt";
    
    std::ofstream logFile(filename);
    
    if (!logFile.is_open()) {
        std::cerr << "Error: Could not create log file " << filename << std::endl;
        return;
    }
    
    // Write header
    logFile << "===============================================" << std::endl;
    logFile << "        MORSE CODE TRANSMISSION LOG           " << std::endl;
    logFile << "===============================================" << std::endl;
    logFile << "Timestamp: " << timestamp << std::endl;
    logFile << "Original Text: " << text << std::endl;
    logFile << "===============================================" << std::endl;
    logFile << std::endl;
    logFile << "Morse Code: " << std::endl;
    
    std::cout << "\n=== Sending Morse Code ===" << std::endl;
    std::cout << "Original text: " << text << std::endl;
    std::cout << "Morse code: ";
    
    // Send each character
    for (char c : text) {
        sendMorseChar(ftHandle, c, logFile);
    }
    
    std::cout << std::endl;
    
    // Write footer
    logFile << std::endl << std::endl;
    logFile << "===============================================" << std::endl;
    logFile << "Full Morse Translation: " << std::endl;
    logFile << textToMorse(text) << std::endl;
    logFile << "===============================================" << std::endl;
    logFile << "Transmission completed successfully." << std::endl;
    
    logFile.close();
    
    std::cout << "\nMorse code saved to: " << filename << std::endl;
    std::cout << "Transmission complete!" << std::endl;
}

// Save morse code to file only (no transmission)
void MorseCode::saveMorseToFile(const std::string& text, const std::string& filename) {
    std::ofstream logFile(filename);
    
    if (!logFile.is_open()) {
        std::cerr << "Error: Could not create file " << filename << std::endl;
        return;
    }
    
    // Write header
    time_t now = time(nullptr);
    struct tm* timeinfo = localtime(&now);
    char timestamp[64];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", timeinfo);
    
    logFile << "===============================================" << std::endl;
    logFile << "           MORSE CODE TRANSLATION             " << std::endl;
    logFile << "===============================================" << std::endl;
    logFile << "Timestamp: " << timestamp << std::endl;
    logFile << "Original Text: " << text << std::endl;
    logFile << "===============================================" << std::endl;
    logFile << std::endl;
    
    // Character by character translation
    logFile << "Character-by-Character Translation:" << std::endl;
    for (char c : text) {
        if (c == ' ') {
            logFile << "[SPACE] / (word separator)" << std::endl;
        } else {
            std::string morse = charToMorse(c);
            if (!morse.empty()) {
                logFile << c << " → " << morse << std::endl;
            }
        }
    }
    
    logFile << std::endl;
    logFile << "===============================================" << std::endl;
    logFile << "Complete Morse Code:" << std::endl;
    logFile << textToMorse(text) << std::endl;
    logFile << "===============================================" << std::endl;
    
    logFile.close();
    
    std::cout << "Morse code saved to: " << filename << std::endl;
}