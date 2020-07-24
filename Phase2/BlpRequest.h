#ifndef BLP_REQUEST_H_
#define BLP_REQUEST_H_

#include "BlpResponse.h"

struct BlpRequest
{
    int blp;
    int is_thread;
    struct BlpResponse* response;
};

#endif