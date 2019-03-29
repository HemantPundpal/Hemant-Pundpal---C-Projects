/*
 * Name: heap_sort.h
 *
 * Description:
 * All definitions and api function declarations required for the Heap Sort Algorithm.
 *
 * Author: Hemant Pundpal                                   Date: 29 Mar 2019
 *
 */
#ifndef __HEAP_SORT_H__
#define __HEAP_SORT_H__

#include "hs_port.h"

 /* Build Heap. */
void build_heap(int32_t Array[], uint32_t iCount);

/* Sort Heap. */
void heap_sort(int32_t Array[], uint32_t iCount);

#endif /* __HEAP_SORT_H__ */
