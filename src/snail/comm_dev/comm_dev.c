#include "comm_dev.h"

struct comm_dev comm_dev;

int
init_comm_dev (comm_t comm_type, char network_interface[])
{
  errno = 0;

  int ret = 0;

  comm_dev.comm_type = comm_type;

  switch (comm_type)
    {
    case BPF:
#ifdef FREE_BSD
      int buf_len = PKG_SIZE;
      ret = get_bpf_dev (&comm_dev.fd, &buf_len, network_interface);
      if (ret == EXIT_FAILURE)
        {
          perror ("Nao consegui dispositvo bpf: ");
          return EXIT_FAILURE;
        }
#endif
      break;
    case SOCKET:
#ifdef LINUX
      ret = get_socket (&comm_dev.fd, network_interface);
      if (ret == EXIT_FAILURE)
        {
          perror ("Nao consegui socket: ");
          return EXIT_FAILURE;
        }
#endif
      break;
    }

  return EXIT_SUCCESS;
}

static inline int
prepare_pkg (struct pkg *pkg, pkg_t pkg_type, uint8_t *data, uint8_t size)
{
  errno = 0;

  static uint32_t seq = 0;

  memset (pkg->data, 0, MAX_DATA);

  pkg->start_marker = START_MARKER;
  pkg->size = size;
  pkg->sequence_number = seq++;
  pkg->type = pkg_type;
  pkg->checksum = 0;

  memcpy (pkg->data, data, min (size, MAX_DATA));

  return EXIT_SUCCESS;
}

int
send_pkg (struct pkg *pkg, pkg_t pkg_type, uint8_t *data, uint8_t size)
{
  errno = 0;

  int ret = 0;

  ret = prepare_pkg (pkg, pkg_type, data, size);
  if (ret == EXIT_FAILURE)
    {
      perror ("Nao pode prepara pacote: ");
      return EXIT_FAILURE;
    }

  switch (comm_dev.comm_type)
    {
    case BPF:
#ifdef FREE_BSD
#endif
      break;
    case SOCKET:
#ifdef LINUX
      ret = send (comm_dev.fd, pkg, PKG_SIZE, 0);
      if (ret == -1)
        {
          perror ("Nao pode enviar o pacote");
          return EXIT_FAILURE;
        }
#endif
      break;
    }

  return EXIT_SUCCESS;
}

int
recv_pkg (struct pkg *pkg)
{
  errno = 0;

  int ret = 0;

  switch (comm_dev.comm_type)
    {
    case BPF:
#ifdef FREE_BSD
#endif
      break;
    case SOCKET:
#ifdef LINUX
      ret = recv (comm_dev.fd, pkg, PKG_SIZE, 0);
      if (ret == -1)
        {
          perror ("Nao pode enviar o pacote");
          return EXIT_FAILURE;
        }
#endif
      break;
    }

  return EXIT_SUCCESS;
}
