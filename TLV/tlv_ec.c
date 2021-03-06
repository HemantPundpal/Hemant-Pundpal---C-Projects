/*
 * Name: tlv_ec.c
 *
 * Description:
 * All error check API definitions required for the TLV encoder and decoder.
 *
 * Author: Hemant Pundpal                                   Date: 04 Mar 2019
 *
 */
#define TLV_OBJECT_SOURCE_CODE

#include "tlv_api.h"
#include "tlv.h"
#include "error_assert.h"

/*
 * Internal function - Error check tag number.
 * Note: currently only few tags are supported thus this tag check function is sufficient.
 */
static uint32_t check_tag(uint32_t tag_number, uint32_t app_data_size);

/* Error check for create TLV object function. */
uint32_t create_tlv_object_ec(tlv_object_t * p_tlv_object, uint32_t tlv_tag, uint8_t * p_tlv_value, uint32_t value_length)
{
    /* Check if TLV point is valid. */
    assert(!p_tlv_object);
    if (!p_tlv_object)
    {
        return TLV_OBJECT_INVALID_PTR;
    }

    /* Check buffer (value) length is valid. */
    if (value_length)
    {
        assert(value_length > APP_TAG_MAX_VALUE_LENGTH);
        if(value_length > APP_TAG_MAX_VALUE_LENGTH)
        {
            return TLV_BAD_DATA_SIZE;
        }

        assert(!p_tlv_value);
        if (!p_tlv_value)
        {
            return TLV_DATA_BUFFER_INVALID;
        }
    }

    /* Check TLV tag and constraints. */
    TLV_STATUS status = TLV_FAIL;
    status = check_tag(tlv_tag, value_length);
    if (TLV_SUCCESS == status)
    {
        status = create_tlv_object(p_tlv_object, tlv_tag, p_tlv_value, value_length);
    }

    /* Return status. */
    return status;
}

/* Error check for add TLV object to TLV container function. */
uint32_t add_tlv_object_to_tlv_container_ec(tlv_object_t * p_container_tlv_object, tlv_object_t * p_child_tlv_object)
{
    /* Check if the container TLV pointer is valid. */
    assert(!p_container_tlv_object);
    if (!p_container_tlv_object)
    {
        return TLV_OBJECT_INVALID_PTR;
    }

    /* Check if the TLV object is a container. */
    assert(FALSE == p_container_tlv_object->b_tlv_container_object);
    if(FALSE == p_container_tlv_object->b_tlv_container_object)
    {
        return TLV_NOT_A_CONTAINER;
    }

    /* Check is child TLV object already belongs to a container TLV object. */
    assert(TRUE == p_child_tlv_object->b_tlv_has_a_parent);
    if (TRUE == p_child_tlv_object->b_tlv_has_a_parent)
    {
        return TLV_CHILD_HAS_PARENT;
    }

    /* TLV object's parent cannot be added to its child. */
    if (TRUE == p_container_tlv_object->b_tlv_has_a_parent)
    {
        assert(p_container_tlv_object->p_tlv_parent_tlv_object == p_child_tlv_object);
        if (p_container_tlv_object->p_tlv_parent_tlv_object == p_child_tlv_object)
        {
            return TLV_PARENT_AS_CHILD;
        }
    }

    /* Check if child TLV pointed is valid. */
    assert(!p_child_tlv_object);
    if (!p_child_tlv_object)
    {
        return TLV_OBJECT_INVALID_PTR;
    }

    /* Container can not have itself as child. */
    assert(p_container_tlv_object == p_child_tlv_object);
    if(p_container_tlv_object == p_child_tlv_object)
    {
        return TLV_CANNOT_CONTAIN_ITSELF;
    }

    TLV_STATUS status = TLV_FAIL;
    status = add_tlv_object_to_tlv_container(p_container_tlv_object, p_child_tlv_object);

    /* Return status. */
    return status;
}

/* Error check for add data to TLV object function. */
uint32_t add_data_to_tlv_object_ec(tlv_object_t * p_tlv_object, uint32_t tlv_tag, const uint8_t * p_tlv_value, uint32_t value_length)
{
    /* Check TLV point is valid. */
    assert(!p_tlv_object);
    if (!p_tlv_object)
    {
        return TLV_OBJECT_INVALID_PTR;
    }

    /* Check buffer (value) pointer is valid. */
    assert(!p_tlv_value);
    if (!p_tlv_value)
    {
        return TLV_DATA_BUFFER_INVALID;
    }

    /* Check buffer (value) length is valid. */
    assert(value_length > APP_TAG_MAX_VALUE_LENGTH);
    if(value_length > APP_TAG_MAX_VALUE_LENGTH)
    {
        return TLV_BAD_DATA_SIZE;
    }

    /* Check TLV tag and constraints. */
    TLV_STATUS status = TLV_FAIL;
    status = check_tag(tlv_tag, value_length);
    if (TLV_SUCCESS == status)
    {
        status = add_data_to_tlv_object(p_tlv_object, tlv_tag, p_tlv_value, value_length);
    }

    /* Return status. */
    return status;
}

/* Error check for parse TLV data buffer function. */
uint32_t parse_tlv_object_ec(const uint8_t * p_tlv_data_buffer, uint32_t buffer_length, tlv_object_t * p_tlv_object)
{
    /* Check TLV point is valid. */
    assert(!p_tlv_object);
    if (!p_tlv_object)
    {
        return TLV_OBJECT_INVALID_PTR;
    }

    assert(!p_tlv_data_buffer);
    if(!p_tlv_data_buffer)
    {
        return TLV_DATA_BUFFER_INVALID;
    }

    /* Check buffer length. */
    assert(!buffer_length);
    if(!buffer_length)
    {
        return TLV_BAD_BUFFER_LENGTH;
    }

    TLV_STATUS status = TLV_FAIL;
    status = parse_tlv_object(p_tlv_data_buffer, buffer_length, p_tlv_object);

    /* Return status. */
    return status;
}

/* Error check for search TLV encoded data object in the TLV data buffer function. */
uint32_t tlv_search_tag_ec(const uint8_t * p_tlv_data_buffer, uint32_t buffer_length, uint32_t tag, bool_t b_recursive, tlv_object_t * p_tlv_object)
{
    /* Check TLV point is valid. */
    assert(!p_tlv_object);
    if (!p_tlv_object)
    {
        return TLV_OBJECT_INVALID_PTR;
    }

    assert(tag != p_tlv_object->tlv_object_tag_number);
    if (tag != p_tlv_object->tlv_object_tag_number)
    {
        return TLV_OBJECT_INVALID_PTR;
    }

    assert(!p_tlv_data_buffer);
    if (!p_tlv_data_buffer)
    {
        return TLV_DATA_BUFFER_INVALID;
    }

    TLV_STATUS status = TLV_FAIL;
    status = tlv_search_tag(p_tlv_data_buffer, buffer_length, tag, b_recursive, p_tlv_object);

    /* Return status. */
    return status;
}

/*
 * Internal function - Error check tag number.
 * Note: currently only few tags are supported thus this tag check function is sufficient.
 */
static uint32_t check_tag(uint32_t tag_number, uint32_t data_length)
{
    /* Check any constraints that application put on the tags. */
    switch (tag_number)
    {
        case TAG_INTEGER:
        {
            assert(!data_length || data_length > MAX_INTEGER_BYTES);
            if (!data_length || data_length > MAX_INTEGER_BYTES)
            {
                return TLV_BAD_DATA_SIZE;
            }
        }
        break;
        case TAG_UTF8STRING:
        {
            assert(!data_length || data_length > MAX_UTF8STRING_LENGTH);
            if (!data_length || data_length > MAX_UTF8STRING_LENGTH)
            {
                return TLV_BAD_DATA_SIZE;
            }
        }
        break;
        /* add required check for each universal tag. */
        default:
        {
            assert((TAG_END_OF_CONTENT == tag_number) || (TAG_MAX < tag_number));
            if ((TAG_END_OF_CONTENT == tag_number) || (TAG_MAX < tag_number))
            {
                return TLV_TAG_NOT_SUPPORTED;
            }
        }
    }

    /* Return Success. */
    return TLV_SUCCESS;
}
