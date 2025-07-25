#include "pkg.h"
#include "snail.h"

static inline void
prepare_pkg (struct pkg *pkg, pkg_t pkg_type, uint8_t seq_num, uint8_t size)
{
  pkg->start_marker = START_MARKER;
  pkg->size = size;
  pkg->sequence_number = seq_num;
  pkg->type = pkg_type;
  pkg->checksum = calculate_checksum (pkg);
}

/* usado para preparar pacotes ACK/NACK/OK+ACK */
void  
prepare_ack_pkg (struct pkg *pkg, uint8_t seq_num, pkg_t pkg_type)
{
  memset (pkg->data, 0, MAX_DATA);
  prepare_pkg (pkg, pkg_type, seq_num, 0);
}

void 
prepare_data_pkg (struct pkg *pkg, void *buf, uint8_t size)
{
  memset (pkg->data, 0, MAX_DATA);
  memcpy (pkg->data, buf, min (size, MAX_DATA));

  prepare_pkg (pkg, DATA, get_seq_num (), size);
}

/* recebe o tamanho do arquivo */
void 
prepare_size_pkg (struct pkg *pkg, uint32_t size)
{
  memset (pkg->data, 0, MAX_DATA);
  memcpy (pkg->data, &size, sizeof size);

  prepare_pkg (pkg, SIZE, get_seq_num (), sizeof size);
}

void 
prepare_treasure_pkg (struct pkg *pkg, pkg_t pkg_type, uint8_t seq_num, char *filename)
{
  memset (pkg->data, 0, MAX_DATA);

  memcpy (pkg->data, filename, strlen(filename));

  prepare_pkg (pkg, pkg_type, seq_num, strlen(filename));
}

void 
prepare_eof_pkg (struct pkg *pkg)
{
  memset (pkg->data, 0, MAX_DATA);
  prepare_pkg (pkg, END_OF_FILE, get_seq_num (), 0);
}

void  
prepare_start_talking_pkg (struct pkg *pkg)
{
  memset (pkg->data, 0, MAX_DATA);
  prepare_pkg (pkg, START_TALKING, get_seq_num(), 0);
}


uint8_t
calculate_checksum (struct pkg *pkg)
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

int
validate_checksum ( struct pkg *pkg)
{
  uint8_t expected = calculate_checksum (pkg);
  return (pkg->checksum == expected);
}    