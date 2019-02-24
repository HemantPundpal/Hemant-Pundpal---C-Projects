/*
 * Name: tlv_app_data.c
 *
 * Description:
 * All API definitions required for the app data layer to abstract tlv encoder and decoder.
 *
 * Author: Hemant Pundpal                                   Date: 24 Feb 2019
 *
 */
#define TLV_APP_DATA_SOURCE_CODE

#include "tlv_app_data_api.h"
#include "tlv_api.h"
#include "tlv_app_data.h"

/* 
 * A simple tag to app data map using array (no hashing).
 * Can be optimized by use map structure and hashing of index. 
 */
tlv_app_data_t * tag_to_app_data_map[TAG_MAX];

bool_t b_tlv_app_data_map_initialized = FALSE;

/*
 * Initialize TLV.
 * Application must call this function before accessing any other app data API
 */
void tlv_initialize(void)
{
    /* Initialize the app data to tags map to NULL. */
    for (uint32_t i = 0; i < TAG_MAX; i++)
    {
        tag_to_app_data_map[i] = NULL;
    }

    /* Set app data map initialized map. */
    b_tlv_app_data_map_initialized = TRUE;
}


/* Function to initialize and create tlv object of the app data with definite size. */
uint32_t tlv_init_and_create_app_data(uint8_t * p_app_data_buffer, uint32_t app_data_size, uint32_t app_data_tag)
{
    TLV_STATUS status = TLV_FAIL;

    /* Allocate memory to store metadata for application data with map to tlv object. */
    tlv_app_data_t * p_tlv_app_data = NULL;
    p_tlv_app_data = (tlv_app_data_t *)calloc(1, sizeof(tlv_app_data_t));

    if (p_tlv_app_data != NULL)
    {
        /* Initialize the app data. */
        p_tlv_app_data->b_container = FALSE;
        p_tlv_app_data->u_size.data_size = app_data_size;
        p_tlv_app_data->p_app_data = p_app_data_buffer;

        /* Allocate memory for tlv object to encode the app data into tlv object. */
        p_tlv_app_data->p_tlv_object = NULL;
        p_tlv_app_data->p_tlv_object = (tlv_object_t *)calloc(1, sizeof(tlv_object_t));
        if (p_tlv_app_data->p_tlv_object != NULL)
        {
            /* Create the TLV object. */
            if (app_data_tag == TAG_INTEGER_UNSIGNED)
            {
                status = create_tlv_object(p_tlv_app_data->p_tlv_object, TAG_INTEGER, p_tlv_app_data->p_app_data, p_tlv_app_data->u_size.data_size);
            }
            else
            {
                status = create_tlv_object(p_tlv_app_data->p_tlv_object, app_data_tag, p_tlv_app_data->p_app_data, p_tlv_app_data->u_size.data_size);
            }
        }
        else
        {
            /* Memory allocation failed, free the app data structure and return error. */
            free(p_tlv_app_data);
            status = TLV_NO_MEMORY;
        }

        if (status == TLV_SUCCESS)
        {
            /* TLV object created successfully. */
            /* Add key: tag_number and value: p_tlv_app_data to the map. */
            tag_to_app_data_map[app_data_tag] = p_tlv_app_data;
            p_tlv_app_data->tag_number = app_data_tag;
        }
        else
        {
            /* TLV object was not created successfully, free the memory and return status. */
            free(p_tlv_app_data->p_tlv_object);
            free(p_tlv_app_data);
        }
    }
    else
    {
        /* Memory Allocation failed, return error. */
        status = TLV_NO_MEMORY;
    }

    /* Return status. */
    return status;
}

/* Function to create constructed container tlv object of the app data with indefinite size. */
uint32_t tlv_create_container_app_data(uint32_t container_app_data_tag)
{
    TLV_STATUS status = TLV_FAIL;

    /* Allocate memory to store metadata for application data with map to tlv object. */
    tlv_app_data_t * p_tlv_app_data = NULL;
    p_tlv_app_data = (tlv_app_data_t *)calloc(1, sizeof(tlv_app_data_t));

    if (p_tlv_app_data != NULL)
    {
        p_tlv_app_data->b_container = TRUE;
        p_tlv_app_data->u_size.child_count = 0;
        p_tlv_app_data->p_app_data = NULL;

        /* Allocate memory for tlv object to encode the container type app data into container tlv object. */
        p_tlv_app_data->p_tlv_object = NULL;
        p_tlv_app_data->p_tlv_object = (tlv_object_t *)calloc(1, sizeof(tlv_object_t));
        if (p_tlv_app_data->p_tlv_object != NULL)
        {
            /* Create the TLV object. */
            status = create_tlv_object(p_tlv_app_data->p_tlv_object, container_app_data_tag, p_tlv_app_data->p_app_data, p_tlv_app_data->u_size.data_size);
        }
        else
        {
            /* Memory allocation failed, free the app data structure and return error. */
            free(p_tlv_app_data);
            status = TLV_NO_MEMORY;
        }

        if (status == TLV_SUCCESS)
        {
            /* Container TLV object created successfully. */
            /* Add key: tag_number and value: p_tlv_app_data to the map. */
            tag_to_app_data_map[container_app_data_tag] = p_tlv_app_data;
            p_tlv_app_data->tag_number = container_app_data_tag;
        }
        else
        {
            /* Container TLV object was not created successfully, free the memory and return status. */
            free(p_tlv_app_data->p_tlv_object);
            free(p_tlv_app_data);
        }
    }
    else
    {
        /* Memory Allocation failed, return error. */
        status = TLV_NO_MEMORY;
    }

    /* Return status. */
    return status;
}

/* Function to create tlv object of the app data and add to container (parent) tlv object as a child. */
uint32_t tlv_add_child_to_container_app_data(uint32_t container_app_data_tag, uint8_t * p_app_data_buffer, uint32_t app_data_size, uint32_t child_app_data_tag)
{
    TLV_STATUS status = TLV_FAIL;

    /* Allocate memory to store metadata for application data with map to tlv object. */
    tlv_app_data_t * p_tlv_app_data = NULL;
    p_tlv_app_data = (tlv_app_data_t *)calloc(1, sizeof(tlv_app_data_t));

    if (p_tlv_app_data != NULL)
    {
        p_tlv_app_data->b_container = FALSE;
        p_tlv_app_data->u_size.data_size = app_data_size;
        p_tlv_app_data->p_app_data = p_app_data_buffer;

        /* Allocate memory for tlv object to encode the app data into tlv object. */
        p_tlv_app_data->p_tlv_object = NULL;
        p_tlv_app_data->p_tlv_object = (tlv_object_t *)calloc(1, sizeof(tlv_object_t));
        if (p_tlv_app_data->p_tlv_object != NULL)
        {
            /* Create the TLV object. */
            if (child_app_data_tag == TAG_INTEGER_UNSIGNED)
            {
                status = create_tlv_object(p_tlv_app_data->p_tlv_object, TAG_INTEGER, p_tlv_app_data->p_app_data, p_tlv_app_data->u_size.data_size);
            }
            else
            {
                status = create_tlv_object(p_tlv_app_data->p_tlv_object, child_app_data_tag, p_tlv_app_data->p_app_data, p_tlv_app_data->u_size.data_size);
            }
        }
        else
        {
            /* Memory allocation failed, free the app data structure and return error. */
            free(p_tlv_app_data);
            status = TLV_NO_MEMORY;
        }

        if (status == TLV_SUCCESS)
        {
            /* Child TLV object created successfully. */

            /* Get the container tag app data */
            tlv_app_data_t * p_tlv_container_app_data = tag_to_app_data_map[container_app_data_tag];
            status = add_tlv_object_to_tlv_container(p_tlv_container_app_data->p_tlv_object, p_tlv_app_data->p_tlv_object);
            if (status == TLV_SUCCESS)
            {
                /* update container app data with child app data details. */
                p_tlv_container_app_data->p_child_app_data[p_tlv_container_app_data->u_size.child_count] = p_tlv_app_data;
                p_tlv_container_app_data->u_size.child_count++;
                p_tlv_app_data->tag_number = child_app_data_tag;
            }
        }

        if(status != TLV_SUCCESS)
        {
            /* Child TLV object was not created successfully, free the memory and return status. */
            free(p_tlv_app_data->p_tlv_object);
            free(p_tlv_app_data);
        }
    }
    else
    {
        /* Memory Allocation failed, return error. */
        status = TLV_NO_MEMORY;
    }

    /* Return status. */
    return status;
}


/* Function to add created child tlv object to container (parent) tlv object. */
uint32_t tlv_add_child_tag_to_container_app_data(uint32_t container_app_data_tag, uint32_t child_app_data_tag)
{
    TLV_STATUS status = TLV_FAIL;

    /* Get the container tag app data */
    tlv_app_data_t * p_tlv_container_app_data = tag_to_app_data_map[container_app_data_tag];
    tlv_app_data_t * p_tlv_child_app_data = tag_to_app_data_map[child_app_data_tag];
    status = add_tlv_object_to_tlv_container(p_tlv_container_app_data->p_tlv_object, p_tlv_child_app_data->p_tlv_object);
    if (status == TLV_SUCCESS)
    {
        /* update container app data with child app data details. */
        p_tlv_container_app_data->p_child_app_data[p_tlv_container_app_data->u_size.child_count] = p_tlv_child_app_data;
        p_tlv_container_app_data->u_size.child_count++;
    }

    return status;
}

/* Function to add data to tlv objects with definite size. */
uint32_t tlv_add_data_to_app_data(uint32_t app_data_tag)
{
    TLV_STATUS status = TLV_FAIL;

    /* Get the app data map. */
    tlv_app_data_t * p_tlv_app_data = tag_to_app_data_map[app_data_tag];

    /* Update the application data to the TLV object. */
    status = add_data_to_tlv_object(p_tlv_app_data->p_tlv_object, app_data_tag, p_tlv_app_data->p_app_data, p_tlv_app_data->u_size.data_size);

    return status;
}

/* Function to add data to constructed container tlv object with indefinite size. */
uint32_t tlv_add_data_to_container_app_data(uint32_t container_app_data_tag, uint32_t child_app_data_tag)
{
    TLV_STATUS status = TLV_FAIL;
    bool_t b_child_found = FALSE;

    /* Get the container tag app data */
    tlv_app_data_t * p_container_app_data = tag_to_app_data_map[container_app_data_tag];
    for (uint32_t i = 0; i < p_container_app_data->u_size.child_count; i++)
    {
        /* Search the child app data. */
        if ((p_container_app_data->p_child_app_data[i])->tag_number == child_app_data_tag)
        {
            b_child_found = TRUE;

            /* update the child application data to the corresponding child tlv object. */
            status = add_data_to_tlv_object((p_container_app_data->p_child_app_data[i])->p_tlv_object, child_app_data_tag, (p_container_app_data->p_child_app_data[i])->p_app_data, (p_container_app_data->p_child_app_data[i])->u_size.data_size);
        }
    }

    if (b_child_found == FALSE)
    {
        /* Child tag was not found in the container. */
        status = TLV_CHILD_NOT_FOUND;
    }

    /* Return status. */
    return status;
}

/* Function to send application data. */
uint32_t tlv_app_data_send(uint32_t tag)
{
    TLV_STATUS status = TLV_FAIL;

    /* 
     * - This function should actually send the data over the serial communication interface.
     * - May be it just writes tlv encoded buffer to the ring buffer.
     * - The serial driver reads the tlv encoded buffer from the ring buffer and send on the serial 
     * Communication channel.
     * - This function will be updated to communicate with serial driver, currently is used for
     * testing tlv encoded object.
     */
    tlv_app_data_t * p_app_data = tag_to_app_data_map[tag];

    if (p_app_data->b_container)
    {
        /* Just for test console printing statements. */
        if(!((p_app_data->p_tlv_object)->b_tlv_object_length_definite))
        {
            printf("Indefinite length TLV object. \n");
        }

        uint32_t i = 0;
        for (i = 0; i < (p_app_data->p_tlv_object)->tlv_curr_encoded_object_length; i++)
        {
            printf("0x%X , ", (p_app_data->p_tlv_object)->p_tlv_object_encoded_buffer[i]);
        }
        printf("\n");

        for (i = 0; i < p_app_data->u_size.child_count; i++)
        {
            printf("Child %d - ", i);
            if((p_app_data->p_child_app_data[i])->b_container == FALSE)
            {
                for (uint32_t y = 0; y < ((p_app_data->p_child_app_data[i])->p_tlv_object)->tlv_curr_encoded_object_length; y++)
                {
                    printf("0x%X , ",((p_app_data->p_child_app_data[i])->p_tlv_object)->p_tlv_object_encoded_buffer[y]);
                }
                printf("\n");
            }
            else
            {
                printf("Container within a container. \n");
                tlv_app_data_send((p_app_data->p_child_app_data[i])->tag_number);
            }
        }
        printf("\n");

        if(!((p_app_data->p_tlv_object)->b_tlv_object_length_definite))
        {
            printf("0x%X (END OF CONTEXT for indefinite length)", TAG_END_OF_CONTENT);
        }

        printf("\n");
    }
    else
    {
        for (uint32_t y = 0; y < (p_app_data->p_tlv_object)->tlv_curr_encoded_object_length; y++)
        {
            printf("%X , ", (p_app_data->p_tlv_object)->p_tlv_object_encoded_buffer[y]);
        }
        printf("\n");
    }

    /* Return status. */
    return status;
}

/* Function to parse app data from TLV data buffer (first found tlv object is parsed.) */
uint32_t tlv_parse_app_data(const uint8_t * p_tlv_data_buffer, uint32_t buffer_length, uint32_t * p_parsed_tag)
{
    TLV_STATUS status = TLV_FAIL;

    tlv_object_t tlv_parsed_object;

    /* Parse the first tag in the buffer. */
    status = parse_tlv_object(p_tlv_data_buffer, buffer_length, &tlv_parsed_object);

    if(status == TLV_SUCCESS)
    {
        /* return the parsed tag number (the app data tag number can be different than tlv tag number
         * this requires better mapping, for now this is good). */
        /* Get the app data map. */
        tlv_app_data_t * p_tlv_app_data = tag_to_app_data_map[tlv_parsed_object.tlv_object_tag_number];

        if((p_tlv_app_data == NULL) && (tlv_parsed_object.tlv_object_tag_number == TAG_INTEGER))
        {
            p_tlv_app_data = tag_to_app_data_map[TAG_INTEGER_UNSIGNED];
        }

        if(p_tlv_app_data != NULL)
        {
            /* Return the found tag. */
            *p_parsed_tag = p_tlv_app_data->tag_number;
            status = TLV_SUCCESS;
        }
        else
        {
            /* No tag found. */
            status = TLV_NO_TAG_FOUND;
        }
    }

    /* Return status. */
    return status;
}

/* Function to search tag and parse the tlv data buffer (if recursive set to true, then search for child app data).*/
uint32_t tlv_search_parse_app_data(const uint8_t * p_tlv_data_buffer, uint32_t buffer_length, uint32_t search_parse_tag, bool_t b_recersive)
{
    TLV_STATUS status = TLV_FAIL;
    /* THIS FUNCTION IS NOT IMPLEMENTED YET, SHOULD BE UPDATED IN NEXT 3 to 4 DAYS. */

    /* This function will be invoked by application, when application wants to find and decode a particular tag in the TLV data buffer received over the serial communication. */
    /* The tag to find can be a child tag. To find a child tag, recursive should be set to TRUE by the application. */
    return status;
}

/* Function to delete a tlv object. */
uint32_t tlv_delete_app_data(uint32_t tag)
{
    TLV_STATUS status = TLV_FAIL;

    return status;
}
