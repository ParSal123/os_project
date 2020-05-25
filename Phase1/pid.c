#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>  
#include <stdint.h>

#include "PidRequest.h"
#include "PidResponse.h"

const char* DEVICE_FILE = "/dev/piddriver";

int main()
{
    int file;
    int i;
    int number;

    file = open(DEVICE_FILE, 0);
    if (file < 0)
    {
        printf("Could not open device file! Device file path: %s\n", DEVICE_FILE);
        return file;
    }

    printf("Please Enter Your Number.\n");
    scanf("%d", &number);
    struct PidRequest req;
    
    req.pid = number;
    req.response = malloc(sizeof(struct PidResponse));
    
    while (1)
    {
        long result = ioctl(file, 0, &req);
        printf("Process Name: %s\n", req.response->comm);
        printf("Start Time: %lu\n", req.response->start_time);
        printf("Real Start Time: %lu\n", req.response->real_start_time);
        printf("Voluntary Context Switches: %ld\n", req.response->nvcsw);
        printf("Involuntary Context Switches: %ld\n", req.response->nivcsw);

        if (!req.response->open_files)
            printf("This process has no open files\n");
        
        else
        {    
            printf("List of open files:\n");
        
            int i;
            for (i = 0; i < req.response->open_files; ++i)
                printf("%d. %s\n", i + 1, req.response->open_file_paths[i]);
        }

        usleep(1 * 1000000);
    }
    return 0;
}
