#ifndef SET_H
#define SET_H

#include <stdio.h>
#include <stdlib.h>

#define BLOCK_SIZE 8

/*The data encryption standard encrypts a block of 8 bytes at a time
 *this is the header file for the data structure that stores the 8 byte block*/

/*DES encrypts blocks of length 64 bits = 8 bytes
 *with a key of 54 bits = 7 bytes*/
typedef struct block_s
{
    unsigned char *b;
    /*since the des involves splitting the block into left and right parts,
     *we are including them here*/
    unsigned char *left;
    unsigned char *right;
    int size;
    int used;  
} Block;

/*initialize the block with a given size*/
Block *init_block(int size);

/*delete the block*/
void delete_block();

/*clear and replace the data without deleting the block*/
void reset_data(Block *block, unsigned char *new_b);

/*clear all of the block's data without deleting it*/
void clear_block(Block *block);

/*check if the block is full*/
int is_full_block(Block *block);

/*print the block (used, size, and array)*/
void print_block(Block block);

/*print an array in general, used for testing purposes*/
void print_array(unsigned char *b, int size);

#endif
