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
    while (right > left)
    {
        mid = (left + right) / 2;
        int result = comparator.compare(item, array[mid]);
        if (result == 0)
        {
            return mid;
        }
        else if (result > 0)
        {
            left = mid;
            continue;
        }
        else
        {
            right = mid;
            continue;
        }
    }
    return (mid + 1) * -1;
}

template <typename T>
long linearSearch(const T& item, const ResizableArray<T>& array, const Comparator<T>& comparator)
{
    for (unsigned long i = 0; i < array.getSize(); i++)
    {
        if (comparator.compare(item, array.get(i)) == 0)
        {
            return i;
        }
    }
    return -1;
}
#endif