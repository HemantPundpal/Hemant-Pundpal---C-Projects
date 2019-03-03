/*
 * Name: tlv_app_data_parser.c
 *
 * Description:
 * All API definitions required for the app data layer to abstract parsing (decoding) of TLV encoded objects.
 *
 * Author: Hemant Pundpal                                   Date: 03 Mar 2019
 *
 */
#define TLV_APP_DATA_SOURCE_CODE

#include "tlv_app_data_api.h"
#include "tlv_api.h"
#include "tlv_app_data.h"
#include "tlv_app_data_tag_check.h"

extern tlv_app_data_t * tag_to_app_data_map[TAG_MAX];

/* App data constraints verification for the parsed TLV object. */
static uint32_t check_parsed_tlv_object_app_data(tlv_object_t * p_tlv_parsed_object, tlv_app_data_t ** p_tlv_app_data);

/* Copy the encoded TLV buffer from the parsed TLV object to the app data mapped of the TLV object */
static void_t copy_tlv_encoded_buffer(tlv_object_t * p_tlv_parsed_object, tlv_app_data_t * p_tlv_app_data);

/* update app data from the decoded TLV object. */
static void_t update_app_data(tlv_app_data_t * p_tlv_app_data);

/* Decode and update app data integer value. */
static void_t update_app_data_integer(tlv_app_data_t * p_tlv_app_data);

/* Generic update app data with TLV value. */
static void_t update_app_data_generic(tlv_app_data_t * p_tlv_app_data);

/* Function to parse app data from TLV data buffer (first found TLV object is parsed.) */
uint32_t tlv_parse_app_data(const uint8_t * p_tlv_data_buffer, uint32_t buffer_length, uint32_t * p_parsed_tag)
{
    TLV_STATUS status = TLV_FAIL;
    tlv_object_t * tlv_parsed_object = NULL;

    /* allocated memory for the TLV object. */
    tlv_parsed_object = (tlv_object_t *)calloc(1U, sizeof(tlv_object_t));
    if (tlv_parsed_object)
    {
        /* Parse the first tag in the buffer. */
        status = parse_tlv_object(p_tlv_data_buffer, buffer_length, tlv_parsed_object);
        if (status == TLV_SUCCESS)
        {
            tlv_app_data_t * p_tlv_app_data = NULL;
            status = check_parsed_tlv_object_app_data(tlv_parsed_object, &p_tlv_app_data);

            if (status == TLV_SUCCESS)
            {
                /* copy the encoded buffer from TLV object to app data. */
                copy_tlv_encoded_buffer(tlv_parsed_object, p_tlv_app_data);

                /* update app data from the decoded TLV object. */
                update_app_data(p_tlv_app_data);

                /* Return the found tag. */
                *p_parsed_tag = p_tlv_app_data->tag_number;
            }
            /* Free the TLV buffer. */
            free(tlv_parsed_object->p_tlv_object_encoded_buffer);
        }
        /* Free the TLV object. */
        free(tlv_parsed_object);
    }


    /* Return status. */
    return status;
}

/* Function to search tag and parse the TLV data buffer (if recursive set to true, then search for child app data).*/
uint32_t tlv_search_parse_app_data(const uint8_t * p_tlv_data_buffer, uint32_t buffer_length, uint32_t search_parse_tag, bool_t b_recursive)
{
    TLV_STATUS status = TLV_FAIL;
    tlv_app_data_t * p_tlv_app_data = NULL;

    p_tlv_app_data = tag_to_app_data_map[search_parse_tag];

    status = tlv_search_tag(p_tlv_data_buffer, buffer_length, search_parse_tag, b_recursive, p_tlv_app_data->p_tlv_object);

    if (TLV_SUCCESS == status)
    {
        /* update app data from the decoded TLV object. */
        update_app_data(p_tlv_app_data);
    }

    return status;
}

/* App data constraints verification for the parsed TLV object. */
static uint32_t check_parsed_tlv_object_app_data(tlv_object_t * p_tlv_parsed_object, tlv_app_data_t ** p_tlv_app_data)
{
    TLV_STATUS status = TLV_FAIL;

    /* check tag and associated constraints */
    status = check_tag(p_tlv_parsed_object->tlv_object_tag_number, p_tlv_parsed_object->tlv_curr_object_value_length);

    if (status == TLV_SUCCESS)
    {
        *p_tlv_app_data = NULL;
        *p_tlv_app_data = tag_to_app_data_map[p_tlv_parsed_object->tlv_object_tag_number];

        if ((*p_tlv_app_data == NULL) && (p_tlv_parsed_object->tlv_object_tag_number == TAG_INTEGER))
        {
            *p_tlv_app_data = tag_to_app_data_map[TAG_INTEGER_UNSIGNED];
        }

        if (*p_tlv_app_data == NULL)
        {
            /* No tag found. May be not created yet. */
            status = TLV_NO_TAG_FOUND;
        }
        else
        {
            if ((!((p_tlv_parsed_object->b_tlv_container_object == TRUE) && ((*p_tlv_app_data)->b_container == TRUE))) &&
                (!((p_tlv_parsed_object->b_tlv_container_object == FALSE) && ((*p_tlv_app_data)->b_container == FALSE))))
            {
                status = TLV_NO_TAG_FOUND;
            }
        }
    }

    /* Return status. */
    return status;
}

/* Copy the encoded TLV buffer from the parsed TLV object to the app data mapped of the TLV object */
static void_t copy_tlv_encoded_buffer(tlv_object_t * p_tlv_parsed_object, tlv_app_data_t * p_tlv_app_data)
{
    /* Copy TLV encoded buffer */
    (p_tlv_app_data->p_tlv_object)->tlv_curr_encoded_object_length = p_tlv_parsed_object->tlv_curr_encoded_object_length;
    (p_tlv_app_data->p_tlv_object)->tlv_curr_object_value_length = p_tlv_parsed_object->tlv_curr_object_value_length;

    for (uint32_t i = 0; i < (p_tlv_app_data->p_tlv_object)->tlv_max_encoded_object_length; i++)
    {
        if (i < p_tlv_parsed_object->tlv_curr_encoded_object_length)
        {
            (p_tlv_app_data->p_tlv_object)->p_tlv_object_encoded_buffer[i] = p_tlv_parsed_object->p_tlv_object_encoded_buffer[i];
        }
        else
        {
            /* stuff zeros in the TLV encoded buffer. */
            (p_tlv_app_data->p_tlv_object)->p_tlv_object_encoded_buffer[i] = (uint8_t)0x0U;
        }
    }
    if (p_tlv_parsed_object->b_tlv_container_object == FALSE)
    {
        /* store start of the TLV value buffer. */
        uint32_t value_index = ((p_tlv_app_data->p_tlv_object)->tlv_curr_encoded_object_length
            - (p_tlv_app_data->p_tlv_object)->tlv_curr_object_value_length);
        (p_tlv_app_data->p_tlv_object)->p_tlv_value_buffer = &((p_tlv_app_data->p_tlv_object)->p_tlv_object_encoded_buffer[value_index]);
    }
    else
    {
        (p_tlv_app_data->p_tlv_object)->p_tlv_value_buffer = NULL;
    }
}

/* update app data based on decoded TLV object. */
static void_t update_app_data(tlv_app_data_t * p_tlv_app_data)
{
    /* Check if it is a container. */
    if (p_tlv_app_data->b_container == FALSE)
    {
        switch (p_tlv_app_data->tag_number)
        {
            case TAG_INTEGER:
            {
                update_app_data_integer(p_tlv_app_data);
            }
            break;
            case TAG_INTEGER_UNSIGNED:
            {
                update_app_data_generic(p_tlv_app_data);
            }
            break;
            default:
            {
                /* For rest just copy all the value octets in the app data. */
                update_app_data_generic(p_tlv_app_data);
            }
        }
    }
    else
    {
        for (uint32_t i = 0; i < p_tlv_app_data->u_size.child_count; i++)
        {
            /* This is a recursive call, please take care of depth and when modifying. */
            update_app_data(p_tlv_app_data->p_child_app_data[i]);
        }
    }

}

/* Decode and update app data integer value. */
static void_t update_app_data_integer(tlv_app_data_t * p_tlv_app_data)
{
    /* Check the size bit and copy the integer value. */
    int32_t index = (int32_t)((p_tlv_app_data->p_tlv_object)->tlv_curr_object_value_length - 1U);
    uint64_t temp_value = 0U;
    uint64_t signed_value = 0U;
    for (; index >= 0; index--)
    {
        if (index == (int32_t)((p_tlv_app_data->p_tlv_object)->tlv_curr_object_value_length - 1U))
        {
            temp_value |= (uint64_t)((p_tlv_app_data->p_tlv_object)->p_tlv_value_buffer[index] & UNSIGNED_VALUE_FILTER_8BIT);
            signed_value |= (uint64_t)((p_tlv_app_data->p_tlv_object)->p_tlv_value_buffer[index] & SIGNED_FILTER_8BIT);
        }
        else
        {
            temp_value = (temp_value << SHIFT_8BIT);
            signed_value = (signed_value << SHIFT_8BIT);
            temp_value |= (uint32_t)(p_tlv_app_data->p_tlv_object)->p_tlv_value_buffer[index];
        }

    }

    /* get the decoded integer value. */
    int64_t decoded_value = 0;
    if (signed_value)
    {
        decoded_value = (int32_t)(temp_value - signed_value);
    }
    else
    {
        decoded_value = (int32_t)temp_value;
    }

    /* Copy the decoded value */
    for (uint32_t i = 0; i < p_tlv_app_data->u_size.data_size; i++)
    {
        p_tlv_app_data->p_app_data[i] = (uint8_t)(decoded_value & MASK_BIT_0_TO_7);
        decoded_value = (decoded_value >> SHIFT_8BIT);
    }
}

/* generic update app data with TLV value. */
static void_t update_app_data_generic(tlv_app_data_t * p_tlv_app_data)
{
    if (p_tlv_app_data->b_container == FALSE)
    {
        for (uint32_t i = 0; i < p_tlv_app_data->u_size.data_size; i++)
        {
            if (i < (p_tlv_app_data->p_tlv_object)->tlv_curr_object_value_length)
            {
                p_tlv_app_data->p_app_data[i] = (p_tlv_app_data->p_tlv_object)->p_tlv_value_buffer[i];
            }
            else
            {
                p_tlv_app_data->p_app_data[i] = 0x0U;
            }
        }
    }
}
