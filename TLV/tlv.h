/*
 * Name: tlv.h
 *
 * Description:
 * All definitions and api function declarations required for the TLV encoder and decoder.
 *
 * Author: Hemant Pundpal                                   Date: 21 Feb 2019
 *
 */

#ifndef __TLV_H__
#define __TLV_H__

/* 
 * Function to create TLV object 
 * - if buffer_length == 0; indefinite buffer length
 * - if buffer_length != 0; definite buffer length
 *
 * Note: Indefinite length is a option for only for constructed container TLV objects, not allowed for primitive TLV objects
 * Note: Universal class tags that can be encoded as "both" (primitive and constructed) are encoded as primitive. 
 */
uint32_t create_tlv_object(tlv_object_t * p_tlv_object, uint32_t tlv_tag, const uint8_t * p_tlv_value, uint32_t value_length);

/* 
 * - Function to add TLV object to TLV container.
 * - The child TLV object can be a container, meaning a container TLV object can be added to a container TLV object as a child TLV object.
 * - This function does not limit the number of child objects that can be added to the container TLV object. Application can define such
 * limits for itself.
 */
uint32_t add_tlv_object_to_tlv_container(tlv_object_t * p_container_tlv_object, tlv_object_t * p_child_tlv_object);

/* 
 * - Function to add data to TLV object.
 * - The container TLV object can be of type primitive or constructed container.
 */
uint32_t add_data_to_tlv_object(tlv_object_t * p_tlv_object, uint32_t tlv_tag, const uint8_t * p_tlv_value, uint32_t value_length);

/* Function to parse TLV data buffer. */
uint32_t parse_tlv_object(const uint8_t * p_tlv_data_buffer, uint32_t buffer_length, tlv_object_t * p_tlv_object);

/* Function to search TLV encoded data object in the TLV data buffer. */
uint32_t tlv_search_tag(const uint8_t * p_tlv_data_buffer, uint32_t buffer_length, uint32_t tag, bool_t b_recursive, tlv_object_t * p_tlv_object);



/* Error check for create TLV object function. */
uint32_t create_tlv_object_ec(tlv_object_t * p_tlv_object, uint32_t tlv_tag, uint8_t * p_tlv_value, uint32_t value_length);

/* Error check for add TLV object to TLV container function. */
uint32_t add_tlv_object_to_tlv_container_ec(tlv_object_t * p_container_tlv_object, tlv_object_t * p_child_tlv_object);

/* Error check for add data to TLV object function. */
uint32_t add_data_to_tlv_object_ec(tlv_object_t * p_tlv_object, uint32_t tlv_tag, const uint8_t * p_tlv_value, uint32_t value_length);

/* Error check for parse TLV data buffer function. */
uint32_t parse_tlv_object_ec(const uint8_t * p_tlv_data_buffer, uint32_t buffer_length, tlv_object_t * p_tlv_object);

/* Error check for search TLV encoded data object in the TLV data buffer function. */
uint32_t tlv_search_tag_ec(const uint8_t * p_tlv_data_buffer, uint32_t buffer_length, uint32_t tag, bool_t b_recursive, tlv_object_t * p_tlv_object);

#endif /* __TLV_H__ */
