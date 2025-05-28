#ifndef SNAIL_H
#define SNAIL_H

#include "comm_dev/comm_dev.h"
#include "common.h"
#include "pkg.h"

struct snail
{
  char network_interface[64];
  struct pkg pkg;
  struct pkg ack;
  uint8_t seq_num : 5;
};

extern struct snail snail;

int init_snail (char network_interface[]);

uint8_t get_seq_num ();

#endif
