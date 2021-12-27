#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "key.h"

/*initial key permuation*/
const static unsigned char pc_1[7][8] = {{56, 48, 40, 32, 24, 16, 8, 0}, {57, 49, 41, 33, 25, 17, 9, 1},
                                {58, 50, 42, 34, 26, 18, 10, 2}, {59, 51, 43, 35, 62, 54, 46, 38},
                                {30, 22, 14, 6, 61, 53, 45, 37}, {29, 21, 13, 5, 60, 52, 44, 36},
                                {28, 20, 12, 4, 27, 19, 11, 3}};

/*round key permutation*/
const static unsigned char pc_2[6][8] = {{13, 16, 10, 23, 0, 4, 2, 27}, {14, 5, 20, 9, 22, 18, 11, 3},
                                {25, 7, 15, 6, 26, 19, 12, 1}, {40, 51, 30, 36, 46, 54, 29, 39},
                                {50, 44, 32, 47, 43, 48, 38, 55},{33, 52, 45, 41, 49, 35, 28, 31}};

/*intial key permutation, maps the _64_bit input to a 56_bit ouput as defined in pc_1*/
key init_56_bit_key(long long _64_bit_block)
{
    int i, n, t = 0, place;
    unsigned char bit = 0;
    key _56_bit_key = 0;
    for(i = 0; i < 7; i++)
    {
        for(n = 0; n < 8; n++)
        {
            place = pc_1[i][n];
            bit = get_n_bit_key(_64_bit_block, place);
            set_n_bit_key(&_56_bit_key, t, bit);
            t++;
        }
    }
    return _56_bit_key;
}

/*takes a _56_bit_key as input (the joined halves left and right) and returns
 *a 48 bit round key */
key get_round_key(key _56_bit_key)
{
    int i, n, t = 0, place;
    unsigned char bit;
    key _48_bit_key = 0ULL;
    for(i = 0; i < 6; i++)
    {
        for(n = 0; n < 8; n++)
        {
            place = pc_2[i][n];
            bit = get_n_bit_key(_56_bit_key, place);
            set_n_bit_key(&_48_bit_key, t, bit);
            t++;
        }
    }
    return _48_bit_key;
}

/*initialize the 28 bit halves, side = 0 means left, side = 1 means right*/
key init_halves(key _56_bit_key, int side)
{
    key half = 0ULL, mask = 0xFFFFFFF;
    half = (side) ? ((_56_bit_key >> 28) & mask) : (_56_bit_key & mask); 
    return half;
}

/*join the halves to recreate the whole, 56 bit key*/
key join_halves(key left, key right)
{
    key whole = 0ULL;
    whole = whole | left;
    whole = whole | (right << 28);
    return whole;
}

/*rotate the 28 bit halve of the key by the SHIFT bits, left = 1 indicates left shift
 *left = 0 indicates right shift*/
void rotate(key *half, int shift, int left)
{
    if(shift == 0) return;
    if(left) *half =( (*half << shift) | (*half >> (28ULL - shift)) ) & 0xFFFFFFF;
    else *half = ( (*half >> shift) | (*half << (28ULL - shift)) ) & 0xFFFFFFF;
}

/*set the nth bit of key b to bit. While this function is implemented in byte_pack,
 *that implementation works only with smaller datatypes. The declaration of 1ULL, specifically
 *makes this work for long datatypes*/
void set_n_bit_key(key *b, int n, unsigned char bit)
{
    key b1 = (bit) ? 1 : 0;
    *b = *b & ~(1ULL <<n);
    *b = *b | (b1 << n); 
}

/*return the bit at position n, formatted as the lsb of a new byte
 *i.e. getting the 5th bit of 0b1000111 returns 0b10000000*/
unsigned char get_n_bit_key(key b, int n)
{
    key new = b&(1ULL << n);
    new = new >> n;
    return (unsigned char)new;
}	

/*print the bits for any data type*/
void print_bits(size_t const size, void const * const ptr)
{
    unsigned char *b = (unsigned char*)ptr;
    unsigned char byte;
    int i, j;
    for(i = size-1; i >= 0; i--)
    {
        for(j = 7; j >= 0; j--)
        {
            byte = (b[i] >> j) & 1;
            printf("%u", byte);
        }
    }
    puts(" ");
}
