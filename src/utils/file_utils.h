#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/statvfs.h>

int 
obter_tamanho_arquivo(char *filename);

unsigned long long 
verifica_arquivo(char *filename);

#endif