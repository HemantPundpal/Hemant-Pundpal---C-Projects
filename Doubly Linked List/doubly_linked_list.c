/*
 * Name: doubly_linked_list.c
 *
 * Description:
 * All API definitions required for the Doubly Linked List.
 *
 * Author: Hemant Pundpal                                   Date: 01 Apr 2019
 *
 */

#include "doubly_linked_list.h"

cell_t * p_linked_link_start = NULL; /* Sentinel start */
cell_t * p_linked_link_end = NULL; /* Sentinel end */

static cell_t * FindSorted(cell_t ** p_cell_arr, uint32_t iCount, uint32_t value);

bool_t ConstuctLinkedList()
{
    bool_t b_status = FALSE;

    if (!p_linked_link_start && !p_linked_link_end)
    {
        p_linked_link_start = (cell_t*)calloc(1, sizeof(cell_t));
        p_linked_link_end = (cell_t*)calloc(1, sizeof(cell_t));

        if (p_linked_link_start && p_linked_link_end)
        {
            p_linked_link_start->value = 0x0U;
            p_linked_link_start->p_cell_next = p_linked_link_end;
            p_linked_link_start->p_cell_prev = NULL;

            p_linked_link_end->value = 0xFFFFFFFFU;
            p_linked_link_end->p_cell_prev = p_linked_link_start;
            p_linked_link_end->p_cell_next = NULL;
            b_status = TRUE;
        }
    }
    return b_status;
}


bool_t AddSorted(cell_t * p_cell)
{
    bool_t b_status = FALSE;

    if (p_cell)
    {
        if (p_linked_link_start && p_linked_link_end)
        {
            cell_t * p_temp_cell = p_linked_link_start;

            while (p_temp_cell->p_cell_next)
            {
                if (p_temp_cell->p_cell_next->value >= p_cell->value)
                {
                    p_cell->p_cell_next = p_temp_cell->p_cell_next;
                    p_cell->p_cell_prev = p_temp_cell;
                    p_temp_cell->p_cell_next->p_cell_prev = p_cell;
                    p_temp_cell->p_cell_next = p_cell;
                    b_status = TRUE;
                    break;
                }
                else
                {
                    p_temp_cell = p_temp_cell->p_cell_next;
                }
            }
        }
    }
    return b_status;
}


bool_t RemoveCell(uint32_t value)
{
    bool_t b_status = FALSE;

    if (p_linked_link_start && p_linked_link_end)
    {
        cell_t * p_temp_cell = p_linked_link_start;
        while (p_temp_cell->p_cell_next)
        {
            if (p_temp_cell->p_cell_next->value == value)
            {
                p_temp_cell->p_cell_next->p_cell_next->p_cell_prev = p_temp_cell;
                p_temp_cell->p_cell_next = p_temp_cell->p_cell_next->p_cell_next;
                b_status = TRUE;
                break;
            }
            else
            {
                p_temp_cell = p_temp_cell->p_cell_next;
            }
        }
    }
    return b_status;
}


bool_t ReverseList(cell_t * p_cell_start)
{
    bool_t b_status = FALSE;

    if (!p_cell_start->p_cell_prev)
    {
        cell_t * p_temp_cell = p_cell_start;
        cell_t * p_temp_prev = NULL;
        while (p_temp_cell->p_cell_next)
        {
            p_temp_prev = p_temp_cell->p_cell_prev;
            p_temp_cell->p_cell_prev = p_temp_cell->p_cell_next;
            p_temp_cell->p_cell_next = p_temp_prev;

            p_temp_cell = p_temp_cell->p_cell_prev;
        }
        p_temp_cell->p_cell_next = p_temp_cell->p_cell_prev;
        p_temp_cell->p_cell_prev = NULL;

        b_status = TRUE;
    }
    return b_status;
}


static cell_t * FindSorted(cell_t ** p_cell_arr, uint32_t iCount, uint32_t value)
{
    cell_t * found_cell_pointer = NULL;
    if (p_cell_arr[(iCount / 2)]->value == value)
    {
        found_cell_pointer = p_cell_arr[(iCount / 2)];
    }
    else if (p_cell_arr[(iCount / 2)]->value > value)
    {
        if (iCount > 1)
        {
            found_cell_pointer = FindSorted(p_cell_arr, (iCount / 2), value);
        }
    }
    else
    {
        if (iCount > 1)
        {
            found_cell_pointer = FindSorted(&p_cell_arr[(iCount / 2)], (iCount - (iCount / 2)), value);
        }
    }

    return found_cell_pointer;
}


bool_t FindCell(uint32_t value,  cell_t ** p_found_cell)
{
    bool_t b_status = FALSE;
    uint32_t iCount;
    cell_t * p_temp_cell = p_linked_link_start;
    cell_t * p_cell_arr[20];

    if (p_temp_cell->p_cell_next)
    {
        for (iCount = 0; p_temp_cell->p_cell_next->p_cell_next; p_temp_cell = p_temp_cell->p_cell_next)
        {
            p_cell_arr[iCount++] = p_temp_cell->p_cell_next;
        }

        cell_t * found_cell_pointer = NULL;
        found_cell_pointer = FindSorted(p_cell_arr, iCount, value);
        if (found_cell_pointer)
        {
            *p_found_cell = found_cell_pointer;
            b_status = TRUE;
        }
    }
    return b_status;
}


void PrintLinkedList(cell_t * p_cell)
{
    if (p_cell)
    {
        cell_t * pCellTemp = p_cell;
        while (pCellTemp->p_cell_next->p_cell_next)
        {
            printf("%d, ", pCellTemp->p_cell_next->value);
            pCellTemp = pCellTemp->p_cell_next;
        }
        printf("\n");
    }
}
