#ifndef PID_RESPONSE_H_
#define PID_RESPONSE_H_

#include <linux/sched.h>
#include <linux/types.h>
#include <linux/limits.h>

#define MAX_OPEN_FILES 1000
#define MAX_PROCESS_NAME_LENGTH 1000

struct PidResponse
{
    char comm[MAX_PROCESS_NAME_LENGTH];
    uint64_t start_time;
    uint64_t real_start_time;
    unsigned long nvcsw;
    unsigned long nivcsw;
    long state;
    int open_files;
    char open_file_paths[MAX_OPEN_FILES][PATH_MAX];
};

#endif