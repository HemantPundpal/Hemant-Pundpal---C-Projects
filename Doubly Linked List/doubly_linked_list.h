/*
 * Name: doubly_linked_list.h
 *
 * Description:
 * All definitions and api function declarations required for the Doubly Linked List.
 *
 * Author: Hemant Pundpal                                   Date: 01 Apr 2019
 *
 */

#ifndef __DOUBLY_LINKED_LIST_H__
#define __DOUBLY_LINKED_LIST_H__

#include "doubly_ll_port.h"

typedef struct Cell
{
    uint32_t value;
    struct Cell * p_cell_next;
    struct Cell * p_cell_prev;
}cell_t;

bool_t ConstuctLinkedList(); 
bool_t AddSorted(cell_t * p_cell);
bool_t RemoveCell(uint32_t value);
bool_t ReverseList(cell_t * p_cell_start);
bool_t FindCell(uint32_t value, cell_t ** p_found_cell);
void PrintLinkedList(cell_t * p_cell);


#endif /* __DOUBLY_LINKED_LIST_H__ */
