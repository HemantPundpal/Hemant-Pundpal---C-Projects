/*
 * Name: error_assert.c
 *
 * Description:
 * All error assert functions and are defined in this file.
 *
 * Author: Hemant Pundpal						Date: 12 Feb 2019
 *
 */

#include "error_assert.h"

/* Assert Error with / without Abort. */
void _assert(const char *mes, const char *fname, uint32_t lno)
{
    fprintf(stderr, "Assert: (%s): --file: %s: --Line: (%d) \r \n", mes, fname, lno);
    fflush(stderr);
#if (0 < ERROR_ASSERT_ABORT)
        abort();
#endif /* ERROR_ASSERT_ABORT */

}
