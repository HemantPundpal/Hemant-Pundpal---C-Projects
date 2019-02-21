/*
 * Name: tlv_app_data.h
 *
 * Description:
 * All definitions and api function declarations required for the tlv app data layer.
 * This function abstracts the TLV encoder and decoder for the application.
 *
 * Note application can choose to directly access TLV encoder and decode by including tlv_api.h
 *
 * Author: Hemant Pundpal                                   Date: 21 Feb 2019
 *
 */
#ifndef __TLV_APP_DATA_H__
#define __TLV_APP_DATA_H__

/* This structure keeps a metadata of the application data and map it to TLV objects. */
typedef struct tlv_app_data
{
    uint32_t        tag_number;
    bool_t          b_container;

    union
    {
        uint32_t        data_size;
        uint32_t        child_count;
    };

    union
    {
        uint8_t       * p_app_data;
        struct tlv_app_data
                      * p_child_app_data[MAX_CONTAINER_CHILD_COUNT];
    };

    tlv_object_t  * p_tlv_object;
}tlv_app_data_t;

/* 
 * Initialize TLV.
 * Application must call this function before accessing any other app data API
 */
void tlv_initialize(void);

/* Function to initialize and create tlv object of the app data with definite size. */
uint32_t tlv_init_and_create_app_data(uint8_t * p_app_data_buffer, uint32_t app_data_size, uint32_t app_data_tag);

/* Function to create constructed container tlv object of the app data with indefinite size. */
uint32_t tlv_create_container_app_data(uint32_t container_app_data_tag);

/* Function to create tlv object of the app data and add to container (parent) tlv object as a child. */
uint32_t tlv_add_child_to_container_app_data(uint32_t container_app_data_tag, uint8_t * p_app_data_buffer, uint32_t app_data_size, uint32_t child_app_data_tag);

/* Function to add created child tlv object to container (parent) tlv object. */
uint32_t tlv_add_child_tag_to_container_app_data(uint32_t container_app_data_tag, uint32_t child_app_data_tag);

/* Function to add data to tlv objects with definite size. */
uint32_t tlv_add_data_to_app_data(uint32_t app_data_tag);

/* Function to add data to constructed container tlv objects with indefinite size. */
uint32_t tlv_add_data_to_container_app_data(uint32_t container_app_data_tag, uint32_t child_app_data_tag);

/* Function to send application data. */
uint32_t tlv_app_data_send(uint32_t tag);

/*
 * - Function to parse app data from TLV data buffer (first found tlv object is parsed.)
 * - The parsed TLV object data will be auto updated to the application variables.
 */
uint32_t tlv_parse_app_data(const uint8_t * p_tlv_buffer, uint32_t * p_parsed_tag);

/*
 * - Function to search tag and parse the tlv data buffer (if recursive set to true, then search for child).
 * - The searched and parsed TLV object data will be auto updated to the application variables.
 */
uint32_t tlv_search_parse_app_data(const uint8_t * p_tlv_buffer, uint32_t search_parse_tag, bool_t b_recersive);

/* Function to delete a tlv object and removes the mapping between the tlv object and the app data. */
uint32_t tlv_delete_app_data(uint32_t tag);




/* Error check for function to initialize and create tlv object of the app data with definite size. */
uint32_t tlv_init_and_create_app_data_ec(uint8_t * p_app_data_buffer, uint32_t app_data_size, uint32_t app_data_tag);

/* Error check for function to create constructed container tlv object of the app data with indefinite size. */
uint32_t tlv_create_container_app_data_ec(uint32_t container_app_data_tag);

/* Error check for function to create tlv object of the app data and add to container (parent) tlv object as a child. */
uint32_t tlv_add_child_to_container_app_data_ec(uint32_t container_app_data_tag, uint8_t * p_app_data_buffer, uint32_t app_data_size, uint32_t child_app_data_tag);

/* Error check for function to add created child tlv object to container (parent) tlv object. */
uint32_t tlv_add_child_tag_to_container_app_data_ec(uint32_t container_app_data_tag, uint32_t child_app_data_tag);

/* Error check for function to add data to tlv objects with definite size. */
uint32_t tlv_add_data_to_app_data_ec(uint32_t app_data_tag);

/* Error check for function to add data to constructed container tlv objects with indefinite size. */
uint32_t tlv_add_data_to_container_app_data_ec(uint32_t container_app_data_tag, uint32_t child_app_data_tag);

/* Error check for function to send application data. */
uint32_t tlv_app_data_send_ec(uint32_t tag);

/* Error check for function to parse app data from TLV data buffer (first found tlv object is parsed.) */
uint32_t tlv_parse_app_data_ec(const uint8_t * p_tlv_buffer, uint32_t * p_parsed_tag);

/* Error check for function to search tag and parse the tlv data buffer (if recessive set to true, then search for child).*/
uint32_t tlv_search_parse_app_data_ec(const uint8_t * p_tlv_buffer, uint32_t search_parse_tag, bool_t b_recersive);

/* Error check for function to delete a tlv object. */
uint32_t tlv_delete_app_data_ec(uint32_t tag);

#endif /* __TLV_APP_DATA_H__ */
