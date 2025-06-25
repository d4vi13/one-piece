#include "game.h"
#include "objetos.h"

Jogo jogo;

int
main (int argc, char *argv[])
{
  if (argc < 3)
    {
      fprintf (stderr, "Uso: %s <interface> <tipos.txt>\n", argv[0]);
      return EXIT_FAILURE;
    }

  /*if (carregar_tipos_do_arquivo(argv[2]) != 0) {
    fprintf(stderr, "Erro ao carregar os tipos de arquivos.\n");
    return 1;
  }*/

  ler_tipos_arquivos_do_usuario();
  init_snail (argv[1]);
  inicializa_jogo ();

  while (1)
    {
      switch (jogo.estado)
        {
        case RECEBE:
          receive_move_state ();
          break;
        case MANDA:
          send_treasure_state ();
          break;
        case PARA_DE_FALAR:
          stop_talking_state ();
          break;
        case COMECA_FALAR:
          start_talking_state ();
          break;
        }
    }
}
