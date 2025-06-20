#ifndef COMM_DEV_H
#define COMM_DEV_H

#include "../common.h"

#include "../pkg.h"
#include "bpf.h"
#include "socket.h"

#define ERRNO_IS_TIMEOUT (errno == EAGAIN || errno == EWOULDBLOCK)

enum comm_t
{
  SOCKET,
  BPF
};

typedef enum comm_t comm_t;

struct comm_dev
{
  comm_t comm_type;
  int fd;
};

extern struct comm_dev comm_dev;

int init_comm_dev (comm_t comm_type, char network_interface[]);

int send_pkg (struct pkg *pkg);

int recv_pkg (struct pkg *pkg);

#endif
