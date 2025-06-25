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

int arquivos[QTD_ARQUIVOS] = {
	TEXTO,
	TEXTO,
	TEXTO,
	TEXTO,
	TEXTO,
	TEXTO,
	VIDEO,
	IMAGEM
};

char* 
filename (int t) {
	char *str = malloc (EXTENSAO_TAM * 2);
	memset(str, 0, EXTENSAO_TAM * 2);

	sprintf(str, extensao[arquivos[t-1]], t);

	return str;
}
