#include "sliding_window.h"

void
stop_talking ()
{
  errno = 0;
  struct pkg pkg;

  prepare_start_talking_pkg (&pkg);
  snail_send_and_wait (&pkg);
}

void 
start_talking ()
{
    struct pkg pkg;
    while (1)
      {
        snail_recv_and_ack (&pkg);
        if (pkg.type == START_TALKING)
            return;
      }
}
