#include "game.h"

Jogo jogo;

int main(int argc, char *argv[]) {
	if (argc < 2) {
		fprintf(stderr, "Uso: %s <interface>\n", argv[0]);
		return EXIT_FAILURE;
	}

	init_snail(argv[1]);
	inicializa_jogo();

	while (1) {
		switch (jogo.estado) {
			case RECEBE:
				receive_move_state ();
				break;
			case MANDA:
				send_treasure_state ();
				break;
			case PARA_DE_FALAR:
				stop_talking_state();
				break;
			case COMECA_FALAR:
				start_talking_state ();
				break;
		}
	}
}
