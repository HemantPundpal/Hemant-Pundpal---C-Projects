/*
 * Name: hs_main.c
 *
 * Description:
 * THIS IS A DEMO CODE JUST TO EXCERCISE / DEMONSTRATE THE HEAP SORT ALGORITHM
 *
 * NOTE: This code may not follow all coding standards.
 *
 * Author: Hemant Pundpal                            Date: 29 Mar 2019
 *
 */
#include "heap_sort.h"

static void build_and_sort_heap(int32_t Arr[], uint32_t iCount);
static void print_array(int32_t Arr[], uint32_t iCount);

int32_t Array[] = { 33, 45, 56, 2, 1, 1, 0, 0, 8, 8, 54, 7, 9, 21, 5, 8, -1, -33, -45 };
int32_t Array1[] = { 33 };
int32_t Array2[] = { -33, -45 };
int32_t Array3[] = { 8, -33, -45 };
int32_t Array4[] = { 8, -33, -45, 0 };


int main(void)
{
    printf("Hello World!\n");

    build_and_sort_heap(Array, (sizeof(Array) / sizeof(uint32_t)));
    build_and_sort_heap(Array1, (sizeof(Array1) / sizeof(uint32_t)));
    build_and_sort_heap(Array2, (sizeof(Array2) / sizeof(uint32_t)));
    build_and_sort_heap(Array3, (sizeof(Array3) / sizeof(uint32_t)));
    build_and_sort_heap(Array4, (sizeof(Array4) / sizeof(uint32_t)));

    printf("Sorted Array: \n");

    print_array(Array, (sizeof(Array) / sizeof(uint32_t)));
    print_array(Array1, (sizeof(Array1) / sizeof(uint32_t)));
    print_array(Array2, (sizeof(Array2) / sizeof(uint32_t)));
    print_array(Array3, (sizeof(Array3) / sizeof(uint32_t)));
    print_array(Array4, (sizeof(Array4) / sizeof(uint32_t)));

    _getch();

    return 0;
}

static void build_and_sort_heap(int32_t Arr[], uint32_t iCount)
{
    /* Build Heap */
    build_heap(Arr, iCount);

    /*Sort Heap*/
    heap_sort(Arr, iCount);
}

static void print_array(int32_t Arr[], uint32_t iCount)
{
    for (uint32_t i = 0; i < iCount; i++)
    {
        printf("%d", Arr[i]);
        if (i < (iCount - 1))
        {
            printf(", ");
        }
    }
    printf("\n");
}
