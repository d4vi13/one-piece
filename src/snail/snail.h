#ifndef SNAIL_H
#define SNAIL_H

#include "comm_dev/comm_dev.h"
#include "common.h"
#include "pkg.h"

struct snail
{
  char network_interface[64];
  struct pkg pkg;
};

extern struct snail snail;

int init_snail (char network_interface[]);

int snail_send (pkg_t pkg_type, char *data, uint8_t size);

int snail_recv ();

#endif
