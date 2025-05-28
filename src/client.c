#include "snail/snail.h"
#include "snail/file_ops.h"

int
main (int argc, char **argv)
{
  init_snail (argv[1]);

  struct pkg pkg[8];
  
  int i = 0;

      recv_file (argv[2]);
}
