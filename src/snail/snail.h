#ifndef SNAIL_H
#define SNAIL_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#define min(a,b) ( (a) < (b) ? (a) : (b) )

struct snail
{
    int socket;
    char network_interface[64];
    struct package pkg;
    uint32_t pkg_size;
};

extern struct snail snail;

void 
init_snail (char network_interface[]);

#endif

