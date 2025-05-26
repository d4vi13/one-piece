#include "snail/snail.h"
#include "snail/file_ops.h"

int
main (int argc, char **argv)
{
  init_snail (argv[1]);

  struct pkg pkg[8];
  
  int i = 0;

  while (i < 8)
    {
      int ret = snail_recv (&pkg[i], 1);
      printf (" RECEBEU %d %s\n", pkg[i].sequence_number, pkg[i].data);
      sleep(1);
      if (ret == EXIT_SUCCESS)
        {
          i++;
        }
    }

  for (int j = 0; j < 8; j++)
    {
      printf ("data %s\n", pkg[j].data);
    }
}
