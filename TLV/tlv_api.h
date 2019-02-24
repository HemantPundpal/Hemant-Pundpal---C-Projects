/*
 * Name: tlv_api.h
 *
 * Description:
 * All definitions and api function declarations required for the tlv encoder and decoder.
 * Application code should #include this file to use the tlv encoder and decoder.
 *
 * Note application can choose to use the app data abstraction from tlv encoder decoder by including tlv_app_data_api.h
 *
 * Author: Hemant Pundpal                                   Date: 24 Feb 2019
 *
 */
#ifndef __TLV_API_H__
#define __TLV_API_H__

#include "tlv_port.h"
#ifndef __TLV_APP_DATA_API_H__
#include "tlv_tag_numbers.h"

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
#define TLV_OBJECT_DISABLE     0

   /* TLV API return type definition. */
#define TLV_STATUS                 uint32_t

/* TLV API return values. */
#define TLV_SUCCESS                0x0U
#define TLV_DATA_BUFFER_INVALID    0x1U
#define TLV_BAD_DATA_SIZE          0x2U
#define TLV_NO_MEMORY              0x3U
#define TLV_TAG_NOT_SUPPORTED      0x4U


/* TLV encoder / decoder API does not support these errors. */
/*
#define TLV_NOT_INITIALIZED        0x5U
#define TLV_TAG_CREATED            0x6U
#define TLV_TAG_NOT_CREATED        0x7U
#define TLV_MAX_CHILD_COUNT        0x8U
#define TLV_CHILD_NOT_FOUND        0x9U
*/

#define TLV_CANNOT_WRITE_VALUE     0xAU
#define TLV_CANNOT_CONTAIN_ITSELF  0xBU
#define TLV_NOT_A_CONTAINER        0xCU
#define TLV_BAD_BUFFER_LENGTH      0xDU
#define TLV_TAG_PTR_INVALID        0XEU
#define TLV_NO_TAG_FOUND           0xFU
#define TLV_OBJECT_INVALID_PTR     0x10U



#define TLV_FAIL                   0xFFFFFFFFU

#endif /* __TLV_APP_DATA_API_H__ */


/*
 * This is a TLV Object Data structure which can be used as container as well as child TLV Object.
 */
typedef struct tlv_object
{
    /* Tag number of the TLV object. */
    uint32_t              tlv_object_tag_number;

    /* Tag length in octets. */
    uint32_t              tlv_tag_length;

    /* TLV object is container of primitive tlv objects and/or constructed tlv objects.  */
    bool_t                b_tlv_container_object;

    /* 
     * Length of TLV Object if set by application: 
     * - definite if the encoding is primitive
     * - definite form or the indefinite form , a application option, if the encoding is constructed and all immediately available
     * - indefinite form if the encoding is constructed and is not all immediately available
     */
    bool_t                b_tlv_object_length_definite;

    /* Number of child TLV object in a container of primitive TLV objects and/or constructed tlv objects. */
    uint32_t              tlv_child_Count;

    /* Maximum length (number of octets) of a encoded of TLV objects (includes tag, length and value octets). */
    uint32_t              tlv_max_encoded_object_length;

    /* Current length (number of octets) of a encoded of TLV objects (includes tag, length and value octets). */
    uint32_t              tlv_curr_encoded_object_length;

    /* Maximum length (number of octets) of TLV encoded value length. (RESERVED) */
    uint32_t              tlv_max_length_of_endcoded_value_length;

    /* TLV encoded buffer. */
    uint8_t             * p_tlv_object_encoded_buffer;

    /*
     * Maximum Length (number of value octet) in a encoding of primitive TLV objects.
     * EG:
     * - integer TLV object encoding
     * - real (float / double) TLV object encoding
     * - UTF8 string TLV object encoding,
     * - fixed octet length primitive TLV object encoding as OCTAT string
     */
    uint32_t              tlv_max_object_value_length;

    /*
     * Current Length (number of value octet) in a encoding of primitive TLV objects.
     * EG:
     * - integer TLV object encoding
     * - real (float / double) TLV object encoding
     * - UTF8 string TLV object encoding,
     * - fixed octet length primitive TLV object encoding as OCTAT string
     */
    uint32_t              tlv_curr_object_value_length;

    /* Start of value octets in TLV encoded buffer */
    uint8_t             * p_tlv_value_buffer;

    /* List of child TLV objects in a container TLV object. */
    struct tlv_object   * p_tlv_child_tlv_object_list;

    /* Pointer to next and previous child TLV object in the list. */
    struct tlv_object   * p_child_tlv_object_next,
                        * p_child_tlv_object_previous;

} tlv_object_t;


/*
 * Defines the tlv api mapping based on error checking selected by the user.
 * This section is only applicable to the application that is using the tlv api.
 * This section (TLV_SOURCE_CODE) is turned off for the tlv implementation source code.
 */
#ifndef TLV_OBJECT_SOURCE_CODE

 /*
  * Determines if error checking is desired. If so, map tlv app data api function to appropriate
  * error checking front-ends. Otherwise, map API functions to core functions that actually perform
  * the work.
  *
  * Note: Error checking is enabled by default. It can be disabled application code is stable.
  */
#if (0 < TLV_OBJECT_DISABLE)

  /* Api functions without error checking. */

#define create_tlv_object                       create_tlv_object
#define add_tlv_object_to_tlv_container         add_tlv_object_to_tlv_container
#define add_data_to_tlv_object                  add_data_to_tlv_object
#define parse_tlv_object                        parse_tlv_object
#define tlv_search_tag                          tlv_search_tag

#else

  /* Api functions with error checking. */

#define create_tlv_object                       create_tlv_object_ec
#define add_tlv_object_to_tlv_container         add_tlv_object_to_tlv_container_ec
#define add_data_to_tlv_object                  add_data_to_tlv_object_ec
#define parse_tlv_object                        parse_tlv_object_ec
#define tlv_search_tag                          tlv_search_tag_ec

#endif /* DISABLE_ERROR_CHECK */


/*
 * Function to create tlv object
 * - if buffer_length == 0; indefinite buffer length
 * - if buffer_length != 0; definite buffer length
 *
 * Note: Indefinite length is a option for only for constructed container tlv objects, not allowed for primitive tlv objects
 * Note: Universal class tags that can be encoded as "both" (primitive and constructed) are encoded as primitive.
 */
TLV_STATUS create_tlv_object(tlv_object_t * p_tlv_object, uint32_t tlv_tag, uint8_t * p_tlv_value, uint32_t value_length);

/*
 * - Function to add tlv object to tlv container.
 * - The child TLV object can be a container, meaning a container tlv object can be added to a container tlv object as a child tlv object.
 * - This function does not limit the number of child objects that can be added to the container TLV object. Application can define such
 * limits for itself.
 */
TLV_STATUS add_tlv_object_to_tlv_container(tlv_object_t * p_container_tlv_object, tlv_object_t * p_child_tlv_object);

/*
 * - Function to add data to tlv object.
 * - The container TLV object can be of type primitive or constructed container.
 */
TLV_STATUS add_data_to_tlv_object(tlv_object_t * p_tlv_object, uint32_t tlv_tag, const uint8_t * p_tlv_value, uint32_t value_length);

/* Function to parse tlv data buffer. */
TLV_STATUS parse_tlv_object(const uint8_t * p_tlv_data_buffer, uint32_t buffer_length, tlv_object_t * p_tlv_object);

/* Function to search tlv encoded data object in the tlv data buffer. */
TLV_STATUS tlv_search_tag(const uint8_t * p_tlv_data_buffer, uint32_t buffer_length, uint32_t tag, bool_t recursive, tlv_object_t * p_tlv_object);

#endif /* TLV_OBJECT_SOURCE_CODE */

#endif /* __TLV_API_H__*/
