#ifndef COMMON_H
#define COMMON_H

#define LINUX

#include <errno.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define min(a, b) ((a) < (b) ? (a) : (b))
#define BPF_BUF_SIZE 256
#define TIME_OUT 100

#endif
