#ifdef LINUX

#ifndef SOCKET_H
#define SOCKET_H

#include "../common.h"

#include <arpa/inet.h>
#include <linux/if_packet.h>
#include <net/ethernet.h>
#include <net/if.h>

int get_socket (int *sock, char network_interface[]);

#endif
#endif
