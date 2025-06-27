#include "snail.h"

struct snail snail;

int
init_snail (char network_interface[])
{
  errno = 0;

  int ret = 0;
  /* Copia nome da interface de rede */
  memcpy (snail.network_interface, network_interface,
          min (strlen (network_interface), 64));

#ifdef FREE_BSD
  printf("Insira seu endereco: ");
  scanf("%d", &snail.addr);
  printf("Insira seu destino: ");
  scanf("%d", &snail.dest_addr);
#endif

  /* Cria raw socket para comunicacao*/
  ret = init_comm_dev (BPF, network_interface);
  if (ret == EXIT_FAILURE)
    {
      perror ("Erro ao iniciar dispositivo de comunicação\n");
      return EXIT_FAILURE;
    }

  init_sliding_window ();
  return EXIT_SUCCESS;
}

