#include "file_ops.h"

int 
send_file (char *filename)
{
  errno = 0;

  int fd = open(filename, O_RDONLY);
  if (fd < 0)
    {
      perror ("Nao pode abrir o arquivo");
      return EXIT_FAILURE;
    }

  struct stat st;
  if (fstat(fd, &st) < 0)
    {
      perror("Erro ao obter informações do arquivo");
      close(fd);
      return EXIT_FAILURE;
    }

  if (st.st_size == 0)
    {
      close(fd);
      return EXIT_SUCCESS;  // Nada a enviar
    }

  void *data = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE , fd, 0);
  if (data == MAP_FAILED)
    {
      perror("Erro ao mapear arquivo na memória");
      close(fd);
      return EXIT_FAILURE;
    }

#ifdef LINUX
  if (madvise(data, st.st_size, MADV_WILLNEED | MADV_HUGEPAGE ) == -1)
  {
    perror ("madvise falhou");
  }
 #endif

  unsigned long long c = 0;
  size_t offset = 0;
  while (offset < st.st_size)
    {
      size_t chunk_size = (st.st_size - offset >= MAX_DATA) ? MAX_DATA : (st.st_size - offset);
      prepare_data_pkg(&snail.pkg, (char *)data + offset, chunk_size);
      snail_send(&snail.pkg);
      c += chunk_size;
      printf ("bytes mandados %llu seq %d\n", c, snail.pkg.sequence_number);
      offset += chunk_size;
    }

  prepare_eof_pkg(&snail.pkg);
  snail_send(&snail.pkg); 

  wait_pkg_n(snail.pkg.sequence_number);

  munmap(data, st.st_size);
  close(fd);

  return EXIT_SUCCESS;
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
      snail_recv_and_ack (&snail.pkg);
      if (snail.pkg.type == END_OF_FILE)
        break;
      write (file, snail.pkg.data, snail.pkg.size);
    }

 return EXIT_SUCCESS; 
} 
