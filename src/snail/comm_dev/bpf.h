#ifdef FREE_BSD
#ifndef BPF_H
#define BPF_H

#include <sys/types.h>
#include <sys/ioctl.h>
#include <net/bpf.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <fcntl.h>
#include <net/if.h>

#define BPF_DEV_NAME "/dev/bpf0"

int
get_bpf_dev(int *bpf, int *buf_len, char *network_interface);

#endif
#endif
