#include "ReqRead.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <bitset>

// Constructor - default
ReqRead::ReqRead() 
    : data(0), bytesRead(0), ftStatus(FT_OK), status(RequestStatus::PENDING), purgeBefore(true) {
    timestamp = generateTimestamp();
}

// Constructor - with purge option
ReqRead::ReqRead(bool purgeBefore) 
    : data(0), bytesRead(0), ftStatus(FT_OK), status(RequestStatus::PENDING), purgeBefore(purgeBefore) {
    timestamp = generateTimestamp();
}

// Destructor
ReqRead::~ReqRead() {
    // Cleanup if needed
}

// Generate timestamp string
std::string ReqRead::generateTimestamp() {
    time_t now = time(nullptr);
    struct tm* timeinfo = localtime(&now);
    
    char buffer[64];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);
    
    return std::string(buffer);
}

// Initialize the request
void ReqRead::init(bool purgeBefore) {
    this->data = 0;
    this->bytesRead = 0;
    this->ftStatus = FT_OK;
    this->status = RequestStatus::PENDING;
    this->errorMessage.clear();
    this->purgeBefore = purgeBefore;
    this->timestamp = generateTimestamp();
}

// Execute the read request
bool ReqRead::execute(FT_HANDLE ftHandle) {
    status = RequestStatus::PENDING;
    errorMessage.clear();
    
    // Purge RX buffer if requested
    if (purgeBefore) {
        FT_STATUS purgeStatus = FT_Purge(ftHandle, FT_PURGE_RX);
        if (purgeStatus != FT_OK) {
            status = RequestStatus::FAILED;
            std::ostringstream oss;
            oss << "FT_Purge failed with status code: " << purgeStatus;
            errorMessage = oss.str();
            return false;
        }
    }
    
    // Perform the read operation
    ftStatus = FT_Read(ftHandle, &data, sizeof(data), &bytesRead);
    
    if (ftStatus != FT_OK) {
        status = RequestStatus::FAILED;
        std::ostringstream oss;
        oss << "FT_Read failed with status code: " << ftStatus;
        errorMessage = oss.str();
        return false;
    }
    
    status = RequestStatus::SUCCESS;
    return true;
}

// Print request details to console
void ReqRead::print() const {
    std::cout << "\n=== Read Request ===" << std::endl;
    std::cout << "Timestamp:   " << timestamp << std::endl;
    std::cout << "Data:        0x" << std::hex << std::uppercase 
              << std::setw(2) << std::setfill('0') << static_cast<int>(data) 
              << std::dec << std::endl;
    std::cout << "Bytes Read:  " << bytesRead << std::endl;
    std::cout << "Status:      " << getStatusString() << std::endl;
    
    if (status == RequestStatus::FAILED && !errorMessage.empty()) {
        std::cout << "Error:       " << errorMessage << std::endl;
    }
    std::cout << "===================" << std::endl;
}

// Log request to file
void ReqRead::logToFile(const std::string& filename) const {
    std::ofstream file(filename, std::ios::app);
    
    if (!file.is_open()) {
        std::cerr << "Error: Could not open log file " << filename << std::endl;
        return;
    }
    
    file << "[" << timestamp << "] READ | Data: 0x" 
         << std::hex << std::uppercase << std::setw(2) << std::setfill('0') 
         << static_cast<int>(data) << std::dec
         << " | Bytes: " << bytesRead 
         << " | Status: " << getStatusString();
    
    if (status == RequestStatus::FAILED && !errorMessage.empty()) {
        file << " | Error: " << errorMessage;
    }
    
    file << std::endl;
    file.close();
}

// Get a human-readable description of the data
std::string ReqRead::getDataDescription() const {
    if (bytesRead == 0) {
        return "No data read";
    }
    
    std::ostringstream oss;
    oss << "0x" << getDataAsHex() 
        << " (decimal: " << getDataAsDecimal() 
        << ", binary: " << getDataAsBinary() << ")";
    
    return oss.str();
}

// Setter for purgeBefore
void ReqRead::setPurgeBefore(bool purge) {
    this->purgeBefore = purge;
}

// Get status as string
std::string ReqRead::getStatusString() const {
    return statusToString(status);
}

// Get data as hex string
std::string ReqRead::getDataAsHex() const {
    std::ostringstream oss;
    oss << std::hex << std::uppercase << std::setw(2) << std::setfill('0') 
        << static_cast<int>(data);
    return oss.str();
}

// Get data as binary string
std::string ReqRead::getDataAsBinary() const {
    return std::bitset<8>(data).to_string();
}

// Reset request to initial state
void ReqRead::reset() {
    data = 0;
    bytesRead = 0;
    ftStatus = FT_OK;
    status = RequestStatus::PENDING;
    errorMessage.clear();
    timestamp = generateTimestamp();
}

// Static method to convert status enum to string
std::string ReqRead::statusToString(RequestStatus status) {
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

// Check if specific bit is set
bool ReqRead::isBitSet(int bitPosition) const {
    if (bitPosition < 0 || bitPosition > 7) {
        return false;
    }
    return (data & (1 << bitPosition)) != 0;
}

// Create byte with specific bit pattern
unsigned char ReqRead::createBitPattern(bool b7, bool b6, bool b5, bool b4, 
                                        bool b3, bool b2, bool b1, bool b0) {
    unsigned char result = 0;
    if (b0) result |= (1 << 0);
    if (b1) result |= (1 << 1);
    if (b2) result |= (1 << 2);
    if (b3) result |= (1 << 3);
    if (b4) result |= (1 << 4);
    if (b5) result |= (1 << 5);
    if (b6) result |= (1 << 6);
    if (b7) result |= (1 << 7);
    return result;
}