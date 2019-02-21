/*
 * Name: tlv_definition.h
 *
 * Description:
 * All definitions and deceleration required for TLV encoding and decoding and associated constraints.
 *
 *
 * Author: Hemant Pundpal                                   Date: 21 Feb 2019
 *
 */

#ifndef __TLV_DEFINITION_H__
#define __TLV_DEFINITION_H__

#define ONE_OCTET_MAX_LENGTH        127U
#define TWO_OCTET_MAX_LENGTH        255U
#define THREE_OCTET_MAX_LENGTH      65535U

#define ONE_OCTET_TAG_MAX           31U
#define TWO_OCTET_TAG_MAX           127U
#define THREE_OCTET_TAG_MAX         254U

/* Universal tlv object length field length of 1 octet. */
#define VALUE_LENGTH_1OCTET         1U
/* Universal tlv object length field length of 2 octet. */
#define VALUE_LENGTH_2OCTET         2U
/* Universal tlv object length field length of 3 octet. */
#define VALUE_LENGTH_3OCTET         3U

/* Long Lenght Form */
#define LONG_LENGTH_FORM            0x80U
#define INDEFINITE_LENGTH           0x80U


/* Application tlv object tag length of 2 octet. */
#define TAG_2OCTET                  2U
/* Application tlv object tag length of 2 octet. */
#define TAG_3OCTET                  3U

/* Tag is extended to two bytes */
#define TAG_EXTENDED_BYTES          31U
#define SHIFT_TO_BIT_7              7U
#define OCTET_SIGNED_BIT            7U

/* Return number of length octets for encoding value length. */
uint32_t get_length_octets(uint32_t buffer_lenght);

/* Return number of tag octets for encoding tag. */
uint32_t get_tag_octets(uint32_t tag);

#endif /* __TLV_DEFINITION_H__ */
