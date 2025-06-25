#ifndef SLIDING_WINDOW_H
#define SLIDING_WINDOW_H

#include "../common.h"
#include "../pkg.h"
#include "../comm_dev/comm_dev.h"


#define WINDOW_SIZE 4

#define IN_RANGE(n) ((((sliding_window).pkgs[(sliding_window).head]).sequence_number <= n) \
&& (((((sliding_window).pkgs[(sliding_window).head]).sequence_number + WINDOW_SIZE -1) % 32)  <= n))

struct sliding_window 
{
  struct pkg pkgs[WINDOW_SIZE]; 
  struct pkg res;
  struct pkg ack;

  uint8_t counter ;
  uint8_t head : 2; // used for sending mode
  
  uint8_t expected_pkg_num : 5; // used for receiving mode

  uint8_t seq_num : 5;
};

extern struct sliding_window sliding_window; 

/* WINDOW OPS*/

void
init_sliding_window ();

int 
window_full ();

uint8_t
pkg_n_pos (uint8_t n);

int
free_pkg_n(uint8_t n);

void 
add_pkg_to_window (struct pkg *pkg);

uint8_t
get_seq_num();

/* RECV OPS*/

int 
snail_recv (struct pkg *pkg);

int 
snail_recv_and_ack (struct pkg *pkg);

int
wait_res ();

struct pkg *
wait_pkg_n (uint8_t n);

/* CONTROL */

void
start_talking ();

void 
stop_talking ();


/* -- SEND OPERATIONS -- */

int 
snail_send (struct pkg *pkg);

struct pkg * 
snail_send_and_wait (struct pkg *pkg);

int 
resend(uint8_t n);

int 
ack_pkg (uint8_t seq_num);

int 
error_pkg (uint8_t seq_num);

int 
ok_ack_pkg (uint8_t seq_num);;

int 
treasure_ack (uint8_t seq_num, pkg_t pkg_type, char *filename);

int 
resend_last_ack ();


#endif
