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

  int ret = 0;

  switch (comm_dev.comm_type)
    {
    case BPF:
#ifdef FREE_BSD
      ret = write(comm_dev.fd, pkg, PKG_SIZE); 
      printf("ret %d\n", ret);
      if (ret == -1)
        {
          perror ("Nao pode enviar o pacote");
          return EXIT_FAILURE;
        }
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
_recv_pkg (struct pkg *pkg)
{
  errno = 0;

  int ret = 0;

  switch (comm_dev.comm_type)
    {
    case BPF:
#ifdef FREE_BSD
      struct bpf_xhdr *bh;
      char *buf = malloc(BPF_BUF_SIZE);
      if (!buf)
      {
        perror ("Nao pode alocar buffer para bpf: ");
        return EXIT_FAILURE;
      }

      ret = read (comm_dev.fd, buf, BPF_BUF_SIZE);
      bh = (struct bpf_xhdr *)buf;

      memcpy(pkg, buf + bh->bh_hdrlen , PKG_SIZE);  

      if (ret == -1)
        {
          perror ("Nao pode receber o pacote");
          return EXIT_FAILURE;
        }
#endif
      break;
    case SOCKET:
#ifdef LINUX
      ret = recv (comm_dev.fd, pkg, PKG_SIZE, 0);
      if (ret == -1)
        {
          return EXIT_FAILURE;
        }
#endif
      break;
    }

  if (!valid_pkg (pkg))
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
