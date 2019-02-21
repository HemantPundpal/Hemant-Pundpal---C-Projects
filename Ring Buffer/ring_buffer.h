/*
 * Name: ring_buffer.h
 *
 * Description:
 * All definition and function declarations required by the ring buffer module are
 * in this file.
 * Application code should not #include this file to use the ring buffer api.
 *
 * Author: Hemant Pundpal                                 Date: 12 Feb 2019
 *
 */

#ifndef __RING_BUFFER__
#define __RING_BUFFER__


/* Minimum size of ring buffer. Do not modify this value. */
#define RINGBUFFER_SIZE_MIN    1U

/* Function to create Ring Buffer */
uint32_t create_ring_buffer(rgbf_t ** p_ring_buffer, uint32_t size);

/* Function to write a Byte to Ring Buffer */
uint32_t byte_write_to_ring_buffer(rgbf_t * p_ring_buffer, const uint8_t * p_byte, bool_t b_over_Write);

/* Function to write a block to Ring Buffer */
uint32_t block_write_to_ring_buffer(rgbf_t * p_ring_buffer, const uint8_t * p_block, uint32_t size, bool_t b_over_write);

/* Function to read a byte from the Ring Buffer */
uint32_t read_byte_from_ring_buffer(rgbf_t * p_ring_buffer, uint8_t * p_byte);

/* Function to read a block from the Ring Buffer */
uint32_t read_block_from_ring_buffer(rgbf_t * p_ring_buffer, uint8_t * p_block, uint32_t size);

/* Function to reset the Ring Buffer */
uint32_t reset_ring_buffer(rgbf_t * p_ring_buffer);

/* Delete the Ring Buffer */
uint32_t delete_ring_buffer(rgbf_t * pRingBuffer);



/* Error check for create Ring Buffer function */
uint32_t create_ring_buffer_ec(rgbf_t ** p_ring_buffer, uint32_t size);

/* Error check for write a Byte to Ring Buffer function */
uint32_t byte_write_to_ring_buffer_ec(rgbf_t * p_ring_buffer, const uint8_t * p_byte, bool_t b_over_Write);

/* Error check for write a block to Ring Buffer */
uint32_t block_write_to_ring_buffer_ec(rgbf_t * p_ring_buffer, const uint8_t * p_block, uint32_t size, bool_t b_over_write);

/* Error check for read a byte from the Ring Buffer */
uint32_t read_byte_from_ring_buffer_ec(rgbf_t * p_ring_buffer, uint8_t * p_byte);

/* Error check for read a block from the Ring Buffer */
uint32_t read_block_from_ring_buffer_ec(rgbf_t * p_ring_buffer, uint8_t * p_block, uint32_t size);

/* Error check for reset the Ring Buffer */
uint32_t reset_ring_buffer_ec(rgbf_t * p_ring_buffer);

/* Error check for delete the Ring Buffer */
uint32_t delete_ring_buffer_ec(rgbf_t * pRingBuffer);

#endif /* __RING_BUFFER__ */
