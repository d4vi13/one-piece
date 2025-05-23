#include "snail/snail.h"
#include "snail/file_ops.h"

int
main (int argc, char **argv)
{
  init_snail (argv[1]);

  while (1)
    {
      recv_file (argv[2]);
    }
}
