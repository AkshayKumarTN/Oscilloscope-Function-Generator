#ifndef REQUESTCOMMON_H
#define REQUESTCOMMON_H

// Enumeration for request status - shared between ReqRead and ReqWrite
enum class RequestStatus {
    PENDING,
    SUCCESS,
    FAILED,
    TIMEOUT
};

#endif // REQUESTCOMMON_H