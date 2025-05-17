#ifdef LINUX

#ifndef SOCKET_H
#define SOCKET_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <net/ethernet.h>
#include <net/if.h>
#include <linux/if_packet.h>

int
get_socket (int* sock, char network_interface[]);

#endif
#endif
