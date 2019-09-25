// Signature must be retained. Implementation must be added.

#ifndef SEARCH_H
#define SEARCH_H

#include <math.h>
#include "Exceptions.h"
#include "ResizableArray.h"
#include "Comparator.h"

// returns FIRST array location matching the given item (based on the comparator)
// if not found, returns -(location to insert + 1)
template <typename T>
long binarySearch(const T& item, const ResizableArray<T>& array, const Comparator<T>& comparator) {
    unsigned long left = 0;
    unsigned long right = array.getSize();
    unsigned long mid
    while (right > left)
    {
        mid = (left + right) / 2;
        int result = comparator.compare(item, array.get(mid))
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
    return mid + 1;
}
#endif