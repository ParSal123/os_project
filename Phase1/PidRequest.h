#ifndef PID_REQUEST_H_
#define PID_REQUEST_H_

#include "PidResponse.h"

struct PidRequest
{
    int pid;
    struct PidResponse* response;
};

#endif