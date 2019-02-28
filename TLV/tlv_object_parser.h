/*
 * Name: tlv_object_parser.h
 *
 * Description:
 * All definitions and api function declarations required for the TLV application and universal tags decoder.
 *
 * Author: Hemant Pundpal                                   Date: 21 Feb 2019
 *
 */

#ifndef __TLV_OBJECT_PARSER_H__
#define __TLV_OBJECT_PARSER_H__

#include "tlv_port.h"

 /* Update parsed TLV object. */
uint32_t update_parsed_tlv_object(const uint8_t * p_tlv_data_buffer, uint32_t buffer_length, tlv_object_t * p_tlv_object);

/* Get parsed TLV object for the found tag. */
uint32_t get_parsed_tlv_object(const uint8_t * p_tlv_data_buffer, uint32_t buffer_length, tlv_object_t * p_tlv_object);

#endif /* __TLV_OBJECT_PARSER_H__ */
