/*
 * Name: ring_buffer_api.h
 *
 * Description:
 * All definitions and api function declarations required by the application to use the ring buffer
 * is included in this file.
 * Application code should #include this file to use the ring buffer api.
 *
 * Author: Hemant Pundpal						Date: 12 Feb 2019
 *
 */

#ifndef __RING_BUFFER_API__
#define __RING_BUFFER_API__


#include "ring_buffer_port.h"

/*
 *	Error Assert can be enabled by setting ERROR_ASSERT to 0
 *  Error Assert can be disabled by setting ERROR_ASSERT to 1
 *
 *  Note: ERROR_ASSERT will enable assert without abort
 */
#define ERROR_ASSERT            1

/*
 *	Error Assert can be enabled by setting ERROR_ASSERT_ABORT to 0
 *  Error Assert can be disabled by setting ERROR_ASSERT_ABORT to 1
 *
 *  Note: ERROR_ASSERT_ABORT will enable assert with abort. ERROR_ASSERT
 *  should be enabled ERROR_ASSERT_ABORT to work.
 *
 */
#define ERROR_ASSERT_ABORT      0

/*
 * Error checking (input parameters checking by api functions) can be disabled by setting
 * DISABLE_ERROR_CHECK to 1
 * Error checking (input parameters checking by api functions) can be enabled by setting
 * DISABLE_ERROR_CHECK to 0
 *
 * Note: Error checking is enabled by default. It can be disabled once application code is stable.
 */
#define DISABLE_ERROR_CHECK     0


/*
 * Maximum size of ring buffer. This value can be modified as per platform and
 * application requirements.
 */
#define RINGBUFFER_SIZE_MAX    1024U

/*
 * Maximum number of ring buffers that can be created. This value can  be modified
 * as per a platform and application requirements.
 */
#define RINGBUFFER_MAX_COUNT   30U



/* Ring Buffer API Return values */
#define RB_SUCCESS             0x0U
#define RB_PTR_INVALID         0x1U
#define RB_DATA_PTR_INVALID    0x2U
#define RB_BUFFER_SIZE_ERROR   0x3U
#define RB_MAX_OUT_ERROR       0x4U
#define RB_NO_MEMORY_ERROR     0x5U

#define RB_FAIL                0xFFFFFFFFU

/* Ring Buffer Structure. */
typedef struct ring_buffer
{
    uint32_t     buffer_id;
    uint8_t    * p_buffer;
    uint32_t     write_index;
    uint32_t     read_index;
    uint32_t     buffer_size;
    bool_t       b_data_unread;
    struct ring_buffer
        * p_next_ring_buffer,
        * p_previous_ring_buffer;

}rgbf_t;


/*
 * Defines the ring buffer api mapping based on error checking selected by the user.
 * This section is only applicable to the application that is using the ring buffer api.
 * This section (RB_SOURCE_CODE) is turned off for the ring buffer implementation source code.
 */
#ifndef RB_SOURCE_CODE

/*
 * Determines if error checking is desired. If so, map ring buffer api function to appropriate
 * error checking front-ends. Otherwise, map API functions to core functions that actually perform
 * the work.
 *
 * Note: Error checking is enabled by default. It can be disabled application code is stable.
 */
#if (0 < DISABLE_ERROR_CHECK)

/* Api functions without error checking. */

#define create_ring_buffer           create_ring_buffer
#define byte_write_to_ring_buffer    byte_write_to_ring_buffer
#define block_write_to_ring_buffer   block_write_to_ring_buffer
#define read_byte_from_ring_buffer   read_byte_from_ring_buffer
#define read_block_from_ring_buffer  read_block_from_ring_buffer
#define reset_ring_buffer            reset_ring_buffer
#define delete_ring_buffer           delete_ring_buffer

#else

/* Api functions with error checking. */

#define create_ring_buffer           create_ring_buffer_ec
#define byte_write_to_ring_buffer    byte_write_to_ring_buffer_ec
#define block_write_to_ring_buffer   block_write_to_ring_buffer_ec
#define read_byte_from_ring_buffer   read_byte_from_ring_buffer_ec
#define read_block_from_ring_buffer  read_block_from_ring_buffer_ec
#define reset_ring_buffer            reset_ring_buffer_ec
#define delete_ring_buffer           delete_ring_buffer_ec

#endif /* DISABLE_ERROR_CHECK */

/* Create Ring Buffer */
uint32_t create_ring_buffer(rgbf_t ** p_ring_buffer, uint32_t size);

/* Write a Byte to Ring Buffer */
uint32_t byte_write_to_ring_buffer(rgbf_t * p_ring_buffer, const uint8_t * p_byte, bool_t b_over_Write);

/* Write a block to Ring Buffer */
uint32_t block_write_to_ring_buffer(rgbf_t * p_ring_buffer, const uint8_t * p_block, uint32_t size, bool_t b_over_write);

/* Read a byte from the Ring Buffer */
uint32_t read_byte_from_ring_buffer(rgbf_t * p_ring_buffer, uint8_t * p_byte);

/* Read a block from the Ring Buffer */
uint32_t read_block_from_ring_buffer(rgbf_t * p_ring_buffer, uint8_t * p_block, uint32_t size);

/* Reset the Ring Buffer */
uint32_t reset_ring_buffer(rgbf_t * p_ring_buffer);

/* Delete the Ring Buffer */
uint32_t delete_ring_buffer(rgbf_t * pRingBuffer);

#endif /* BR_SOURCE_CODE */

#endif /* __RING_BUFFER_API__ */
