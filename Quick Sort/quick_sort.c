/*
 * Name: quick_sort.c
 *
 * Description:
 * All API definitions required for the Quick Sort Algorithm
 *
 * Author: Hemant Pundpal                                   Date: 29 Mar 2019
 *
 */

#include "quick_sort.h"

#define ONEARRAYELEMENT   1U
#define TWOARRAYELEMENT   2U
#define THREEARRAYELEMENT 3U

static void swap_value(int32_t * pValue1, int32_t * pValue2);

void quick_sort(int32_t ArrIn[], uint32_t len)
{
    if (len > ONEARRAYELEMENT)
    {
        if (ArrIn[0] > ArrIn[len / 2])
        {
            swap_value(&ArrIn[0], &ArrIn[len / 2]);
        }

        if (len > TWOARRAYELEMENT)
        {
            if (ArrIn[len / 2] > ArrIn[len - 1])
            {
                swap_value(&ArrIn[len / 2], &ArrIn[len - 1]);

                if (ArrIn[0] > ArrIn[len / 2])
                {
                    swap_value(&ArrIn[0], &ArrIn[len / 2]);
                }
            }
        }
    }


    if (len > THREEARRAYELEMENT)
    {
        int32_t iTemp = ArrIn[len / 2];
        uint32_t iFreeElement = len / 2;
        uint32_t iLeftElement = 0;
        uint32_t iRightElement = len - 1;

        while (iLeftElement < iFreeElement)
        {
            if (ArrIn[iLeftElement] > iTemp)
            {
                ArrIn[iFreeElement] = ArrIn[iLeftElement];
                iFreeElement = iLeftElement;
                while (iRightElement > iFreeElement)
                {
                    if (ArrIn[iRightElement] < iTemp)
                    {
                        ArrIn[iFreeElement] = ArrIn[iRightElement];
                        iFreeElement = iRightElement;
                        iRightElement--;
                        break;
                    }
                    iRightElement--;
                }
            }
            iLeftElement++;
        }

        ArrIn[iFreeElement] = iTemp;

        quick_sort(&ArrIn[0], iFreeElement);
        quick_sort(&ArrIn[(iFreeElement + 1)], (len - (iFreeElement + 1)));
    }
}

static void swap_value(int32_t * pValue1, int32_t * pValue2)
{
    int32_t iSwap = 0;

    iSwap = *pValue1;
    *pValue1 = *pValue2;
    *pValue2 = iSwap;
}
