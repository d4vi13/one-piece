#include "snail/snail.h"
#include "snail/file_ops.h"

int
main (int argc, char **argv)
{
  init_snail (argv[1]);

  send_file (argv[2]);

  return 0;
}
