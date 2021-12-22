#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "block.h"
#include "byte_pack.h"

/*initiate a block (data structure containing size bytes)*/
Block *init_block(int size)
{
    Block *block = (Block*)malloc(sizeof(Block));
    block->left = block->right = NULL;
    block->used = 0;
    block->size = size;
    /*because we will always have 8 bytes in a block*/    
    block->b = (unsigned char*)calloc(size, sizeof(unsigned char));
    return block;
}

/*delete a block*/
void delete_block(Block *block)
{
    free(block->b);
    free(block->left);
    free(block->right);
    free(block);
}

/*clear the data in the block without deleting it*/
void clear_block(Block *block)
{
    block->used = 0;
    block->size = BLOCK_SIZE;
    free(block->b);
    block->b = (unsigned char*)calloc(BLOCK_SIZE, sizeof(unsigned char)); 
}

/*reset the data in the block without affecting anything else*/
void reset_data(Block *block, unsigned char *new_b)
{
    free(block->b);
    block->b = new_b;
}

/*return 1 if block is full, 0 if not*/
int is_full_block(Block *block)
{
    return block->used == block->size;
}

void print_block(Block block)
{
    int i, used = block.used, size = block.size;
    printf("used = %d\n", used);
    printf("size = %d\n", size);
    for(i = 0; i < used; i++)
    {
	printf("b[%d] = %c = %d = ", i, block.b[i], block.b[i]); PRINT_BIN(block.b[i]); printf("\n");
    }
}

void print_array(unsigned char *b, int size)
{
    int i;
    for(i = 0; i < size; i++)
    {
	    printf("b[%d] = %c = %d = ", i, b[i], b[i]); PRINT_BIN(b[i]); printf("\n");
    }
}
