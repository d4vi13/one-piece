#include "socket.h"
#ifdef LINUX

int
get_socket (int *sock, char network_interface[])
{
  errno = 0;
  // Cria arquivo para o socket sem qualquer protocolo
  *sock = socket (AF_PACKET, SOCK_RAW, htons (ETH_P_ALL));
  if (*sock == -1)
    {
      perror ("Erro ao criar socket: Verifique se você é root!\n");
      return EXIT_FAILURE;
    }

  int ifindex = if_nametoindex (network_interface);

  struct sockaddr_ll endereco = { 0 };
  endereco.sll_family = AF_PACKET;
  endereco.sll_protocol = htons (ETH_P_ALL);
  endereco.sll_ifindex = ifindex;
  // Inicializa socket
  if (bind (*sock, (struct sockaddr *)&endereco, sizeof (endereco)) == -1)
    {
      perror ("Erro ao fazer bind no socket\n");
      return EXIT_FAILURE;
    }

  struct packet_mreq mr = { 0 };
  mr.mr_ifindex = ifindex;
  mr.mr_type = PACKET_MR_PROMISC;
  // Não joga fora o que identifica como lixo: Modo promíscuo
  if (setsockopt (*sock, SOL_PACKET, PACKET_ADD_MEMBERSHIP, &mr, sizeof (mr))
      == -1)
    {
      perror ("Erro ao fazer setsockop\n");
      return EXIT_FAILURE;
    }

  struct timeval timeout = { .tv_sec = TIME_OUT / 1000, .tv_usec = (TIME_OUT % 1000) * 1000 };

  if (setsockopt (*sock, SOL_PACKET, SO_RCVTIMEO, (char *) &timeout, sizeof (timeout)) == -1)
    {
      perror ("Nao pode configurar o timeout: ");
      return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}

#endif
