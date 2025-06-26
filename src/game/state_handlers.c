#include "game.h"
/* STATE_HANDLERS */

void
receive_move_state ()
{
  desenha ();
  if (snail_recv (&jogo.recv) != EXIT_SUCCESS)
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
  
  if (jogo.treasure == 8)
  {
    printf ("Fim de jogo! Você encontrou todos os tesouros.\n");
    exit (0);
  }

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
  struct pkg *res = snail_send_and_wait (&jogo.send);
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
      jogo.treasure++;
      jogo.file_name = strdup ((char *)res->data);
      jogo.estado = PARA_DE_FALAR;
      return;
    }
}

void
receive_treasure_state ()
{
  struct pkg tamanho_pkg;
  if (snail_recv (&tamanho_pkg) != EXIT_SUCCESS || tamanho_pkg.type != SIZE)
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


  unsigned long long espaco_livre = verifica_arquivo (jogo.file_name);
  if ((espaco_livre == 0) || ((espaco_livre - 4096) < tamanho))
    {
      fprintf(stderr, "Nao sera posivel receber arquivo.\n");
      error_pkg(tamanho_pkg.sequence_number);
      free(jogo.file_name);
      jogo.file_name = NULL;

      jogo.estado = COMECA_FALAR;
      return;
    }

  // se tem espaco suficiente, envia ACK e recebe arquivo
  ok_ack_pkg (tamanho_pkg.sequence_number);

  if (recv_file (jogo.file_name) != EXIT_SUCCESS)
      fprintf (stderr, "Erro ao receber arquivo %s\n", jogo.file_name);
  else
      printf ("Arquivo salvo com sucesso: %s\n", jogo.file_name);

  char cmd[64];
  sprintf(cmd, "open %s 2>/dev/null", jogo.file_name);
  system(cmd);

  free (jogo.file_name);
  jogo.file_name = NULL;
  jogo.estado = COMECA_FALAR;
}

void
send_treasure_state ()
{
  int tamanho = obter_tamanho_arquivo (filename (jogo.treasure)); // pega tamanho do arquivo
  
  prepare_size_pkg (&jogo.send, tamanho);
  struct pkg *res = snail_send_and_wait (&jogo.send);
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
  sleep(1);

  stop_talking (); // espera por qualquer coisa e sai
  jogo.estado = RECEBE;
}

void
start_talking_state ()
{
  printf ("Começando a falar...\n");
  sleep (1); // espera 1 segundo para mostrar a mensagem

  start_talking ();
  jogo.estado = MANDA;
}


