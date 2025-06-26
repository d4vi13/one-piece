#include "file_utils.h"

int
obter_tamanho_arquivo (char *filename)
{
  FILE *file = fopen (filename, "rb");
  if (!file)
    {
      perror ("Erro ao abrir arquivo");
      return -1;
    }

  fseek (file, 0, SEEK_END);
  int size = ftell (file);
  fclose (file);

  return size;
}

unsigned long long
verifica_arquivo (char *filename)
{
      // verifica se o arquivo eh regular
  struct stat st;
  if (stat (filename, &st) == 0)
    {
      if (!S_ISREG (st.st_mode))
        {
          fprintf (stderr, "Erro: o destino não é um arquivo regular.\n");
          return 0;
        }
    }

  FILE *file = fopen (filename, "w");
  if (!file)
    {
      perror("failed to open file");
    }   
  else
    {   
      fclose (file);
    }     

  // verifica se ha espaco disponivel
  struct statvfs fs;
  if (statvfs (filename, &fs) != 0)
    {
      perror ("Erro ao aessar o sistema de arquivos com statvfs\n");
      return 0;
    }

  return fs.f_bsize * fs.f_bavail;
}
