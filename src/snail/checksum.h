#ifndef CHECKSUM_H
#define CHECKSUM_H

#include <stdint.h>
#include "pkg.h"

/*
 * Calcula o checksum de um pacote.
 * A soma considera: size + sequence_number + type + soma dos bytes de data.
 * O valor retornado é o byte menos significativo (uint8_t) da soma.
 */
uint8_t calculate_checksum (const struct pkg *pkg);

#endif