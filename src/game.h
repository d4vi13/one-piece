#ifndef GAME_H
#define GAME_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "snail/snail.h"
#include "snail/file_ops.h"
#include "snail/pkg.h"

#include "objetos.h"

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
	int treasure;
	char* file_name;
};

typedef struct Jogo Jogo, *pJogo;

extern Jogo jogo;

void inicializa_jogo();

void 
receive_move_state ();

void 
send_move_state ();

void
receive_treasure_state ();

void
send_treasure_state ();

void 
stop_talking_state ();

void
start_talking_state ();


#endif
