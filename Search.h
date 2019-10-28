// Signature must be retained. Implementation must be added.

#ifndef SEARCH_H
#define SEARCH_H

#include "Exceptions.h"
#include "ResizableArray.h"
#include "Comparator.h"

// returns FIRST array location matching the given item (based on the comparator)
// if not found, returns -(location to insert + 1)
template <typename T>
long binarySearch(const T& item, const ResizableArray<T>& array, const Comparator<T>& comparator) {
    unsigned long left = 0;
    unsigned long right = array.getSize() - 1;
    unsigned long mid;
    while (left <= right)
    {
        mid = (long) (left + right) / 2;
        std::cout << mid << std::endl;
        int result = comparator.compare(item, array[mid]);
        if (result == 0)
        {
            for (long i = mid; i >= 0; --i)
            {
                if (comparator.compare(item, array[i]) == 0)
                {
                    mid = i;
                }
                else 
                {
                    break;
                }
            }
            return mid;
        }
        else if (result > 0)
        {
            left = mid + 1;
        }
        else
        {
            right = mid - 1;
        }
    }
    return (mid + 1) * -1;
}

template <typename T>
long linearSearch(const T& item, const ResizableArray<T>& array, const Comparator<T>& comparator)
{
    for (unsigned long i = 0; i < array.getSize(); i++)
    {
        int result = comparator.compare(item, array[i]);
        if (result == 0)
        {
            return i;
        }
    }
    return -1;
}
#endif