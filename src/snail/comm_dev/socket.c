#ifdef LINUX
#include "socket.h"

static inline int
get_socket (int *sock, char network_interface[])
{
  errno = 0;
  // Cria arquivo para o socket sem qualquer protocolo
  *soquete = socket (AF_PACKET, SOCK_RAW, htons (ETH_P_ALL));
  if (*soquete == -1)
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
  if (bind (*soquete, (struct sockaddr *)&endereco, sizeof (endereco)) == -1)
    {
      perror ("Erro ao fazer bind no socket\n");
      return EXIT_FAILURE;
    }

  struct packet_mreq mr = { 0 };
  mr.mr_ifindex = ifindex;
  mr.mr_type = PACKET_MR_PROMISC;
  // Não joga fora o que identifica como lixo: Modo promíscuo
  if (setsockopt (*soquete, SOL_PACKET, PACKET_ADD_MEMBERSHIP, &mr,
                  sizeof (mr))
      == -1)
    {
      perror ("Erro ao fazer setsockop\n");
      return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}

#endif
