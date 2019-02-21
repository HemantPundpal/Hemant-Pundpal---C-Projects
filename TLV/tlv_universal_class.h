/*
 * Name: tlv_universal_class.h
 *
 * Description:
 * All definitions and api function declarations required for the tlv universal tags encoder and decoder.
 *
 * Author: Hemant Pundpal                                   Date: 21 Feb 2019
 *
 */

#ifndef __TLV_UNIVERSAL_CLASS_H__
#define __TLV_UNIVERSAL_CLASS_H__

#include "tlv_port.h"

#define UNIVERSAL_TAG_LENGTH        1U


#define TAG_UNIVERSAL_PRIMITIVE     0x0U
#define TAG_UNIVERSAL_CONSTRUCTED   0x20U



/*
 * Function to create universal class tlv object
 * - if buffer_length == 0; indefinite buffer length
 * - if buffer_length != 0; definite buffer length
 *
 * Note: indefinite buffer length is only allowed for (constructed container tlv objects, not allowed for primitive tlv objects)
 */
uint32_t create_universal_tlv_object(tlv_object_t * p_tlv_object, uint32_t tlv_tag, const uint8_t * p_tlv_buffer, uint32_t buffer_length);

/* Encode the integer value into TLV object. */
void add_data_integer_tlv_object(tlv_object_t * p_tlv_object, const uint8_t * p_tlv_buffer, uint32_t buffer_length);

/* Encode the universal value into TLV object. */
void add_data_universal_tlv_object(tlv_object_t * p_tlv_object, const uint8_t * p_tlv_buffer, uint32_t buffer_length);


#endif /* __TLV_UNIVERSAL_CLASS_H__ */
