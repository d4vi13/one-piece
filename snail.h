#ifndef SNAIL_H
#define SNAIL_H

#define MAX_DATA 127
#define PACKET_BASE_SIZE 32

enum PACKET_TYPE {
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
    FREE,
    ERROR
};

typedef enum PACKET_TYPE PACKET_TYPE;

enum ERROR_CODE {
    PERMISSION_DENIED,
    NO_STORAGE
};

typedef enum ERROR_CODE ERROR_CODE;

struct __attribute__((packed)) packet
{
  unsigned char start_marker : 8;
  unsigned char size: 7;
  unsigned char sequence_number : 5;
  unsigned char type : 4;
  unsigned char checksum : 8;
  unsigned char data[MAX_DATA];
};

#endif
