/*
 * Name: singly_ll_main.c
 *
 * Description:
 * THIS IS A DEMO CODE JUST TO EXCERCISE / DEMONSTRATE THE SINGLY LINKED LIST
 *
 * NOTE: This code may not follow all coding standards.
 *
 * Author: Hemant Pundpal                            Date: 01 Apr 2019
 *
 */

#include "singly_linked_list.h"

extern cell_t * p_cell_list_start;

int32_t main(void)
{
    printf("Hello World!\n");

    cell_t cell1;
    cell_t cell2;
    cell_t cell3;
    cell_t cell4;
    cell_t cell5;
    cell_t cell6;
    cell_t cell7;
    cell_t cell8;

    cell1.value = 1;
    cell2.value = 2;
    cell3.value = 3;
    cell4.value = 4;
    cell5.value = 5;
    cell6.value = 6;
    cell7.value = 7;
    cell8.value = 8;

    if (AddtoList(&cell1) &&
        AddtoList(&cell2) &&
        AddtoList(&cell3) &&
        AddtoList(&cell4) &&
        AddtoList(&cell5) &&
        AddtoList(&cell6) &&
        AddtoList(&cell7))
    {
        printf("Cells added sorted to singly linked list.\n");
    }
    PrintList();
    printf("\n");

    if (ReverseList())
    {
        printf("Singly linked list is reversed.\n");
    }
    PrintList();
    printf("\n");

    if (AddAfterListItem(7, &cell8))
    {
        printf("Node added to the list after 7.\n");
    }
    PrintList();
    printf("\n");

    if (RemoveFromList(4))
    {
        printf("Removed from the list - 4.\n");
    }
    else
    {
        printf("Number not found in the list.\n");
    }
    PrintList();
    printf("\n");

    cell_t* CellArr[10];
    cell_t* p_temp_cell = p_cell_list_start;
    int32_t value;
    for (value = 0; p_temp_cell->p_cell_next; value++)
    {
        CellArr[value] = p_temp_cell;
        p_temp_cell = p_temp_cell->p_cell_next;
    }
    CellArr[value] = p_temp_cell;
    value++;

    printf("Use merge sort to sort the singly linked list.\n");
    MergeSort(CellArr, value);

    int32_t iCount = value;
    p_cell_list_start = NULL;
    for (value = 0; value < iCount; value++)
    {
        AddtoList(CellArr[value]);
    }
    PrintList();
    printf("\n");

    cell_t cell0;
    cell0.value = 0;
    if (AddSorted(&cell0))
    {
        printf("Node added sorted to the list - 0\n");
    }
    PrintList();
    printf("\n");

    if (AddSorted(&cell4))
    {
        printf("Node added sorted to the list - 4\n");
    }
    PrintList();
    printf("\n");

    if (RemoveFromList(0))
    {
        printf("Removed from the list - 0.\n");
    }
    else
    {
        printf("Number not found in the list.\n");
    }
    PrintList();
    printf("\n");

    _getch();

    return 0;
}
