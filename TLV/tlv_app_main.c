
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

/* This is User Data (as defined in test) */
typedef struct transaction_data
{
    char           s_txn_string[MAX_TXN_REF_LEN + 1U]; /*  null terminated string */
    int32_t        txn_amount;
    uint8_t        txn_type;
    uint16_t       txn_currency_code;
} txn_data_t;

txn_data_t txn_info;

int main()
{

    tlv_initialize();

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
        if (TLV_SUCCESS == tlv_init_and_create_app_data((uint8_t *)txn_info.s_txn_string, (MAX_TXN_REF_LEN + 1U), TAG_UTF8STRING))
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
}
