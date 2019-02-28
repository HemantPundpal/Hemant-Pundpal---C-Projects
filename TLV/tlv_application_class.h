/*
 * Name: tlv_application_class.h
 *
 * Description:
 * All definitions and api function declarations required for the TLV application tags encoder and decoder.
 *
 * Author: Hemant Pundpal                                   Date: 21 Feb 2019
 *
 */

#ifndef __TLV_APPLICATION_CLASS_H__
#define __TLV_APPLICATION_CLASS_H__

#include "tlv_port.h"

/* Application class primitive. */
/* Definite size application class tag number is considered primitive. */
#define TAG_APPL_CLS_PRIMITIVE           0x40U
/* Application class constructed */
#define TAG_APPL_CLS_CONSTRUCTED         0x60U

/* Context specific class primitive. */
/* Definite size context specific tag number is considered primitive. */
#define TAG_CS_CLS_PRIMITIVE             0x80U
/* Context specific class constructed. */
#define TAG_CS_CLS_CONSTRUCTED           0xA0U

/* Private class primitive. */
#define TAG_PRIVATE_CLS_PRIMITIVE        0xC0U /* not supported. */
/* Private class constructed. */
#define TAG_PRIVATE_CLS_CONSTRUCTED      0xE0U /* not supported. */


/*
 * Function to create application class TLV object
 * - if buffer_length == 0; indefinite buffer length
 * - if buffer_length != 0; definite buffer length
 *
 * Note: indefinite buffer length is a option for only for constructed container TLV objects, not allowed for primitive TLV objects
 */
uint32_t create_application_tlv_object(tlv_object_t * p_tlv_object, uint32_t tlv_tag, const uint8_t * p_tlv_buffer, uint32_t buffer_length);


/* Encode the definite length value into TLV object. */
void_t add_data_to_definite_length_application_tlv_object(tlv_object_t * p_tlv_object, const uint8_t * p_tlv_buffer, uint32_t buffer_length);


#endif /* __TLV_APPLICATION_CLASS_H__ */
