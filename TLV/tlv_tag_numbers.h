/*
 * Name: tlv_tag_numbers.h
 *
 * Description:
 * All definitions of TLV tag numbers, range and any associated constraints.
 * Application should configure application tag numbers in this header file.
 *
 * CAUTON: Application should not modify the universal tag numbers in range 0 to 30.
 * (Universal tags will be moved to a separate header file to avoid accidental update.)
 *
 * Author: Hemant Pundpal                                   Date: 21 Feb 2019
 *
 */

#ifndef __TLV_TAG_NUMBERS_H__
#define __TLV_TAG_NUMBERS_H__

/******** BER UNIVERSAL TAG NUMBERS ***********/
/* Universal tag numbers are encoded in 1 byte(8 bits) TLV object tags. */

/*
 * - Universal class tag numbers range <0 to 30>.
 * - Application class tag numbers are encoded in 1 byte (8 bits) TLV object tags
 * - CAUTION: do not modify universal tag numbers.
 */
#define TAG_END_OF_CONTENT        0U /* Supported */
#define TAG_BOOLEAN               1U
#define TAG_INTEGER               2U  /* Supported */
#define TAG_BIT_STRING            3U
#define TAG_OCTET_STRING          4U
#define TAG_NULL                  5U
#define TAG_OBJECT_IDENTIFIER     6U
#define TAG_OBJECT_DESCRIPTOR     7U
#define TAG_EXTERNAL              8U
#define TAG_REAL                  9U
#define TAG_ENUMERATED            10U
#define TAG_EMBEDDED_PDV          11U
#define TAG_UTF8STRING            12U /* Supported */
#define TAG_RELATIVE_OID          13U
#define TAG_RESERVED_14           14U
#define TAG_RESERVED_15           15U
#ifdef __TLV_APP_DATA_API_H__
#define TAG_INTEGER_UNSIGNED      14U
#endif
#define TAG_SEQUENCE_OF           16U
#define TAG_SET_OF                17U
#define TAG_NUMERICSTRING         18U
#define TAG_PRINTABLESTRING       19U
#define TAG_T61STRING             20U
#define TAG_VIDEOTEXSTRING        21U
#define TAG_IA5STRING             22U
#define TAG_UTCTIME               23U
#define TAG_GENERALIZEDTIME       24U
#define TAG_GRAPHICSTRING         25U
#define TAG_VISIBLESTRING         26U
#define TAG_GENERALSTRING         27U
#define TAG_UNIVERSALSTRING       28U
#define TAG_CHARACTERSTRING       29U
#define TAG_BMPSTRING             30U
 /* Max universal tags. */
#define TAG_MAX_UNIVERSAL         30U


/* Maximum integer size allowed. */
#define MAX_INTEGER_BYTES           8U /* CAUTION: Can be 4 OR 8; default 8 */
#define MAX_UTF8STRING_LENGTH       25U /* CAUTION: Max 65535U. */


/********** APPLICATION TAG NUMBERS ***********/
/* Application tag numbers are in range <32 to 254> (127). */
/* CAUTION: Application should not have tag numbers 0 to 31 to avoid conflict with universal tag numbers */
/* Application tag numbers are encoded in 2 byte or 3 byte (16 bits or 24 bits) TLV object tags. */

/*
 * - Application class class tag numbers range <32 to 158>.
 */
#define TAG_APP_TXN_INFO            32U /* Supported. */
/* Add more application or context specific tag numbers here. */

#define TAG_APP_CLS_MAX             158U

/*
 * - Context specific class tag numbers range <159 to 254>.
 */
/* Add context specific tag numbers here. */
#define TAG_CONT_SPCF_UINT_16       159U
#define TAG_CONT_SPCF_UINT_8        160U

#define TAG_CONT_SPCF_CLS_MAX       254U

 /* Max application tag numbers */
#define TAG_MAX                     254U /* CAUTION: Do not change the max tags allowed. */

/* Note: more than 2 byte TLV object tags are not supported in this implementation. */


#define MAX_CONTAINER_CHILD_COUNT   10U
#define APP_TAG_MAX_VALUE_LENGTH    65535U

#endif /* __TLV_TAG_NUMBERS_H__ */
