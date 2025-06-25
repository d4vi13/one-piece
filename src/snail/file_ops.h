#ifndef FILE_OPS_H
#define FILE_OPS_H

#include "common.h"
#include "snail.h"
#include "sliding_window/sliding_window.h"

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>

int 
send_file (char *filename);

int 
recv_file (char *filename);

#endif
