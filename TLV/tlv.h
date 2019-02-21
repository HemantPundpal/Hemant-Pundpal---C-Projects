/*
 * Name: tlv.h
 *
 * Description:
 * All definitions and api function declarations required for the tlv encoder and decoder.
 *
 * Author: Hemant Pundpal                                   Date: 21 Feb 2019
 *
 */

#ifndef __TLV_H__
#define __TLV_H__

/* 
 * Function to create tlv object 
 * - if buffer_length == 0; indefinite buffer length
 * - if buffer_length != 0; definite buffer length
 *
 * Note: Indefinite length is a option for only for constructed container tlv objects, not allowed for primitive tlv objects
 * Note: Universal class tags that can be encoded as "both" (primitive and constructed) are encoded as primitive. 
 */
uint32_t create_tlv_object(tlv_object_t * p_tlv_object, uint32_t tlv_tag, const uint8_t * p_tlv_value, uint32_t value_length);

/* 
 * - Function to add tlv object to tlv container.
 * - The child TLV object can be a container, meaning a container tlv object can be added to a container tlv object as a child tlv object.
 * - This function does not limit the number of child objects that can be added to the container TLV object. Application can define such
 * limits for itself.
 */
uint32_t add_tlv_object_to_tlv_container(tlv_object_t * p_container_tlv_object, tlv_object_t * p_child_tlv_object);

/* 
 * - Function to add data to tlv object.
 * - The container TLV object can be of type primitive or constructed container.
 */
uint32_t add_data_to_tlv_object(tlv_object_t * p_tlv_object, uint32_t tlv_tag, const uint8_t * p_tlv_value, uint32_t value_length);

/* Function to parse tlv data buffer. */
uint32_t parse_tlv_object(const uint8_t * p_tlv_data_buffer, uint32_t buffer_length, tlv_object_t * p_tlv_object);

/* Function to search tlv encoded data object in the tlv data buffer. */
uint32_t tlv_search_tag(const uint8_t * p_tlv_data_buffer, uint32_t buffer_length, uint32_t tag, bool_t recursive, tlv_object_t * p_tlv_object);



/* Error check for create tlv object function. */
uint32_t create_tlv_object_ec(tlv_object_t * p_tlv_object, uint32_t tlv_tag, uint8_t * p_tlv_value, uint32_t value_length);

/* Error check for add tlv object to tlv container function. */
uint32_t add_tlv_object_to_tlv_container_ec(tlv_object_t * p_container_tlv_object, tlv_object_t * p_child_tlv_object);

/* Error check for add data to tlv object function. */
uint32_t add_data_to_tlv_object_ec(tlv_object_t * p_tlv_object, uint32_t tlv_tag, const uint8_t * p_tlv_value, uint32_t value_length);

/* Error check for parse tlv data buffer function. */
uint32_t parse_tlv_object_ec(const uint8_t * p_tlv_data_buffer, uint32_t buffer_length, tlv_object_t * p_tlv_object);

/* Error check for search tlv encoded data object in the tlv data buffer function. */
uint32_t tlv_search_tag_ec(const uint8_t * p_tlv_data_buffer, uint32_t buffer_length, uint32_t tag, bool_t recursive, tlv_object_t * p_tlv_object);

#endif /* __TLV_H__ */
