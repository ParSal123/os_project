#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>  

#include "FibRequest.h"

const char* DEVICE_FILE = "/dev/fibdriver";

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
    struct FibRequest req;
    req.mem = malloc(sizeof(int) * number);
    req.num = number;
    long result = ioctl(file, 0, &req);

    printf("The first %d terms of the Fibonacci series are:\n", number);
    for (i = 0; i < number; ++i)
        printf("%d ", req.mem[i]);
    
    printf("\n");
    free(req.mem);

    return 0;
}
