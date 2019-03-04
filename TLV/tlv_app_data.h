/*
 * Name: tlv_app_data.h
 *
 * Description:
 * All definitions and api function declarations required for the TLV app data layer.
 * These functions abstracts the TLV encoder and decoder for the application.
 *
 * Note application can choose to directly access TLV encoder and decode by including tlv_api.h
 *
 * Author: Hemant Pundpal                                   Date: 21 Feb 2019
 *
 */
#ifndef __TLV_APP_DATA_H__
#define __TLV_APP_DATA_H__

#define UNSIGNED_VALUE_FILTER_8BIT      0x7FU
#define SIGNED_FILTER_8BIT              0x80U
#define MASK_BIT_0_TO_7                 0xFFU

#define SHIFT_8BIT                      8U

/* This structure keeps a meta-data of the application data and map it to TLV objects. */
typedef struct tlv_app_data
{
    uint32_t        tag_number;
    bool_t          b_container;
    bool_t          b_has_parent;

    union
    {
        uint32_t        data_size;
        uint32_t        child_count;
    }u_size;

    uint8_t       * p_app_data;
    struct tlv_app_data
                  * p_child_app_data[MAX_CONTAINER_CHILD_COUNT];
    struct tlv_app_data
                  * p_parent_app_data;

    tlv_object_t  * p_tlv_object;
}tlv_app_data_t;

/* 
 * Initialize TLV.
 * Application must call this function before accessing any other app data API
 */
void_t tlv_initialize(void_t);

/* Function to initialize and create TLV object of the app data with definite size. */
uint32_t tlv_init_and_create_app_data(uint8_t * p_app_data_buffer, uint32_t app_data_size, uint32_t app_data_tag);

/* Function to create constructed container TLV object of the app data with indefinite size. */
uint32_t tlv_create_container_app_data(uint32_t container_app_data_tag);

/* Function to create TLV object of the app data and add to container (parent) TLV object as a child. */
uint32_t tlv_add_child_to_container_app_data(uint32_t container_app_data_tag, uint8_t * p_app_data_buffer, uint32_t app_data_size, uint32_t child_app_data_tag);

/* Function to add created child TLV object to container (parent) TLV object. */
uint32_t tlv_add_child_tag_to_container_app_data(uint32_t container_app_data_tag, uint32_t child_app_data_tag);

/* Function to add data to TLV objects with definite size. */
uint32_t tlv_add_data_to_app_data(uint32_t app_data_tag);

/* Function to add data to constructed container TLV objects with indefinite size. */
uint32_t tlv_add_data_to_container_app_data(uint32_t container_app_data_tag, uint32_t child_app_data_tag);

/* Function to send application data. */
uint32_t tlv_app_data_send(uint32_t tag);

/*
 * - Function to parse app data from TLV data buffer (first found TLV object is parsed.)
 * - The parsed TLV object data will be auto updated to the application variables.
 * - The tlv_parse_app_data() find the first valid tag and decodes the TLV encoded data, else returns error tag not found or bad TLV data buffer
 * - If the tag found of a definite length then the application is gets with tag received and associated value updated in the application variable.
 *
 * NOTE: If the tag found is of indefinite length (container type), then the application gets the tag received and the application should call tlv_search_parse_app_data() api
 * to parse a child tag in the tag found with indefinite length (container type). Application can also parse the entire tag found with indefinite length (container type)
 * with all its child tags by calling tlv_search_parse_app_data() api.
 */
uint32_t tlv_parse_app_data(const uint8_t * p_tlv_data_buffer, uint32_t buffer_length, uint32_t * p_parsed_tag);

/*
 * - Function to search tag and parse the TLV data buffer (if recursive set to true, then search for child).
 * - The searched and parsed TLV object data will be auto updated to the application variables.
 */
uint32_t tlv_search_parse_app_data(const uint8_t * p_tlv_data_buffer, uint32_t buffer_length, uint32_t search_parse_tag, bool_t b_recursive);

/* Function to delete a TLV object and removes the mapping between the TLV object and the app data. */
uint32_t tlv_delete_app_data(uint32_t tag);


/* Error check for function to initialize and create TLV object of the app data with definite size. */
uint32_t tlv_init_and_create_app_data_ec(uint8_t * p_app_data_buffer, uint32_t app_data_size, uint32_t app_data_tag);

/* Error check for function to create constructed container TLV object of the app data with indefinite size. */
uint32_t tlv_create_container_app_data_ec(uint32_t container_app_data_tag);

/* Error check for function to create TLV object of the app data and add to container (parent) TLV object as a child. */
uint32_t tlv_add_child_to_container_app_data_ec(uint32_t container_app_data_tag, uint8_t * p_app_data_buffer, uint32_t app_data_size, uint32_t child_app_data_tag);

/* Error check for function to add created child TLV object to container (parent) TLV object. */
uint32_t tlv_add_child_tag_to_container_app_data_ec(uint32_t container_app_data_tag, uint32_t child_app_data_tag);

/* Error check for function to add data to TLV objects with definite size. */
uint32_t tlv_add_data_to_app_data_ec(uint32_t app_data_tag);

/* Error check for function to add data to constructed container TLV objects with indefinite size. */
uint32_t tlv_add_data_to_container_app_data_ec(uint32_t container_app_data_tag, uint32_t child_app_data_tag);

/* Error check for function to send application data. */
uint32_t tlv_app_data_send_ec(uint32_t tag);

/* Error check for function to parse app data from TLV data buffer (first found TLV object is parsed.) */
uint32_t tlv_parse_app_data_ec(const uint8_t * p_tlv_data_buffer, uint32_t buffer_length, uint32_t * p_parsed_tag);

/* Error check for function to search tag and parse the TLV data buffer (if recessive set to true, then search for child).*/
uint32_t tlv_search_parse_app_data_ec(const uint8_t * p_tlv_data_buffer, uint32_t buffer_length, uint32_t search_parse_tag, bool_t b_recursive);

/* Error check for function to delete a TLV object. */
uint32_t tlv_delete_app_data_ec(uint32_t tag);

#endif /* __TLV_APP_DATA_H__ */
