#include "game.h"
#include "snail/pkg.h"
#include <sys/stat.h>
#include <sys/statvfs.h>

/* UTILS */

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

int
obter_tamanho_arquivo (char *filename)
{
  FILE *file = fopen (filename, "rb");
  if (!file)
    {
      perror ("Erro ao abrir arquivo");
      return -1;
    }

  fseek (file, 0, SEEK_END);
  int size = ftell (file);
  fclose (file);

  return size;
}

/* STATE_HANDLERS */

void
receive_move_state ()
{
  desenha ();
  if (snail_recv (&jogo.recv, 0) != EXIT_SUCCESS)
    {
      fprintf (stderr, "Erro ao receber pacote do cliente.\n");
      return;
    }

  if (jogo.recv.type < SHIFT_RIGHT || jogo.recv.type > SHIFT_LEFT)
    {
      printf ("Tipo de pacote inválido recebido: %d\n", jogo.recv.type);
      return;
    }

  atualiza_posicao (jogo.recv.type);

  jogo.treasure = tesouro_na_posicao ();
  if (jogo.treasure > 0)
    {
      treasure_ack (jogo.recv.sequence_number,
                    pkg_type[arquivos[jogo.treasure - 1]],
                    filename (jogo.treasure));
      jogo.estado = COMECA_FALAR;
    }
  else
    {
      ok_ack_pkg (jogo.recv.sequence_number);
    }
}

void
send_move_state ()
{
  limpar ();
  desenha ();

  char c = ler_comando ();
  if (c == 'q')
    exit (0);

  pkg_t dir = direcao_para_tipo (c);
  if (dir == ERROR)
    {
      printf ("Movimento inválido!\n");
      return;
    }

  if (!jogo.y && !jogo.x)
  {
    jogo.grid[jogo.y][jogo.x] = -2;
  }
  // struct pkg movimento;
  prepare_ack_pkg (&jogo.send, get_seq_num (), dir);
  snail_send (&jogo.send);

  struct pkg *res = wait_pkg_n (jogo.send.sequence_number);
  if (res->type == OK_ACK)
    {
      printf ("Movimento realizado com sucesso!\n");
      atualiza_posicao (dir);
      jogo.grid[jogo.y][jogo.x] = -2;
      return;
    }

  if (res->type >= TEXT_ACK_NAME && res->type <= IMG_ACK_NAME)
    {
      printf ("Tesouro encontrado: %s@\n", res->data);
      atualiza_posicao (dir);
      jogo.grid[jogo.y][jogo.x] = -1;
      jogo.file_name = strdup ((char *)res->data);
      jogo.estado = PARA_DE_FALAR;
      return;
    }
}

void
receive_treasure_state ()
{
  struct pkg tamanho_pkg;
  if (snail_recv (&tamanho_pkg, 0) != EXIT_SUCCESS || tamanho_pkg.type != SIZE)
    {
      fprintf (stderr, "Erro ao receber tamanho do arquivo.\n");
      return;
    }

  uint32_t tamanho;
  memcpy (&tamanho, tamanho_pkg.data, sizeof (uint32_t));

  if (!jogo.file_name)
    {
      fprintf (stderr, "Erro: nome do arquivo não está definido!\n");
      return;
    }

  // verifica se o arquivo eh regular
  struct stat st;
  if (stat (jogo.file_name, &st) == 0)
    {
      if (!S_ISREG (st.st_mode))
        {
          fprintf (stderr, "Erro: o destino não é um arquivo regular.\n");

          error_pkg(tamanho_pkg.sequence_number);
          free (jogo.file_name);
          jogo.file_name = NULL;
          return;
        }
    }


  FILE *file = fopen (jogo.file_name, "w");
  if (!file)
    {
      perror("failed to open file");
    }
  else
    {
      fclose (file);
    }


  // verifica se ha espaco disponivel
  struct statvfs fs;
  if (statvfs (jogo.file_name, &fs) != 0)
    {
      perror ("Erro ao aessar o sistema de arquivos com statvfs\n");
      error_pkg(tamanho_pkg.sequence_number);
      free (jogo.file_name);
      jogo.file_name = NULL;
      return;
    }

  unsigned long long espaco_livre = fs.f_bsize * fs.f_bavail;

  // adiciona tolerância de 1% ou 8KB
  const unsigned long long tolerancia = 8192;
  if (tamanho > espaco_livre - tolerancia)
    {
      fprintf (stderr,
               "Espaço insuficiente: %u bytes requeridos, %llu disponíveis.\n",
               tamanho, espaco_livre);

      jogo.estado = COMECA_FALAR;
      error_pkg(tamanho_pkg.sequence_number);
      free (jogo.file_name);
      jogo.file_name = NULL;
      return;
    }

  // se tem espaco suficiente, envia ACK e recebe arquivo
  ack_pkg (tamanho_pkg.sequence_number);

  if (recv_file (jogo.file_name) != EXIT_SUCCESS)
    {
      fprintf (stderr, "Erro ao receber arquivo %s\n", jogo.file_name);
    }
  else
    {
      printf ("Arquivo salvo com sucesso: %s\n", jogo.file_name);
    }

  char cmd[64];
  sprintf(cmd, "open %s", jogo.file_name);
  system(cmd);

  free (jogo.file_name);
  jogo.file_name = NULL;
  jogo.estado = COMECA_FALAR;
}

void
send_treasure_state ()
{
  int tamanho = obter_tamanho_arquivo (
      filename (jogo.treasure)); // pega tamanho do arquivo
  prepare_size_pkg (&jogo.send, tamanho);
  snail_send (&jogo.send);

  struct pkg *res = wait_pkg_n (jogo.send.sequence_number);
  if (res->type == ERROR)
    {
      fprintf (stderr, "Sem espaço suficiente para enviar o arquivo.\n");
      jogo.estado = PARA_DE_FALAR;
      return;
    }

  send_file (filename (jogo.treasure));



  jogo.estado = PARA_DE_FALAR;
}

void
stop_talking_state ()
{
  printf ("Parando de falar...\n");
  sleep (1);             // espera 1 segundo para mostrar a mensagem
  send_start_talking (); // espera por qualquer coisa e sai
  jogo.estado = RECEBE;
}

void
start_talking_state ()
{
  printf ("Começando a falar...\n");
  sleep (1); // espera 1 segundo para mostrar a mensagem
  while (1)
    {
      snail_recv (&jogo.recv, 1);
      if (jogo.recv.type == FREE)
        {
          jogo.estado = MANDA;
          break;
        }
    }

    set_didnt_hear_back();
}
