#ifndef BPF_H
#define BPF_H

#include "../common.h"

#ifdef FREE_BSD

#include <net/bpf.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/types.h>

#define BPF_DEV_NAME "/dev/bpf0"

int get_bpf_dev (int *bpf, int *buf_len, char *network_interface);

#endif
#endif
