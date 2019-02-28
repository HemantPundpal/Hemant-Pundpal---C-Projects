/*
 * Name: tlv_app_data_tag_check.h
 *
 * Description:
 * All definitions and api function declarations required for the TLV app data layer to check the tags and associated constraints.
 *
 *
 * Author: Hemant Pundpal                                   Date: 27 Feb 2019
 *
 */

#ifndef __TLV_APP_DATA_TAG_CHECK_H__
#define __TLV_APP_DATA_TAG_CHECK_H__

 /*
  * Internal function - Error check tag number.
  * Note: currently only few tags are supported thus this tag check function is sufficient.
  */
uint32_t check_tag(uint32_t tag_number, uint32_t app_data_size);



#endif /* __TLV_APP_DATA_TAG_CHECK_H__ */