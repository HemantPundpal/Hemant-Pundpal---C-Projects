/*
 * Name: ring_buffer_main.c
 *
 * Description:
 * THIS IS A DEMO CODE JUST TO EXCERCISE / DEMONSTRATE THE RING BUFFER API
 *
 * Author: Hemant Pundpal                            Date: 12 Feb 2019
 *
 */


#include "ring_buffer_api.h"


int main(void)
{
    rgbf_t *p_ring_buffer = NULL;
    uint32_t ring_buffer_size = 5;

    if (RB_SUCCESS == create_ring_buffer_ec(&p_ring_buffer, ring_buffer_size))
    {
        printf("Ring Buffer created successfully \n");
    }

    uint8_t byte = 0XEF;
    uint8_t * p_byte = &byte;
    if (RB_SUCCESS == byte_write_to_ring_buffer_ec(p_ring_buffer, p_byte, FALSE))
    {
        printf("Byte write to the Ring Buffer 1 \n");
    }

    *p_byte = 0XFF;
    if (RB_SUCCESS == byte_write_to_ring_buffer_ec(p_ring_buffer, p_byte, FALSE))
    {
        printf("Byte write to the Ring Buffer 2 \n");
    }

    uint32_t count = 1;
    while (RB_SUCCESS == byte_write_to_ring_buffer_ec(p_ring_buffer, p_byte, FALSE))
    {
        count++;
        printf("Byte write to the Ring Buffer %d \n", count);
    }

    while (RB_SUCCESS == read_byte_from_ring_buffer_ec(p_ring_buffer, p_byte))
    {
        printf("Byte read from the Ring Buffer %d - %X\n", count, *p_byte);
        count--;
    }

    count = 0;
    while (RB_SUCCESS == byte_write_to_ring_buffer_ec(p_ring_buffer, p_byte, FALSE))
    {
        count++;
        printf("Byte write to the Ring Buffer %d \n", count);
    }

    count = 0;
    uint32_t block_size = 5;
    uint8_t * p_block0 = (uint8_t*)calloc(block_size, sizeof(uint8_t));
    if (RB_SUCCESS == read_block_from_ring_buffer_ec(p_ring_buffer, p_block0, block_size))
    {
        while(count < block_size)
        {
            printf("Block read from the Ring Buffer %d - %X \n", count, p_block0[count]);
            count++;
        }
    }

    count = 0;
    block_size = 3;
    uint8_t * p_block1 = (uint8_t*)calloc(block_size, sizeof(uint8_t));
    while (RB_SUCCESS == block_write_to_ring_buffer_ec(p_ring_buffer, p_block1, block_size, FALSE))
    {
        count++;
        printf("Block write to the Ring Buffer %d \n", count);
    }

    block_size = 5;
    uint8_t * p_block2 = (uint8_t*)calloc(block_size, sizeof(uint8_t));
    if (RB_SUCCESS == block_write_to_ring_buffer_ec(p_ring_buffer, p_block2, block_size, TRUE))
    {
        printf("Block write (overwrite) to the Ring Buffer \n");
    }

    block_size = 4;
    uint8_t * p_block3 = (uint8_t*)calloc(block_size, sizeof(uint8_t));
    if (RB_SUCCESS == block_write_to_ring_buffer_ec(p_ring_buffer, p_block3, block_size, TRUE))
    {
        printf("Block write (overwrite) to the Ring Buffer \n");
    }

    if (RB_SUCCESS == read_byte_from_ring_buffer_ec(p_ring_buffer, p_byte))
    {
        printf("Byte read from the Ring Buffer \n");
    }

    block_size = 2;
    uint8_t * p_block4 = (uint8_t*)calloc(block_size, sizeof(uint8_t));
    if (RB_SUCCESS == read_block_from_ring_buffer_ec(p_ring_buffer, p_block4, block_size))
    {
        printf("Block read from the Ring Buffer \n");
    }

    block_size = 4;
    if (RB_SUCCESS == block_write_to_ring_buffer_ec(p_ring_buffer, p_block3, block_size, FALSE))
    {
        printf("Block write to the Ring Buffer \n");
    }
    else
    {
        printf("Block write to the Ring Buffer - failed \n");
    }

    if (RB_SUCCESS == reset_ring_buffer_ec(p_ring_buffer))
    {
        printf("Reset Ring Buffer \n");
    }

    if (RB_SUCCESS == delete_ring_buffer_ec(p_ring_buffer))
    {
        printf("Delete Ring Buffer \n");
    }

    free(p_block1);
    free(p_block2);
    free(p_block3);
    free(p_block4);

    return 0;
}
