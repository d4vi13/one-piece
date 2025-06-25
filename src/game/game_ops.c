#include "game.h"

void
inicializa_jogo ()
{
  memset (&jogo, 0, sizeof (Jogo));
  jogo.x = 0;
  jogo.x = 0;

  srand (time (NULL));
  int colocados = 0;
  while (colocados < 8)
    {
      int x = rand () % GRID_SIZE;
      int y = rand () % GRID_SIZE;
      if ((jogo.grid[y][x] == 0) && (y || x))
        {
          colocados++;
          jogo.grid[y][x] = colocados;
          printf ("Tesouro %d: (%d, %d)\n", colocados, x, y);
        }
    }

  printf ("Tesouros gerados!\n");
}

void
atualiza_posicao (pkg_t tipo)
{
  int x = jogo.x;
  int y = jogo.y;

  switch (tipo)
    {
    case SHIFT_UP:
      if (y < GRID_SIZE - 1)
        y++;
      break;
    case SHIFT_DOWN:
      if (y > 0)
        y--;
      break;
    case SHIFT_LEFT:
      if (x > 0)
        x--;
      break;
    case SHIFT_RIGHT:
      if (x < GRID_SIZE - 1)
        x++;
      break;
    default:
      return;
    }

  jogo.x = x;
  jogo.y = y;
}

int
tesouro_na_posicao ()
{
  int x, y, ret;
  x = jogo.x;
  y = jogo.y;
  ret = jogo.grid[y][x];

  if (ret > 0)
    jogo.grid[y][x] = -1; // coletado!  
  else if (ret < 0) {
    ret = 0;
  }

  return ret;
}

void
limpar ()
{
  printf ("\033[2J\033[H");
}

void
desenha ()
{
  printf ("Posicao: (%d, %d)\n", jogo.x, jogo.y);
  for (int j = GRID_SIZE - 1; j >= 0; j--)
    {
      for (int i = 0; i < GRID_SIZE; i++)
        {
          if (jogo.x == i && jogo.y == j)
            printf ("@ ");
          else if (jogo.grid[j][i] > 0)
            printf ("$ ");
          else if (jogo.grid[j][i] == -1)
            printf ("X ");
          else if (jogo.grid[j][i] == -2)
            printf (". ");
          else
            printf ("- ");
        }
      printf ("\n");
    }
}

char
ler_comando ()
{
  printf ("Comando (WASD, Q p/ sair): ");
  char c = getchar ();
  while (getchar () != '\n')
    ;
  return c;
}

pkg_t
direcao_para_tipo (char c)
{
  switch (c)
    {
    case 'w':
      return SHIFT_UP;
    case 'a':
      return SHIFT_LEFT;
    case 's':
      return SHIFT_DOWN;
    case 'd':
      return SHIFT_RIGHT;
    default:
      return ERROR;
    }
}
