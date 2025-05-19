#include "checksum.h"

uint8_t
calculate_checksum (const struct pkg *pkg)
{
	uint16_t sum = 0;
	
	sum += pkg->size;
	sum += pkg->sequence_number;
	sum += pkg->type;

	for (int i = 0; i < pkg->size; i++)
		sum += pkg->data[i];

	/* Retorna os 8 bits menos significativos */
	return (uint8_t)(sum & 0xFF);
}
