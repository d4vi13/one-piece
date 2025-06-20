#ifndef SLIDING_WINDOW_H
#define SLIDING_WINDOW_H

#include "common.h"
#include "pkg.h"
#include "comm_dev/comm_dev.h"


#define WINDOW_SIZE 4
#define COUNTER_SIZE 2

struct sliding_window 
{
  struct pkg pkgs[WINDOW_SIZE]; 
  struct pkg res;

  uint8_t counter;
  uint8_t head : 2; // used for sending mode
  
  uint8_t expected_pkg_num : 5; // used for receiving mode
};

extern struct sliding_window sliding_window; 

void
init_sliding_window ();

int
wait_res ();

int
wait_pkg_n (uint8_t n);

int 
snail_send (struct pkg *pkg);

int 
snail_recv (struct pkg *pkg, int ack);

#endif
