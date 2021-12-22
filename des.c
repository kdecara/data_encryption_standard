/*
 * implementation of data encryption standard (DES) 
 * TODO: implementation is complete except for key scheduling 
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "file_pack.h"
#include "block.h"
#include "byte_pack.h"
#define MOD(x, n) ((x % n + n) % n)
#define GET_BYTE_NUM(bit_index) (bit_index/8)
#define GET_BIT_NUM(bit_index) MOD(bit_index, 8)
#define RING_SIZE 64
#define BLOCK_SIZE 8

typedef unsigned char byte;

/*the s boxes are explicitly defined tables that map a 6 bit input to a 4 bit output
 *the row is given by the lsb and msb and the column is given by the 4 middle bits
 *so the number 100101 = 8 would mean row 11(b) = 4 and column 0010(b) = 3
 *since this is a learning project only be using S1, but the others are explictly defined*/

/*as this is intented to be crypto algorithm, declaring the variables static restrict their scope to 
 *this file.*/

const static byte s_box_1[4][16] = { {14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7},
      		    {0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8},
		    {4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0},
		    {15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13}};

const static byte p_box[8][4] = { {15, 6, 19, 20,}, {28, 11, 27, 16}, {0, 14, 22, 25}, {4, 17, 30, 9},
		     {1, 7, 23, 13}, {31, 26, 2, 8}, {18, 12, 29, 5}, {21, 10, 3, 24} };

/*we technically don't need to explicitly define the e_box, ip_box, and inv_ip_box as their elements
 *can be expressed as a linear combination of the elements before them, but i do so anyways:*/
const static byte e_box[8][6] = {{31, 0, 1, 2, 3, 4}, {3, 4, 5, 6, 7, 8}, {7, 8, 9, 10, 11, 12},
                                {11, 12, 13, 14, 15, 16}, {15, 16, 17, 18, 19, 20}, {19, 20, 21, 22, 23, 24},
                                {23, 24, 25, 26, 27, 28}, {27, 28, 29, 30, 31, 0}};

const static byte ip_box[8][8] = { {57, 49, 41, 33, 25, 17, 9, 1}, {59, 51, 43, 35, 27, 19, 11, 3},
                                {61, 53, 45, 37, 29, 21, 13, 5}, {63, 55, 47, 39, 31, 23, 15, 7},
                                {56, 48, 40, 32, 24, 16, 8, 0}, {58, 50, 42, 34, 26, 18, 10, 2},
                                {60, 52, 44, 36, 28, 20, 12, 4}, {62, 54, 46, 38, 30, 22, 14, 6} };

const static byte inv_ip_box[8][8] = {{39, 7, 47, 15, 55, 23, 63, 31}, {38, 6, 46, 14, 54, 22, 62, 30},
                                {37, 5, 45, 13, 53, 21, 61, 29}, {36, 4, 44, 12, 52, 20, 60, 28},
                                {35, 3, 43, 11, 51, 19, 59, 27}, {34, 2, 42, 10, 50, 18, 58, 26},
                                {33, 1, 41, 9, 49, 17, 57, 25}, {32, 0, 40, 8, 48, 16, 56, 24} };

/*initial key permuation*/
const static byte pc_1[7][8] = {{56, 48, 40, 32, 24, 16, 8, 0}, {57, 49, 41, 33, 25, 17, 9, 1},
                                {58, 50, 42, 34, 26, 28, 10, 2}, {59, 51, 43, 35, 62, 54, 46, 38},
                                {30, 22, 14, 6, 61, 53, 45, 37}, {29, 21, 13, 5, 60, 52, 44, 36},
                                {28, 20, 12, 4, 27, 19, 11, 3}};

const static byte pc_2[6][8] = {{13, 16, 10, 23, 0, 4, 2, 27}, {14, 5, 20, 9, 22, 18, 11, 3},
                                {25, 7, 15, 6, 26, 19, 12, 1}, {40, 51, 30, 36, 46, 54, 29, 39},
                                {50, 44, 32, 47, 43, 48, 38, 55},{33, 52, 44, 41, 49, 35, 28, 31}};
                                
void encrypt(FILE *in, FILE *out, int IP_cipher);
void decrypt(FILE *in, FILE *out, int IP_cipher);

void feistel(Block **block, int inv, long IP_cipher);
byte* ip(byte *_64_bit_block, int inv);
byte* f(int round_key, byte *half_block);
byte* e(byte *half_block);
byte* s(byte *e_block);
byte* p(byte *s_block);

byte** init_key(byte* _64_bit_block);
byte* key_schedule(byte *c, byte *d);
void rotate(byte **_28_bit_block, int shift);
void rotate_right(byte **_28_bit_block, int shift);
void swap(byte **left, byte **right);
byte *get_half(byte *b, int half, int size, int starting);

/*takes a byte array *b and permutes it vial caesar cipher rules with cipher,
 *if we want to call the inverse to unpermute it, we can pass with inv = 1 else inv = 0*/
byte* IP_caesar(byte *b, int cipher, int inv);

int main(int argc, char *argv[])
{
    file_s *in_file = open_file(argv[1], "r");
    file_s *out_file = open_file(argv[2], "w");
    int seed = atoi(argv[3]);
    char *mode = argv[4];
   
    if(mode[0] == 'e') encrypt(in_file->fp, out_file->fp, seed);
    
    else if(mode[0] == 'd') decrypt(in_file->fp, out_file->fp, seed);
    
    close_file(in_file);
    close_file(out_file);
    return 0;
}


void encrypt(FILE *in, FILE *out, int IP_cipher)
{
    Block *block = init_block(BLOCK_SIZE);
    byte *permuted_block;
    int num_read = 0;
    while((num_read = fread(block->b, 1, 8, in)) > 0)
    {
	    block->used += num_read;
        permuted_block = ip(block->b, 0);
        reset_data(block, permuted_block);
        feistel(&block, 0, IP_cipher);
	    fwrite(block->b, 1, BLOCK_SIZE, out);
	    clear_block(block);    
    } 
    delete_block(block);
}

void decrypt(FILE *in, FILE*out, int IP_cipher)
{
    Block *block = init_block(BLOCK_SIZE);
    byte *un_permuted_block;
    int num_read = 0;
    while((num_read = fread(block->b, 1, 8, in)) > 0)
    {
	    block->used += num_read;
        feistel(&block, 1, IP_cipher);
        un_permuted_block = ip(block->b, 1);
        reset_data(block, un_permuted_block); 
	    fwrite(block->b, 1, BLOCK_SIZE, out);
	    clear_block(block);
        
    } 
    delete_block(block);
}

void feistel(Block **block, int inv, long cipher)
{
    int i, n, k = (inv) ? 15 : 0, size = BLOCK_SIZE/2;
    byte *right = get_half((*block)->b, 1, size, size), *left = get_half((*block)->b, 0, size, 0), *enc_b, *result;
    for(i = 0; i < 16; i++)
    {
        /*on rounds 1, 2, 9, 16, rotate both halves left by 1, otherwise rotate left by two*/
          
        result = f(k, right);
        /*the result of f() is used as a mask for XOR encryption of the left side*/ 
        for(n = 0; n < BLOCK_SIZE/2; n++) left[n] ^= result[n];
        k = (inv) ? k - 1 : k + 1;
        swap(&left, &right);
        free(result);
    }
    /*the final swap is necessary:*/
    swap(&left, &right);
    /*finally, join right and left and set them equal to block->b*/
    enc_b = (byte*)malloc(BLOCK_SIZE * sizeof(byte));
    memmove(enc_b, left, BLOCK_SIZE/2);
    memmove(enc_b + BLOCK_SIZE/2, right, BLOCK_SIZE/2);
    reset_data(*block, enc_b);
    free(right);
    free(left);
}

/*the initial permutation of the plain cipher_text, a 64 bit block*/
byte* ip(byte *_64_bit_block, int inv)
{
    int i, n, place, byte_num, bit_num;   
    byte* permuted_block = (byte*)calloc(BLOCK_SIZE, sizeof(byte)), bit, temp_byte;
    for(i = 0; i < 8; i++)
    {
        for(n = 0; n < 8; n++)
        {
            /*if inverse = 1 use the inv_ip_box table, otherwise use the ip_box table
             *get the bit at variable place, and put it into
             *byte i at position n of our permuted block*/
            place = (inv) ? inv_ip_box[i][n] : ip_box[i][n];
            byte_num = GET_BYTE_NUM(place);
            bit_num = GET_BIT_NUM(place);
            temp_byte = _64_bit_block[byte_num];
            bit = get_n_bit(temp_byte, bit_num);
            set_n_bit(&permuted_block[i], n, bit);
        }
    } 
    return permuted_block; 
}

/*f() takes the 32 bit block (either left or right) and round key as input
 *it is bad practice to not free memory in the block it is allocated, but I do so here*/
byte* f(int round_key, byte *_32_bit_block)
{
    int n;
    byte *result = e(_32_bit_block); 
    /*TODO: fix round key[n] later*/
    for(n = 0; n < BLOCK_SIZE; n++) result[n] ^= round_key;
    result = s(result);
    result = p(result);
    return result;    
}

/*
 * expansion permutation
 * using the ebox defined above map
 * 32-bit half-block into 64-bit (8 unsigned chars) block with only the first 6 bits being used
 */
byte* e(byte *half_block)
{
    int i, n, half_bit_num, half_byte_num, place;
    byte *new_half_block = (byte*)calloc(BLOCK_SIZE, sizeof(byte));
    byte bit, temp;
    for(i = 0; i < 8; i++)
    {
        for(n = 0; n < 6; n++)
        {
            place = e_box[i][n];
            /*get the byte number of bit at l*/
            half_byte_num = GET_BYTE_NUM(place);
            /*find what index the bit at l is in its byte*/
            half_bit_num = GET_BIT_NUM(place);
            temp = half_block[half_byte_num];
            bit = get_n_bit(temp, half_bit_num);
            set_n_bit(&new_half_block[i], n, bit);
        }
    }
    return new_half_block;
}

/*function which maps each 6 bit input in the passed block (e_block) to a 4 bit output using the
 *s_box variable explicitly defined above. the least sig bit and most sig bit of each byte determine
 * the row, while the 4 inner bits determine the column, so in the byte 100001, the row would be 11,
 * and the column would be 000*/
byte* s(byte *e_block)
{
    int i, n;
    byte row = 0, column = 0, bit, lsb, msb;
    byte *s_block = (byte*)calloc(BLOCK_SIZE, sizeof(byte));
    for(i = 0; i < BLOCK_SIZE; i++)
    {
        /*get the lsb and msb to determine the row*/
	    lsb = get_n_bit(e_block[i], 0), msb = get_n_bit(e_block[i], 5);
	    set_n_bit(&row, 0, lsb), set_n_bit(&row, 1, msb);
        /*get the 4 middle bits to determine the column*/
	    for(n = 0; n < 4; n++)
	    {
	        bit = get_n_bit(e_block[i], n+1);
	        set_n_bit(&column, n, bit);
	    }
        s_block[i] = s_box_1[row][column];
    }
    free(e_block);
    return s_block;
}

/*the last stage of the f-function: the 32 bit output is permuted bitwise according to the P permutation
 *like the s_block, this is explicitly defined table above*/
byte* p(byte *s_block)
{
    int i, n, l = 0, s_bit_num, s_byte_num, place;
    byte *p_block = (byte*)calloc(4, sizeof(byte));
    byte bit, temp;
    for(i = 0; i < 4; i++)
    {
        for(n = 0; n < 8; n++)
        {
            place = p_box[i][n];
            /*get the byte number of bit at l*/
            s_byte_num = GET_BYTE_NUM(place);
            /*find what index the bit at l is in its byte*/
            s_bit_num = GET_BIT_NUM(place);
            temp = s_block[s_byte_num];
            bit = get_n_bit(temp, s_bit_num);
            set_n_bit(&p_block[i], n, bit);
            l++;
        }
    }
    free(s_block);
    return p_block;
}

/*when the block is split into 2 parts, a left and right half*/
void swap(byte **left, byte **right)
{
    byte *temp = *left;
    *left = *right;
    *right = temp;
}

/*return the left half of the 64 bits (the first 0-31 bits)
 *OR return the right of which is the 32-63 bits
 *half = 0 refers to the first half, half = 1 refers to the second half*/
byte* get_half(byte *b, int half, int size, int starting)
{
    /*int size = BLOCK_SIZE/2;*/
    byte *h = (byte*)malloc(sizeof(byte) * size);
    /*if half = 0 we return the first part of the array the LEFT HALF*/
    if(!half) memmove(h, b, size * sizeof(byte));
    /*if half = 1 we return the second part of the array the RIGHT HALF*/
    else memmove(h, b + starting, size * sizeof(byte));
    return h;
}

/*complete the initial permuration (IP) in a manner similar to the caesar cipher
 *so new_pos = (old_pos + cipher) % 64
 *or in the case of the inverse new_pos = (old_pos - cipher) % 64
 *then change the bit at new_pos to be the one at old_pos
 *if inv = 1 we are inverting the shift if 0 we are performing the shift*/
byte* IP_caesar(byte *b, int cipher, int inv)
{
    int pos, byte_num, bit_num, new_pos, new_byte_num, new_bit_num;
    byte *new_b = (byte*)malloc(sizeof(byte) * BLOCK_SIZE), bit;
    for( pos = 0; pos < 64; pos++)
    {
	    if(inv) new_pos = MOD(pos - cipher, RING_SIZE);
	    else new_pos = MOD(pos + cipher, RING_SIZE);
	    byte_num = pos/8, bit_num = MOD(pos, 8);
	    new_byte_num = new_pos/8, new_bit_num = MOD(new_pos, 8);
	    bit = get_n_bit(b[byte_num], bit_num);
	    set_n_bit(&new_b[new_byte_num], new_bit_num, bit);
    }
    return new_b;
}

