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
	TEXTO,
	TEXTO
};

char* 
filename (int t) {
	char *str = malloc (EXTENSAO_TAM + 1);
	str[EXTENSAO_TAM] = 0;

	sprintf(str, extensao[arquivos[t]], t);

	return str;
}
