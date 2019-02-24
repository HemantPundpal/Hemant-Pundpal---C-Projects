/*
 * Name: tlv_app_data_ec.c
 *
 * Description:
 * All error check API definitions required for the app data layer to abstract tlv encoder and decoder.
 *
 * Author: Hemant Pundpal                                   Date: 24 Feb 2019
 *
 */
#define TLV_APP_DATA_SOURCE_CODE

#include "tlv_app_data_api.h"
#include "tlv_api.h"
#include "tlv_app_data.h"
#include "error_assert.h"

extern tlv_app_data_t * tag_to_app_data_map[TAG_MAX];
extern bool_t b_tlv_app_data_map_initialized;

static uint32_t check_tag(uint32_t tag_number, uint32_t app_data_size);

/* Error check for function to initialize and create tlv object of the app data with definite size. */
uint32_t tlv_init_and_create_app_data_ec(uint8_t * p_app_data_buffer, uint32_t app_data_size, uint32_t app_data_tag)
{
    /* Check if initialized before use. */
    assert(!b_tlv_app_data_map_initialized);
    if (!b_tlv_app_data_map_initialized)
    {
        return TLV_NOT_INITIALIZED;
    }

    /* Check if data pointer is valid. */
    assert(!p_app_data_buffer);
    if (!p_app_data_buffer)
    {
        return TLV_DATA_BUFFER_INVALID;
    }

    /* Check if the TLV tag is created already. */
    assert(tag_to_app_data_map[app_data_tag] != NULL);
    if (tag_to_app_data_map[app_data_tag] != NULL)
    {
        return TLV_TAG_CREATED;
    }

    /* Check TLV tag and constraints. */
    TLV_STATUS status = TLV_FAIL;
    status = check_tag(app_data_tag, app_data_size);
    if (status != TLV_SUCCESS)
    {
        return status;
    }

    status = tlv_init_and_create_app_data(p_app_data_buffer, app_data_size, app_data_tag);

    /* Return status. */
    return status;
}

/* Error check for function to create tlv object of the app data and add to container (parent) tlv object as a child. */
uint32_t tlv_create_container_app_data_ec(uint32_t container_app_data_tag)
{
    /* Check if initialized before use. */
    assert(!b_tlv_app_data_map_initialized);
    if (!b_tlv_app_data_map_initialized)
    {
        return TLV_NOT_INITIALIZED;
    }

    /* Check if the TLV tag is created already. */
    assert(tag_to_app_data_map[container_app_data_tag] != NULL);
    if (tag_to_app_data_map[container_app_data_tag] != NULL)
    {
        return TLV_TAG_CREATED;
    }

    /* Check TLV tag and constraints. */
    TLV_STATUS status = TLV_FAIL;
    status = check_tag(container_app_data_tag, 0);
    if (status != TLV_SUCCESS)
    {
        return status;
    }

    status = tlv_create_container_app_data(container_app_data_tag);

    /* Return status. */
    return status;
}

/* Error check for function to add created child tlv object to container (parent) tlv object. */
uint32_t tlv_add_child_to_container_app_data_ec(uint32_t container_app_data_tag, uint8_t * p_app_data_buffer, uint32_t app_data_size, uint32_t child_app_data_tag)
{
    /* Check if initialized before use. */
    assert(!b_tlv_app_data_map_initialized);
    if (!b_tlv_app_data_map_initialized)
    {
        return TLV_NOT_INITIALIZED;
    }

    /* Check if the TLV container tag is not created already. */
    assert(tag_to_app_data_map[container_app_data_tag] == NULL);
    if (tag_to_app_data_map[container_app_data_tag] == NULL)
    {
        return TLV_TAG_NOT_CREATED;
    }

    /* Check the tag is container type. */
    assert((tag_to_app_data_map[container_app_data_tag])->b_container == FALSE);
    if((tag_to_app_data_map[container_app_data_tag])->b_container == FALSE)
    {
        return TLV_NOT_A_CONTAINER;
    }

    /* Container can not have itself as child. */
    assert(container_app_data_tag == child_app_data_tag);
    if(container_app_data_tag == child_app_data_tag)
    {
        return TLV_CANNOT_CONTAIN_ITSELF;
    }

    /* Check if container has max number of child added already. */
    tlv_app_data_t * p_container_app_data = tag_to_app_data_map[container_app_data_tag];
    assert(p_container_app_data->u_size.child_count == MAX_CONTAINER_CHILD_COUNT);
    if (p_container_app_data->u_size.child_count == MAX_CONTAINER_CHILD_COUNT)
    {
        return TLV_MAX_CHILD_COUNT;
    }

    /* Check if data pointer is valid. */
    assert(!p_app_data_buffer);
    if (!p_app_data_buffer)
    {
        return TLV_DATA_BUFFER_INVALID;
    }

    /* Check TLV tag and constraints for child tag. */
    TLV_STATUS status = TLV_FAIL;
    status = check_tag(child_app_data_tag, app_data_size);
    if (status != TLV_SUCCESS)
    {
        return status;
    }

    status = tlv_add_child_to_container_app_data(container_app_data_tag, p_app_data_buffer, app_data_size, child_app_data_tag);

    /* Return status. */
    return status;
}

/* Error Check for function to add child app data tag to container (parent) app data. */
uint32_t tlv_add_child_tag_to_container_app_data_ec(uint32_t container_app_data_tag, uint32_t child_app_data_tag)
{
    /* Check if initialized before use. */
    assert(!b_tlv_app_data_map_initialized);
    if (!b_tlv_app_data_map_initialized)
    {
        return TLV_NOT_INITIALIZED;
    }

    /* Check if the TLV container tag is not created already. */
    assert(tag_to_app_data_map[container_app_data_tag] == NULL);
    if (tag_to_app_data_map[container_app_data_tag] == NULL)
    {
        return TLV_TAG_NOT_CREATED;
    }

    /* Check the tag is container type. */
    assert((tag_to_app_data_map[container_app_data_tag])->b_container == FALSE);
    if((tag_to_app_data_map[container_app_data_tag])->b_container == FALSE)
    {
        return TLV_NOT_A_CONTAINER;
    }

    /* Check if the TLV child tag is not created already. */
    assert(tag_to_app_data_map[child_app_data_tag] == NULL);
    if (tag_to_app_data_map[child_app_data_tag] == NULL)
    {
        return TLV_TAG_NOT_CREATED;
    }

    /* Container can not have itself as child. */
    assert(container_app_data_tag == child_app_data_tag);
    if(container_app_data_tag == child_app_data_tag)
    {
        return TLV_CANNOT_CONTAIN_ITSELF;
    }

    TLV_STATUS status = TLV_FAIL;
    status = tlv_add_child_tag_to_container_app_data(container_app_data_tag, child_app_data_tag);

    /* Return status. */
    return status;
}

/* Error check for function to add data to tlv objects with definite size. */
uint32_t tlv_add_data_to_app_data_ec(uint32_t app_data_tag)
{
    /* Check if initialized before use. */
    assert(!b_tlv_app_data_map_initialized);
    if (!b_tlv_app_data_map_initialized)
    {
        return TLV_NOT_INITIALIZED;
    }

    /* Check if the TLV tag is not created already or is of container type. */
    assert((tag_to_app_data_map[app_data_tag] == NULL) && ((tag_to_app_data_map[app_data_tag])->b_container == FALSE));
    if ((tag_to_app_data_map[app_data_tag] == NULL) && ((tag_to_app_data_map[app_data_tag])->b_container == FALSE))
    {
        return TLV_CANNOT_WRITE_VALUE;
    }

    TLV_STATUS status = TLV_FAIL;
    status = tlv_add_data_to_app_data(app_data_tag);

    /* Return status. */
    return status;
}

/* Error check for function to add data to constructed container tlv objects with indefinite size. */
uint32_t tlv_add_data_to_container_app_data_ec(uint32_t container_app_data_tag, uint32_t child_app_data_tag)
{
    /* Check if initialized before use. */
    assert(!b_tlv_app_data_map_initialized);
    if (!b_tlv_app_data_map_initialized)
    {
        return TLV_NOT_INITIALIZED;
    }

    /* Check if the TLV tag is not created already or its not setup as container. */
    assert((tag_to_app_data_map[container_app_data_tag] == NULL) || ((tag_to_app_data_map[container_app_data_tag])->b_container != TRUE));
    if ((tag_to_app_data_map[container_app_data_tag] == NULL) || ((tag_to_app_data_map[container_app_data_tag])->b_container != TRUE))
    {
        return TLV_TAG_NOT_CREATED;
    }

    TLV_STATUS status = TLV_FAIL;
    status = tlv_add_data_to_container_app_data(container_app_data_tag, child_app_data_tag);

    /* Return status. */
    return status;
}

/* Error check for function to send application data. */
uint32_t tlv_app_data_send_ec(uint32_t tag)
{
    /* Check if initialized before use. */
    assert(!b_tlv_app_data_map_initialized);
    if (!b_tlv_app_data_map_initialized)
    {
        return TLV_NOT_INITIALIZED;
    }

    /* Check if the TLV tag is not created already. */
    assert(tag_to_app_data_map[tag] == NULL);
    if (tag_to_app_data_map[tag] == NULL)
    {
        return TLV_TAG_NOT_CREATED;
    }

    TLV_STATUS status = TLV_FAIL;
    status = tlv_app_data_send(tag);

    /* Return status. */
    return status;
}

/* Error check for function to parse app data from TLV data buffer (first found tlv object is parsed.) */
uint32_t tlv_parse_app_data_ec(const uint8_t * p_tlv_data_buffer, uint32_t buffer_length, uint32_t * p_parsed_tag)
{
    /* Check if the tlv data buffer pointer is valid. */
    assert(!p_tlv_data_buffer);
    if(!p_tlv_data_buffer)
    {
        return TLV_DATA_BUFFER_INVALID;
    }

    /* Check if the tlv data buffer length is not zero. */
    assert(!buffer_length);
    if(!buffer_length)
    {
        return TLV_BAD_BUFFER_LENGTH;
    }

    /* Make sure parsed tag is valid pointer. */
    assert(!p_parsed_tag);
    if(!p_parsed_tag)
    {
        return TLV_TAG_PTR_INVALID;
    }

    TLV_STATUS status = TLV_FAIL;
    status = tlv_parse_app_data(p_tlv_data_buffer, buffer_length, p_parsed_tag);

    /* Return status. */
    return status;
}

/* Error check for function to search tag and parse the tlv data buffer (if recursive set to true, then search for child).*/
uint32_t tlv_search_parse_app_data_ec(const uint8_t * p_tlv_data_buffer, uint32_t buffer_length, uint32_t search_parse_tag, bool_t b_recersive)
{
    TLV_STATUS status = TLV_FAIL;

    return status;
}

/* Error check for function to delete a tlv object. */
uint32_t tlv_delete_app_data_ec(uint32_t tag)
{
    TLV_STATUS status = TLV_FAIL;

    return status;
}

/* 
 * Internal function - Error check tag number.
 * Note: currently only few tags are supported thus this tag check function is sufficient.
 */
static uint32_t check_tag(uint32_t tag_number, uint32_t app_data_size)
{
    /* Check any constraints that application put on the tags. */
    switch (tag_number)
    {
        case TAG_INTEGER:
        {
            assert(!app_data_size || app_data_size > MAX_INTEGER_BYTES);
            if (!app_data_size || app_data_size > MAX_INTEGER_BYTES)
            {
                return TLV_BAD_DATA_SIZE;
            }
        }
        break;
        case TAG_UTF8STRING:
        {
            assert(!app_data_size || app_data_size > MAX_UTF8STRING_LENGTH);
            if (!app_data_size || app_data_size > MAX_UTF8STRING_LENGTH)
            {
                return TLV_BAD_DATA_SIZE;
            }
        }
        break;
        case TAG_APP_TXN_INFO:
        {
            /* do nothing */
        }
        break;
        case TAG_APP_ADDNL_TXN_INFO:
        {
            /* do nothing */
        }
        break;
        case TAG_CONT_SPCF_UINT_16:
        {
            /* do nothing */
        }
        break;
        case TAG_CONT_SPCF_UINT_8:
        {
            /* do nothing */
        }
        break;
        default:
        {
            /* tag not supported. */
            uint8_t tag_invalid = 0;
            assert(!tag_invalid);
            return TLV_TAG_NOT_SUPPORTED;
        }
    }

    return TLV_SUCCESS;
}
