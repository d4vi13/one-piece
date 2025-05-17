#include "snail/snail.h"

int
main (int argc, char **argv)
{
  init_snail (argv[1]);

  while (1)
    {
      snail_recv ();
    }
}
