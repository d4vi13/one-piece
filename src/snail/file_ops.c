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
  
  unsigned long long c = 0;
  while (!feof (file))
    {
      int bytes_read = fread (buf, 1, MAX_DATA, file);      
      if (bytes_read <= 0)
        {
          perror ("Nao consegui ler do arquivo: ");
          return EXIT_FAILURE;
        }

      c += bytes_read;
      printf ("bytes mandados %d\n", c);
      prepare_data_pkg (&snail.pkg, buf, bytes_read);
      snail_send (&snail.pkg); 
    }

  prepare_eof_pkg (&snail.pkg);
  snail_send (&snail.pkg); 

  wait_pkg_n (snail.pkg.sequence_number);

  return EXIT_FAILURE;
}

void print (char *buf)
{
  for (int i = 0; i < sizeof(struct pkg); i++)
    printf ("%x ", (unsigned char) buf[i]);
  printf("\n");
}

int 
recv_file (char *filename)
{
   errno = 0;

  int file = open (filename, O_WRONLY);
  if (file == -1)
    {
      perror ("Nao pode abrir o arquivo");
      return EXIT_FAILURE;
    } 

  while (1)
    {
      snail_recv (&snail.pkg, 1);
      if (snail.pkg.type == END_OF_FILE)
        break;
      write (file, snail.pkg.data, snail.pkg.size);
    }

 return EXIT_SUCCESS; 
} 
