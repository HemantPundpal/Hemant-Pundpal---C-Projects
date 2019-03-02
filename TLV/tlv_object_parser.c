/*
 * Name: tlv_object_parser.c
 *
 * Description:
 * All API definitions required for the TLV application and universal tags decoder.
 *
 * Author: Hemant Pundpal                                   Date: 27 Feb 2019
 *
 */

#define TLV_APP_DATA_SOURCE_CODE

#include "tlv_api.h"
#include "tlv_application_class.h"
#include "tlv_definition.h"

/* Parse application TLV object's tag. */
static uint32_t parse_tlv_object_tag(const uint8_t * p_tlv_data_buffer, uint32_t buffer_length, tlv_object_t * p_tlv_object);

/* Parse application TLV object's lenght. */
static uint32_t parse_tlv_object_lenght(const uint8_t * p_tlv_data_buffer, uint32_t buffer_length, tlv_object_t * p_tlv_object);

/* Get parsed primitive TLV object for the found tag. */
static uint32_t get_parsed_tlv_object_primitive(const uint8_t * p_tlv_data_buffer, uint32_t buffer_length, tlv_object_t * p_tlv_object);

/* Get parsed primitive TLV object for the found tag. */
static uint32_t get_parsed_tlv_object_constructed(const uint8_t * p_tlv_data_buffer, uint32_t buffer_length, tlv_object_t * p_tlv_object);

/* Get parsed TLV object for the found tag. */
uint32_t get_parsed_tlv_object(const uint8_t * p_tlv_data_buffer, uint32_t buffer_length, tlv_object_t * p_tlv_object)
{
    TLV_STATUS status = TLV_FAIL;

    uint8_t tag_class = (uint8_t)(*p_tlv_data_buffer & (uint8_t)TLV_TAG_CLASS_FILTER);
    switch (tag_class)
    {
        case TAG_UNIVERSAL_PRIMITIVE:
        {
            /* Get the parsed primitive TLV object. */
            status = get_parsed_tlv_object_primitive(p_tlv_data_buffer, buffer_length, p_tlv_object);
        }
        break;
        case TAG_UNIVERSAL_CONSTRUCTED:
        {
            /* Get the parsed constructed TLV object. */
            status = get_parsed_tlv_object_constructed(p_tlv_data_buffer, buffer_length, p_tlv_object);
        }
        break;
        case TAG_APPL_CLS_PRIMITIVE:
        {
            /* Get the parsed primitive TLV object. */
            status = get_parsed_tlv_object_primitive(p_tlv_data_buffer, buffer_length, p_tlv_object);
        }
        break;
        case TAG_APPL_CLS_CONSTRUCTED:
        {
            /* Get the parsed constructed TLV object. */
            status = get_parsed_tlv_object_constructed(p_tlv_data_buffer, buffer_length, p_tlv_object);
        }
        break;
        case TAG_CS_CLS_PRIMITIVE:
        {
            /* Get the parsed primitive TLV object. */
            status = get_parsed_tlv_object_primitive(p_tlv_data_buffer, buffer_length, p_tlv_object);
        }
        break;
        case TAG_CS_CLS_CONSTRUCTED:
        {
            /* Get the parsed constructed TLV object. */
            status = get_parsed_tlv_object_constructed(p_tlv_data_buffer, buffer_length, p_tlv_object);
        }
        break;
        case TAG_PRIVATE_CLS_PRIMITIVE:
        {
            /* not supported. */
        }
        break;
        case TAG_PRIVATE_CLS_CONSTRUCTED:
        {
            /* not supported. */
        }
        break;
        default:
        {
            /* Do nothing. */
        }
        break;
    }

    /* Return status. */
    return status;
}

/* Get parsed primitive TLV object for the found tag. */
static uint32_t get_parsed_tlv_object_primitive(const uint8_t * p_tlv_data_buffer, uint32_t buffer_length, tlv_object_t * p_tlv_object)
{
    TLV_STATUS status = TLV_FAIL;

    /* First get the tag number. */
    status = parse_tlv_object_tag(p_tlv_data_buffer, buffer_length, p_tlv_object);

    if (TLV_SUCCESS == status)
    {
        /* Now get the length. */
        status = parse_tlv_object_lenght(p_tlv_data_buffer, buffer_length, p_tlv_object);
    }

    if (TLV_SUCCESS == status)
    {
        /* check is the primitive tlv object constraints are met. */
        if ((p_tlv_object->b_tlv_container_object == TRUE) || (p_tlv_object->b_tlv_object_length_definite == FALSE))
        {
            status = TLV_BAD_TAG;
        }
        else
        {
            status = TLV_SUCCESS;
        }
    }

    /* Return status. */
    return status;
}

/* Get parsed primitive TLV object for the found tag. */
static uint32_t get_parsed_tlv_object_constructed(const uint8_t * p_tlv_data_buffer, uint32_t buffer_length, tlv_object_t * p_tlv_object)
{
    TLV_STATUS status = TLV_FAIL;

    /* First get the tag number. */
    status = parse_tlv_object_tag(p_tlv_data_buffer, buffer_length, p_tlv_object);

    if (TLV_SUCCESS == status)
    {
        /* Now get the length. */
        status = parse_tlv_object_lenght(p_tlv_data_buffer, buffer_length, p_tlv_object);
    }

    /* Return status. */
    return status;
}

/* Update parsed TLV object. */
uint32_t update_parsed_tlv_object(const uint8_t * p_tlv_data_buffer, uint32_t buffer_length, tlv_object_t * p_tlv_object)
{
    TLV_STATUS status = TLV_FAIL;

    /* Update TLV encoded buffer in the TLV object. */
    if (p_tlv_object->tlv_curr_encoded_object_length <= buffer_length)
    {
        /* Allocate memory for the TLV encoded buffer. */
        p_tlv_object->p_tlv_object_encoded_buffer = NULL;
        p_tlv_object->p_tlv_object_encoded_buffer = (uint8_t*)calloc(p_tlv_object->tlv_curr_encoded_object_length, sizeof(uint8_t));
        if (p_tlv_object->p_tlv_object_encoded_buffer)
        {
            /* If successful copy the TLV encoded buffer. */
            for (uint32_t i = 0; i < p_tlv_object->tlv_curr_encoded_object_length; i++)
            {
                p_tlv_object->p_tlv_object_encoded_buffer[i] = p_tlv_data_buffer[i];
            }
            /* store the start of value buffer. */
            uint32_t value_index = ((p_tlv_object->tlv_curr_encoded_object_length) - (p_tlv_object->tlv_curr_object_value_length));
            p_tlv_object->p_tlv_value_buffer = &(p_tlv_object->p_tlv_object_encoded_buffer[value_index]);

            status = TLV_SUCCESS;
        }
        else
        {
            p_tlv_object->tlv_tag_length = 0U;
            p_tlv_object->tlv_object_tag_number = 0;
            p_tlv_object->tlv_curr_object_value_length = 0U;
            p_tlv_object->tlv_curr_encoded_object_length = 0U;
            p_tlv_object->b_tlv_container_object = FALSE;
            p_tlv_object->b_tlv_object_length_definite = FALSE;

            /* Could not allocate memory. */
            status = TLV_NO_MEMORY;
        }
    }
    else
    {
        p_tlv_object->tlv_tag_length = 0U;
        p_tlv_object->tlv_object_tag_number = 0;
        p_tlv_object->tlv_curr_object_value_length = 0U;
        p_tlv_object->tlv_curr_encoded_object_length = 0U;
        p_tlv_object->b_tlv_container_object = FALSE;
        p_tlv_object->b_tlv_object_length_definite = FALSE;

        status = TLV_BAD_BUFFER_LENGTH;
    }

    /* Initialize the attributes of the TLV object. */
    p_tlv_object->p_child_tlv_object_next = NULL;
    p_tlv_object->p_child_tlv_object_previous = NULL;
    p_tlv_object->p_tlv_child_tlv_object_list = NULL;
    p_tlv_object->tlv_child_Count = 0;
    p_tlv_object->tlv_max_encoded_object_length = 0;
    p_tlv_object->tlv_max_length_of_endcoded_value_length = 0;
    p_tlv_object->tlv_max_object_value_length = 0;

    /* Return status. */
    return status;
}

/* Parse application TLV object's tag. */
static uint32_t parse_tlv_object_tag(const uint8_t * p_tlv_data_buffer, uint32_t buffer_length, tlv_object_t * p_tlv_object)
{
    TLV_STATUS status = TLV_FAIL;

    p_tlv_object->tlv_tag_length = 0U;
    p_tlv_object->tlv_object_tag_number = 0U;
    if ((((p_tlv_data_buffer[0] & (uint8_t)TLV_TAG_NUMBER_FILTER) <= (uint8_t)TAG_MAX_UNIVERSAL)) &&
        (buffer_length >= TAG_1OCTET))
    {
        p_tlv_object->tlv_tag_length = TAG_1OCTET;
        p_tlv_object->tlv_object_tag_number = (uint32_t)(p_tlv_data_buffer[0] & TLV_TAG_NUMBER_FILTER);
    }
    else if ((((p_tlv_data_buffer[0] & (uint8_t)TLV_TAG_NUMBER_FILTER) == (uint8_t)ONE_OCTET_TAG_MAX)) &&
        (buffer_length >= TAG_2OCTET))
    {
        if (!(p_tlv_data_buffer[1] & (uint8_t)EXTENDED_TAG))
        {
            p_tlv_object->tlv_tag_length = (uint32_t)TAG_2OCTET;
            p_tlv_object->tlv_object_tag_number = (uint32_t)(p_tlv_data_buffer[1] & EXTENDED_TAG_FILTER);
        }
        else
        {
            if (buffer_length >= TAG_3OCTET)
            {
                p_tlv_object->tlv_tag_length = (uint32_t)TAG_3OCTET;
                p_tlv_object->tlv_object_tag_number = (uint32_t)(p_tlv_data_buffer[1] & EXTENDED_TAG_FILTER);
                p_tlv_object->tlv_object_tag_number = p_tlv_object->tlv_object_tag_number << 7U;
                p_tlv_object->tlv_object_tag_number |= (uint32_t)(p_tlv_data_buffer[2] & EXTENDED_TAG_FILTER);
            }
            else
            {
                /* Tag number is not correct for application TLV object. */
                status = TLV_BAD_TAG;
            }
        }
    }
    else
    {
        /* Tag number is not correct for application TLV object. */
        status = TLV_BAD_TAG;
    }

    if (p_tlv_object->tlv_object_tag_number != 0x0 && p_tlv_object->tlv_tag_length != 0x0)
    {
        status = TLV_SUCCESS;
    }

    /* Return status. */
    return status;
}

/* Parse application TLV object's length. */
static uint32_t parse_tlv_object_lenght(const uint8_t * p_tlv_data_buffer, uint32_t buffer_length, tlv_object_t * p_tlv_object)
{
    TLV_STATUS status = TLV_FAIL;
    uint32_t length_octets = 0U;

    p_tlv_object->tlv_curr_object_value_length = 0U;
    p_tlv_object->tlv_curr_encoded_object_length = 0U;
    if ((buffer_length >= (p_tlv_object->tlv_tag_length + VALUE_LENGTH_1OCTET)))
    {
        /* Get the length of the TLV value. */
        if (!(p_tlv_data_buffer[p_tlv_object->tlv_tag_length] & LONG_LENGTH_FORM))
        {
            p_tlv_object->tlv_curr_object_value_length = (uint32_t)p_tlv_data_buffer[p_tlv_object->tlv_tag_length];
            p_tlv_object->b_tlv_container_object = FALSE;
            p_tlv_object->b_tlv_object_length_definite = TRUE;
            /* Length decoded. */
            status = TLV_SUCCESS;
        }
        else
        {
            /* Get number of length octets. */
            length_octets = (uint32_t)(p_tlv_data_buffer[p_tlv_object->tlv_tag_length] & LENGTH_FILTER);
            if (buffer_length >= (p_tlv_object->tlv_tag_length + VALUE_LENGTH_1OCTET + length_octets))
            {
                if (length_octets > VALUE_LENGTH_2OCTET)
                {
                    /* Bad buffer length. */
                    status = TLV_BAD_BUFFER_LENGTH;
                }
                else if (!length_octets)
                {
                    p_tlv_object->tlv_curr_object_value_length = 0;
                    p_tlv_object->b_tlv_container_object = TRUE;
                    p_tlv_object->b_tlv_object_length_definite = FALSE;

                    /* Length decoded. */
                    status = TLV_SUCCESS;
                }
                else
                {
                    /* Definite length. */
                    for (uint32_t i = 1; i <= length_octets; i++)
                    {
                        p_tlv_object->tlv_curr_object_value_length |= (uint32_t)p_tlv_data_buffer[(p_tlv_object->tlv_tag_length + i)];

                        if (i < length_octets)
                        {
                            p_tlv_object->tlv_curr_object_value_length = ((p_tlv_object->tlv_curr_object_value_length << 8U) & 0xFF00U);
                        }
                    }
                    p_tlv_object->b_tlv_container_object = FALSE;
                    p_tlv_object->b_tlv_object_length_definite = TRUE;

                    /* Length decoded. */
                    status = TLV_SUCCESS;
                }
            }
            else
            {
                /* Bad buffer length. */
                status = TLV_BAD_BUFFER_LENGTH;
            }
        }
    }
    if (status == TLV_SUCCESS)
    {
        p_tlv_object->tlv_curr_encoded_object_length = (uint32_t)(p_tlv_object->tlv_tag_length + VALUE_LENGTH_1OCTET + length_octets + p_tlv_object->tlv_curr_object_value_length);
    }

    /* Return status. */
    return status;
}
