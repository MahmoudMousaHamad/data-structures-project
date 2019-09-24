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
public:
    static void sort(ResizableArray<T>& array, const Comparator<T>& comparator);
};

// implementation goes here


#endif