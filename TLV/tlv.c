/*
 * Name: tlv.c
 *
 * Description:
 * All API definitions required for the TLV encoder and decoder.
 *
 * Author: Hemant Pundpal                                   Date: 12 Feb 2019
 *
 */

#define TLV_OBJECT_SOURCE_CODE

#include "tlv_api.h"
#include "tlv.h"
#include "tlv_universal_class.h"
#include "tlv_application_class.h"
#include "tlv_object_parser.h"
#include "tlv_definition.h"

/*
 * Function to create TLV object
 * - if buffer_length == 0; indefinite buffer length
 * - if buffer_length != 0; definite buffer length
 *
 * Note: Indefinite length is a option for only for constructed container TLV objects, not allowed for primitive TLV objects
 * Note: Universal class tags that can be encoded as "both" (primitive and constructed) are encoded as primitive.
 */
uint32_t create_tlv_object(tlv_object_t * p_tlv_object, uint32_t tlv_tag, const uint8_t * p_tlv_value, uint32_t value_length)
{
    TLV_STATUS status = TLV_FAIL;

    /* Check if it is a universal tag number */
    if (tlv_tag <= TAG_MAX_UNIVERSAL)
    {
        /* Create universal class TLV object. */
        status = create_universal_tlv_object(p_tlv_object, tlv_tag, p_tlv_value, value_length);
    }
    else
    {
        /* It is a application tag, create application class TLV object. */
        status = create_application_tlv_object(p_tlv_object, tlv_tag, p_tlv_value, value_length);
    }

    return status;
}

/*
 * - Function to add TLV object to TLV container.
 * - The child TLV object can be a container, meaning a container TLV object can be added to a container TLV object as a child TLV object.
 * - This function does not limit the number of child objects that can be added to the container TLV object. Application can define such
 * limits for itself.
 */
uint32_t add_tlv_object_to_tlv_container(tlv_object_t * p_container_tlv_object, tlv_object_t * p_child_tlv_object)
{
    TLV_STATUS status = TLV_FAIL;

    /* check if child TLV objects list is empty. */
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
 * Function to add data to TLV object.
 * The container TLV object can be of type primitive or constructed container.
 */
uint32_t add_data_to_tlv_object(tlv_object_t * p_tlv_object, uint32_t tlv_tag, const uint8_t * p_tlv_value, uint32_t value_length)
{
    TLV_STATUS status = TLV_FAIL;

    /* Check the TLV object tag. */
    if (p_tlv_object->tlv_object_tag_number == tlv_tag)
    {
        /* If tag match, Check TLV object is a container and data value length is less than or equal to max value
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
        /* Check if the TLV object is a container. */
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

/*
 * - Function to parse TLV data buffer (first found TLV object is parsed.)
 * - The parse_tlv_object() find the first valid tag and decodes the TLV encoded data, else returns error tag not found or bad TLV data buffer
 * - If the tag found of a definite length then the application gets TLV object parsed with value.
 *
 * NOTE: If the tag found is of indefinite length (container type), then the caller gets the container TLV object of indefinite length and the caller application
 * should call tlv_search_tag() api to parse a child TLV 0bjects in the container TLV object. Application can also parse the entire container TLV object of indefinite
 * length with all its child TLV objects by calling tlv_search_tag() api.
 */
uint32_t parse_tlv_object(const uint8_t * p_tlv_data_buffer, uint32_t buffer_length, tlv_object_t * p_tlv_object)
{
    TLV_STATUS status = TLV_NO_TAG_FOUND;

    for (uint32_t i = 0; i < buffer_length; i++)
    {
        uint8_t tag_class = (uint8_t)(p_tlv_data_buffer[i] & (uint8_t)TLV_TAG_CLASS_FILTER);
        switch (tag_class)
        {
            case TAG_UNIVERSAL_PRIMITIVE:
            {
                /* Get the parsed TLV object. */
                if(TLV_SUCCESS == get_parsed_tlv_object(&p_tlv_data_buffer[i], (buffer_length - i), p_tlv_object))
                {
                    if ((p_tlv_object->b_tlv_container_object == TRUE) || (p_tlv_object->b_tlv_object_length_definite == FALSE))
                    {
                        status = TLV_BAD_TAG;
                    }
                    else
                    {
                        status = TLV_SUCCESS;
                    }
                }
            }
            break;
            case TAG_UNIVERSAL_CONSTRUCTED:
            {
                /* Get the parsed TLV object. */
                status = get_parsed_tlv_object(&p_tlv_data_buffer[i], (buffer_length - i), p_tlv_object);
            }
            break;
            case TAG_APPL_CLS_PRIMITIVE:
            {
                /* Get the parsed TLV object. */
                if (TLV_SUCCESS == get_parsed_tlv_object(&p_tlv_data_buffer[i], (buffer_length - i), p_tlv_object))
                {
                    if ((p_tlv_object->b_tlv_container_object == TRUE) || (p_tlv_object->b_tlv_object_length_definite == FALSE))
                    {
                        status = TLV_BAD_TAG;
                    }
                    else
                    {
                        status = TLV_SUCCESS;
                    }
                }
            }
            break;
            case TAG_APPL_CLS_CONSTRUCTED:
            {
                /* Get the parsed TLV object. */
                status = get_parsed_tlv_object(&p_tlv_data_buffer[i], (buffer_length - i), p_tlv_object);
            }
            break;
            case TAG_CS_CLS_PRIMITIVE:
            {
                /* Get the parsed TLV object. */
                if (TLV_SUCCESS == get_parsed_tlv_object(&p_tlv_data_buffer[i], (buffer_length - i), p_tlv_object))
                {
                    if ((p_tlv_object->b_tlv_container_object == TRUE) || (p_tlv_object->b_tlv_object_length_definite == FALSE))
                    {
                        status = TLV_BAD_TAG;
                    }
                    else
                    {
                        status = TLV_SUCCESS;
                    }
                }
            }
            break;
            case TAG_CS_CLS_CONSTRUCTED:
            {
                /* Get the parsed TLV object. */
                status = get_parsed_tlv_object(&p_tlv_data_buffer[i], (buffer_length - i), p_tlv_object);
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
        if (TLV_SUCCESS == status)
        {
            /* First TLV object parsed successfully, update the TLV buffer in the TLV object. */
            status = update_parsed_tlv_object(&p_tlv_data_buffer[i], (buffer_length - i), p_tlv_object);
            /* If successfully updated, exit the search for tags. */
            if (TLV_SUCCESS == status)
            {
                break;
            }
        }
    }

    /* Return status. */
    return status;
}

/* Function to search TLV encoded data object in the TLV data buffer. */
uint32_t tlv_search_tag(const uint8_t * p_tlv_data_buffer, uint32_t buffer_length, uint32_t tag, bool_t b_recursive, tlv_object_t * p_tlv_object)
{
    TLV_STATUS status = TLV_FAIL;

    /* THIS FUNCTION IS NOT IMPLEMENTED YET, SHOULD BE UPDATED IN NEXT 3 to 4 DAYS. */

    /* This function will be invoked by app data layer, when application wants to find and decode a particular tag in the TLV data buffer received over the serial communication. */
    /* The tag to find can be a child tag. To find a child tag, recursive should be set to TRUE by the app data layer. */

    if ((!p_tlv_data_buffer) || (!buffer_length) || (!tag) || (b_recursive) || (!p_tlv_object))
    {
        /* Suppress the warning. */
    }

    return status;
}

