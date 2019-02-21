/*
 * Name: tlv_definition.c
 *
 * Description:
 * All definitions required for TLV encoding and decoding and associated constraints.
 *
 *
 * Author: Hemant Pundpal                                   Date: 21 Feb 2019
 *
 */

#define TLV_OBJECT_SOURCE_CODE

#include "tlv_api.h"
#include "tlv_definition.h"


/* Return number of length octets for encoding value length. */
uint32_t get_length_octets(uint32_t buffer_length)
{
    uint32_t length_octets = 0U;

    if (buffer_length <= ONE_OCTET_MAX_LENGTH)
    {
        length_octets = VALUE_LENGTH_1OCTET;
    }
    else if (buffer_length <= TWO_OCTET_MAX_LENGTH)
    {
        length_octets = VALUE_LENGTH_2OCTET;
    }
    else if (buffer_length <= THREE_OCTET_MAX_LENGTH)
    {
        length_octets = VALUE_LENGTH_3OCTET;
    }
    else
    {
        /* do nothing. */
        /* validity of the buffer length is checked in error check functions. */
    }

    return length_octets;
}


/* Return number of tag octets for encoding tag. */
uint32_t get_tag_octets(uint32_t tlv_tag)
{
    uint32_t tag_octets = 0;

    /* check if application class of context specific class tag number. */
    if (tlv_tag > ONE_OCTET_TAG_MAX && tlv_tag <= TWO_OCTET_TAG_MAX)
    {
        tag_octets = TAG_2OCTET;
    }
    else if (tlv_tag > TWO_OCTET_TAG_MAX && tlv_tag <= THREE_OCTET_TAG_MAX)
    {
        tag_octets = TAG_3OCTET;
    }
    else
    {
        /* do nothing. */
        /* validity of the tag is checked in error check functions. */
    }

    return tag_octets;
}
