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
  memset (pkg->data, 0, MAX_DATA);
  prepare_pkg (pkg, pkg_type, seq_num, 0);
}

void 
prepare_data_pkg (struct pkg *pkg, void *buf, uint8_t size)
{
  prepare_pkg (pkg, DATA, get_seq_num (), size);

  memset (pkg->data, 0, MAX_DATA);
  memcpy (pkg->data, buf, min (size, MAX_DATA));
}

/* recebe o tamanho do arquivo */
void 
prepare_size_pkg (struct pkg *pkg, uint32_t size)
{
  prepare_pkg (pkg, SIZE, get_seq_num (), sizeof size);

  memset (pkg->data, 0, MAX_DATA);
  memcpy (pkg->data, &size, sizeof size);
}

void 
prepare_treasure_pkg (struct pkg *pkg, pkg_t pkg_type, uint8_t seq_num, char *filename)
{
  prepare_pkg (pkg, pkg_type, seq_num, strlen(filename));
  
  memset (pkg->data, 0, MAX_DATA);

  memcpy (pkg->data, seq_num, sizeof seq_num);
  memcpy (pkg->data + sizeof seq_num, filename, strlen(filename));
}

void 
prepare_eof_pkg (struct pkg *pkg)
{
  memset (pkg->data, 0, MAX_DATA);
  prepare_pkg (pkg, END_OF_FILE, get_seq_num (), 0);
}

int 
ack_pkg (uint8_t seq_num)
{
  errno = 0;

  prepare_ack_pkg (&snail.ack, seq_num, ACK);
  while (send_pkg (&snail.ack) == EXIT_FAILURE);

  return EXIT_SUCCESS;
} 

int 
ok_ack_pkg (uint8_t seq_num)
{
  errno = 0;

  prepare_ack_pkg (&snail.ack, seq_num, OK_ACK);
  while (send_pkg (&snail.ack) == EXIT_FAILURE);

  return EXIT_SUCCESS;
}

int 
treasure_ack (uint8_t seq_num, pkg_t pkg_type, char *filename)
{
  errno = 0;

  prepare_treasure_pkg (&snail.ack, pkg_type, seq_num, filename);
  while (send_pkg (&snail.ack) == EXIT_FAILURE);

  return EXIT_SUCCESS;
}


int 
resend_last_ack ()
{
  errno = 0;

  if (snail.pkg.sequence_number == 0)
    {
      fprintf (stderr, "Nenhum pacote enviado ainda.\n");
      return EXIT_FAILURE;
    }

  while (send_pkg (&snail.ack) == EXIT_FAILURE);

  return EXIT_SUCCESS;
}

void
send_start_talking ()
{
  errno = 0;

  struct pkg pkg;
  prepare_ack_pkg (&snail.pkg, get_seq_num (), FREE);
  while (1) {
    send_pkg (&snail.pkg);
    
    if (recv_pkg (&pkg) == EXIT_SUCCESS)
    {
      break;
    }
  }

}