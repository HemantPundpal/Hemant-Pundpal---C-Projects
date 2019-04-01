/*
 * Name: singly_linked_list.c
 *
 * Description:
 * All API definitions required for the Singly Linked List.
 *
 * Author: Hemant Pundpal                                   Date: 01 Apr 2019
 *
 */
#include "singly_linked_list.h"

cell_t * p_cell_list_start = NULL;

static void MergeSorted(cell_t * CellList1[], uint32_t iCount1, cell_t * CellList2[], uint32_t iCount2);

bool_t AddtoList(cell_t * cell)
{
    bool_t b_status = FALSE;

    if (cell)
    {
        cell->p_cell_next = NULL;
        if (p_cell_list_start)
        {
            cell_t * p_temp_cell = p_cell_list_start;
            while (p_temp_cell->p_cell_next)
            {
                p_temp_cell = p_temp_cell->p_cell_next;
            }
            p_temp_cell->p_cell_next = cell;
            b_status = TRUE;
        }
        else
        {
            p_cell_list_start = cell;
            b_status = TRUE;
        }
    }
    return b_status;
}

bool_t RemoveFromList(uint32_t value)
{
    bool_t b_status = FALSE;

    if (p_cell_list_start)
    {
        if (p_cell_list_start->value == value)
        {
            p_cell_list_start = p_cell_list_start->p_cell_next;
            b_status = TRUE;
        }
        else
        {
            cell_t * p_temp_cell = p_cell_list_start->p_cell_next;
            cell_t * prevCell = p_cell_list_start;

            while (p_temp_cell)
            {
                if (p_temp_cell->value == value)
                {
                    prevCell->p_cell_next = p_temp_cell->p_cell_next;
                    b_status = TRUE;
                    break;
                }
                else
                {
                    prevCell = p_temp_cell;
                    p_temp_cell = p_temp_cell->p_cell_next;
                }
            }
        }
    }
    return b_status;
}


bool_t AddAfterListItem(uint32_t value, cell_t * cell)
{
    bool_t b_status = FALSE;

    if (cell)
    {
        cell_t * p_temp_cell = p_cell_list_start;
        while (p_temp_cell)
        {
            if (p_temp_cell->value == value)
            {
                cell->p_cell_next = p_temp_cell->p_cell_next;
                p_temp_cell->p_cell_next = cell;
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

bool_t ReverseList()
{
    bool_t b_status = FALSE;

    if (p_cell_list_start)
    {
        if (p_cell_list_start->p_cell_next)
        {
            cell_t * p_temp_cell = p_cell_list_start->p_cell_next;
            cell_t * p_current_cell = p_cell_list_start;
            cell_t * p_next_cell = p_temp_cell->p_cell_next;
            b_status = TRUE;

            while (p_temp_cell)
            {
                if (p_temp_cell->p_cell_next)
                {
                    p_next_cell = p_temp_cell->p_cell_next;
                }
                else
                {
                    p_next_cell = NULL;
                }
                p_temp_cell->p_cell_next = p_current_cell;
                p_current_cell = p_temp_cell;
                p_temp_cell = p_next_cell;
            }

            p_cell_list_start->p_cell_next = NULL;
            p_cell_list_start = p_current_cell;
        }
    }
    return b_status;
}

bool_t PrintList()
{
    bool_t b_status = FALSE;

    if (p_cell_list_start)
    {
        b_status = TRUE;
        cell_t * p_temp_cell = p_cell_list_start;

        do
        {
            printf("%d, ", p_temp_cell->value);
            p_temp_cell = p_temp_cell->p_cell_next;
        } while (p_temp_cell);
    }
    printf("\n");
    return b_status;
}


bool_t AddSorted(cell_t * cell)
{
    bool_t b_status = FALSE;

    if (cell)
    {
        cell->p_cell_next = NULL;
        if (p_cell_list_start)
        {
            if (p_cell_list_start->value > cell->value)
            {
                cell->p_cell_next = p_cell_list_start;
                p_cell_list_start = cell;
                b_status = TRUE;
            }
            else
            {
                cell_t * p_temp_cell = p_cell_list_start;
                while (p_temp_cell->p_cell_next && !b_status)
                {
                    if (p_temp_cell->p_cell_next->value > cell->value)
                    {
                        cell->p_cell_next = p_temp_cell->p_cell_next;
                        p_temp_cell->p_cell_next = cell;
                        b_status = TRUE;
                    }
                    p_temp_cell = p_temp_cell->p_cell_next;
                }
                if (b_status == FALSE)
                {
                    p_temp_cell->p_cell_next = cell;
                    b_status = TRUE;
                }
            }
        }
        else
        {
            p_cell_list_start = cell;
            b_status = TRUE;
        }
    }
    return b_status;
}


static void MergeSorted(cell_t * CellList1[], uint32_t iCount1, cell_t * CellList2[], uint32_t iCount2)
{

    cell_t ** p_arr_cell_sorted = NULL;
    p_arr_cell_sorted = (cell_t**)calloc((iCount1 + iCount2), sizeof(cell_t *));

    if (p_arr_cell_sorted)
    {

        uint32_t iTemp = 0;
        uint32_t jTemp = 0;
        uint32_t kTemp = 0;

        while ((iTemp < iCount1) && (jTemp < iCount2))
        {
            if (CellList1[iTemp]->value <= CellList2[jTemp]->value)
            {
                p_arr_cell_sorted[kTemp] = CellList1[iTemp];
                iTemp++;
                kTemp++;
            }
            else
            {
                p_arr_cell_sorted[kTemp] = CellList2[jTemp];
                jTemp++;
                kTemp++;
            }
        }

        for (; iTemp < iCount1; kTemp++, iTemp++)
        {
            p_arr_cell_sorted[kTemp] = CellList1[iTemp];
        }

        for (; jTemp < iCount2; kTemp++, jTemp++)
        {
            p_arr_cell_sorted[kTemp] = CellList2[jTemp];
        }

        for (kTemp = 0; kTemp < (iCount1 + iCount2); kTemp++)
        {
            CellList1[kTemp] = p_arr_cell_sorted[kTemp];
        }

        free(p_arr_cell_sorted);
    }
}


void MergeSort(cell_t * CellList[], uint32_t iCount)
{
    uint32_t iTemp = 0;

    if (CellList && iCount > 1)
    {
        if (2 == iCount)
        {
            cell_t * p_temp_cell;
            if (CellList[iTemp]->value > CellList[iTemp + 1]->value)
            {
                p_temp_cell = CellList[iTemp];
                CellList[iTemp] = CellList[iTemp + 1];
                CellList[iTemp + 1] = p_temp_cell;
            }
        }
        else
        {
            MergeSort(CellList, (iCount - (iCount / 2)));
            MergeSort(&CellList[(iCount - (iCount / 2))], (iCount / 2));

            MergeSorted(CellList, (iCount - (iCount / 2)), &CellList[(iCount - (iCount / 2))], (iCount / 2));
        }
    }
}
