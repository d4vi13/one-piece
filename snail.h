#ifndef SNAIL_H
#define SNAIL_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <net/ethernet.h>
#include <net/if.h>
#include <linux/if_packet.h>

#define MAX_DATA 127
#define PACKET_BASE_SIZE 32
#define PKG_SIZE 159
#define START_MARKER 0b01111110

#define min(a,b) ( (a) < (b) ? (a) : (b) )
#define FAILED(c) ( c != SUCCESS )

/* Tipos de pacotes a serem usados pelo protocolo */
enum PKG_TYPE {
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

typedef enum PKG_TYPE PKG_TYPE;

/* Codigos de erros a serem usados pelo protocolo */
enum ERROR_CODE
{
    PERMISSION_DENIED,
    NO_STORAGE,
    SOCKET_CREATE_FAILURE,
    SEND_PKG_FAILED,
    RECV_PKG_FAILED,
    SUCCESS
};

typedef enum ERROR_CODE ERROR_CODE;


/* Estrutura do pacote a ser usados pelo protocolo */
struct __attribute__((packed)) package
{
  uint8_t start_marker : 8;
  uint8_t size : 7;
  uint8_t sequence_number : 5;
  uint8_t type : 4;
  uint8_t checksum : 8;
  uint8_t data[MAX_DATA];
};

struct snail
{
    int socket;
    char network_interface[64];
    struct package pkg;
    uint32_t pkg_size;
};

extern struct snail snail;

void 
init_snail(char network_interface[]);

ERROR_CODE 
send_pkg(PKG_TYPE pkg_type, uint8_t *data, uint8_t size);

ERROR_CODE 
recv_pkg();

#endif
