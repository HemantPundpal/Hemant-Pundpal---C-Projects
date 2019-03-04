/*
 * Name: tlv_universal_class_encoder.c
 *
 * Description:
 * All API definitions required for the TLV universal tags encoder.
 *
 * Author: Hemant Pundpal                                   Date: 21 Feb 2019
 *
 */
#define TLV_APP_DATA_SOURCE_CODE

#include "tlv_api.h"
#include "tlv_universal_class.h"
#include "tlv_definition.h"

/* Create (universal class) integer TLV object. */
static uint32_t create_integer_tlv_object(tlv_object_t * p_tlv_object, uint32_t tlv_tag, const uint8_t * p_tlv_buffer, uint32_t buffer_length);

/* Create (universal class) integer TLV object. */
static uint32_t create_utf8_string_tlv_object(tlv_object_t * p_tlv_object, uint32_t tlv_tag, const uint8_t * p_tlv_buffer, uint32_t buffer_length);

/*
 * Function to create universal class TLV object
 * - if buffer_length == 0; indefinite buffer length
 * - if buffer_length != 0; definite buffer length
 *
 * Note: indefinite length is a option for only for constructed container TLV objects, not allowed for primitive TLV objects
 */
uint32_t create_universal_tlv_object(tlv_object_t * p_tlv_object, uint32_t tlv_tag, const uint8_t * p_tlv_buffer, uint32_t buffer_length)
{
    TLV_STATUS status = TLV_FAIL;

    switch (tlv_tag)
    {
        case TAG_INTEGER:
        {
            /* Check is buffer length is zero */
            if (buffer_length)
            {
                /* Definite length TLV object. */
                p_tlv_object->b_tlv_object_length_definite = TRUE;
                status = create_integer_tlv_object(p_tlv_object, tlv_tag, p_tlv_buffer, buffer_length);
            }
            else
            {
                /* Indefinite length in primitive tag number is not allowed. */
                status = TLV_NOT_A_CONTAINER;
            }
        }
        break;
        case TAG_UTF8STRING:
        {
            /* Check is buffer length is zero */
            if (buffer_length)
            {
                /* Definite length TLV object. */
                p_tlv_object->b_tlv_object_length_definite = TRUE;
                status = create_utf8_string_tlv_object(p_tlv_object, tlv_tag, p_tlv_buffer, buffer_length);
            }
            else
            {
                /* Indefinite length in primitive tag number is not allowed. */
                status = TLV_NOT_A_CONTAINER;
            }
        }
        break;
        default:
        {
            /* do nothing */
            status = TLV_TAG_NOT_SUPPORTED;
        }
    }

    return status;
}


/* Create (universal class) integer TLV object. */
static uint32_t create_integer_tlv_object(tlv_object_t * p_tlv_object, uint32_t tlv_tag, const uint8_t * p_tlv_buffer, uint32_t buffer_length)
{
    TLV_STATUS status = TLV_FAIL;

    /* Encode a integer TLV object. */

    /* Allocate the TLV encoded buffer. */

    uint8_t tlv_encoded_buffer_length = (uint8_t)UNIVERSAL_TAG_LENGTH + (uint8_t)VALUE_LENGTH_1OCTET + (uint8_t)MAX_INTEGER_BYTES;
    p_tlv_object->p_tlv_object_encoded_buffer = NULL;
    p_tlv_object->p_tlv_object_encoded_buffer = (uint8_t *)calloc(tlv_encoded_buffer_length, sizeof(uint8_t));

    if (p_tlv_object->p_tlv_object_encoded_buffer != NULL)
    {
        /* Set has a parent flag to FALSE. */
        p_tlv_object->b_tlv_has_a_parent = FALSE;
        p_tlv_object->p_tlv_parent_tlv_object = NULL;

        /* Initialize the TLV tag length. */
        p_tlv_object->tlv_tag_length = UNIVERSAL_TAG_LENGTH;

        /* Initialize the tLV encoded buffer. */
        p_tlv_object->tlv_max_encoded_object_length = (uint32_t)tlv_encoded_buffer_length;

        /* Save length of the value. */
        p_tlv_object->tlv_max_object_value_length = MAX_INTEGER_BYTES;

        /* save max length (number of octets) of the encoded value length. */
        p_tlv_object->tlv_max_length_of_endcoded_value_length = VALUE_LENGTH_1OCTET;

        /* Add integer tag number and class. */
        p_tlv_object->p_tlv_object_encoded_buffer[0] = (uint8_t)TAG_UNIVERSAL_PRIMITIVE | (uint8_t)TAG_INTEGER;

        /* Mark as not a container TLV object. */
        p_tlv_object->b_tlv_container_object = FALSE;
        p_tlv_object->tlv_child_Count = 0U;

        /* Encode the integer value into TLV object. */
        add_data_integer_tlv_object(p_tlv_object, p_tlv_buffer, buffer_length);

        p_tlv_object->p_tlv_child_tlv_object_list = NULL;
        p_tlv_object->p_child_tlv_object_next = NULL;
        p_tlv_object->p_child_tlv_object_previous = NULL;

        /* Store the tag number. */
        p_tlv_object->tlv_object_tag_number = tlv_tag;

        /* Set status success. */
        status = TLV_SUCCESS;
    }
    else
    {
        /* Memory allocation failed for the TLV encoded buffer. */
        status = TLV_NO_MEMORY;
    }

    /* Return Status. */
    return status;
}


/* Create (universal class) integer TLV object. */
static uint32_t create_utf8_string_tlv_object(tlv_object_t * p_tlv_object, uint32_t tlv_tag, const uint8_t * p_tlv_buffer, uint32_t buffer_length)
{
    TLV_STATUS status = TLV_FAIL;

    /* encode the UTF8 STRING TLV object. */

    /* Allocate the TLV encoder buffer. */

    uint32_t length_octets = 0U;
    length_octets = get_length_octets(buffer_length);

    uint32_t tlv_encoded_buffer_length = UNIVERSAL_TAG_LENGTH + length_octets + buffer_length;
    p_tlv_object->p_tlv_object_encoded_buffer = NULL;
    p_tlv_object->p_tlv_object_encoded_buffer = (uint8_t *)calloc(tlv_encoded_buffer_length, sizeof(uint8_t));

    if (p_tlv_object->p_tlv_object_encoded_buffer != NULL)
    {
        /* Set has a parent flag to FALSE. */
        p_tlv_object->b_tlv_has_a_parent = FALSE;
        p_tlv_object->p_tlv_parent_tlv_object = NULL;

        /* Initialize the TLV tag length. */
        p_tlv_object->tlv_tag_length = UNIVERSAL_TAG_LENGTH;

        /* Initialize max the tLV encoded buffer length. */
        p_tlv_object->tlv_max_encoded_object_length = (uint32_t)tlv_encoded_buffer_length;

        /* Save max length of the value. */
        p_tlv_object->tlv_max_object_value_length = buffer_length;

        /* save max length (number of octets) of the encoded value length. */
        p_tlv_object->tlv_max_length_of_endcoded_value_length = length_octets;

        /* Add UTF8 STRING tag number and class. */
        p_tlv_object->p_tlv_object_encoded_buffer[0] = (uint8_t)TAG_UNIVERSAL_PRIMITIVE | (uint8_t)TAG_UTF8STRING;

        /* Mark as not a container TLV object. */
        p_tlv_object->b_tlv_container_object = FALSE;
        p_tlv_object->tlv_child_Count = 0;

        /* Encode the integer value into TLV object. */
        add_data_universal_tlv_object(p_tlv_object, p_tlv_buffer, buffer_length);

        p_tlv_object->p_tlv_child_tlv_object_list = NULL;
        p_tlv_object->p_child_tlv_object_next = NULL;
        p_tlv_object->p_child_tlv_object_previous = NULL;

        /* Store the tag number. */
        p_tlv_object->tlv_object_tag_number = tlv_tag;

        /* Set status success. */
        status = TLV_SUCCESS;
    }
    else
    {
        /* Memory allocation failed for the TLV encoded buffer. */
        status = TLV_NO_MEMORY;
    }

    /* Return status. */
    return status;
}


/* Encode the integer value into TLV object. */
void_t add_data_integer_tlv_object(tlv_object_t * p_tlv_object, const uint8_t * p_tlv_buffer, uint32_t buffer_length)
{
    uint32_t int_value_octet_count = buffer_length;
    bool_t b_set_negative_bit = FALSE;

    /* if the integer is -ve or +ve */
    if (p_tlv_buffer[int_value_octet_count - 1U] == 0xFF)
    {
        b_set_negative_bit = TRUE;
    }

    /* get the number octets to encode for the integer value. */
    for (; int_value_octet_count > 1; int_value_octet_count--)
    {
        if (((uint8_t)CHECK_POSITIVE == p_tlv_buffer[(int_value_octet_count - 1U)]) || ((uint8_t)CHECK_NEGATIVE == p_tlv_buffer[(int_value_octet_count - 1)]))
        {
            /* Do nothing. */
        }
        else
        {
            break;
        }
    }

    /* Add length of the tag value */
    p_tlv_object->p_tlv_object_encoded_buffer[p_tlv_object->tlv_tag_length] = (uint8_t)int_value_octet_count;

    /* Save start of value buffer. */
    p_tlv_object->p_tlv_value_buffer = &(p_tlv_object->p_tlv_object_encoded_buffer[(p_tlv_object->tlv_tag_length + VALUE_LENGTH_1OCTET)]);

    p_tlv_object->tlv_curr_encoded_object_length = (p_tlv_object->tlv_tag_length + VALUE_LENGTH_1OCTET + int_value_octet_count);

    /* Save current length of the value. */
    p_tlv_object->tlv_curr_object_value_length = int_value_octet_count;

    /* Copy the TLV encoded value */
    for (uint32_t i = 0; i < p_tlv_object->tlv_max_object_value_length; i++)
    {
        if (i < p_tlv_object->tlv_curr_object_value_length)
        {
            p_tlv_object->p_tlv_value_buffer[(i)] = p_tlv_buffer[i];

            if ((p_tlv_object->tlv_curr_object_value_length == (i + 1U)) && b_set_negative_bit)
            {
                /* set -ve if not set already */
                p_tlv_object->p_tlv_value_buffer[i] |= (uint8_t)(1U << OCTET_SIGNED_BIT);
            }
        }
        else
        {
            /* Stuff zeros in empty TLV buffer space. */
            p_tlv_object->p_tlv_value_buffer[(i)] = (uint8_t)0x0U;
        }
    }
}


/* Encode the universal value into TLV object. */
void_t add_data_universal_tlv_object(tlv_object_t * p_tlv_object, const uint8_t * p_tlv_buffer, uint32_t buffer_length)
{
    uint32_t length_octets = 0U;
    length_octets = get_length_octets(buffer_length);

    switch (length_octets)
    {
        case VALUE_LENGTH_1OCTET:
        {
            /* Add length of the tag value. */
            p_tlv_object->p_tlv_object_encoded_buffer[p_tlv_object->tlv_tag_length] = (uint8_t)buffer_length;
        }
        break;
        case VALUE_LENGTH_2OCTET:
        {
            /* Add length of the tag value. */
            p_tlv_object->p_tlv_object_encoded_buffer[p_tlv_object->tlv_tag_length] = (uint8_t)LONG_LENGTH_FORM | (uint8_t)VALUE_LENGTH_1OCTET;
            p_tlv_object->p_tlv_object_encoded_buffer[(p_tlv_object->tlv_tag_length + 1U)] = (uint8_t)buffer_length;
        }
        break;
        case VALUE_LENGTH_3OCTET:
        {
            /* Add length of the tag value. */
            p_tlv_object->p_tlv_object_encoded_buffer[p_tlv_object->tlv_tag_length] = (uint8_t)LONG_LENGTH_FORM | (uint8_t)VALUE_LENGTH_2OCTET;
            p_tlv_object->p_tlv_object_encoded_buffer[(p_tlv_object->tlv_tag_length + 1U)] = (uint8_t)((buffer_length & MASK_BIT_8_TO_15) >> SHIFT_8BIT);
            p_tlv_object->p_tlv_object_encoded_buffer[(p_tlv_object->tlv_tag_length + 2U)] = (uint8_t)(buffer_length & MASK_BIT_0_TO_7);
        }
        break;
        default:
        {
            /* do nothing. */
            /* validity of the buffer length is checked in error check functions. */
        }
    }

    /* Save start of value buffer. */
    p_tlv_object->p_tlv_value_buffer = &(p_tlv_object->p_tlv_object_encoded_buffer[(p_tlv_object->tlv_tag_length + length_octets)]);

    /* Initialize current the tLV encoded buffer length. */
    p_tlv_object->tlv_curr_encoded_object_length = (p_tlv_object->tlv_tag_length + length_octets + buffer_length);

    /* Save current length of the value. */
    p_tlv_object->tlv_curr_object_value_length = buffer_length;

    /* Copy the TLV encoded value */
    for (uint32_t i = 0; i < p_tlv_object->tlv_max_object_value_length; i++)
    {
        if (i < p_tlv_object->tlv_curr_object_value_length)
        {
            p_tlv_object->p_tlv_value_buffer[i] = p_tlv_buffer[i];
        }
        else
        {
            /* Stuff zeros in empty TLV buffer space. */
            p_tlv_object->p_tlv_value_buffer[i] = (uint8_t)0x0U;
        }
    }
}

