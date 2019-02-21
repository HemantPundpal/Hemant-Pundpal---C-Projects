/*
 * Name: ring_buffer_ec.c
 *
 * Description:
 * All Ring Buffer error checking functions and are defined in this file.
 *
 * Author: Hemant Pundpal                            Date: 12 Feb 2019
 *
 */

#define RB_SOURCE_CODE

#include "ring_buffer_api.h"
#include "ring_buffer.h"
#include "error_assert.h"

/* Error check for create Ring Buffer function */
uint32_t create_ring_buffer_ec(rgbf_t ** p_ring_buffer, uint32_t size)
{
    /* Check is ring buffer pointer is valid */
    assert(!p_ring_buffer || *p_ring_buffer != NULL);
    if (!p_ring_buffer || *p_ring_buffer != NULL)
    {
        return RB_PTR_INVALID;
    }

    /* Check if the the buffer size is correct */
    assert(size > RINGBUFFER_SIZE_MAX || size < RINGBUFFER_SIZE_MIN);
    if (size > RINGBUFFER_SIZE_MAX || size < RINGBUFFER_SIZE_MIN)
    {
        return RB_BUFFER_SIZE_ERROR;
    }

    uint32_t status = RB_FAIL;
    status = create_ring_buffer(p_ring_buffer, size);

    /* Return Status */
    return status;
}

/* Error check for write a Byte to Ring Buffer function */
uint32_t byte_write_to_ring_buffer_ec(rgbf_t * p_ring_buffer, const uint8_t * p_byte, bool_t b_over_Write)
{
    /* Check if ring buffer pointer is valid */
    assert((!p_ring_buffer) || (p_ring_buffer->buffer_id != (uint32_t)p_ring_buffer));
    if ((!p_ring_buffer) || (p_ring_buffer->buffer_id != (uint32_t)p_ring_buffer))
    {
        return RB_PTR_INVALID;
    }

    /* Check if data pointer is valid */
    assert(!p_byte);
    if (!p_byte)
    {
        return RB_DATA_PTR_INVALID;
    }

    uint32_t status = RB_FAIL;
    status = byte_write_to_ring_buffer(p_ring_buffer, p_byte, b_over_Write);

    /* Return Status */
    return status;
}

/* Error check for write a block to Ring Buffer */
uint32_t block_write_to_ring_buffer_ec(rgbf_t * p_ring_buffer, const uint8_t * p_block, uint32_t size, bool_t b_over_write)
{
    /* Check if ring buffer pointer is valid */
    assert(!p_ring_buffer || (p_ring_buffer->buffer_id != (uint32_t)p_ring_buffer));
    if (!p_ring_buffer || (p_ring_buffer->buffer_id != (uint32_t)p_ring_buffer))
    {
        return RB_PTR_INVALID;
    }

    /* Check if data pointer is valid */
    assert(!p_block);
    if (!p_block)
    {
        return RB_DATA_PTR_INVALID;
    }

    /* Check if block size to copy into ring buffer is correct. */
    assert(!size || (size > p_ring_buffer->buffer_size));
    if (!size || (size > p_ring_buffer->buffer_size))
    {
        return RB_BUFFER_SIZE_ERROR;
    }

    uint32_t status = RB_FAIL;
    status = block_write_to_ring_buffer(p_ring_buffer, p_block, size, b_over_write);

    /* Return Status */
    return status;
}

/* Error check for read a byte from the Ring Buffer */
uint32_t read_byte_from_ring_buffer_ec(rgbf_t * p_ring_buffer, uint8_t * p_byte)
{
    /* Check if ring buffer pointer is valid */
    assert(!p_ring_buffer || (p_ring_buffer->buffer_id != (uint32_t)p_ring_buffer));
    if (!p_ring_buffer || (p_ring_buffer->buffer_id != (uint32_t)p_ring_buffer))
    {
        return RB_PTR_INVALID;
    }

    /* Check if data pointer is valid */
    assert(!p_byte);
    if (!p_byte)
    {
        return RB_DATA_PTR_INVALID;
    }

    uint32_t status = RB_FAIL;
    status = read_byte_from_ring_buffer(p_ring_buffer, p_byte);

    return status;
}

/* Error check for read a block from the Ring Buffer */
uint32_t read_block_from_ring_buffer_ec(rgbf_t * p_ring_buffer, uint8_t * p_block, uint32_t size)
{

    /* Check if ring buffer pointer is valid */
    assert(!p_ring_buffer || (p_ring_buffer->buffer_id != (uint32_t)p_ring_buffer));
    if (!p_ring_buffer || (p_ring_buffer->buffer_id != (uint32_t)p_ring_buffer))
    {
        return RB_PTR_INVALID;
    }

    /* Check if data pointer is valid */
    assert(!p_block);
    if (!p_block)
    {
        return RB_DATA_PTR_INVALID;
    }

    /* Check the block size to read is correct. */
    assert(!size || (size > p_ring_buffer->buffer_size));
    if(!size || (size > p_ring_buffer->buffer_size))
    {
        return RB_BUFFER_SIZE_ERROR;
    }

    uint32_t status = RB_FAIL;
    status = read_block_from_ring_buffer(p_ring_buffer, p_block, size);

    return status;
}

/* Error check for reset the Ring Buffer */
uint32_t reset_ring_buffer_ec(rgbf_t * p_ring_buffer)
{

    /* Check if ring buffer pointer is valid */
    assert(!p_ring_buffer || (p_ring_buffer->buffer_id != (uint32_t)p_ring_buffer));
    if (!p_ring_buffer || (p_ring_buffer->buffer_id != (uint32_t)p_ring_buffer))
    {
        return RB_PTR_INVALID;
    }

    uint32_t status = RB_FAIL;
    reset_ring_buffer(p_ring_buffer);

    return status;
}

/* Error check for delete the Ring Buffer */
uint32_t delete_ring_buffer_ec(rgbf_t * p_ring_buffer)
{

    /* Check if ring buffer pointer is valid */
    assert(!p_ring_buffer || (p_ring_buffer->buffer_id != (uint32_t)p_ring_buffer));
    if (!p_ring_buffer || (p_ring_buffer->buffer_id != (uint32_t)p_ring_buffer))
    {
        return RB_PTR_INVALID;
    }

    uint32_t status = RB_FAIL;
    delete_ring_buffer(p_ring_buffer);

    return status;
}
