/*
 * Name: doubly_ll_main.c
 *
 * Description:
 * THIS IS A DEMO CODE JUST TO EXCERCISE / DEMONSTRATE THE DOUBLY LINKED LIST
 *
 * NOTE: This code may not follow all coding standards.
 *
 * Author: Hemant Pundpal                            Date: 01 Apr 2019
 *
 */

#include "doubly_linked_list.h"

extern cell_t * p_linked_link_start; /* Sentinel start */
extern cell_t * p_linked_link_end; /* Sentinel end */

int main()
{
    printf("Hello World!\n");

    if (ConstuctLinkedList())
    {
        printf("Doubly Linked List Constructed.\n\n");
    }

    cell_t cell1;
    cell_t cell3;
    cell_t cell16;
    cell_t cell25;
    cell_t cell51;
    cell_t cell37;
    cell_t cell23;
    cell_t cell35;
    cell_t cell12;
    cell_t cell11;
    cell_t cell8;
    cell_t cell9;

    cell1.value = 1;
    cell3.value = 3;
    cell16.value = 16;
    cell25.value = 25;
    cell51.value = 51;
    cell37.value = 37;
    cell23.value = 23;
    cell35.value = 35;
    cell12.value = 12;
    cell11.value = 11;
    cell8.value = 8;

    if (AddSorted(&cell1)
        && AddSorted(&cell3)
        && AddSorted(&cell16)
        && AddSorted(&cell25)
        && AddSorted(&cell51)
        && AddSorted(&cell37)
        && AddSorted(&cell23)
        && AddSorted(&cell35)
        && AddSorted(&cell12)
        && AddSorted(&cell11)
        && AddSorted(&cell8))
    {
        printf("Cells added sorted to doubly linked list.\n");
    }
    PrintLinkedList(p_linked_link_start);
    printf("\n");

    if (ReverseList(p_linked_link_start))
    {
        printf("Dcoubly linked list is reversed.\n");
    }
    PrintLinkedList(p_linked_link_end);
    printf("\n");

    if (ReverseList(p_linked_link_end))
    {
        printf("Dcoubly linked list is reversed again.\n");
    }
    PrintLinkedList(p_linked_link_start);
    printf("\n");

    cell_t * FoundCell;
    if (FindCell(11, &FoundCell))
    {
        printf("Found Cell: %d\n\n", FoundCell->value);
    }
    else
    {
        printf("Number not found - 11\n\n");
    }

    cell9.value = 8;
    if (AddSorted(&cell9))
    {
        printf("Cell 9 added sorted to doubly linked list.\n");
    }
    PrintLinkedList(p_linked_link_start);
    printf("\n");

    if (FindCell(52, &FoundCell))
    {
        printf("Found Cell: %d\n\n", FoundCell->value);
    }
    else
    {
        printf("Number not found - 52\n\n");
    }

    if (ReverseList(p_linked_link_start))
    {
        printf("Dcoubly linked list is reversed.\n");
    }
    PrintLinkedList(p_linked_link_end);
    printf("\n");

    if (ReverseList(p_linked_link_end))
    {
        printf("Dcoubly linked list is reversed again.\n");
    }
    PrintLinkedList(p_linked_link_start);
    printf("\n");

    _getch();

    return 0;
}