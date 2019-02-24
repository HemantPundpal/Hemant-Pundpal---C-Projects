/*
 * Name: tlv.c
 *
 * Description:
 * All API definitions required for the tlv encoder and decoder.
 *
 * Author: Hemant Pundpal                                   Date: 12 Feb 2019
 *
 */

#define TLV_OBJECT_SOURCE_CODE

#include "tlv_api.h"
#include "tlv.h"
#include "tlv_universal_class.h"
#include "tlv_application_class.h"

/*
 * Function to create tlv object
 * - if buffer_length == 0; indefinite buffer length
 * - if buffer_length != 0; definite buffer length
 *
 * Note: Indefinite length is a option for only for constructed container tlv objects, not allowed for primitive tlv objects
 * Note: Universal class tags that can be encoded as "both" (primitive and constructed) are encoded as primitive.
 */
uint32_t create_tlv_object(tlv_object_t * p_tlv_object, uint32_t tlv_tag, const uint8_t * p_tlv_value, uint32_t value_length)
{
    TLV_STATUS status = TLV_FAIL;

    /* Check if it is a universal tag number */
    if (tlv_tag <= TAG_MAX_UNIVERSAL)
    {
        /* Create universal class tlv object. */
        status = create_universal_tlv_object(p_tlv_object, tlv_tag, p_tlv_value, value_length);
    }
    else
    {
        /* It is a application tag, create application class tlv object. */
        status = create_application_tlv_object(p_tlv_object, tlv_tag, p_tlv_value, value_length);
    }

    return status;
}


/*
 * - Function to add tlv object to tlv container.
 * - The child TLV object can be a container, meaning a container tlv object can be added to a container tlv object as a child tlv object.
 * - This function does not limit the number of child objects that can be added to the container TLV object. Application can define such
 * limits for itself.
 */
uint32_t add_tlv_object_to_tlv_container(tlv_object_t * p_container_tlv_object, tlv_object_t * p_child_tlv_object)
{
    TLV_STATUS status = TLV_FAIL;

    /* check if child tlv objects list is empty. */
    if (p_container_tlv_object->p_tlv_child_tlv_object_list)
    {
        /* Not empty, then add to the end of the circular list */
        tlv_object_t * p_temp_tlv_object = NULL;
        p_temp_tlv_object = (p_container_tlv_object->p_tlv_child_tlv_object_list)->p_child_tlv_object_previous;

        (p_container_tlv_object->p_tlv_child_tlv_object_list)->p_child_tlv_object_previous = p_child_tlv_object;
        p_temp_tlv_object->p_child_tlv_object_next = p_child_tlv_object;

        p_child_tlv_object->p_child_tlv_object_next = p_container_tlv_object->p_tlv_child_tlv_object_list;
        p_child_tlv_object->p_child_tlv_object_previous = p_temp_tlv_object;

    }
    else
    {
        /* first child TLV object, add to list */
        p_container_tlv_object->p_tlv_child_tlv_object_list = p_child_tlv_object;
        (p_container_tlv_object->p_tlv_child_tlv_object_list)->p_child_tlv_object_previous = p_child_tlv_object;
        (p_container_tlv_object->p_tlv_child_tlv_object_list)->p_child_tlv_object_next = p_child_tlv_object;
    }

    /* increase child count. */
    p_container_tlv_object->tlv_child_Count++;
    status = TLV_SUCCESS;

    return status;
}


/*
 * Function to add data to tlv object.
 * The container TLV object can be of type primitive or constructed container.
 */
uint32_t add_data_to_tlv_object(tlv_object_t * p_tlv_object, uint32_t tlv_tag, const uint8_t * p_tlv_value, uint32_t value_length)
{
    TLV_STATUS status = TLV_FAIL;

    /* Check the tlv object tag. */
    if (p_tlv_object->tlv_object_tag_number == tlv_tag)
    {
        /* If tag match, Check tlv object is a container and data value length is less than or equal to max value
        length of the TLV object */
        if ((p_tlv_object->b_tlv_container_object == FALSE) && (p_tlv_object->tlv_max_object_value_length >= value_length))
        {
            /* Check if it is a universal tag number */
            if (tlv_tag <= TAG_MAX_UNIVERSAL)
            {
                switch (tlv_tag)
                {
                    case TAG_INTEGER:
                    {
                        add_data_integer_tlv_object(p_tlv_object, p_tlv_value, value_length);
                        status = TLV_SUCCESS;
                    }
                    break;
                    case TAG_UTF8STRING:
                    {
                        add_data_universal_tlv_object(p_tlv_object, p_tlv_value, value_length);
                        status = TLV_SUCCESS;
                    }
                    break;
                    default:
                    {
                        /* do nothing */
                    }
                }
            }
            else
            {
                /* Application tag: application class or context specific class. */
                add_data_to_definite_length_application_tlv_object(p_tlv_object, p_tlv_value, value_length);
                status = TLV_SUCCESS;
            }
        }
        else
        {
            /* Cannot write value to a container TLV object, value can be written to a child TLV object in the
            container TLV object tag. */
            status = TLV_CANNOT_WRITE_VALUE;
        }
    }
    else
    {
        /* Check if the tlv object is a container. */
        if ((p_tlv_object->b_tlv_container_object == TRUE) && (p_tlv_object->tlv_child_Count != 0))
        {
            /* Get the child TLV objects of the container TLV object. */
            tlv_object_t * p_temp_tlv_Object = p_tlv_object->p_tlv_child_tlv_object_list;
            do
            {
                /* Check if the tag matches with each of the child TLV object. */
                if (p_temp_tlv_Object->tlv_object_tag_number == tlv_tag)
                {
                    /* This is a recursive call, so take caution when modifying the function. */
                    status = add_data_to_tlv_object(p_tlv_object, tlv_tag, p_tlv_value, value_length);
                    break;
                }
            } while (p_temp_tlv_Object->p_child_tlv_object_next != p_tlv_object->p_tlv_child_tlv_object_list);
        }
        else
        {
            /* Cannot write value as the tag did not match with the input TLV object and TLV object is not
            a container. */
            status = TLV_CANNOT_WRITE_VALUE;
        }
    }

    /* Return Status. */
    return status;
}


/* Function to parse tlv data buffer. */
uint32_t parse_tlv_object(const uint8_t * p_tlv_data_buffer, uint32_t buffer_length, tlv_object_t * p_tlv_object)
{
    TLV_STATUS status = TLV_FAIL;

    /* THIS FUNCTION IS NOT IMPLEMENTED YET, SHOULD BE UPDATED IN NEXT 3 to 4 DAYS. */

    /* This function will be invoked by app data layer, when application receives interrupt for TLV data buffer
    received over the serial communication. */

    return status;
}

/* Function to search tlv encoded data object in the tlv data buffer. */
uint32_t tlv_search_tag(const uint8_t * p_tlv_data_buffer, uint32_t buffer_length, uint32_t tag, bool_t recursive, tlv_object_t * p_tlv_object)
{
    TLV_STATUS status = TLV_FAIL;

    /* THIS FUNCTION IS NOT IMPLEMENTED YET, SHOULD BE UPDATED IN NEXT 3 to 4 DAYS. */

    /* This function will be invoked by app data layer, when application wants to find and decode a particular tag in the TLV data buffer received over the serial communication. */
    /* The tag to find can be a child tag. To find a child tag, recursive should be set to TRUE by the app data layer. */

    return status;
}

