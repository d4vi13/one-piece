#include "pkg.h"
#include "snail.h"

static inline void
prepare_pkg (struct pkg *pkg, pkg_t pkg_type, uint8_t seq_num, uint8_t size)
{
  pkg->start_marker = START_MARKER;
  pkg->size = size;
  pkg->sequence_number = seq_num;
  pkg->type = pkg_type;
  pkg->checksum = 0;
}

/* usado para preparar pacotes ACK/NACK/OK+ACK */
void  
prepare_ack_pkg (struct pkg *pkg, uint8_t seq_num, pkg_t pkg_type)
{
  prepare_pkg (pkg, ACK, pkg_type, 0);
}

void 
prepare_data_pkg (struct pkg *pkg, void *buf, uint8_t size)
{
  prepare_pkg (pkg, DATA, get_seq_num (), size);

  memset (pkg->data, 0, MAX_DATA);
  memcpy (pkg->data, buf, min (size, MAX_DATA));
}
