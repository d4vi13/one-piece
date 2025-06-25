#ifndef SNAIL_H
#define SNAIL_H

#include "comm_dev/comm_dev.h"
#include "common.h"
#include "pkg.h"
#include "sliding_window/sliding_window.h"

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

size_t serialize_pkg (const struct pkg *pkg, uint8_t *out_buf);

void deserialize_pkg (struct pkg *pkg, const uint8_t *in_buf, size_t len);



#endif
