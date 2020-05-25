#ifndef PID_REQUEST_H_
#define PID_REQUEST_H_

#include "PidResponse.h"

struct PidRequest
{
    int pid;
    int is_thread;
    struct PidResponse* response;
};

#endif