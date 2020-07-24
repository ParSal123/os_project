#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>  
#include <stdint.h>
#include <getopt.h>

#include "BlpRequest.h"
#include "BlpResponse.h"

const char* DEVICE_FILE = "/dev/blpdriver";

int main(int argc, char* argv[])
{
    int file;
    int i;
    int number = -1;
    const char* const short_options = "P:p:t:";
    int period = -1;

    const struct option long_options[] = {
        {"period", 1, NULL, 'p'},
        {"blp", 1, NULL, 'P'},
        {"tid", 1, NULL, 't'},
        {NULL, 0, NULL, 0}
    };
    int is_thread = 0;
    int next_option;
    do
    {
        next_option = getopt_long_only(argc, argv, short_options, long_options, NULL);
        switch (next_option)
        {
            case 'p':
                period = atoi(optarg);
                break;
            case 't':
                is_thread = 1;
            case 'P':
                number = atoi(optarg);
                break;
        }
    } while (next_option != -1);
    if (number == -1)
    {
        printf("Please enter blp or tid\n");
        return -1;
    }
    if (period == -1)
    {
        printf("Please enter valid period\n");
        return -1;
    }

    file = open(DEVICE_FILE, 0);
    if (file < 0)
    {
        printf("Could not open device file! Device file path: %s\n", DEVICE_FILE);
        return file;
    }
    struct BlpRequest req;
    
    req.blp = number;
    req.response = malloc(sizeof(struct BlpResponse));
    while (1)
    {
        long result = ioctl(file, 0, &req);
        printf("Process Name: %s\n", req.response->comm);
        if (is_thread)
            printf("Parent Process: %d\n", req.response->parent_process);

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

        usleep(period * 1000000);
    }
    return 0;
}
