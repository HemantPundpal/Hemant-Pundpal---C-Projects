/*
 * Name: tlv_app_data_ec.c
 *
 * Description:
 * All error check API definitions required for the app data layer to abstract TLV encoder and decoder.
 *
 * Author: Hemant Pundpal                                   Date: 04 Mar 2019
 *
 */
#define TLV_APP_DATA_SOURCE_CODE

#include "tlv_app_data_api.h"
#include "tlv_api.h"
#include "tlv_app_data.h"
#include "tlv_app_data_tag_check.h"
#include "error_assert.h"

extern tlv_app_data_t * tag_to_app_data_map[TAG_MAX];
extern bool_t b_tlv_app_data_map_initialized;

/* Error check for function to initialize and create TLV object of the app data with definite size. */
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
    if (TLV_SUCCESS == status)
    {
        status = tlv_init_and_create_app_data(p_app_data_buffer, app_data_size, app_data_tag);
    }

    /* Return status. */
    return status;
}

/* Error check for function to create TLV object of the app data and add to container (parent) TLV object as a child. */
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
    if (TLV_SUCCESS == status)
    {
        status = tlv_create_container_app_data(container_app_data_tag);
    }

    /* Return status. */
    return status;
}

/* Error check for function to add created child TLV object to container (parent) TLV object. */
uint32_t tlv_add_child_to_container_app_data_ec(uint32_t container_app_data_tag, uint8_t * p_app_data_buffer, uint32_t app_data_size, uint32_t child_app_data_tag)
{
    /* Check if initialized before use. */
    assert(!b_tlv_app_data_map_initialized);
    if (!b_tlv_app_data_map_initialized)
    {
        return TLV_NOT_INITIALIZED;
    }

    /* Check if the TLV container tag is not created already. */
    assert(NULL == tag_to_app_data_map[container_app_data_tag]);
    if (NULL == tag_to_app_data_map[container_app_data_tag])
    {
        return TLV_TAG_NOT_CREATED;
    }

    /* Check the tag is container type. */
    assert(FALSE == (tag_to_app_data_map[container_app_data_tag])->b_container);
    if(FALSE == (tag_to_app_data_map[container_app_data_tag])->b_container)
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
    assert(MAX_CONTAINER_CHILD_COUNT == p_container_app_data->u_size.child_count);
    if (MAX_CONTAINER_CHILD_COUNT == p_container_app_data->u_size.child_count)
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
    if (TLV_SUCCESS == status)
    {
        status = tlv_add_child_to_container_app_data(container_app_data_tag, p_app_data_buffer, app_data_size, child_app_data_tag);
    }

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
    assert(NULL == tag_to_app_data_map[container_app_data_tag]);
    if (NULL == tag_to_app_data_map[container_app_data_tag])
    {
        return TLV_TAG_NOT_CREATED;
    }

    /* Check the tag is container type. */
    assert(FALSE == (tag_to_app_data_map[container_app_data_tag])->b_container);
    if(FALSE == (tag_to_app_data_map[container_app_data_tag])->b_container)
    {
        return TLV_NOT_A_CONTAINER;
    }

    /* Check if the TLV child tag is not created already. */
    assert(NULL == tag_to_app_data_map[child_app_data_tag]);
    if (NULL == tag_to_app_data_map[child_app_data_tag])
    {
        return TLV_TAG_NOT_CREATED;
    }

    /* Check is child already belongs to a container. */
    assert(TRUE == (tag_to_app_data_map[child_app_data_tag])->b_has_parent);
    if (TRUE == (tag_to_app_data_map[child_app_data_tag])->b_has_parent)
    {
        return TLV_CHILD_HAS_PARENT;
    }

    /* Container (parent) cannot be added to its child. */
    if (TRUE == (tag_to_app_data_map[container_app_data_tag])->b_has_parent)
    {
        assert(((tag_to_app_data_map[container_app_data_tag])->p_parent_app_data)->tag_number == child_app_data_tag);
        if (((tag_to_app_data_map[container_app_data_tag])->p_parent_app_data)->tag_number == child_app_data_tag)
        {
            return TLV_PARENT_AS_CHILD;
        }
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

/* Error check for function to add data to TLV objects with definite size. */
uint32_t tlv_add_data_to_app_data_ec(uint32_t app_data_tag)
{
    /* Check if initialized before use. */
    assert(!b_tlv_app_data_map_initialized);
    if (!b_tlv_app_data_map_initialized)
    {
        return TLV_NOT_INITIALIZED;
    }

    /* Check if the TLV tag is not created already or is of container type. */
    assert((NULL == tag_to_app_data_map[app_data_tag]) && (FALSE == (tag_to_app_data_map[app_data_tag])->b_container));
    if ((NULL == tag_to_app_data_map[app_data_tag]) && (FALSE == (tag_to_app_data_map[app_data_tag])->b_container))
    {
        return TLV_CANNOT_WRITE_VALUE;
    }

    TLV_STATUS status = TLV_FAIL;
    status = tlv_add_data_to_app_data(app_data_tag);

    /* Return status. */
    return status;
}

/* Error check for function to add data to constructed container TLV objects with indefinite size. */
uint32_t tlv_add_data_to_container_app_data_ec(uint32_t container_app_data_tag, uint32_t child_app_data_tag)
{
    /* Check if initialized before use. */
    assert(!b_tlv_app_data_map_initialized);
    if (!b_tlv_app_data_map_initialized)
    {
        return TLV_NOT_INITIALIZED;
    }

    /* Check if the TLV tag is not created already or its not setup as container. */
    assert((NULL == tag_to_app_data_map[container_app_data_tag]) || (FALSE == (tag_to_app_data_map[container_app_data_tag])->b_container));
    if ((NULL == tag_to_app_data_map[container_app_data_tag]) || (FALSE == (tag_to_app_data_map[container_app_data_tag])->b_container))
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
    assert(NULL == tag_to_app_data_map[tag]);
    if (NULL == tag_to_app_data_map[tag])
    {
        return TLV_TAG_NOT_CREATED;
    }

    TLV_STATUS status = TLV_FAIL;
    status = tlv_app_data_send(tag);

    /* Return status. */
    return status;
}

/* Error check for function to parse app data from TLV data buffer (first found TLV object is parsed.) */
uint32_t tlv_parse_app_data_ec(const uint8_t * p_tlv_data_buffer, uint32_t buffer_length, uint32_t * p_parsed_tag)
{
    /* Check if the TLV data buffer pointer is valid. */
    assert(!p_tlv_data_buffer);
    if(!p_tlv_data_buffer)
    {
        return TLV_DATA_BUFFER_INVALID;
    }

    /* Check if the TLV data buffer length is not zero. */
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

/* Error check for function to search tag and parse the TLV data buffer (if recursive set to true, then search for child).*/
uint32_t tlv_search_parse_app_data_ec(const uint8_t * p_tlv_data_buffer, uint32_t buffer_length, uint32_t search_parse_tag, bool_t b_recursive)
{
    /* Check if the TLV data buffer pointer is valid. */
    assert(!p_tlv_data_buffer);
    if (!p_tlv_data_buffer)
    {
        return TLV_DATA_BUFFER_INVALID;
    }

    /* Check if the TLV data buffer length is not zero. */
    assert(!buffer_length);
    if (!buffer_length)
    {
        return TLV_BAD_BUFFER_LENGTH;
    }

    /* Check if the TLV tag is not created already. */
    assert(NULL == tag_to_app_data_map[search_parse_tag]);
    if (NULL == tag_to_app_data_map[search_parse_tag])
    {
        return TLV_TAG_NOT_CREATED;
    }

    TLV_STATUS status = TLV_FAIL;
    status = tlv_search_parse_app_data(p_tlv_data_buffer, buffer_length, search_parse_tag, b_recursive);

    /* Return status. */
    return status;
}

/* Error check for function to delete a TLV object. */
uint32_t tlv_delete_app_data_ec(uint32_t tag)
{
    /* Check if the TLV tag is not created already. */
    assert(NULL == tag_to_app_data_map[tag]);
    if (NULL == tag_to_app_data_map[tag])
    {
        return TLV_TAG_NOT_CREATED;
    }

    TLV_STATUS status = TLV_FAIL;
    status = tlv_delete_app_data(tag);

    return status;
}
