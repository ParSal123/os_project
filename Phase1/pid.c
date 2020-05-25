#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>  

#include "PidRequest.h"

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
    long result = ioctl(file, 0, &req);

    return 0;
}
