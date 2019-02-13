/*
 * Name: asssert_error.h
 *
 * Description:
 * '_assert' or '_assert_abort' macros are defined in this header file.
 * '_assert' can be enabled or disabled with help of ERROR_ASSERT.
 * '_assert_abort' can be enabled or disabled with help of ERROR_ASSERT_ABORT.
 *
 * Author: Hemant Pundpal						Date: 12 Feb 2019
 *
 */

#ifndef __ERROR_ASSERT__
#define __ERROR_ASSERT__


#include "ring_buffer_api.h"

#if (0 < ERROR_ASSERT)
#define assert(exp) if(exp) {\
_assert(#exp, __FILE__, __LINE__);\
}
#else
#define assert(exp) 0 /* no action */
#endif /* ERROR_ASSERT */

void _assert(const char *mes, const char *fname, uint32_t lno);

#endif /* __ERROR_ASSERT__ */
