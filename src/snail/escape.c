#include "escape.h"

int 
escape_bytes (uint8_t *src, int len, uint8_t *dst) 
{
  int j = 0;
  for (int i = 0; i < len; i++) 
  {
  	dst[j++] = src[i];
    if (src[i] == VLAN_1 || src[i] == VLAN_2)
      dst[j++] = ESCAPE_MARKER;
  }
  return j;
}

int 
unescape_bytes (uint8_t *src, int len, uint8_t *dst) 
{
  int j = 0;
  for (int i = 0; i < len; i++) 
  {
    dst[j++] = src[i];
    if ((src[i] == VLAN_1 || src[i] == VLAN_2) 
    		&& i + 1 < len && src[i + 1] == ESCAPE_MARKER)
      i++; // pula o 0xFF
  }
  return j;
}
