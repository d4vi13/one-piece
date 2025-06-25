#ifndef SLIDING_WINDOW_H
#define SLIDING_WINDOW_H

#include "common.h"
#include "pkg.h"
#include "comm_dev/comm_dev.h"


#define WINDOW_SIZE 4
#define COUNTER_SIZE 2

#define IN_RANGE(n) ((((sliding_window).pkgs[(sliding_window).head]).sequence_number <= n) \
&& (((((sliding_window).pkgs[(sliding_window).head]).sequence_number + WINDOW_SIZE -1) % 32)  <= n))

struct sliding_window 
{
  struct pkg pkgs[WINDOW_SIZE]; 
  struct pkg res;

  uint8_t counter ;
  uint8_t head : 2; // used for sending mode
  
  uint8_t expected_pkg_num : 5; // used for receiving mode
};

extern struct sliding_window sliding_window; 

void
init_sliding_window ();

int
wait_res ();

struct pkg *
wait_pkg_n (uint8_t n);

int 
snail_send (struct pkg *pkg);

int 
snail_recv (struct pkg *pkg, int ack);

void
send_start_talking ();

#endif
