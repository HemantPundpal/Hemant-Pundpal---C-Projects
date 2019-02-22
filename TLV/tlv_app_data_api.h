/*
 * Name: tlv_app_data_api.h
 *
 * Description:
 * All definitions and api function declarations required for the tlv app data layer.
 * This function abstrats the TLV encoder and decoder for the application.
 *
 * Application code should #include this file to use the app data layer.
 * Note application can choose to diectly access TLV encoder and decode by including tlv_api.h
 *
 * Author: Hemant Pundpal                                   Date: 21 Feb 2019
 *
 */
#ifndef __TLV_APP_DATA_API_H__
#define __TLV_APP_DATA_API_H__

#include "tlv_port.h"
#include "tlv_tag_numbers.h" /* app data layer can define application friendly tags and abatract tlv standard tags from application */


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
#define DISABLE_ERROR_CHECK       0

/* TLV API return type definition. */
typedef uint32_t                  TLV_STATUS;

/* TLV API return values. */
#define TLV_SUCCESS                0x0U
#define TLV_DATA_BUFFER_INVALID    0x1U
#define TLV_BAD_DATA_SIZE          0x2U
#define TLV_NO_MEMORY              0x3U
#define TLV_TAG_NOT_SUPPORTED      0x4U
#define TLV_NOT_INITIALIZED        0x5U
#define TLV_TAG_CREATED            0x6U
#define TLV_TAG_NOT_CREATED        0x7U
#define TLV_MAX_CHILD_COUNT        0x8U
#define TLV_CHILD_NOT_FOUND        0x9U
#define TLV_CANNOT_WRITE_VALUE     0xAU


#define TLV_FAIL                   0xFFFFFFFFU


/*
 * Defines the tlv app data api mapping based on error checking selected by the user.
 * This section is only applicable to the application that is using the tlv app data api.
 * This section (TLV_SOURCE_CODE) is turned off for the tlv implementation source code.
 */
#ifndef TLV_APP_DATA_SOURCE_CODE

/*
 * Determines if error checking is desired. If so, map tlv app data api function to appropriate
 * error checking front-ends. Otherwise, map API functions to core functions that actually perform
 * the work.
 *
 * Note: Error checking is enabled by default. It can be disabled application code is stable.
 */
#if (0 < DISABLE_ERROR_CHECK)

/* Api functions without error checking. */

#define tlv_initialize                          tlv_initialize
#define tlv_init_and_create_app_data            tlv_init_and_create_app_data
#define tlv_create_container_app_data           tlv_create_container_app_data
#define tlv_add_child_to_container_app_data     tlv_add_child_to_container_app_data
#define tlv_add_data_to_app_data                tlv_add_data_to_app_data
#define tlv_add_data_to_container_app_data      tlv_add_data_to_container_app_data
#define tlv_add_child_tag_to_container_app_data tlv_add_child_tag_to_container_app_data
#define tlv_app_data_send                       tlv_app_data_send
#define tlv_parse_app_data                      tlv_parse_app_data
#define tlv_search_parse_app_data               tlv_search_parse_app_data
#define tlv_delete_app_data                     tlv_delete_app_data

#else

/* Api functions with error checking. */

#define tlv_initialize                          tlv_initialize
#define tlv_init_and_create_app_data            tlv_init_and_create_app_data_ec
#define tlv_create_container_app_data           tlv_create_container_app_data_ec
#define tlv_add_child_to_container_app_data     tlv_add_child_to_container_app_data_ec
#define tlv_add_data_to_app_data                tlv_add_data_to_app_data_ec
#define tlv_add_data_to_container_app_data      tlv_add_data_to_container_app_data_ec
#define tlv_add_child_tag_to_container_app_data tlv_add_child_tag_to_container_app_data_ec
#define tlv_app_data_send                       tlv_app_data_send_ec
#define tlv_parse_app_data                      tlv_parse_app_data_ec
#define tlv_search_parse_app_data               tlv_search_parse_app_data_ec
#define tlv_delete_app_data                     tlv_delete_app_data_ec

#endif /* DISABLE_ERROR_CHECK */

/*
 * Initialize TLV.
 * Application must call this function before accessing any other app data API
 */
void tlv_initialize(void);

/* Function to initialize and create tlv object of the app data with definite size. */
uint32_t tlv_init_and_create_app_data(uint8_t * p_app_data_buffer, uint32_t app_data_size, uint32_t app_data_tag);

/* Function to create constructed container tlv obect of the app data with indefinite size. */
uint32_t tlv_create_container_app_data(uint32_t container_app_data_tag);

/* Function to create tlv object of the app data and add to container (parent) tlv object as a child. */
uint32_t tlv_add_child_to_container_app_data(uint32_t container_app_data_tag, uint8_t * p_app_data_buffer, uint32_t app_data_size, uint32_t child_app_data_tag);

/* Function to add created child tlv object to container (parent) tlv object. */
uint32_t tlv_add_child_tag_to_container_app_data(uint32_t container_app_data_tag, uint32_t child_app_data_tag);

/* Function to add data to constructed and primitive tlv objects with definite size. */
uint32_t tlv_add_data_to_app_data(uint32_t app_data_tag);

/* Function to add data to constructed container tlv objects with indefinite size. */
uint32_t tlv_add_data_to_container_app_data(uint32_t container_app_data_tag, uint32_t child_app_data_tag);

/* Function to send application data. */
uint32_t tlv_app_data_send(uint32_t tag);

/* 
 * - Function to parse app data from TLV data buffer (first found tlv object is parsed.)
 * - The parsed TLV object data will be auto updated to the application variables.
 */
uint32_t tlv_parse_app_data(const uint8_t * p_tlv_buffer, uint32_t * p_parsed_tag);

/* 
 * - Function to search tag and parse the tlv data buffer (if recursive set to true, then search for child).
 * - The searched and parsed TLV object data will be auto updated to the application variables.
 */
uint32_t tlv_search_parse_app_data(const uint8_t * p_tlv_buffer, uint32_t search_parse_tag, bool_t b_recersive);

/* Function to delete a tlv object and removes the mapping between the tlv object and the app data. */
uint32_t tlv_delete_app_data(uint32_t tag);

#endif /* TLV_SOURCE_CODE */

#endif /* __TLV_APP_DATA_API_H__ */

