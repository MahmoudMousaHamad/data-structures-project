// Signature must be retained. Implementation must be added.

#ifndef SORT_H
#define SORT_H

#include "Exceptions.h"
#include "ResizableArray.h"
#include "Comparator.h"

template <typename T>
class Sorter {
private:
    static void quicksort(ResizableArray<T>& array, long left, long right, const Comparator<T>& comparator);
public:
    static void sort(ResizableArray<T>& array, const Comparator<T>& comparator);
};

template <typename T>
void Sorter<T>::quicksort(ResizableArray<T>& array, long left, long right, const Comparator<T>& comparator)
{
    if (left < right)
    {
        long i, j;
        long pivot_point = (long) (left + right) / 2;
        i = left - 1;
        j = right + 1;
        do
        {
            do
            {
                i += 1;
            } while (i < (long) array.getSize() && comparator.compare(array[i], array[pivot_point]) <= 0);
            
            do
            {
                j -= 1;
            } while (j >= i && comparator.compare(array[j], array[pivot_point]) >= 0);
            if (i < j)
            {
                T temp = array[i];
                array.replaceAt(array[j], i);
                array.replaceAt(temp, j);
            }
        }
        while (i < j);
        if (i < pivot_point)
        {
            j = i;
        }
        T temp = array[pivot_point];
        array.replaceAt(array[j], pivot_point);
        array.replaceAt(temp, j);
        quicksort(array, left, j - 1, comparator);
        quicksort(array, j + 1, right, comparator);
    }
    return;
}

template <typename T>
void Sorter<T>::sort(ResizableArray<T>& array, const Comparator<T>& comparator)
{
    quicksort(array, 0, array.getSize() - 1, comparator);
}

#endif