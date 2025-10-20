#include "ReqWrite.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>

// Constructor - default
ReqWrite::ReqWrite() 
    : data(0), bytesWritten(0), ftStatus(FT_OK), status(RequestStatus::PENDING) {
    timestamp = generateTimestamp();
}

// Constructor - with data
ReqWrite::ReqWrite(unsigned char data) 
    : data(data), bytesWritten(0), ftStatus(FT_OK), status(RequestStatus::PENDING) {
    timestamp = generateTimestamp();
}

// Destructor
ReqWrite::~ReqWrite() {
    // Cleanup if needed
}

// Generate timestamp string
std::string ReqWrite::generateTimestamp() {
    time_t now = time(nullptr);
    struct tm* timeinfo = localtime(&now);
    
    char buffer[64];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);
    
    return std::string(buffer);
}

// Initialize the request with data
void ReqWrite::init(unsigned char data) {
    this->data = data;
    this->bytesWritten = 0;
    this->ftStatus = FT_OK;
    this->status = RequestStatus::PENDING;
    this->errorMessage.clear();
    this->timestamp = generateTimestamp();
}

// Execute the write request
bool ReqWrite::execute(FT_HANDLE ftHandle) {
    status = RequestStatus::PENDING;
    errorMessage.clear();
    
    // Perform the write operation
    ftStatus = FT_Write(ftHandle, &data, sizeof(data), &bytesWritten);
    
    if (ftStatus != FT_OK) {
        status = RequestStatus::FAILED;
        std::ostringstream oss;
        oss << "FT_Write failed with status code: " << ftStatus;
        errorMessage = oss.str();
        return false;
    }
    
    if (bytesWritten != sizeof(data)) {
        status = RequestStatus::FAILED;
        std::ostringstream oss;
        oss << "Expected to write " << sizeof(data) 
            << " bytes but wrote " << bytesWritten << " bytes";
        errorMessage = oss.str();
        return false;
    }
    
    status = RequestStatus::SUCCESS;
    return true;
}

// Print request details to console
void ReqWrite::print() const {
    std::cout << "\n=== Write Request ===" << std::endl;
    std::cout << "Timestamp:     " << timestamp << std::endl;
    std::cout << "Data:          0x" << std::hex << std::uppercase 
              << std::setw(2) << std::setfill('0') << static_cast<int>(data) 
              << std::dec << std::endl;
    std::cout << "Bytes Written: " << bytesWritten << std::endl;
    std::cout << "Status:        " << getStatusString() << std::endl;
    
    if (status == RequestStatus::FAILED && !errorMessage.empty()) {
        std::cout << "Error:         " << errorMessage << std::endl;
    }
    std::cout << "====================" << std::endl;
}

// Log request to file
void ReqWrite::logToFile(const std::string& filename) const {
    std::ofstream file(filename, std::ios::app);
    
    if (!file.is_open()) {
        std::cerr << "Error: Could not open log file " << filename << std::endl;
        return;
    }
    
    file << "[" << timestamp << "] WRITE | Data: 0x" 
         << std::hex << std::uppercase << std::setw(2) << std::setfill('0') 
         << static_cast<int>(data) << std::dec
         << " | Bytes: " << bytesWritten 
         << " | Status: " << getStatusString();
    
    if (status == RequestStatus::FAILED && !errorMessage.empty()) {
        file << " | Error: " << errorMessage;
    }
    
    file << std::endl;
    file.close();
}

// Setter for data
void ReqWrite::setData(unsigned char newData) {
    this->data = newData;
}

// Get status as string
std::string ReqWrite::getStatusString() const {
    return statusToString(status);
}

// Reset request to initial state
void ReqWrite::reset() {
    bytesWritten = 0;
    ftStatus = FT_OK;
    status = RequestStatus::PENDING;
    errorMessage.clear();
    timestamp = generateTimestamp();
}

// Static method to convert status enum to string
std::string ReqWrite::statusToString(RequestStatus status) {
    switch (status) {
        case RequestStatus::PENDING:
            return "PENDING";
        case RequestStatus::SUCCESS:
            return "SUCCESS";
        case RequestStatus::FAILED:
            return "FAILED";
        case RequestStatus::TIMEOUT:
            return "TIMEOUT";
        default:
            return "UNKNOWN";
    }
}

// Get data as hex string
std::string ReqWrite::getDataAsHex() const {
    std::ostringstream oss;
    oss << std::hex << std::uppercase << std::setw(2) << std::setfill('0') 
        << static_cast<int>(data);
    return oss.str();
}