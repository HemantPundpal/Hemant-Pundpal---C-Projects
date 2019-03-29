/*
 * Name: heap_sort.c
 *
 * Description:
 * All API definitions required for the Heap Sort Algorithm
 *
 * Author: Hemant Pundpal                                   Date: 29 Mar 2019
 *
 */

#include "heap_sort.h"

#define GetParent(iChild, iParent) if(iChild < 2){\
iParent = (uint32_t)0;\
} \
else \
{ \
if((iChild-1)%2) \
{\
iParent = (uint32_t)(((iChild-1)/2)+0.5);\
}\
else\
{\
iParent = (uint32_t)((iChild-1)/2);\
}\
}

static void swap_value(int32_t * pValue1, int32_t * pValue2);

/* Build Heap. */
void build_heap(int32_t Array[], uint32_t iCount)
{
    for (uint32_t i = 0; i < iCount; i++)
    {
        unsigned int iParent = 0;
        unsigned int iChild = i;
        GetParent(iChild, iParent);

        do
        {
            if (Array[iChild] > Array[iParent])
            {
                swap_value(&Array[iParent], &Array[iChild]);
            }
            iChild = iParent;
            GetParent(iChild, iParent);
        } while (iChild);
    }
}

/* Sort Heap. */
void heap_sort(int32_t Array[], uint32_t iCount)
{
    for (int32_t j = (iCount - 1); j >= 0; j--)
    {
        int32_t iParent = 0;
        int32_t iChild1 = (2 * iParent) + 1;
        int32_t iChild2 = (2 * iParent) + 2;

        swap_value(&Array[iParent], &Array[j]);

        while ((j > iChild1) || (j > iChild2))
        {
            if (j > iChild2)
            {
                if (Array[iChild1] <= Array[iChild2])
                {
                    if (Array[iParent] < Array[iChild2])
                    {
                        swap_value(&Array[iParent], &Array[iChild2]);
                        iParent = iChild2;
                    }
                    else
                    {
                        break;
                    }
                }
                else
                {
                    if (Array[iParent] < Array[iChild1])
                    {
                        swap_value(&Array[iParent], &Array[iChild1]);
                        iParent = iChild1;
                    }
                    else
                    {
                        break;
                    }
                }
                iChild1 = (2 * iParent) + 1;
                iChild2 = (2 * iParent) + 2;
            }
            else if (j > iChild1)
            {
                if (Array[iParent] < Array[iChild1])
                {
                    swap_value(&Array[iParent], &Array[iChild1]);
                    iParent = iChild1;
                }
                break;
            }
        }
    }
}

static void swap_value(int32_t * pValue1, int32_t * pValue2)
{
    int32_t iSwap = 0;

    iSwap = *pValue1;
    *pValue1 = *pValue2;
    *pValue2 = iSwap;
}
