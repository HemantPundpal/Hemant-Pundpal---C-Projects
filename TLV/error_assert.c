/*
 * Name: error_assert.c
 *
 * Description:
 * All error assert functions and are defined in this file.
 *
 * Author: Hemant Pundpal                     Date: 12 Feb 2019
 *
 */
#include "tlv_port.h"
#include "error_assert.h"

 /* Assert Error with / without Abort. */
void_t _assert(const char *p_message, const char *fp_name, uint32_t line_number)
{
    fprintf(stderr, "Assert: (%s): --file: %s: --Line: (%d) \r \n", p_message, fp_name, line_number);
    fflush(stderr);
#if (0 < ERROR_ASSERT_ABORT)
    abort();
#endif /* ERROR_ASSERT_ABORT */

}
