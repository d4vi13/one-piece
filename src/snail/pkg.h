#ifndef PKG_H
#define PKG_H

#include "common.h"

#ifdef FREE_BSD

#include "net/bpf.h"

#endif 

#define PKG_SIZE sizeof (struct pkg)
#define START_MARKER 0b01111110
#define MAX_DATA 127

#define ACKED(t) ((t) == ACK || (t) == OK_ACK || (t) == TEXT_ACK_NAME || (t) == VIDEO_ACK_NAME || (t) == IMG_ACK_NAME || (t) ==  ERROR)  

/* Tipos de pacotes a serem usados pelo protocolo */
enum pkg_t
{
  ACK,
  NACK,
  OK_ACK,
  FREE,
  SIZE,
  DATA,
  TEXT_ACK_NAME,
  VIDEO_ACK_NAME,
  IMG_ACK_NAME,
  END_OF_FILE,
  SHIFT_RIGHT,
  SHIFT_UP,
  SHIFT_DOWN,
  SHIFT_LEFT,
  FREE2,
  ERROR
};

typedef enum pkg_t pkg_t;

/* Estrutura do pacote a ser usados pelo protocolo */
struct __attribute__ ((packed)) pkg
{
  uint8_t start_marker : 8;
  uint8_t size : 7;
  uint8_t sequence_number : 5;
  uint8_t type : 4;
  uint8_t checksum : 8;
  uint8_t data[MAX_DATA];
};

void  
prepare_ack_pkg (struct pkg *pkg, uint8_t seq_num, pkg_t pkg_type);

void 
prepare_treasure_pkg (struct pkg *pkg, pkg_t pkg_type, uint8_t seq_num, char *filename);

void 
prepare_data_pkg (struct pkg *pkg, void *buf, uint8_t size);
  
void 
prepare_size_pkg (struct pkg *pkg, uint32_t size);

void 
prepare_eof_pkg (struct pkg *pkg);
 
int 
ack_pkg (uint8_t seq_num);

int
treasure_ack (uint8_t seq_num, pkg_t pkg_type, char *filename);

int
ok_ack_pkg (uint8_t seq_num);

void
send_start_talking ();

int
resend_last_ack ();

/*
 * Calcula o checksum de um pacote.
 * A soma considera: size + sequence_number + type + soma dos bytes de data.
 * O valor retornado é o byte menos significativo (uint8_t) da soma.
 */
uint8_t calculate_checksum (struct pkg *pkg); 

int
validate_checksum (struct pkg *pkg);


#endif
