/*
 * Name: merge_sort.c
 *
 * Description:
 * All API definitions required for the Merge Sort Algorithm
 *
 * Author: Hemant Pundpal                                   Date: 29 Mar 2019
 *
 */

#include "merge_sort.h"

static void merge_sorted(int32_t ArrInLeft[], uint32_t lenLeft, int32_t ArrInRight[], uint32_t lenRight);

void merge_sort(int32_t ArrIn[], uint32_t len)
{
    int32_t temp = 0;
    uint32_t i = 0;

    if (len == 2)
    {
        if (ArrIn[i] > ArrIn[i + 1])
        {
            temp = ArrIn[i];
            ArrIn[i] = ArrIn[i + 1];
            ArrIn[i + 1] = temp;
        }
    }
    else if (len >= 3)
    {
        merge_sort(ArrIn, (len - (len / 2)));
        merge_sort((ArrIn + (len - (len / 2))), (len / 2));

        merge_sorted(ArrIn, (len - (len / 2)), (ArrIn + (len - (len / 2))), (len / 2));
    }
}

static void merge_sorted(int32_t ArrInLeft[], uint32_t lenLeft, int32_t ArrInRight[], uint32_t lenRight)
{
    uint32_t i = 0;
    uint32_t j = 0;
    uint32_t k = 0;

    int32_t *tempArr = (int32_t *)calloc((lenLeft + lenRight), sizeof(int32_t));

    while ((i < lenLeft) && (j < lenRight))
    {
        if (ArrInLeft[i] < ArrInRight[j])
        {
            tempArr[k] = ArrInLeft[i];
            i++;
            k++;
        }
        else
        {
            tempArr[k] = ArrInRight[j];
            j++;
            k++;
        }
    }

    while (i < lenLeft)
    {
        tempArr[k] = ArrInLeft[i];
        i++;
        k++;
    }

    while (j < lenRight)
    {
        tempArr[k] = ArrInRight[j];
        j++;
        k++;
    }

    for (i = 0; i < (lenLeft + lenRight); i++)
    {
        ArrInLeft[i] = tempArr[i];
    }

    free(tempArr);
}
