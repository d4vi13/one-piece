#include "game.h"

/* STATE_HANDLERS */

void 
receive_move_state ()
{
	if (snail_recv(&jogo.recv, 1) != EXIT_SUCCESS)
	 	{
			fprintf(stderr, "Erro ao receber pacote do cliente.\n");
			return;
    }

	if (jogo.recv.type < SHIFT_RIGHT || jogo.recv.type > SHIFT_LEFT) 
		{
			printf("Tipo de pacote inválido recebido: %d\n", jogo.recv.type);
			return;
		}

		atualiza_posicao(p.type);

		int t = tesouro_na_posicao();
		if (t)
			{
				reportar_tesouro(t);
			}
		else
			{
				ack_move (); 	
			}
}

void 
send_move_state ()
{
	char c = ler_comando();

	pkg_t tipo = direcao_para_tipo(c);
	if (tipo == ERROR){
		printf("Movimento inválido!\n");
		continue;
	}

	send_move (); // manda move e recebe res (jogo.res)

	switch (jogo.res) 
	{
		case OK_ACK:
			break;
		case TEXT_ACK_NAME ... IMG_ACK_NAME:
			jogo.estado = PARAR_DE_FALAR;
			break;
	}
}

void
receive_treasure_state ()
{
	recebe tamanho
	verifica se tem espaco suficiente

	recebe arquivo

	jogo.estado = MANDA
}

void
send_treasure_state ()
{
	pegar nome inteiro do arquivo	

	enviar tamanho
	senao houver erro
		enviar arquivo

}

void 
stop_talking_state ()
{
	send_start_talking() // espera por qualquer coisa e sai 
	jogo.estado = RECEBE
}

void
start_talking_state ()
{
	receber
	se receber pacote passado
		remandar ultimo ack
	se start talking
		ack start talking
		troca de estado para MANDA	
}

/* UTILS */

void inicializa_jogo() {
  memset(&jogo, 0, sizeof(Jogo));
  jogo.x = 0;
  jogo.x = 0;

  srand(time(NULL));
  int colocados = 0;
  while (colocados < 8) {
    int x = rand() % GRID_SIZE;
    int y = rand() % GRID_SIZE;
    if (jogo.grid[y][x] == 0) {
      colocados++;
      jogo.grid[y][x] = colocados;
      printf("Tesouro %d: (%d, %d)\n", colocados, x, y);
    }
  }

  printf("Tesouros gerados!\n");
}

void atualiza_posicao(pkg_t tipo) {
  int x = jogo.x;
  int y = jogo.y;

  switch (tipo) {
    case SHIFT_UP:    if (y < GRID_SIZE - 1) y++; break;
    case SHIFT_DOWN:  if (y > 0) y--; break;
    case SHIFT_LEFT:  if (x > 0) x--; break;
    case SHIFT_RIGHT: if (x < GRID_SIZE - 1) x++; break;
    default: return;
  }

  jogo.x = x;
  jogo.y = y;
}

int tesouro_na_posicao() {
	int x, y, ret;
  x = jogo.x;
  y = jogo.y;
	ret = jogo.grid[y][x];

	jogo.grid[y][x] = 0;  // coletado!

  return ret;
}

void limpar() {
  printf("\033[2J\033[H");
}

void desenha() {
  printf("Posicao: (%d, %d)\n", x, y);
  for (int j = GRID_SIZE - 1; j >= 0; j--) {
    for (int i = 0; i < GRID_SIZE; i++) {
      if (x == i && y == j)
        printf("@ ");
      else if (jogo.grid[j][i])
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
