
/*
 * Name: asssert_error.h
 *
 * Description:
 * 'assert' macros are defined in this header file.
 * 'assert' can be with or without abort().
 *
 * Author: Hemant Pundpal                       Date: 12 Feb 2019
 *
 */

#ifndef __ERROR_ASSERT_H__
#define __ERROR_ASSERT_H__


#if (0 < ERROR_ASSERT)
#define assert(exp) if(exp) {\
_assert(#exp, __FILE__, __LINE__);\
}
#else
#define assert(exp) 0 /* no action */
#endif /* ERROR_ASSERT */

void_t _assert(const char *p_message, const char *fp_name, uint32_t line_number);

#endif /* __ERROR_ASSERT_H__ */

