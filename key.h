#ifndef KEY_H
#define KEY_H

#include <stdio.h>
#include <stdlib.h>

typedef unsigned long long key;

void set_n_bit_key(key *b, int n, unsigned char bit);

unsigned char get_n_bit_key(key b, int n);

key init_56_bit_key(long long _64_bit_block);

key init_halves(key _56_bit_key, int side);

key get_round_key(key _56_bit_key);

key join_halves(key left, key right);

void rotate(key *half, int shift, int left);

void print_bits(size_t const size, void const * const ptr);




#endif
