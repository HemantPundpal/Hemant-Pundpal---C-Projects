/*
 * Name: ring_buffer.c
 *
 * Description:
 * All Ring Buffer functions and variables are defined in this file.
 *
 * Author: Hemant Pundpal						Date: 12 Feb 2019
 *
 */

#define RB_SOURCE_CODE

#include "ring_buffer_api.h"
#include "ring_buffer.h"


/* Buffer count */
static uint32_t g_ring_buffer_count = 0;

/* Created Ring Buffer List (doubly circular) */
static rgbf_t * gp_ring_buffer_created_list = NULL;

/* Add created ring buffer to the ring buffer list */
static void add_to_ring_buffer_list(rgbf_t * p_ring_buffer);

/* Add created ring buffer to the ring buffer list */
static void remove_ring_buffer_from_list(rgbf_t * p_ring_buffer);

/* Function to create Ring Buffer */
uint32_t create_ring_buffer(rgbf_t ** p_ring_buffer, uint32_t size)
{
    uint32_t status = RB_FAIL;

    /* Check conditions are OK */
    if (g_ring_buffer_count < RINGBUFFER_MAX_COUNT)
    {
        /* Allocate the memory for ring buffer */
        *p_ring_buffer = NULL;
        *p_ring_buffer = (rgbf_t *)calloc(1, sizeof(rgbf_t));

        if (*p_ring_buffer != NULL)
        {
            /* Allocate the buffer */
            (*p_ring_buffer)->p_buffer = NULL;
            (*p_ring_buffer)->p_buffer = (uint8_t *)calloc(size, sizeof(uint8_t));

            if ((*p_ring_buffer)->p_buffer)
            {
                /* Initialize the RingBuffer */

                /* Initialize all pointer */
                (*p_ring_buffer)->write_index = 0;
                (*p_ring_buffer)->read_index = 0;
                (*p_ring_buffer)->buffer_size = size;
                (*p_ring_buffer)->b_data_unread = FALSE;

                /* Add Ring Buffer to global list of Ring Buffers */
                add_to_ring_buffer_list(*p_ring_buffer);

                /* increase the Ring Buffer count */
                g_ring_buffer_count++;

                /* now set the Ring Buffer id, so it is read for use. */
                (*p_ring_buffer)->buffer_id = (uint32_t)*p_ring_buffer;

                status = RB_SUCCESS;
            }
            else
            {
                /* memory is not available for buffer size requested. */
                /* Free the memory allocated for the ring buffer */
                free(*p_ring_buffer);
                status = RB_NO_MEMORY_ERROR;
            }
        }
        else
        {
            /* memory if not available for ring buffer. */
            /* memory is not available. */
            status = RB_NO_MEMORY_ERROR;
        }
    }
    else
    {
        status = RB_MAX_OUT_ERROR;
    }

    /* Return Status */
    return status;
}

/* Function to write a Byte to Ring Buffer */
uint32_t byte_write_to_ring_buffer(rgbf_t * p_ring_buffer, const uint8_t * p_byte, bool_t b_over_Write)
{
    uint32_t status = RB_FAIL;

    if (p_ring_buffer->b_data_unread != TRUE)
    {
        /* Set status success */
        status = RB_SUCCESS;
    }
    else
    {
        /* Check size if available */
        if (p_ring_buffer->write_index != p_ring_buffer->read_index)
        {
            /* Set status success */
            status = RB_SUCCESS;
        }
    }

    /* If size available or over write set*/
    if ((status == RB_SUCCESS) || b_over_Write)
    {
        /* Write the byte */
        p_ring_buffer->p_buffer[p_ring_buffer->write_index] = *p_byte;

        /* Increment the write index */
        p_ring_buffer->write_index++;

        /* Check is write index is rolling over */
        if (p_ring_buffer->write_index == p_ring_buffer->buffer_size)
        {
            p_ring_buffer->write_index = 0;
        }

        /* Set data unread */
        p_ring_buffer->b_data_unread = TRUE;

        /* Check if it is a overwrite */
        if ((status == RB_FAIL) && b_over_Write)
        {
            /* set read is equal to write */
            p_ring_buffer->read_index = p_ring_buffer->write_index;

            /* Set status success */
            status = RB_SUCCESS;
        }
    }

    /* Return Status */
    return status;
}

/* Function to write a block to Ring Buffer */
uint32_t block_write_to_ring_buffer(rgbf_t * p_ring_buffer, const uint8_t * p_block, uint32_t size, bool_t b_over_write)
{
    uint32_t status = RB_FAIL;
    uint32_t available_size = 0;

    /* Check is buffer is empty */
    if (p_ring_buffer->b_data_unread != TRUE)
    {
        status = RB_SUCCESS;
    }
    else
    {
        if (p_ring_buffer->read_index != p_ring_buffer->write_index)
        {
            /* Get free size */
            available_size = (p_ring_buffer->read_index < p_ring_buffer->write_index) ?
                (p_ring_buffer->buffer_size - (p_ring_buffer->write_index - p_ring_buffer->read_index)) :
                (p_ring_buffer->read_index - p_ring_buffer->write_index);

            /* Check if required size is smaller than available size. */
            if (available_size >= size)
            {
                status = RB_SUCCESS;
            }
        }
    }

    if ((status == RB_SUCCESS) || b_over_write)
    {
        uint32_t count = 0;
        /* Write the block */
        do
        {
            p_ring_buffer->p_buffer[p_ring_buffer->write_index] = p_block[count];

            /* Increment the write index */
            p_ring_buffer->write_index++;
            count++;

            /* Check is write index is rolling over */
            if (p_ring_buffer->write_index == p_ring_buffer->buffer_size)
            {
                p_ring_buffer->write_index = 0;
            }
        } while (count < size);

        /* Set data unread */
        p_ring_buffer->b_data_unread = TRUE;

        /* Check if it is a overwrite */
        if (status == RB_FAIL && b_over_write)
        {
            /* set read is equal to write */
            p_ring_buffer->read_index = p_ring_buffer->write_index;

            /* Set status success */
            status = RB_SUCCESS;
        }
    }

    /* Return Status */
    return status;
}

/* Function to read a byte from the Ring Buffer */
uint32_t read_byte_from_ring_buffer(rgbf_t * p_ring_buffer, uint8_t * p_byte)
{
    uint32_t status = RB_FAIL;

    /* Check if there is unread data. */
    if (p_ring_buffer->b_data_unread)
    {
        /* read the byte */
        *p_byte = p_ring_buffer->p_buffer[p_ring_buffer->read_index];

        /* Increment the read index. */
        p_ring_buffer->read_index++;

        /* Check if read index is rolling over */
        if (p_ring_buffer->read_index == p_ring_buffer->buffer_size)
        {
            p_ring_buffer->read_index = 0;
        }

        /* Check if read and write index is same */
        if (p_ring_buffer->read_index == p_ring_buffer->write_index)
        {
            /* All data read, reset data unread flag. */
            p_ring_buffer->b_data_unread = FALSE;
        }

        /* Set status success */
        status = RB_SUCCESS;
    }

    return status;
}

/* Function to read a block from the Ring Buffer */
uint32_t read_block_from_ring_buffer(rgbf_t * p_ring_buffer, uint8_t * p_block, uint32_t size)
{
    uint32_t status = RB_FAIL;
    uint32_t available_size = 0;

    if (p_ring_buffer->read_index != p_ring_buffer->write_index)
    {
        /* Get free size */
        available_size = (p_ring_buffer->write_index < p_ring_buffer->read_index) ?
            (p_ring_buffer->buffer_size - (p_ring_buffer->read_index - p_ring_buffer->write_index)) :
            (p_ring_buffer->write_index - p_ring_buffer->read_index);

        /* Check if required size is smaller than available size. */
        if (available_size >= size)
        {
            status = RB_SUCCESS;
        }
    }
    else
    {
        if (p_ring_buffer->b_data_unread == TRUE)
        {
            status = RB_SUCCESS;
        }
    }


    /* check condition */
    if (status == RB_SUCCESS)
    {
        uint32_t count = 0;
        do
        {
            /* read the byte */
            p_block[count] = p_ring_buffer->p_buffer[p_ring_buffer->read_index];

            /* Increment the read index. */
            p_ring_buffer->read_index++;

            /* Check if read index is rolling over */
            if (p_ring_buffer->read_index == p_ring_buffer->buffer_size)
            {
                p_ring_buffer->read_index = 0;
            }

            count++;
        } while (count < size);

        if (p_ring_buffer->read_index == p_ring_buffer->write_index)
        {
            /* Reset data unread */
            p_ring_buffer->b_data_unread = FALSE;
        }

        /* Set status success */
        status = RB_SUCCESS;
    }

    return status;
}

/* Function to reset the Ring Buffer */
uint32_t reset_ring_buffer(rgbf_t * p_ring_buffer)
{
    uint32_t status = RB_FAIL;

    /* reset read and write pointer */
    p_ring_buffer->write_index = 0;
    p_ring_buffer->read_index = 0;
    p_ring_buffer->b_data_unread = FALSE;

    /* set status success */
    status = RB_SUCCESS;

    return status;
}

/* Function to delete the Ring Buffer */
uint32_t delete_ring_buffer(rgbf_t * p_ring_buffer)
{
    uint32_t status = RB_FAIL;

    /* First disable the ring buffer so no one can use it. */
    p_ring_buffer->buffer_id = 0x0U;

    /* reset read and write pointer */
    p_ring_buffer->write_index = 0x0U;
    p_ring_buffer->read_index = 0x0U;
    p_ring_buffer->buffer_size = 0x0U;
    p_ring_buffer->b_data_unread = FALSE;

    /* Remove the ring buffer from the created list */
    remove_ring_buffer_from_list(p_ring_buffer);

    /* decrease the Ring Buffer count */
    g_ring_buffer_count--;

    /* delete the buffer */
    free(p_ring_buffer->p_buffer);
    /* delete the structure */
    free(p_ring_buffer);

    /* set status success */
    status = RB_SUCCESS;

    return status;
}

/* local / internal function to add created ring buffer to the ring buffer list */
static void add_to_ring_buffer_list(rgbf_t * p_ring_buffer)
{
    /* check if ring buffer list is empty. */
    if (gp_ring_buffer_created_list)
    {
        /* Add to the end of the circular list */
        rgbf_t * p_temp_ring_buffer = NULL;
        p_temp_ring_buffer = gp_ring_buffer_created_list->p_previous_ring_buffer;

        gp_ring_buffer_created_list->p_previous_ring_buffer = p_ring_buffer;
        p_temp_ring_buffer->p_next_ring_buffer = p_ring_buffer;

        p_ring_buffer->p_next_ring_buffer = gp_ring_buffer_created_list;
        p_ring_buffer->p_previous_ring_buffer = p_temp_ring_buffer;

    }
    else
    {
        /* first ring buffer created, add to list */
        gp_ring_buffer_created_list = p_ring_buffer;
        gp_ring_buffer_created_list->p_previous_ring_buffer = p_ring_buffer;
        gp_ring_buffer_created_list->p_next_ring_buffer = p_ring_buffer;
    }
}

/* Local / internal function to remove created ring buffer from ring buffer list */
static void remove_ring_buffer_from_list(rgbf_t * p_ring_buffer)
{

    /* Check if it is the only thread in the list. */
    if(p_ring_buffer == p_ring_buffer->p_next_ring_buffer)
    {
        gp_ring_buffer_created_list = NULL;
    }
    else
    {
        /* not the only ring buffer created, link up the neighbors. */
        (p_ring_buffer->p_previous_ring_buffer)->p_next_ring_buffer =
                p_ring_buffer->p_next_ring_buffer;
        (p_ring_buffer->p_next_ring_buffer)->p_previous_ring_buffer =
                p_ring_buffer->p_previous_ring_buffer;

        if(gp_ring_buffer_created_list == p_ring_buffer)
        {
            gp_ring_buffer_created_list = p_ring_buffer->p_next_ring_buffer;
        }
    }
}
