#include <iostream>
#include <string>
#ifdef _WIN32
#include <windows.h>
#define usleep(x) Sleep((x) / 1000)
#else
#include <unistd.h>
#endif
#include "ftd2xx.h"
#include "ReqWrite.h"
#include "ReqRead.h"
#include "MorseCode.h"

using namespace std;

// Function prototypes
void initializeDevice(FT_HANDLE *ftHandle);
void writeByteExample(FT_HANDLE ftHandle);
void readByteExample(FT_HANDLE ftHandle);
void batchWriteExample(FT_HANDLE ftHandle);
void batchReadExample(FT_HANDLE ftHandle);
void interactiveWrite(FT_HANDLE ftHandle);
void interactiveRead(FT_HANDLE ftHandle);
void morseCodeSender(FT_HANDLE ftHandle);

int main() {
    FT_HANDLE ftHandle;
    int choice;

    initializeDevice(&ftHandle);

    while (true) {
        cout << "\n=== Control Menu ===" << endl;
        cout << "1. Write byte (simple example)" << endl;
        cout << "2. Read byte (simple example)" << endl;
        cout << "3. Batch write example" << endl;
        cout << "4. Batch read example" << endl;
        cout << "5. Interactive write" << endl;
        cout << "6. Interactive read" << endl;
        cout << "7. Send Morse Code (Text to Morse)" << endl;
        cout << "8. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        cin.clear();
        cin.ignore(10000, '\n');  // Clear input buffer

        if (choice == 8) {
            break;
        }

        switch (choice) {
            case 1:
                writeByteExample(ftHandle);
                break;
            case 2:
                readByteExample(ftHandle);
                break;
            case 3:
                batchWriteExample(ftHandle);
                break;
            case 4:
                batchReadExample(ftHandle);
                break;
            case 5:
                interactiveWrite(ftHandle);
                break;
            case 6:
                interactiveRead(ftHandle);
                break;
            case 7:
                morseCodeSender(ftHandle);
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    }

    // Close the device
    FT_Close(ftHandle);
    cout << "Device closed." << endl;

    return EXIT_SUCCESS;
}

void initializeDevice(FT_HANDLE *ftHandle) {
    FT_STATUS ftStatus;

    // Open the device
    ftStatus = FT_Open(0, ftHandle);
    if (ftStatus != FT_OK) {
        cerr << "Error: FT_Open failed" << endl;
        exit(EXIT_FAILURE);
    }
    cout << "Device opened successfully." << endl;

    // Reset the device
    ftStatus = FT_ResetDevice(*ftHandle);
    if (ftStatus != FT_OK) {
        cerr << "Error: FT_ResetDevice failed" << endl;
        FT_Close(*ftHandle);
        exit(EXIT_FAILURE);
    }
    cout << "Device reset successfully." << endl;

    // Purge USB receive and transmit buffers
    ftStatus = FT_Purge(*ftHandle, FT_PURGE_RX | FT_PURGE_TX);
    if (ftStatus != FT_OK) {
        cerr << "Error: FT_Purge failed" << endl;
        FT_Close(*ftHandle);
        exit(EXIT_FAILURE);
    }
    cout << "Purged USB buffers successfully." << endl;

    // Set the USB parameters
    ftStatus = FT_SetUSBParameters(*ftHandle, 64, 0);
    if (ftStatus != FT_OK) {
        cerr << "Error: FT_SetUSBParameters failed" << endl;
        FT_Close(*ftHandle);
        exit(EXIT_FAILURE);
    }
    cout << "USB parameters set successfully." << endl;

    // Set the device to synchronous bit bang mode
    ftStatus = FT_SetBitMode(*ftHandle, 0xFF, 0x01);
    if (ftStatus != FT_OK) {
        cerr << "Error: FT_SetBitMode failed" << endl;
        FT_Close(*ftHandle);
        exit(EXIT_FAILURE);
    }
    cout << "Set synchronous bit bang mode successfully." << endl;
}

void writeByteExample(FT_HANDLE ftHandle) {
    cout << "\n=== Write Byte Example ===" << endl;
    
    // Create a ReqWrite object
    ReqWrite writeReq(0xFF);
    
    // Execute the write
    if (writeReq.execute(ftHandle)) {
        cout << "Write successful!" << endl;
    } else {
        cout << "Write failed!" << endl;
    }
    
    // Print details
    writeReq.print();
    
    // Log to file
    writeReq.logToFile("request_log.txt");
    cout << "Logged to request_log.txt" << endl;
}

void readByteExample(FT_HANDLE ftHandle) {
    cout << "\n=== Read Byte Example ===" << endl;
    
    // Create a ReqRead object (with purge)
    ReqRead readReq(true);
    
    // Execute the read
    if (readReq.execute(ftHandle)) {
        cout << "Read successful!" << endl;
        cout << "Data description: " << readReq.getDataDescription() << endl;
    } else {
        cout << "Read failed!" << endl;
    }
    
    // Print details
    readReq.print();
    
    // Log to file
    readReq.logToFile("request_log.txt");
    cout << "Logged to request_log.txt" << endl;
}

void batchWriteExample(FT_HANDLE ftHandle) {
    cout << "\n=== Batch Write Example ===" << endl;
    
    const int BATCH_SIZE = 5;
    unsigned char values[] = {0x00, 0xFF, 0xAA, 0x55, 0xF0};
    
    for (int i = 0; i < BATCH_SIZE; i++) {
        ReqWrite writeReq(values[i]);
        
        if (writeReq.execute(ftHandle)) {
            cout << "Write " << (i+1) << " successful: 0x" 
                 << writeReq.getDataAsHex() << endl;
            writeReq.logToFile("batch_write_log.txt");
        } else {
            cout << "Write " << (i+1) << " failed!" << endl;
        }
        
        usleep(100000); // Small delay between writes
    }
    
    cout << "Batch write completed. Check batch_write_log.txt" << endl;
}

void batchReadExample(FT_HANDLE ftHandle) {
    cout << "\n=== Batch Read Example ===" << endl;
    
    const int BATCH_SIZE = 5;
    
    for (int i = 0; i < BATCH_SIZE; i++) {
        ReqRead readReq(false); // No purge for batch reads
        
        if (readReq.execute(ftHandle)) {
            cout << "Read " << (i+1) << ": " 
                 << readReq.getDataDescription() << endl;
            readReq.logToFile("batch_read_log.txt");
        } else {
            cout << "Read " << (i+1) << " failed!" << endl;
        }
        
        usleep(100000); // Small delay between reads
    }
    
    cout << "Batch read completed. Check batch_read_log.txt" << endl;
}

void interactiveWrite(FT_HANDLE ftHandle) {
    cout << "\n=== Interactive Write ===" << endl;
    
    unsigned int byteValue;
    int logChoice;
    
    cout << "Enter byte value to write (in hex, e.g., FF): ";
    cin >> hex >> byteValue;
    cin.clear();
    cin.ignore(10000, '\n');  // Clear input buffer
    
    ReqWrite writeReq(static_cast<unsigned char>(byteValue));
    
    cout << "Executing write request..." << endl;
    
    if (writeReq.execute(ftHandle)) {
        cout << "Write operation successful!" << endl;
    } else {
        cout << "Write operation failed!" << endl;
    }
    
    writeReq.print();
    
    cout << "Log this request to file? (1=Yes, 0=No): ";
    cin >> dec >> logChoice;
    cin.clear();
    cin.ignore(10000, '\n');  // Clear input buffer
    
    if (logChoice == 1) {
        writeReq.logToFile("request_log.txt");
        cout << "Request logged to request_log.txt" << endl;
    }
}

void interactiveRead(FT_HANDLE ftHandle) {
    cout << "\n=== Interactive Read ===" << endl;
    
    int purgeChoice, logChoice;
    
    cout << "Purge RX buffer before reading? (1=Yes, 0=No): ";
    cin >> purgeChoice;
    cin.clear();
    cin.ignore(10000, '\n');  // Clear input buffer
    
    ReqRead readReq(purgeChoice == 1);
    
    cout << "Executing read request..." << endl;
    
    if (readReq.execute(ftHandle)) {
        cout << "Read operation successful!" << endl;
        cout << "\nData Details:" << endl;
        cout << "  Hex:     " << readReq.getDataAsHex() << endl;
        cout << "  Decimal: " << readReq.getDataAsDecimal() << endl;
        cout << "  Binary:  " << readReq.getDataAsBinary() << endl;
        
        cout << "\nBit Analysis:" << endl;
        for (int i = 7; i >= 0; i--) {
            cout << "  Bit " << i << ": " 
                 << (readReq.isBitSet(i) ? "1" : "0") << endl;
        }
    } else {
        cout << "Read operation failed!" << endl;
    }
    
    readReq.print();
    
    cout << "Log this request to file? (1=Yes, 0=No): ";
    cin >> logChoice;
    cin.clear();
    cin.ignore(10000, '\n');  // Clear input buffer
    
    if (logChoice == 1) {
        readReq.logToFile("request_log.txt");
        cout << "Request logged to request_log.txt" << endl;
    }
}

void morseCodeSender(FT_HANDLE ftHandle) {
    cout << "\n=== Morse Code Sender ===" << endl;
    cout << "Enter text to convert to Morse code: " << endl;
    
    string inputText;
    getline(cin, inputText);
    
    if (inputText.empty()) {
        cout << "No text entered. Returning to menu." << endl;
        return;
    }
    
    cout << "\nYou entered: \"" << inputText << "\"" << endl;
    
    // Create MorseCode object
    MorseCode morse;
    
    // Show morse translation
    string morseTranslation = morse.textToMorse(inputText);
    cout << "Morse translation: " << morseTranslation << endl;
    
    int choice;
    cout << "\nChoose an option:" << endl;
    cout << "1. Send via FTDI and save to file" << endl;
    cout << "2. Save to file only (no transmission)" << endl;
    cout << "3. Cancel" << endl;
    cout << "Enter choice: ";
    cin >> choice;
    cin.clear();
    cin.ignore(10000, '\n');
    
    switch (choice) {
        case 1:
            cout << "\nTransmitting morse code..." << endl;
            morse.sendAndSaveMorse(ftHandle, inputText);
            break;
        case 2:
            {
                string filename;
                cout << "Enter filename (e.g., morse_output.txt): ";
                getline(cin, filename);
                if (filename.empty()) {
                    filename = "morse_output.txt";
                }
                morse.saveMorseToFile(inputText, filename);
            }
            break;
        case 3:
            cout << "Cancelled." << endl;
            break;
        default:
            cout << "Invalid choice." << endl;
    }
}