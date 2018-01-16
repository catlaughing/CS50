/**
 * helpers.c
 *
 * Helper functions for Problem Set 3.
 */

#include <cs50.h>

#include "helpers.h"

/**
 * Returns true if value is in array of n values, else false.
 */
bool search(int value, int values[], int n)
{
    // TODO: implement a searching algorithm
    int left = 0;
    int right = n-1;
    int ixmiddle;
    if (n <= 0)
    {
        return false;
    }
    while (left <= right)
    {
        ixmiddle = (right+left)/2;
        if (value == values[ixmiddle])
        {
            return true;
        }
        else if (value > values[ixmiddle])
        {
            left = ixmiddle + 1;
        }
        else if (value < values[ixmiddle])
        {
            right = ixmiddle - 1;
        }
    }
    return false;
}

/**
 * Sorts array of n values.
 */
void sort(int values[], int n)
{
    // TODO: implement a sorting algorithm
    int ixmin;
    int temp;
    for (int i = 0; i < n-1; i++)
    {
        ixmin = i;
        for (int j = i+1; j < n; j++)
        {
            if (values[j] < values[ixmin])
            {
                ixmin = j;
            }
        }
        temp = values[i];
        values[i] = values[ixmin];
        values[ixmin] = temp;
    }
    return;
}
