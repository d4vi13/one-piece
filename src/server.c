#include "snail/snail.h"
#include "snail/file_ops.h"

int
main (int argc, char **argv)
{
  init_snail (argv[1]);

  struct pkg pkg[4];

  while (1)
    {
      prepare_data_pkg (&pkg[0], "alo0", strlen("alo0"));
      prepare_data_pkg (&pkg[1], "alo1", strlen("alo1"));
      prepare_data_pkg (&pkg[2], "alo2", strlen("alo2"));
      prepare_data_pkg (&pkg[3], "alo3", strlen("alo3"));

      for (int i = 0; i < 4; i++)
        {
          snail_send (&pkg[i]);
        }
    }

  return 0;
}
