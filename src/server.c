#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "snail/snail.h"

#define GRID_SIZE 8

typedef struct {
  int tesouros[GRID_SIZE][GRID_SIZE];
  int jogador_x;
  int jogador_y;
} Jogo;

Jogo jogo;

void inicializa_jogo() {
  memset(&jogo, 0, sizeof(Jogo));
  jogo.jogador_x = 0;
  jogo.jogador_y = 0;

  srand(time(NULL));
  int colocados = 0;
  while (colocados < 8) {
    int x = rand() % GRID_SIZE;
    int y = rand() % GRID_SIZE;
    if (jogo.tesouros[y][x] == 0) {
      jogo.tesouros[y][x] = 1;
      colocados++;
      printf("Tesouro %d: (%d, %d)\n", colocados, x, y);
    }
  }

  printf("Tesouros gerados!\n");
}

void atualiza_posicao(pkg_t tipo) {
  int x = jogo.jogador_x;
  int y = jogo.jogador_y;

  switch (tipo) {
    case SHIFT_UP:    if (y < GRID_SIZE - 1) y++; break;
    case SHIFT_DOWN:  if (y > 0) y--; break;
    case SHIFT_LEFT:  if (x > 0) x--; break;
    case SHIFT_RIGHT: if (x < GRID_SIZE - 1) x++; break;
    default: return;
  }

  jogo.jogador_x = x;
  jogo.jogador_y = y;
}

int tesouro_na_posicao() {
  int x = jogo.jogador_x;
  int y = jogo.jogador_y;
  if (jogo.tesouros[y][x]) {
    jogo.tesouros[y][x] = 0;  // coletado!
    return 1;
  }
  return 0;
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(stderr, "Uso: %s <interface>\n", argv[0]);
    return EXIT_FAILURE;
  }

  if (init_snail(argv[1]) != EXIT_SUCCESS) {
    fprintf(stderr, "Erro ao iniciar protocolo SNAIL.\n");
    return EXIT_FAILURE;
  }

  inicializa_jogo();

   while (1) {
    struct pkg p;
    if (snail_recv(&p, 1) != EXIT_SUCCESS) {
        fprintf(stderr, "Erro ao receber pacote do cliente.\n");
        continue;
    }

    if (p.type == FREE2) {
      printf("Cliente encerrou o jogo. Servidor finalizando...\n");
      break;
    }

    // Ignora pacotes de ACK (tipo 0)
    if (p.type == ACK)
        continue;

    // Verifica se é um tipo válido de movimento
    if (p.type < SHIFT_RIGHT || p.type > SHIFT_LEFT) {
        printf("Tipo de pacote inválido recebido: %d\n", p.type);
        continue;
    }

    printf("Recebido: tipo %d, seq %d\n", p.type, p.sequence_number);

    atualiza_posicao(p.type);

    struct pkg resposta;
    if (tesouro_na_posicao()) {
        prepare_data_pkg(&resposta, "TESOURO!", 8);
    } else {
        prepare_ack_pkg(&resposta, get_seq_num(), FREE);
    }

    snail_send(&resposta);
    wait_pkg_n(resposta.sequence_number);

    struct pkg troca;
    prepare_ack_pkg(&troca, get_seq_num(), OK_ACK);
    snail_send(&troca);
    wait_pkg_n(resposta.sequence_number);
}


  return EXIT_SUCCESS;
}
