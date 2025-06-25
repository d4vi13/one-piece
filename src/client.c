#include "game.h"

Jogo jogo;

int
main (int argc, char *argv[])
{
  if (argc < 2)
    {
      fprintf (stderr, "Uso: %s <interface>\n", argv[0]);
      return 1;
    }

  init_snail (argv[1]);
  memset (jogo.grid, 0, sizeof (jogo.grid));
  jogo.grid[jogo.y][jogo.x] = 1;
  jogo.estado = MANDA;
  jogo.treasure = 0;

  while (1)
    {
      switch (jogo.estado)
        {
        case MANDA:
          send_move_state ();
          break;
        case RECEBE:
          receive_treasure_state ();
          break;
        case PARA_DE_FALAR:
          stop_talking_state ();
          break;
        case COMECA_FALAR:
          start_talking_state ();
          break;
        }
    }
  return 0;
}
