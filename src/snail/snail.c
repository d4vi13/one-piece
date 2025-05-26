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
  ret = init_comm_dev (SOCKET, network_interface);
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

/* Serializa um pacote em um buffer de bytes */
size_t 
serialize_pkg (const struct pkg *pkg, uint8_t *out_buf) 
{
  out_buf[0] = pkg->start_marker;
  out_buf[1] = (pkg->size & 0x7F) | ((pkg->sequence_number & 0x01) << 7);
  out_buf[2] = ((pkg->sequence_number >> 1) & 0x0F) << 4 | (pkg->type & 0x0F);
  out_buf[3] = pkg->checksum;

  memcpy(&out_buf[4], pkg->data, pkg->size);
  return 4 + pkg->size;
}


/* Desserializa um buffer de bytes em uma estrutura de pacote */
void 
deserialize_pkg (struct pkg *pkg, const uint8_t *in_buf, size_t len) 
{
  pkg->start_marker = in_buf[0];
  pkg->size = in_buf[1] & 0x7F;

  uint8_t seq_lo = (in_buf[1] >> 7) & 0x01;
  uint8_t seq_hi = (in_buf[2] >> 4) & 0x0F;
  pkg->sequence_number = (seq_hi << 1) | seq_lo;

  pkg->type = in_buf[2] & 0x0F;
  pkg->checksum = in_buf[3];

  memset(pkg->data, 0, MAX_DATA);
  memcpy(pkg->data, &in_buf[4], pkg->size);
}

