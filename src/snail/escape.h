#ifndef ESCAPE_H
#define ESCAPE_H

#include "pkg.h"
#define VLAN_1 0x81
#define VLAN_2 0x88
#define ESCAPE_MARKER 0xFF

int escape_bytes (uint8_t *src, int len, uint8_t *dst);

int unescape_bytes (uint8_t *src, int len, uint8_t *dst);

#endif