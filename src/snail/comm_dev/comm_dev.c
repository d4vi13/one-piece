#include "comm_dev.h"
#include "../checksum.h"
#include "../escape.h"
#include "../snail.h"

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
      int buf_len = BPF_BUF_SIZE;
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

int
valid_pkg (struct pkg *pkg)
{
  return pkg->start_marker == 0b01111110;
}

int
send_pkg (struct pkg *pkg)
{
  errno = 0;
  int ret;

  //Buffer para serialização e escape
  uint8_t raw_buf[PKG_SIZE];
  uint8_t escaped_buf[2 * PKG_SIZE]; // espaço extra para escapes

  //Serializa pacote -> raw_buf
  //size_t serialized_len = serialize_pkg(pkg, raw_buf);

  //Aplica escape -> escaped_buf
  int escaped_len = escape_bytes((uint8_t *)pkg, PKG_SIZE, escaped_buf);

  //Envia pela rede conforme o tipo
  switch (comm_dev.comm_type) {

    case BPF:
#ifdef FREE_BSD
      ret = write(comm_dev.fd, escaped_buf, escaped_len);
      if (ret == -1) {
        perror("Nao pode enviar o pacote");
        return EXIT_FAILURE;
      }
#endif
      break;
    case SOCKET:
#ifdef LINUX
      ret = send(comm_dev.fd, escaped_buf, escaped_len, 0);
      if (ret == -1) {
        perror("Nao pode enviar o pacote");
        return EXIT_FAILURE;
      }
#endif
      break;
  }

  return EXIT_SUCCESS;
}

int
_recv_pkg (struct pkg *pkg)
{
  errno = 0;
  int ret;

  //Buffer para leitura (máximo possível com escapes)
  uint8_t escaped_buf[2 * PKG_SIZE];
  int received_len = 0;

  switch (comm_dev.comm_type) {
    case BPF:
#ifdef FREE_BSD
      struct bpf_xhdr *bh;
      char *buf = malloc(BPF_BUF_SIZE);
      if (!buf) {
        perror("Nao pode alocar buffer para bpf");
        return EXIT_FAILURE;
      }

      ret = read(comm_dev.fd, buf, BPF_BUF_SIZE);
      if (ret == -1) {
        perror("Nao pode receber o pacote");
        free(buf);
        return EXIT_FAILURE;
      }

      bh = (struct bpf_xhdr *)buf;
      memcpy(escaped_buf, buf + bh->bh_hdrlen, ret - bh->bh_hdrlen);
      received_len = ret - bh->bh_hdrlen;
      free(buf);
#endif
      break;

    case SOCKET:
#ifdef LINUX
      ret = recv(comm_dev.fd, escaped_buf, sizeof(escaped_buf), 0);
      if (ret == -1) {
        perror("Nao pode receber o pacote");
        return EXIT_FAILURE;
      }
      received_len = ret;
#endif
      break;
  }

  //Remove escapes
  int raw_len = unescape_bytes(escaped_buf, received_len, (uint8_t *) pkg);

  if (!valid_pkg (pkg) || !validate_checksum (pkg))
    {
      return EXIT_FAILURE;
    }
  return EXIT_SUCCESS;
}

long long 
timestamp()
{
    struct timeval tp;
    gettimeofday (&tp, NULL);
    return tp.tv_sec*1000 + tp.tv_usec/1000;
}


int
recv_pkg (struct pkg *pkg)
{
  errno = 0;
  int ret = 0;

  long long comeco = timestamp ();

  do 
    {
      ret = _recv_pkg (pkg);
      if (ret == EXIT_SUCCESS)
        break;
    }
  while (timestamp () - comeco <= TIME_OUT);

  if (ret == EXIT_FAILURE)
    errno = EAGAIN;

  return ret;
}
