#ifndef GAME_H
#define GAME_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "snail/snail.h"

#define GRID_SIZE 8

enum state{
	RECEBE,
	MANDA,
	PARA_DE_FALAR,
	COMECA_FALAR
};

typedef enum state state, *pstate;

struct Jogo{
  int grid[GRID_SIZE][GRID_SIZE];
  int x;
  int y;

	struct pkg send;
	struct pkg recv;

	state estado;
};

typedef struct Jogo Jogo, *pJogo;

extern Jogo jogo;

#endif
