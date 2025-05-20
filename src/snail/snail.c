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

  /* Cria raw socket para comunicacao*/
  ret = init_comm_dev (BPF, network_interface);
  if (ret == EXIT_FAILURE)
    {
      perror ("Erro ao iniciar dispositivo de comunicação\n");
      return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}

static inline void
print_pkg (struct pkg pkg)
{
//if (pkg.start_marker != START_MARKER)
//    return;

  printf ("start_marker: %d\n", pkg.start_marker);
  printf ("size: %d\n", pkg.size);
  printf ("sequence_number: %d\n", pkg.sequence_number);
  printf ("type: %d\n", pkg.type);
  printf ("checksum: %d\n", pkg.checksum);
  printf ("data: %s\n", pkg.data);
}

int
snail_send (pkg_t pkg_type, char *data, uint8_t size)
{
  errno = 0;

  int ret = send_pkg (&snail.pkg, pkg_type, data, size);
  if (ret == EXIT_FAILURE)
    {
      perror ("Nao conseguir enviar pacote: ");
      return EXIT_FAILURE;
    }
  return EXIT_SUCCESS;
}

int
snail_recv ()
{
  errno = 0;

  memset(&snail.pkg, 0, sizeof snail.pkg);

  int ret = recv_pkg (&snail.pkg);
  if (ret == EXIT_FAILURE)
    {
      perror ("Nao pode receber um pacote: ");
      return EXIT_FAILURE;
    }

  print_pkg(snail.pkg);
  return EXIT_SUCCESS;
}


