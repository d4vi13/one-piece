#ifndef OBJETOS_H
#define OBJETOS_H

#include "snail/snail.h"
#include "string.h"
#include <stdio.h>

enum obj_types
{
  TEXTO,
  VIDEO,
  IMAGEM
};

typedef enum obj_types obj_types;

extern int arquivos[];
extern char extensao[][15];
extern int pkg_type[];

char *filename (int t);

#endif
