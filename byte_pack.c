#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "byte_pack.h"

/*set bit n of byte b to the passed bit*/
void set_n_bit(unsigned char *b, int n, int bit)
{
    *b = *b & ~(1<<n);
    *b = *b | (bit << n); /*shift the bit to the place you want to place it*/
}

/*return the bit at position n, formatted as the lsb of a new byte
 *i.e. getting the 5th bit of 0b1000111 returns 0b10000000*/
unsigned char get_n_bit(unsigned char b, int n)
{
    unsigned char new = b&(1<<n);
    new = new >> n;
    return new;
}	

/*generate a random byte, mostly used for testing purposes*/
unsigned char generate_random_byte()
{
    unsigned char b = 0;
    int i, x;
    for(i = 0; i < 8; i++)
    {
	x = rand() % 11;
	if(x >= 5) set_n_bit(&b, i, 1);
	else set_n_bit(&b, i, 0);
    }
    return b;
}

