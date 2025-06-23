#ifndef OBJETOS_H
#define OBJETOS_H

#include <stdio.h>
#include "string.h"
#include "snail/snail.h"

enum obj_types {
	TEXTO,
	VIDEO,
	IMAGEM
};

typedef enum obj_types obj_types;

extern int arquivos[];
extern char extensao[][13];
extern int pkg_type[];

char* 
filename (int t);

#endif
