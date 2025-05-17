#ifndef SNAIL_H
#define SNAIL_H

#include "common.h"
#include "pkg.h"
#include "comm_dev/comm_dev.h"

struct snail
{
    char network_interface[64];
    struct pkg pkg;
};

extern struct snail snail;

int 
init_snail (char network_interface[]);

#endif

