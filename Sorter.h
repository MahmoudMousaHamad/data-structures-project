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
    static void merge(ResizableArray<T>& array, unsigned long left, unsigned long middle, unsigned long right, const Comparator<T>& comparator);
    static void mergesort(ResizableArray<T>& array, unsigned long left, unsigned long right, const Comparator<T>& comparator);
    static void quicksort(ResizableArray<T>& array, unsigned long left, unsigned long right, const Comparator<T>& comparator);
public:
    static void sort(ResizableArray<T>& array, const Comparator<T>& comparator);
};

// merges two subarrays in array
// First subarray is array[left..middle]
// Second subarray is array[middle+1..right]
template <typename T>
void Sorter<T>::merge(ResizableArray<T>& array, unsigned long left, unsigned long middle, unsigned long right, const Comparator<T>& comparator)
{
    // create temp subarrays
    unsigned long lenl, lenr;
    lenl = middle - left + 1;
    lenr = right - middle;
    T* L = new T[lenl];
    T* R = new T[lenr];

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
            array.replaceAt(L[i], k);
        }
        else 
        {
            array.replaceAt(R[j], k);
        }
        k++;
    }

    // copy remaining elements
    while (i < lenl)
    {
        array.replaceAt(L[i], k);
        i++;
        k++;
    }

    while (j < lenl)
    {
        array.replaceAt(R[j], k);
        j++;
        k++;
    }
}

template <typename T>
void Sorter<T>::mergesort(ResizableArray<T>& array, unsigned long left, unsigned long right, const Comparator<T>& comparator)
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
void Sorter<T>::quicksort(ResizableArray<T>& array, unsigned long left, unsigned long right, const Comparator<T>& comparator)
{
    if (left < right)
    {
        unsigned long i, j;
        unsigned long pivot_point = (unsigned long) (left + right) / 2;
        i = left - 1;
        j = right + 1;
        do
        {
            do
            {
                i += 1;
            } while (j >= i && comparator.compare(array[i], array[pivot_point]) <= 0);
            
            do
            {
                j -= 1;
            } while (i < j && comparator.compare(array[j], array[pivot_point]) >= 0);
            if (i < j)
            {
                T temp = array[i];
                array[i] = array[j];
                array[j] = temp;
            }
        }
        while (i < j);
        if (i < pivot_point)
        {
            j = i;
        }
        T temp = array[pivot_point];
        array[pivot_point] = array[j];
        array[j] = temp;
        std::cout << "Pivot: " << std::to_string(pivot_point) << " Array size: " << std::to_string(array.getSize()) << "\n";
        if (left < j - 1) 
        {
            quicksort(array, left, j - 1, comparator);
        }
        if (right > j + 1)
        {
            quicksort(array, j + 1, right, comparator);
        }
    }
}

template <typename T>
void Sorter<T>::sort(ResizableArray<T>& array, const Comparator<T>& comparator)
{
    quicksort(array, 0, array.getSize() - 1, comparator);
}

#endif