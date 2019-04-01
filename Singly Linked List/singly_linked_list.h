/*
 * Name: singly_linked_list.h
 *
 * Description:
 * All definitions and api function declarations required for the Singly Linked List.
 *
 * Author: Hemant Pundpal                                   Date: 01 Apr 2019
 *
 */

#ifndef __SINGLY_LINKED_LIST_H__
#define __SINGLY_LINKED_LIST_H__

#include "singly_ll_port.h"

typedef struct Cell
{
    uint32_t value;
    struct Cell * p_cell_next;
}cell_t;

bool_t AddtoList(cell_t * cell);
bool_t RemoveFromList(uint32_t value);
bool_t AddAfterListItem(uint32_t value, cell_t * cell);
bool_t ReverseList();
bool_t PrintList();
bool_t AddSorted(cell_t * cell);
void MergeSort(cell_t * CellList[], uint32_t iCount);

#endif /* __SINGLY_LINKED_LIST_H__ */
