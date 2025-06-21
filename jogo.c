#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#define GRID_SIZE 8

typedef struct {
  int x, y;
  int visited[GRID_SIZE][GRID_SIZE];
  int tesouros[GRID_SIZE][GRID_SIZE];
} Mapa;

void inicializa_mapa(Mapa *m) {
  m->x = 0;
  m->y = 0;
  for (int y = 0; y < GRID_SIZE; y++) {
    for (int x = 0; x < GRID_SIZE; x++) {
      m->visited[y][x] = 0;
      m->tesouros[y][x] = 0;
    }
  }

  srand(time(NULL));
  int colocados = 0;
  while (colocados < 8) {
    int x = rand() % GRID_SIZE;
    int y = rand() % GRID_SIZE;
    if (!m->tesouros[y][x]) {
      m->tesouros[y][x] = 1;
      colocados++;
    }
  }
  m->visited[m->y][m->x] = 1;
}

void marcar_visitado(Mapa *m) {
  m->visited[m->y][m->x] = 1;
}

int mover_jogador(Mapa *m, char direcao) {
  int novo_x = m->x, novo_y = m->y;
  switch (direcao) {
    case 'w': if (m->y < GRID_SIZE - 1) novo_y++; break;
    case 's': if (m->y > 0) novo_y--; break;
    case 'a': if (m->x > 0) novo_x--; break;
    case 'd': if (m->x < GRID_SIZE - 1) novo_x++; break;
    default: return 0; // movimento inválido
  }
  m->x = novo_x;
  m->y = novo_y;
  marcar_visitado(m);
  return 1;
}

int encontrou_tesouro(Mapa *m) {
  if (m->tesouros[m->y][m->x]) {
    m->tesouros[m->y][m->x] = 0;
    return 1;
  }
  return 0;
}


void limpar_tela() {
  printf("\033[2J\033[H"); // limpa terminal
}

void desenha_mapa(const Mapa *m) {
  for (int y = GRID_SIZE - 1; y >= 0; y--) {
    for (int x = 0; x < GRID_SIZE; x++) {
      if (x == m->x && y == m->y)
        printf("@ ");
      else if (m->tesouros[y][x])
        printf("$ ");
      else if (m->visited[y][x])
        printf(". ");
      else
        printf("- ");
    }
    printf("\n");
  }
}

char ler_comando() {
  printf("Use W A S D para mover. Q para sair.\n");
  char c = getchar();
  while (getchar() != '\n'); // consome linha
  return c;
}


int main() {
  Mapa mapa;
  inicializa_mapa(&mapa);

  while (1) {
    limpar_tela();
    desenha_mapa(&mapa);
    char cmd = ler_comando();
    if (cmd == 'q') break;

    if (!mover_jogador(&mapa, cmd)) {
      printf("Movimento inválido.\n");
      sleep(1);
      continue;
    }

    if (encontrou_tesouro(&mapa)) {
      printf("Você encontrou um tesouro!\n");
      sleep(1);
    }
  }

  return 0;
}
