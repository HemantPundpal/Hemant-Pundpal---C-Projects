/*
 * Name: tlv_app_data_ec.c
 *
 * Description:
 * All definitions required for the TLV app data layer to check the tags and associated constraints.
 *
 * Author: Hemant Pundpal                                   Date: 21 Feb 2019
 *
 */
#define TLV_APP_DATA_SOURCE_CODE

#include "tlv_app_data_api.h"
#include "tlv_app_data_tag_check.h"
#include "error_assert.h"

 /*
  * Internal function - Error check tag number.
  * Note: currently only few tags are supported thus this tag check function is sufficient.
  */
uint32_t check_tag(uint32_t tag_number, uint32_t app_data_size)
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
            uint8_t tag_invalid = 0U;
            assert(!tag_invalid);
            return TLV_TAG_NOT_SUPPORTED;
        }
    }

    return TLV_SUCCESS;
}
