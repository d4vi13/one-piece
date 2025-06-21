#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "snail/snail.h"

#define GRID_SIZE 8

int x = 0, y = 0;
int visitado[GRID_SIZE][GRID_SIZE];

void limpar() {
  printf("\033[2J\033[H");
}

void desenha() {
  printf("Posicao: (%d, %d)\n", x, y);
  for (int j = GRID_SIZE - 1; j >= 0; j--) {
    for (int i = 0; i < GRID_SIZE; i++) {
      if (x == i && y == j)
        printf("@ ");
      else if (visitado[j][i])
        printf(". ");
      else
        printf("- ");
    }
    printf("\n");
  }
}

char ler_comando() {
  printf("Comando (WASD, Q p/ sair): ");
  char c = getchar();
  while (getchar() != '\n');
  return c;
}

pkg_t direcao_para_tipo(char c) {
  switch (c) {
    case 'w': return SHIFT_UP;
    case 'a': return SHIFT_LEFT;
    case 's': return SHIFT_DOWN;
    case 'd': return SHIFT_RIGHT;
    default: return ERROR;
  }
}

void atualiza_pos(pkg_t tipo) {
  switch (tipo) {
    case SHIFT_UP: if (y < GRID_SIZE - 1) y++; break;
    case SHIFT_DOWN: if (y > 0) y--; break;
    case SHIFT_LEFT: if (x > 0) x--; break;
    case SHIFT_RIGHT: if (x < GRID_SIZE - 1) x++; break;
    default: break;
  }
  visitado[y][x] = 1;
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(stderr, "Uso: %s <interface>\n", argv[0]);
    return 1;
  }
  init_snail(argv[1]);
  memset(visitado, 0, sizeof(visitado));
  visitado[y][x] = 1;

  while (1) {
    limpar();
    desenha();
    char c = ler_comando();
    if (c == 'q') {
        struct pkg quit;
        prepare_ack_pkg(&quit, get_seq_num(), FREE2);
        snail_send(&quit);
        wait_pkg_n(quit.sequence_number);
        break;
    }

    pkg_t tipo = direcao_para_tipo(c);
    if (tipo == ERROR){
        printf("Movimento inválido!\n");
        continue;
    }

    struct pkg moviment;
    prepare_ack_pkg(&moviment, get_seq_num(), tipo);
    snail_send(&moviment);
    wait_pkg_n(moviment.sequence_number);

    struct pkg resposta;
    if (snail_recv(&resposta, 1) != EXIT_SUCCESS) continue;
    
    if (resposta.type == DATA)
      printf("Mensagem do servidor: %s\n", resposta.data);
    else if (resposta.type == FREE)
      printf("Nada aqui.\n");
    else
      printf("Resposta desconhecida.\n");

    struct pkg troca;
    if (snail_recv(&troca, 1) == EXIT_SUCCESS && troca.type == OK_ACK)
        atualiza_pos(tipo);

    sleep(1);
  }

  return 0;
}
