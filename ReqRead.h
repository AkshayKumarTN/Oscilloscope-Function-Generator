#ifndef REQREAD_H
#define REQREAD_H

#include "ftd2xx.h"
#include "RequestCommon.h"
#include <string>
#include <ctime>

class ReqRead {
private:
    unsigned char data;           // Data byte read
    DWORD bytesRead;             // Number of bytes actually read
    FT_STATUS ftStatus;          // FTDI status code
    RequestStatus status;         // Request status
    std::string timestamp;        // Timestamp of request
    std::string errorMessage;     // Error message if any
    bool purgeBefore;            // Flag to purge buffers before reading

    // Private method to generate timestamp
    std::string generateTimestamp();

public:
    // Constructor
    ReqRead();
    ReqRead(bool purgeBefore);

    // Destructor
    ~ReqRead();

    // Initialize the request
    void init(bool purgeBefore = true);

    // Execute the read request
    bool execute(FT_HANDLE ftHandle);

    // Print request details to console
    void print() const;

    // Log request to file
    void logToFile(const std::string& filename) const;

    // Get a human-readable description of the data
    std::string getDataDescription() const;

    // Getters
    unsigned char getData() const { return data; }
    DWORD getBytesRead() const { return bytesRead; }
    FT_STATUS getFtStatus() const { return ftStatus; }
    RequestStatus getStatus() const { return status; }
    std::string getTimestamp() const { return timestamp; }
    std::string getErrorMessage() const { return errorMessage; }
    bool getPurgeBefore() const { return purgeBefore; }

    // Setters
    void setPurgeBefore(bool purge);

    // Check if request was successful
    bool isSuccessful() const { return status == RequestStatus::SUCCESS; }

    // Get status as string
    std::string getStatusString() const;

    // Get data in different formats
    std::string getDataAsHex() const;
    std::string getDataAsBinary() const;
    int getDataAsDecimal() const { return static_cast<int>(data); }

    // Reset request to initial state
    void reset();

    // Static method to get status string from enum
    static std::string statusToString(RequestStatus status);

    // Check if specific bit is set
    bool isBitSet(int bitPosition) const;

    // Get byte with specific bit pattern
    static unsigned char createBitPattern(bool b7, bool b6, bool b5, bool b4, 
                                          bool b3, bool b2, bool b1, bool b0);
};

#endif // REQREAD_H