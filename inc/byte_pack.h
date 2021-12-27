#ifndef BYTEPACK_H
#define BYTEPACK_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define BYTE_TO_BINARY_PATTERN "%d%d%d%d%d%d%d%d"

#define BYTE_TO_BINARY(x) \
	(x & 0x80 ? 1 : 0), \
	(x & 0x40 ? 1 : 0), \
	(x & 0x20 ? 1 : 0), \
	(x & 0x10 ? 1 : 0), \
	(x & 0x08 ? 1 : 0), \
	(x & 0x04 ? 1 : 0), \
	(x & 0x02 ? 1 : 0), \
	(x & 0x01 ? 1 : 0)

#define PRINT_BIN(x) printf(BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(x));


/*given the byte b, set the bit at n to the passed bit*/
void set_n_bit(unsigned char *b, int n, int new_bit);

/*return the bit at position n*/
unsigned char get_n_bit(unsigned char b, int n);

/*generate a random byte, mostly used for testing purposes*/
unsigned char generate_random_byte();

#endif
