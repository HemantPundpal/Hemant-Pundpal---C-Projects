/*
 * Name: asssert_error.h
 *
 * Description:
 * 'assert' macros are defined in this header file.
 * 'assert' can be with or without abort().
 *
 * Author: Hemant Pundpal						Date: 16 Feb 2019
 *
 */

#ifndef __ERROR_ASSERT__
#define __ERROR_ASSERT__


#if (0 < ERROR_ASSERT)
#define assert(exp) if(exp) {\
_assert(#exp, __FILE__, __LINE__);\
}
#else
#define assert(exp) 0 /* no action */
#endif /* ERROR_ASSERT */

void _assert(const char *mes, const char *fname, uint32_t lno);

#endif /* __ERROR_ASSERT__ */
