#ifndef OBJETOS_H
#define OBJETOS_H

#include "snail/snail.h"
#include "string.h"
#include <stdio.h>

#define QTD_ARQUIVOS 8
#define EXTENSAO_TAM 15

enum obj_types
{
  TEXTO,
  VIDEO,
  IMAGEM
};

typedef enum obj_types obj_types;

extern int arquivos[];
extern char extensao[][EXTENSAO_TAM];
extern int pkg_type[];

char *filename (int t);

#endif
