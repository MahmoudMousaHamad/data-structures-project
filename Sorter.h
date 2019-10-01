// Signature must be retained. Implementation must be added.

#ifndef SORT_H
#define SORT_H

#include "Exceptions.h"
#include "ResizableArray.h"
#include "Comparator.h"

template <typename T>
class Sorter {
private:
    // additional member functions (methods) and variables (fields) may be added
    void merge(ResizableArray<T>& array, unsigned long left, unsigned long middle, unsigned long right, Comparator<T>& comparator);
    void mergesort(ResizableArray<T>& array, unsigned long left, unsigned long right, Comparator<T>& comparator);
public:
    static void sort(ResizableArray<T>& array, const Comparator<T>& comparator);
};

// merges two subarrays in array
// First subarray is array[left..middle]
// Second subarray is array[middle+1..right]
template <typename T>
void Sorter<T>::merge(ResizableArray<T>& array, unsigned long left, unsigned long middle, unsigned long right, Comparator<T>& comparator)
{
    // create temp subarrays
    int lenl, lenr;
    lenl = middle - left + 1;
    lenr = right - middle;
    unsigned long L[lenl], R[lenr];

    for (unsigned long i = 0; i < lenl; i++)
    {
        L[i] = array.get(left + i);
    }

    for (unsigned long i = 0; i < lenr; i++)
    {
        R[i] = array.get(middle + i + 1);
    }

    unsigned long i, j, k;
    // initial index of left subarray
    i = 0;
    // initial index of right subarray
    j = 0;
    // initial index of merged array
    k = left;

    while (i < lenl && j < lenr)
    {
        int result = comparator.compare(L[i], R[j]);
        if (result <= 0)
        {
            array.replace(L[i], k);
        }
        else 
        {
            array.replace(R[j], k);
        }
        k++;
    }

    // copy remaining elements
    while (i < lenl)
    {
        array.replace(L[i], k);
        i++;
        k++;
    }

    while (j < lenl)
    {
        array.replace(R[j], k);
        j++;
        k++;
    }
}

template <typename T>
void Sorter<T>::mergesort(ResizableArray<T>& array, unsigned long left, unsigned long right, Comparator<T>& comparator)
{
    if (right > left )
    {
        unsigned long middle = (right + left) / 2;
        mergesort(array, left, middle, comparator);
        mergesort(array, middle + 1, right, comparator);
        merge(array, left, middle, right, comparator);
    }
}

template <typename T>
void Sorter<T>::sort(ResizableArray<T>& array, const Comparator<T>& comparator)
{
    mergesort(array, 0, array.getSize() - 1, comparator);
}

#endif