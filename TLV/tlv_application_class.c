/*
 * Name: tlv_application_class.c
 *
 * Description:
 * All API definitions required for the tlv application tags encoder and decoder.
 *
 * Author: Hemant Pundpal                                   Date: 21 Feb 2019
 *
 */

#define TLV_APP_DATA_SOURCE_CODE

#include "tlv_api.h"
#include "tlv_application_class.h"
#include "tlv_definition.h"

/*
 * Create indefinite length application class TLV object.
 * This function can also create indefinite length context specific class TLV object.
 */
static uint32_t create_indefinite_length_application_tlv_object(tlv_object_t * p_tlv_object, uint32_t tlv_tag);

/*
 * Create definite length application class TLV object.
 * This function can also create definite length context specific class TLV object.
 */
static uint32_t create_definite_length_application_tlv_object(tlv_object_t * p_tlv_object, uint32_t tlv_tag, const uint8_t * p_tlv_buffer, uint32_t buffer_length);

/*
 * Function to create context specific class tlv object
 * - if buffer_length == 0; indefinite buffer length
 * - if buffer_length != 0; definite buffer length
 *
 * Note: indefinite buffer length is a option for only for constructed container tlv objects, not allowed for primitive tlv objects
 * NOT REQUIRED.
 */
static uint32_t create_context_specific_class_tlv_object(tlv_object_t * p_tlv_object, uint32_t tlv_tag, const uint8_t * p_tlv_buffer, uint32_t buffer_length);

/*
 * Function to create application class or context specific class tlv object
 * - if buffer_length == 0; indefinite buffer length
 * - if buffer_length != 0; definite buffer length
 *
 * Note: indefinite buffer length is a option for only for constructed container tlv objects, not required for primitive tlv objects
 */
uint32_t create_application_tlv_object(tlv_object_t * p_tlv_object, uint32_t tlv_tag, const uint8_t * p_tlv_buffer, uint32_t buffer_length)
{
    TLV_STATUS status = TLV_FAIL;

    /*
     * The tag number is only checked to be within allowed application tag number range. 
     * It is application responsibility to not create constructed container TLV object with indefinite
     * length for primitive data type.
     */

   /* Check is buffer length is zero */
   if (buffer_length)
   {
       /* Not Supported. */
       p_tlv_object->b_tlv_object_length_definite = TRUE;
       status = create_definite_length_application_tlv_object(p_tlv_object, tlv_tag, p_tlv_buffer, buffer_length);
   }
   else
   {
       /* Not Supported. */
       p_tlv_object->b_tlv_object_length_definite = FALSE;
       status = create_indefinite_length_application_tlv_object(p_tlv_object, tlv_tag);
   }

    return status;
}

/* 
 * Create indefinite length application class TLV object.
 * This function can also create indefinite length context specific class TLV object.
 */
static uint32_t create_indefinite_length_application_tlv_object(tlv_object_t * p_tlv_object, uint32_t tlv_tag)
{
    TLV_STATUS status = TLV_FAIL;

    /* Allocate the TLV encoder buffer. */

    uint32_t tag_octets = 0;
    tag_octets = get_tag_octets(tlv_tag);
    uint32_t tlv_encoded_buffer_length = (tag_octets + VALUE_LENGTH_1OCTET);

    /* Allocate the TLV encoded buffer. */
    p_tlv_object->p_tlv_object_encoded_buffer = NULL;
    p_tlv_object->p_tlv_object_encoded_buffer = (uint8_t *)calloc(tlv_encoded_buffer_length, sizeof(uint8_t));

    if (p_tlv_object->p_tlv_object_encoded_buffer != NULL)
    {
        /* Initialize the tLV encoded buffer. */
        p_tlv_object->tlv_max_encoded_object_length = tlv_encoded_buffer_length;
        p_tlv_object->tlv_curr_encoded_object_length = tlv_encoded_buffer_length;
        p_tlv_object->tlv_tag_length = tag_octets;
        p_tlv_object->tlv_max_length_of_endcoded_value_length = VALUE_LENGTH_1OCTET;

        if (tlv_tag < TAG_APP_CLS_MAX)
        {
            /* Add application constructed TLV object tag number and class. */
            p_tlv_object->p_tlv_object_encoded_buffer[0] = (uint8_t)TAG_APPL_CLS_CONSTRUCTED | (uint8_t)TAG_EXTENDED_BYTES;
        }
        else if (tlv_tag < TAG_CONT_SPCF_CLS_MAX)
        {
            /* Add context specific constructed TLV object tag number and class. */
            p_tlv_object->p_tlv_object_encoded_buffer[0] = (uint8_t)TAG_CS_CLS_CONSTRUCTED | (uint8_t)TAG_EXTENDED_BYTES;
        }
        else
        {
            /* do nothing */
            /* validity of the tag is checked in error check functions. */
        }

        switch (p_tlv_object->tlv_tag_length)
        {
            case TAG_2OCTET:
            {
                /* encode the tag. */
                p_tlv_object->p_tlv_object_encoded_buffer[1] = (uint8_t)tlv_tag;
            }
            break;
            case TAG_3OCTET:
            {
                /* encode the tag. */
                p_tlv_object->p_tlv_object_encoded_buffer[1] = (uint8_t)((tlv_tag & 0xFF00) >> 8U);
                p_tlv_object->p_tlv_object_encoded_buffer[1] |= (uint8_t)(1U << SHIFT_TO_BIT_7);
                p_tlv_object->p_tlv_object_encoded_buffer[2] = (uint8_t)(tlv_tag & 0xFF);
            }
            break;
            default:
            {
                /* do nothing. */
            }
        }

        /* encode the indefinite length. */
        p_tlv_object->p_tlv_object_encoded_buffer[p_tlv_object->tlv_tag_length] = (uint8_t)INDEFINITE_LENGTH;

        /* Setup tlv object as container. */
        p_tlv_object->b_tlv_container_object = TRUE;
        p_tlv_object->tlv_child_Count = 0;

        /* Zero length of the value. */
        p_tlv_object->tlv_max_object_value_length = 0;
        p_tlv_object->tlv_curr_object_value_length = 0;

        /* Null start of value buffer. */
        p_tlv_object->p_tlv_value_buffer = NULL;

        /* Initialize the child list to NULL. */
        p_tlv_object->p_tlv_child_tlv_object_list = NULL;
        p_tlv_object->p_child_tlv_object_next = NULL;
        p_tlv_object->p_child_tlv_object_previous = NULL;

        /* Final initialize the tag. Tlv object ready to use. */
        p_tlv_object->tlv_object_tag_number = tlv_tag;

        status = TLV_SUCCESS;
    }
    else
    {
        status = TLV_NO_MEMORY;
    }

    return status;
}

/* 
 * Create definite length application class TLV object.
 * This function can also create definite length context specific class TLV object.
 */
static uint32_t create_definite_length_application_tlv_object(tlv_object_t * p_tlv_object, uint32_t tlv_tag, const uint8_t * p_tlv_buffer, uint32_t buffer_length)
{
    TLV_STATUS status = TLV_FAIL;

    /* Allocate the TLV encoder buffer. */

    uint32_t tag_octets = 0;
    tag_octets = get_tag_octets(tlv_tag);

    uint32_t length_octets = 0;
    length_octets = get_length_octets(buffer_length);

    uint32_t tlv_encoded_buffer_length = tag_octets + length_octets + buffer_length;
    p_tlv_object->p_tlv_object_encoded_buffer = NULL;
    p_tlv_object->p_tlv_object_encoded_buffer = (uint8_t *)calloc(tlv_encoded_buffer_length, sizeof(uint8_t));

    if (p_tlv_object->p_tlv_object_encoded_buffer != NULL)
    {
        /* Initialize the tlv tag length. */
        p_tlv_object->tlv_tag_length = tag_octets;

        /* Initialize max the tLV encoded buffer length. */
        p_tlv_object->tlv_max_encoded_object_length = (uint32_t)tlv_encoded_buffer_length;

        /* Save max length of the value. */
        p_tlv_object->tlv_max_object_value_length = buffer_length;

        /* save max length (number of octets) of the encoded value length. */
        p_tlv_object->tlv_max_length_of_endcoded_value_length = length_octets;

        /* Add tag number and class. */

        if (tlv_tag < TAG_APP_CLS_MAX)
        {
            /* Add application constructed TLV object tag number and class. */
            p_tlv_object->p_tlv_object_encoded_buffer[0] = (uint8_t)TAG_APPL_CLS_CONSTRUCTED | (uint8_t)TAG_EXTENDED_BYTES;
        }
        else if (tlv_tag < TAG_CONT_SPCF_CLS_MAX)
        {
            /* Add context specific constructed TLV object tag number and class. */
            p_tlv_object->p_tlv_object_encoded_buffer[0] = (uint8_t)TAG_CS_CLS_CONSTRUCTED | (uint8_t)TAG_EXTENDED_BYTES;
        }
        else
        {
            /* do nothing */
            /* validity of the tag is checked in error check functions. */
        }

        /* Encode the tag */
        switch (p_tlv_object->tlv_tag_length)
        {
            case TAG_2OCTET:
            {
                /* encode the tag. */
                p_tlv_object->p_tlv_object_encoded_buffer[1] = (uint8_t)tlv_tag;
            }
            break;
            case TAG_3OCTET:
            {
                /* encode the tag. */
                p_tlv_object->p_tlv_object_encoded_buffer[1] = (uint8_t)((tlv_tag & 0xFF00) >> 8U);
                p_tlv_object->p_tlv_object_encoded_buffer[1] |= (uint8_t)(1U << SHIFT_TO_BIT_7);
                p_tlv_object->p_tlv_object_encoded_buffer[2] = (uint8_t)(tlv_tag & 0xFF);
            }
            break;
            default:
            {
                /* do nothing. */
            }
        }

        p_tlv_object->b_tlv_container_object = FALSE;
        p_tlv_object->tlv_child_Count = 0;

        /* Encode the integer value into TLV object. */
        add_data_to_definite_length_application_tlv_object(p_tlv_object, p_tlv_buffer, buffer_length);

        p_tlv_object->p_tlv_child_tlv_object_list = NULL;
        p_tlv_object->p_child_tlv_object_next = NULL;
        p_tlv_object->p_child_tlv_object_previous = NULL;

        p_tlv_object->tlv_object_tag_number = tlv_tag;

        /* Set status success. */
        status = TLV_SUCCESS;
    }
    else
    {
        status = TLV_NO_MEMORY;
    }

    return status;
}

/* Encode the definite length value into TLV object. */
void add_data_to_definite_length_application_tlv_object(tlv_object_t * p_tlv_object, const uint8_t * p_tlv_buffer, uint32_t buffer_length)
{
    uint32_t length_octets = 0;
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
            p_tlv_object->p_tlv_object_encoded_buffer[(p_tlv_object->tlv_tag_length + 1U)] = (uint8_t)((buffer_length & 0xFF00) >> 8U);
            p_tlv_object->p_tlv_object_encoded_buffer[(p_tlv_object->tlv_tag_length + 2U)] = (uint8_t)(buffer_length & 0xFF);
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
    p_tlv_object->tlv_curr_encoded_object_length = (uint32_t)(p_tlv_object->tlv_tag_length + length_octets + buffer_length);

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
            /* Stuff zeros in empty tlv buffer space. */
            p_tlv_object->p_tlv_value_buffer[i] = (uint8_t)0x0U;
        }
    }
}

