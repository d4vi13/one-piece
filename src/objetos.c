#include "objetos.h"

int pkg_type[3] = {
	TEXT_ACK_NAME,
	VIDEO_ACK_NAME,
	IMG_ACK_NAME
};

char extensao[3][13] = {
	"objetos/%d.txt",
	"objetos/%d.mp4",
	"objetos/%d.jpg"
};

int arquivos[8] = {
	TEXTO,
	IMAGEM,
	VIDEO,
	TEXTO,
	IMAGEM,
	VIDEO,
	TEXTO,
	IMAGEM
};

char* 
filename (int t) {
	char *str = malloc (6);
	str[5] = 0;

	sprintf(str, extensao[arquivos[t]], t);

	return str;
}
