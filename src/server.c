#include "snail/snail.h"

int
main (int argc, char **argv)
{
  init_snail (argv[1]);
  char msg[] = "james";


  while (1)
    {
      snail_send (DATA, msg, strlen (msg));
    }
}
