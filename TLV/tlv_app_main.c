
/*
 * Name: tlv_app_main.c
 *
 * Description:
 * THIS IS A DEMO CODE JUST TO EXCERCISE / DEMONSTRATE THE TLV ENCODER-DECODER
 *
 * NOTE: This code may not follow all coding standards.
 *
 * Author: Hemant Pundpal                            Date: 17 Feb 2019
 *
 */

#include "tlv_app_data_api.h"

#define MAX_TXN_REF_LEN         7U
#define MAX_ADDNL_TXN_REF_LEN   10U

/* This is User Data (as defined in test) */
typedef struct transaction_data
{
    char           s_txn_string[MAX_TXN_REF_LEN + 1U]; /*  null terminated string */
    int32_t        txn_amount;
    uint8_t        txn_type;
    uint16_t       txn_currency_code;
} txn_data_t;

txn_data_t txn_info;

/* This is User Data (as defined in test) */
typedef struct additional_transaction_data
{
    char           s_txn_tax_info[MAX_ADDNL_TXN_REF_LEN + 1U]; /*  null terminated string */
    int32_t        txn_tax;
    uint8_t        txn_tax_type;
    uint16_t       txn_tax_currency_code;
} addnl_txn_data_t;

addnl_txn_data_t addnl_txn_info;

void parse_data_received();

int main()
{

    tlv_initialize();

    printf("DEMO FOR ENCODING TLV OBJECTS\n");
    printf("\n");

    char work_string[MAX_TXN_REF_LEN + 1U] = { 'T', 'E', 'S', 'T', 'S', 'T', 'R', '\0' };
    for (uint32_t i = 0; i < (MAX_TXN_REF_LEN + 1U); i++)
    {
        txn_info.s_txn_string[i] = work_string[i];
    }
    txn_info.txn_amount = -5678;
    txn_info.txn_type = 0xEFU;
    txn_info.txn_currency_code = 0xEFFFU;

    if (TLV_SUCCESS == tlv_create_container_app_data(TAG_APP_TXN_INFO))
    {
        if (TLV_SUCCESS == tlv_init_and_create_app_data((uint8_t *)txn_info.s_txn_string, (MAX_TXN_REF_LEN), TAG_UTF8STRING))
        {
            if (TLV_SUCCESS == tlv_add_child_tag_to_container_app_data(TAG_APP_TXN_INFO, TAG_UTF8STRING))
            {
                printf("Child added successfully \n");
            }
        }

        if (TLV_SUCCESS == tlv_init_and_create_app_data((uint8_t *)&txn_info.txn_amount, sizeof(txn_info.txn_amount), TAG_INTEGER))
        {
            if (TLV_SUCCESS == tlv_add_child_tag_to_container_app_data(TAG_APP_TXN_INFO, TAG_INTEGER))
            {
                printf("Child added successfully \n");
            }
        }

        if (TLV_SUCCESS == tlv_init_and_create_app_data((uint8_t *)&txn_info.txn_type, sizeof(txn_info.txn_type), TAG_CONT_SPCF_UINT_8))
        {
            if (TLV_SUCCESS == tlv_add_child_tag_to_container_app_data(TAG_APP_TXN_INFO, TAG_CONT_SPCF_UINT_8))
            {
                printf("Child added successfully \n");
            }
        }

        if (TLV_SUCCESS == tlv_init_and_create_app_data((uint8_t *)&txn_info.txn_currency_code, sizeof(txn_info.txn_currency_code), TAG_CONT_SPCF_UINT_16))
        {
            if (TLV_SUCCESS == tlv_add_child_tag_to_container_app_data(TAG_APP_TXN_INFO, TAG_CONT_SPCF_UINT_16))
            {
                printf("Child added successfully \n");
            }
        }
    }

    printf("\n");

    printf("Send Container TLV Object - \n");
    tlv_app_data_send(TAG_APP_TXN_INFO);

    printf("\n");

    printf("Send String TLV Object - \n");
    tlv_app_data_send(TAG_UTF8STRING);

    printf("Send Integer TLV Object - \n");
    tlv_app_data_send(TAG_INTEGER);

    printf("Send context specific TLV Object 1 - \n");
    tlv_app_data_send(TAG_CONT_SPCF_UINT_8);

    printf("Send context specific TLV Object 2 - \n");
    tlv_app_data_send(TAG_CONT_SPCF_UINT_16);

    printf("\n");


    char work_string1[MAX_TXN_REF_LEN + 1U] = { 'M', 'O', 'D', 'I', 'F', 'I', 'D', '\0' };
    for (uint32_t i = 0; i < (MAX_TXN_REF_LEN + 1U); i++)
    {
        txn_info.s_txn_string[i] = work_string1[i];
    }
    txn_info.txn_amount = -1;
    txn_info.txn_type = 0xAAU;
    txn_info.txn_currency_code = 0x1234U;

    if (TLV_SUCCESS == tlv_add_data_to_app_data(TAG_INTEGER))
    {
        printf("Send modified Integer TLV Object - \n");
        tlv_app_data_send(TAG_INTEGER);
    }

    if (TLV_SUCCESS == tlv_add_data_to_app_data(TAG_CONT_SPCF_UINT_8))
    {
        printf("Send modified context specific TLV Object 1 - \n");
        tlv_app_data_send(TAG_CONT_SPCF_UINT_8);
    }

    if (TLV_SUCCESS == tlv_add_data_to_container_app_data(TAG_APP_TXN_INFO, TAG_UTF8STRING))
    {
        printf("Send modified String TLV Object - \n");
        tlv_app_data_send(TAG_UTF8STRING);
    }

    if (TLV_SUCCESS == tlv_add_data_to_container_app_data(TAG_APP_TXN_INFO, TAG_CONT_SPCF_UINT_16))
    {
        printf("Send modified context specific TLV Object 2 - \n");
        tlv_app_data_send(TAG_CONT_SPCF_UINT_16);
    }

    printf("\n");

    printf("Send modified Container TLV Object - \n");
    tlv_app_data_send(TAG_APP_TXN_INFO);
    printf("\n");
    printf("\n");


    if (TLV_SUCCESS == tlv_add_child_tag_to_container_app_data(TAG_APP_TXN_INFO, TAG_APP_TXN_INFO))
    {
        printf("Send Container TLV Object containing Container TLV Object - \n");
        tlv_app_data_send(TAG_APP_TXN_INFO);
        printf("\n");
        printf("\n");
    }
    else
    {
        printf("Cannot add container added as child to a container successfully (container cannot contain itself). \n");
        printf("\n");
        printf("\n");
    }


    char work_string2[MAX_ADDNL_TXN_REF_LEN + 1U] = { 'A', 'D', 'D', 'T', 'X', 'N', 'I', 'N', 'F', 'O', '\0' };
    for (uint32_t i = 0; i < (MAX_ADDNL_TXN_REF_LEN + 1U); i++)
    {
        addnl_txn_info.s_txn_tax_info[i] = work_string2[i];
    }
    addnl_txn_info.txn_tax = -12345;
    addnl_txn_info.txn_tax_type = 0xDDU;
    addnl_txn_info.txn_tax_currency_code= 0xBDAAU;

    if (TLV_SUCCESS == tlv_create_container_app_data(TAG_APP_ADDNL_TXN_INFO))
    {
        if (TLV_SUCCESS == tlv_add_child_to_container_app_data(TAG_APP_ADDNL_TXN_INFO, (uint8_t *)addnl_txn_info.s_txn_tax_info, (MAX_ADDNL_TXN_REF_LEN), TAG_UTF8STRING))
        {
            printf("Child added successfully \n");
        }

        if (TLV_SUCCESS == tlv_add_child_to_container_app_data(TAG_APP_ADDNL_TXN_INFO, (uint8_t *)&addnl_txn_info.txn_tax, sizeof(addnl_txn_info.txn_tax), TAG_INTEGER))
        {
            printf("Child added successfully \n");
        }

        if (TLV_SUCCESS == tlv_add_child_to_container_app_data(TAG_APP_ADDNL_TXN_INFO, (uint8_t *)&addnl_txn_info.txn_tax_type, sizeof(addnl_txn_info.txn_tax_type), TAG_INTEGER))
        {
            printf("Child added successfully \n");
        }

        if (TLV_SUCCESS == tlv_add_child_to_container_app_data(TAG_APP_ADDNL_TXN_INFO, (uint8_t *)&addnl_txn_info.txn_tax_currency_code, sizeof(addnl_txn_info.txn_tax_currency_code), TAG_INTEGER))
        {
            printf("Child added successfully \n");
        }
    }
    printf("\n");

    if (TLV_SUCCESS == tlv_add_child_tag_to_container_app_data(TAG_APP_TXN_INFO, TAG_APP_ADDNL_TXN_INFO))
    {
        printf("Send Container TLV Object containing Container TLV Object - \n");
        tlv_app_data_send(TAG_APP_TXN_INFO);
        printf("\n");
        printf("\n");
    }
    else
    {
        printf("Cannot add container added as child to a container successfully (container cannot contain itself). \n");
    }

    /* Rarsing received set of data. */
    parse_data_received();

    return 0;
}

/* String test. universal tag. */
uint8_t tlv_buffer_0[] = { 0xC , 0x7 , 'R' , 'E' , 'C' , 'E' , 'I' , 'V' , 'E' , '\0' };

/* String test. universal tag. */
uint8_t tlv_buffer_1[] = { 0xC , 0x4 , 'R' , 'E' , 'C' , 'E' , '\0', '\0', '\0', '\0' };

/* Container test. application tag. */
uint8_t tlv_buffer_2[] = { 0x0 , 0x0 , 0x7F , 0x20 , 0x80 , 0xC , 0x7 , 0x4D , 0x4F , 0x44 , 0x49 , 
0x46 , 0x49 , 0x44 , 0x2 , 0x1 , 0xFF , 0xBF , 0x81 , 0x20 , 0x1 , 0xAA , 0xBF , 0x81 , 0x1F ,
0x2 , 0x34 , 0x12 , 0x7F , 0x21 , 0x80 , 0xC , 0xA , 0x41 , 0x44 , 0x44 , 0x54 , 0x58 , 0x4E , 0x49 , 
0x4E , 0x46 , 0x4F , 0x2 , 0x2 , 0xC7 , 0xCF , 0x2 , 0x1 , 0x88 , 0x2 , 0x2 , 0x11 , 0x11 , 0x0 ,
0x0 , 0x0 };

/* Integer (32 bit) test. universal tag. */
uint8_t tlv_buffer_3[] = { 0x0, 0x2, 0x1, 0xFE };

/* Unsigned integer (16 bit) test. application context specific tag. */
uint8_t tlv_buffer_4[] = { 0xBF, 0x81, 0x1F, 0x2, 0x77, 0x77 };

/* Container test. application tag. */
uint8_t tlv_buffer_5[] = { 0x0 , 0x0 , 0x7F , 0x20 , 0x80 , 0xC , 0x7 , 0x4D , 0x4F , 0x44 , 0x49 ,
0x46 , 0x49 , 0x44 , 0x2 , 0x1 , 0xFF , 0xBF , 0x81 , 0x20 , 0x1 , 0xAA , 0xBF , 0x81 , 0x1F ,
0x2 , 0x34 , 0x12 , 0x7F , 0x21 , 0x80 , 0xC , 0xA , 0x41 , 0x44 , 0x44 , 0x54 , 0x58 , 0x4E , 0x49 ,
0x4E , 0x46 , 0x4F , 0x2 , 0x2 , 0xF7 , 0xFF , 0x2 , 0x1 , 0x77 , 0x2 , 0x2 , 0x33 , 0x33 , 0x0 ,
0x0 , 0x0 };

void parse_data_received()
{
    /*
     * - This demo shows parsing of the TLV buffer
     * - Say for example a buffer is received from the serial communication channel
     * - Application to get the tag received using tlv_parse_app_data().
     * - The tlv_parse_app_data() find the first valid tag and decodes the TLV encoded data, else returns error tag not found or bad TLV data buffer
     * - If the tag found of a definite length then the application is gets with tag received and associated value updated in the application variable.
     *
     * NOTE: If the tag found is of indefinite length (container type), then the application gets the tag received and the application should call tlv_search_parse_app_data() api
     * to parse a child tag in the tag found with indefinite length (container type). Application can also parse the entire tag found with indefinite length (container type)
     * with all its child tags by calling tlv_search_parse_app_data() api.
     */

    printf("DEMO FOR PARSING TLV OBJECTS\n");
    printf("\n");

    /* String test. universal tag. */
    uint32_t tag = 0;
    uint32_t buffer_length = sizeof(tlv_buffer_0);
    if (TLV_SUCCESS == tlv_parse_app_data(tlv_buffer_0, buffer_length, &tag))
    {
        if (tag == TAG_UTF8STRING)
        {
            printf("tlv_buffer_0: Received TAG (%d) - TAG_UTF8STRING: ", tag);
            uint32_t i = 0;
            while (txn_info.s_txn_string[i] != '\0')
            {
                printf("%c , ", txn_info.s_txn_string[i]);
                i++;
            }
        }
        printf("\n");
        printf("\n");
    }

    /* String test. universal tag. */
    buffer_length = sizeof(tlv_buffer_1);
    if (TLV_SUCCESS == tlv_parse_app_data(tlv_buffer_1, buffer_length, &tag))
    {
        if (tag == TAG_UTF8STRING)
        {
            printf("tlv_buffer_1: Received TAG (%d) - TAG_UTF8STRING: ", tag);
            uint32_t i = 0;
            while (txn_info.s_txn_string[i] != '\0')
            {
                printf("%c , ", txn_info.s_txn_string[i]);
                i++;
            }
        }
        printf("\n");
        printf("\n");
    }

    /* Container test. application tag. */
    buffer_length = sizeof(tlv_buffer_2);
    if (TLV_SUCCESS == tlv_parse_app_data(tlv_buffer_2, buffer_length, &tag))
    {
        if (tag == TAG_APP_TXN_INFO)
        {
            printf("tlv_buffer_2: Received Container TAG (%d) - TAG_APP_TXN_INFO: ", tag);
        }
        printf("\n");
        printf("\n");
    }

    /* Integer (32 bit) test. universal tag. */
    buffer_length = sizeof(tlv_buffer_3);
    if (TLV_SUCCESS == tlv_parse_app_data(tlv_buffer_3, buffer_length, &tag))
    {
        if (tag == TAG_INTEGER)
        {
            printf("tlv_buffer_3: Received TAG (%d) - TAG_INTEGER: ", tag);
            printf("Integer value is: %d", txn_info.txn_amount);
        }
        printf("\n");
        printf("\n");
    }

    /* Unsigned integer (16 bit) test. application context specific tag. */
    buffer_length = sizeof(tlv_buffer_4);
    if (TLV_SUCCESS == tlv_parse_app_data(tlv_buffer_4, buffer_length, &tag))
    {
        if (tag == TAG_CONT_SPCF_UINT_16)
        {
            printf("tlv_buffer_4: Received TAG (%d) - TAG_INTEGER_UNSIGNED: ", tag);
            printf("Integer value is: %d", txn_info.txn_currency_code);
        }
        printf("\n");
        printf("\n");
    }

    /* Search and decode a child (primitive) in a container with recursive = FALSE */
    buffer_length = sizeof(tlv_buffer_2);
    tag = TAG_CONT_SPCF_UINT_8;
    if (TLV_SUCCESS == tlv_search_parse_app_data(tlv_buffer_2, buffer_length, tag, FALSE))
    {
        printf("tlv_buffer_4: Received TAG (%d) - TAG_CONT_SPCF_UINT_8: ", tag);
        printf("Integer value is: %d", txn_info.txn_type);
    }
    else
    {
        printf("could not find TAG_CONT_SPCF_UINT_8 as recursive was set to FALSE; try again with TRUE");
    }
    printf("\n");
    printf("\n");
    /* Search and decode a child (primitive) in a container with recursive = TRUE */
    if (TLV_SUCCESS == tlv_search_parse_app_data(tlv_buffer_2, buffer_length, tag, TRUE))
    {
        printf("Recursive set to TRUE:\n");
        printf("tlv_buffer_4: Received TAG (%d) - TAG_CONT_SPCF_UINT_8: ", tag);
        printf("Integer value is: %d", txn_info.txn_type);
    }
    printf("\n");
    printf("\n");

    /* Search and decode a container (constructed) in a container with recursive = FALSE */
    buffer_length = sizeof(tlv_buffer_2);
    tag = TAG_APP_ADDNL_TXN_INFO;
    if (TLV_SUCCESS == tlv_search_parse_app_data(tlv_buffer_2, buffer_length, tag, FALSE))
    {
        printf("tlv_buffer_4: Received TAG (%d) - TAG_APP_ADDNL_TXN_INFO: \n", tag);

        uint32_t i = 0;
        while (addnl_txn_info.s_txn_tax_info[i] != '\0')
        {
            printf("%c , ", addnl_txn_info.s_txn_tax_info[i]);
            i++;
        }
        printf("\n");
        printf("Integer value is: %d \n", addnl_txn_info.txn_tax);
        printf("Integer value is: %d \n", addnl_txn_info.txn_tax_currency_code);
        printf("Integer value is: %d \n", addnl_txn_info.txn_tax_type);
    }
    else
    {
        printf("could not find TAG_APP_ADDNL_TXN_INFO as recursive was set to FALSE; try agian with TRUE");
    }
    printf("\n");
    printf("\n");
    /* Search and decode a container (constructed) in a container with recursive = TRUE */
    if (TLV_SUCCESS == tlv_search_parse_app_data(tlv_buffer_2, buffer_length, tag, TRUE))
    {
        printf("Recursive set to TRUE:\n");
        printf("tlv_buffer_4: Received TAG (%d) - TAG_APP_ADDNL_TXN_INFO: \n", tag);

        uint32_t i = 0;
        while (addnl_txn_info.s_txn_tax_info[i] != '\0')
        {
            printf("%c , ", addnl_txn_info.s_txn_tax_info[i]);
            i++;
        }
        printf("\n");
        printf("Integer value is: %d \n", addnl_txn_info.txn_tax);
        printf("Integer value is: %d \n", addnl_txn_info.txn_tax_currency_code);
        printf("Integer value is: %d \n", addnl_txn_info.txn_tax_type);
    }
    printf("\n");
    printf("\n");

    /* Search and decode a container (@ root level in TLV buffer) containing container and primitives with recursive = FALSE */
    buffer_length = sizeof(tlv_buffer_5);
    tag = TAG_APP_TXN_INFO;
    if (TLV_SUCCESS == tlv_search_parse_app_data(tlv_buffer_5, buffer_length, tag, FALSE))
    {
        printf("Recursive set to FALSE:\n");
        printf("tlv_buffer_4: Received TAG (%d) - TAG_APP_TXN_INFO: \n", tag);

        uint32_t i = 0;
        while (txn_info.s_txn_string[i] != '\0')
        {
            printf("%c , ", txn_info.s_txn_string[i]);
            i++;
        }
        printf("\n");
        printf("Integer value is: %d \n", txn_info.txn_amount);
        printf("Integer value is: %d \n", txn_info.txn_currency_code);
        printf("Integer value is: %d \n", txn_info.txn_type);


        while (addnl_txn_info.s_txn_tax_info[i] != '\0')
        {
            printf("%c , ", addnl_txn_info.s_txn_tax_info[i]);
            i++;
        }
        printf("\n");
        printf("Integer value is: %d \n", addnl_txn_info.txn_tax);
        printf("Integer value is: %d \n", addnl_txn_info.txn_tax_currency_code);
        printf("Integer value is: %d \n", addnl_txn_info.txn_tax_type);
    }
    printf("\n");
    printf("\n");
}
