#include "objetos.h"

int pkg_type[3] = {
	TEXT_ACK_NAME,
	VIDEO_ACK_NAME,
	IMG_ACK_NAME
};

char extensao[3][EXTENSAO_TAM] = {
	"objetos/%d.txt",
	"objetos/%d.mp4",
	"objetos/%d.jpg"
};

int arquivos[8];

void ler_tipos_arquivos_do_usuario() {
  printf("Digite os 8 tipos de arquivos (0=TEXTO, 1=VIDEO, 2=IMAGEM):\n");

  for (int i = 0; i < 8; i++) {
    int tipo = -1;
    while (tipo < 0 || tipo > 2) {
      printf("Arquivo %d: ", i + 1);
      scanf("%d", &tipo);
      if (tipo < 0 || tipo > 2) {
        printf("Tipo inválido. Tente 0, 1 ou 2.\n");
      }
    }
    arquivos[i] = tipo;
  }
}

int carregar_tipos_do_arquivo(const char *path) {
  FILE *f = fopen(path, "r");
  if (!f) {
    perror("Erro ao abrir arquivo de tipos");
    return -1;
  }

  int tipo, i = 0;
  while (i < 8 && fscanf(f, "%d", &tipo) == 1) {
    if (tipo < 0 || tipo > 2) {
      fprintf(stderr, "Tipo inválido na linha %d: %d\n", i + 1, tipo);
      fclose(f);
      return -1;
    }
    arquivos[i++] = tipo;
  }

  fclose(f);

  if (i != 8) {
    fprintf(stderr, "Esperado 8 tipos, mas lidos %d.\n", i);
    return -1;
  }

  return 0;
}

char* 
filename (int t) {
	char *str = malloc (EXTENSAO_TAM * 2);
	memset(str, 0, EXTENSAO_TAM * 2);

	sprintf(str, extensao[arquivos[t-1]], t);

	return str;
}
