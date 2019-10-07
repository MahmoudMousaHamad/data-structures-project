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
    static void mergesort(ResizableArray<T>& arr, long l, long r, const Comparator<T>& comparator);
    static void merge(ResizableArray<T>& arr, long l, long m, long r, const Comparator<T>& comparator);
public:
    static void sort(ResizableArray<T>& array, const Comparator<T>& comparator);
};

template <typename T>
void Sorter<T>::merge(ResizableArray<T>& arr, long l, long m, long r, const Comparator<T>& comparator)
{
    long i, j, k; 
    long n1 = m - l + 1; 
    long n2 =  r - m; 
  
    /* create temp arrays */
    ResizableArray<T> L(n1);
    ResizableArray<T> R(n2);
  
    /* Copy data to temp arrays L[] and R[] */
    for (i = 0; i < n1; i++) 
        L.add(arr[l + i]); 
    for (j = 0; j < n2; j++)
        R.add(arr[m + 1+ j]); 
  
    /* Merge the temp arrays back into arr[l..r]*/
    i = 0; // Initial index of first subarray 
    j = 0; // Initial index of second subarray 
    k = l; // Initial index of merged subarray 
    while (i < n1 && j < n2) 
    { 
        if (comparator.compare(L[i], R[j]) <= 0) 
        { 
            arr.replaceAt(L[i], k); 
            i++; 
        } 
        else
        { 
            arr.replaceAt(R[j], k);
            j++; 
        } 
        k++; 
    } 
  
    /* Copy the remaining elements of L[], if there 
       are any */
    while (i < n1) 
    { 
        arr.replaceAt(L[i], k);
        i++; 
        k++; 
    } 
  
    /* Copy the remaining elements of R[], if there 
       are any */
    while (j < n2) 
    { 
        arr.replaceAt(R[j], k);
        j++; 
        k++; 
    } 
}

/* l is for left index and r is right index of the 
   sub-array of arr to be sorted */
template <typename T>
void Sorter<T>::mergesort(ResizableArray<T>& arr, long l, long r, const Comparator<T>& comparator) 
{
    if (l < r) 
    { 
        // Same as (l+r)/2, but avoids overflow for 
        // large l and h 
        long m = l + (r - l) / 2; 
  
        // Sort first and second halves 
        mergesort(arr, l, m, comparator); 
        mergesort(arr, m + 1, r, comparator); 
  
        merge(arr, l, m, r, comparator); 
    } 
} 

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
    mergesort(array, 0, array.getSize() - 1, comparator);
}

#endif