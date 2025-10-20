#ifndef REQWRITE_H
#define REQWRITE_H

#include "ftd2xx.h"
#include "RequestCommon.h"
#include <string>
#include <ctime>

class ReqWrite {
private:
    unsigned char data;           // Data byte to write
    DWORD bytesWritten;          // Number of bytes actually written
    FT_STATUS ftStatus;          // FTDI status code
    RequestStatus status;         // Request status
    std::string timestamp;        // Timestamp of request
    std::string errorMessage;     // Error message if any

    // Private method to generate timestamp
    std::string generateTimestamp();

public:
    // Constructor
    ReqWrite();
    ReqWrite(unsigned char data);

    // Destructor
    ~ReqWrite();

    // Initialize the request with data
    void init(unsigned char data);

    // Execute the write request
    bool execute(FT_HANDLE ftHandle);

    // Print request details to console
    void print() const;

    // Log request to file
    void logToFile(const std::string& filename) const;

    // Getters
    unsigned char getData() const { return data; }
    DWORD getBytesWritten() const { return bytesWritten; }
    FT_STATUS getFtStatus() const { return ftStatus; }
    RequestStatus getStatus() const { return status; }
    std::string getTimestamp() const { return timestamp; }
    std::string getErrorMessage() const { return errorMessage; }

    // Setters
    void setData(unsigned char newData);

    // Check if request was successful
    bool isSuccessful() const { return status == RequestStatus::SUCCESS; }

    // Get status as string
    std::string getStatusString() const;

    // Reset request to initial state
    void reset();

    // Static method to get status string from enum
    static std::string statusToString(RequestStatus status);

    // Get data as hex string
    std::string getDataAsHex() const;
};

#endif // REQWRITE_H