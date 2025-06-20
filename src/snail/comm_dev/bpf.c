#include "bpf.h"
#ifdef FREE_BSD

static inline int
open_bpf_dev (int *bpf, char *dev_name)
{
  errno = 0;

  *bpf = open (dev_name, O_RDWR);
  if (*bpf < 0)
    {
      printf("Nao pode abrir dispositivo bpf\n");
      return EXIT_FAILURE;
    }
  return EXIT_SUCCESS;
}

static inline int
setup_bpf_dev (int bpf, char network_interface[], int *buf_len)
{
  errno = 0;

  /* Estrutura de interface de comunicacao
   * para configurar interface de rede */
  struct ifreq ifr;
  int ret = 0;


  /* BIOCSGBLEN - configura tamanho do buffer de comunicacao */
  ret = ioctl (bpf, BIOCSBLEN, buf_len);
  if (ret == -1)
    {
      perror ("Nao pode desativar modo de imediato");
      return EXIT_FAILURE;
    }

  printf("buf_len %d\n", *buf_len);

  /* BIOCSETIF - vincula interface de rede ao dispositivo bpf*/
  strcpy (ifr.ifr_name, network_interface);
  ret = ioctl (bpf, BIOCSETIF, &ifr);
  if (ret == -1)
    {
      perror ("Nao pode vincular interface de rede: ");
      return EXIT_FAILURE;
    }

  int flag =1;
  ret = ioctl (bpf, BIOCSHDRCMPLT, &flag);
  if (ret == -1)
    {
      perror ("Nao pode desativar header complete");
      return EXIT_FAILURE;
    }

  /* BIOCPROMISC - configura dispositivo para modo promiscuo */
  ret = ioctl (bpf, BIOCPROMISC, 0);
  if (ret == -1)
    {
      perror ("Nao pode entrar em modo promiscuo: ");
      return EXIT_FAILURE;
    }

  /* BIOCIMMEDIATE - pacotes nao sao bufferizados pelo kernel */
  ret = ioctl (bpf, BIOCIMMEDIATE, buf_len);
  if (ret == -1)
    {
      perror ("Nao pode desativar modo de imediato");
      return EXIT_FAILURE;
    }

  struct timeval timeout = {
        .tv_sec = TIME_OUT / 1000,
        .tv_usec = (TIME_OUT % 1000) * 1000
    };
  ret = ioctl (bpf, BIOCSRTIMEOUT, &timeout);
  if (ret == -1)
    {
      perror ("Nao pode configurar timeout");
      return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}

int
get_bpf_dev (int *bpf, int *buf_len, char *network_interface)
{
  errno = 0;

  printf("allal\n");

  int ret = 0;
  ret = open_bpf_dev (bpf, BPF_DEV_NAME);
  if (ret == EXIT_FAILURE)
    {
      perror ("Nao pode abrir dispositivo: ");
      return EXIT_FAILURE;
    }

  ret = setup_bpf_dev (*bpf, network_interface, buf_len);
  if (ret == EXIT_FAILURE)
    {
      perror ("Nao pode configurar dispositivo: ");
      return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}

#endif
