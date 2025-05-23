#include "file_ops.h"

int 
send_file (char *filename)
{
  errno = 0;

  FILE* file = fopen (filename, "r");
  if (!file)
    {
      perror ("Nao pode abrir o arquivo");
      return EXIT_FAILURE;
    } 

  void *buf = malloc (MAX_DATA);
  if (!buf)
    {
      perror ("Nao pode alocar buffer de leitura: ");
      return EXIT_FAILURE;
    }

  while (!feof (file))
    {
      int bytes_read = fread (buf, 1, MAX_DATA, file);      
      if (bytes_read <= 0)
        {
          perror ("Nao consegui ler do arquivo: ");
          return EXIT_FAILURE;
        }

      prepare_data_pkg (&snail.pkg, buf, bytes_read);
      snail_send (&snail.pkg); 
    }

  prepare_eof_pkg (&snail.pkg);
  snail_send (&snail.pkg); 

  wait_pkg_n (snail.pkg.sequence_number);

  return EXIT_FAILURE;
}

int 
recv_file (char *filename)
{
   errno = 0;

  FILE* file = fopen (filename, "w");
  if (!file)
    {
      perror ("Nao pode abrir o arquivo");
      return EXIT_FAILURE;
    } 

  while (!feof (file))
    {
      snail_recv (&snail.pkg, 1);
      if (snail.pkg.type == END_OF_FILE)
        break;
      while (fwrite (&snail.pkg.data, 1, snail.pkg.size, file) <= 0);
    }

 return EXIT_SUCCESS; 
} 
