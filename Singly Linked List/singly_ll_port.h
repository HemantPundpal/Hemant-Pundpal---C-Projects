/*
 * Name: singly_ll_port.h
 *
 * Description:
 * Any platform specific definitions should be added to this file.
 * Application code need not #include this file to use the singly linked list API
 *
 * Author: Hemant Pundpal                               Date: 01 Apr 2019
 *
 */

#ifndef __DATA_TYPES__
#define __DATA_TYPES__


#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <conio.h>


/*
 * If the compiler recognize bool data type, then define BOOL_AWARE in this file.
 */
#ifndef int8_t
typedef char int8_t;
#endif /* int8_t */

#ifndef int16_t
typedef short int16_t;
#endif /* int16_t */

#ifndef int32_t
typedef int int32_t;
#endif /* int32_t */

#ifndef int64_t
typedef long long int int64_t;
#endif /* int64_t */

#ifndef uint8_t
typedef unsigned char uint8_t;
#endif /* uint8_t */

#ifndef uint16_t
typedef unsigned short uint16_t;
#endif /* uint16_t */

#ifndef uint32_t
typedef unsigned int uint32_t;
#endif /* uint32_t */

#ifndef uint64_t
typedef long long int uint64_t;
#endif /* uint64_t */

#ifndef void_t
typedef void void_t;
#endif /* void_t */

#ifndef bool_t

#ifndef BOOL_AWARE
typedef enum
{
    FALSE = 0x00000000,
    TRUE
} bool_t;
#else
typedef bool_t bool_t;
#define FALSE    false
#define TRUE     true
#endif /* BOOL_AWARE */



#endif /* bool_t */

#endif /* __DATA_TYPES__ */
