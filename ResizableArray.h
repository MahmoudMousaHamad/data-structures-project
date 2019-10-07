#ifndef RESIZABLE_ARRAY_H
#define RESIZABLE_ARRAY_H

#include "Exceptions.h"

const unsigned long DEFAULT_ARRAY_CAPACITY = 10;        // capacity used in no arg constructor

template <typename T>
class ResizableArray {
private:
    unsigned long capacity = DEFAULT_ARRAY_CAPACITY;    // maximum capacity, in items
    unsigned long size = 0;                             // actual number of items currently in array
    T* data = NULL;                                     // pointer to array of any type
    void doubleCapacity();                              // method to double array capacity
    void halveCapacity();                               // method to halve array capacity
public:
    ResizableArray();                                   // constructs array with default capacity
    ResizableArray(unsigned long capacity);             // constructs array with specified capacity
    virtual ~ResizableArray();                          // frees array space as object is deleted
    void add(T item);                                   // adds item, increments size, doubles capacity as necessary
    
    // all indexed member functions must throw ExceptionIndexOutOfRange for bad index values
    void addAt(T item, unsigned long index);            // adds item at index, shifts following, doubles capacity as necessary
    void replaceAt(T item, unsigned long index);        // replaces item at index, otherwise unchanged
    void removeAt(unsigned long index);                 // removes item at index, shifts following back
    T get(unsigned long index) const;                   // returns (copy of) item at index
    T operator[](unsigned long index) const;            // returns (copy of) item at index
    
    unsigned long getSize() const;                      // returns number of items currently in array
    unsigned long getCapacity() const;                  // returns the current capacity of the array
};

// Add your implementation below this line. Do not add or modify anything above this line.

template <typename T>
ResizableArray<T>::ResizableArray() 
{
	data = new T[this->capacity];
    if (data == nullptr)
	{
		throw new ExceptionMemoryNotAvailable();
	}
}

template <typename T>
ResizableArray<T>::ResizableArray(unsigned long capacity) 
{
	this->capacity = capacity;
	this->data = new T[capacity];
    if (data == nullptr)
	{
		throw new ExceptionMemoryNotAvailable();
	}
}

template <typename T>
ResizableArray<T>::~ResizableArray() 
{
	delete[] this->data;
}

template <typename T>
void ResizableArray<T>::add(T record) 
{
	if (this->size == this->capacity - 1) 
    {
		doubleCapacity();
	}
	this->data[this->size++] = record;
}

template <typename T>
void ResizableArray<T>::addAt(T item, unsigned long index)
{
    if (index >= this->size || index < 0)
    {
        throw new ExceptionIndexOutOfRange();
    }
    if (this->size == this->capacity - 1) 
    {
		doubleCapacity();
	}
    // shift elements
    for (unsigned long i = this->size - 1; i >= index; --i)
    {
        this->data[i + 1] = this->data[i];
    }
    // add item at index
    this->data[index] = item;
    this->size++;
}

template <typename T>
void ResizableArray<T>::replaceAt(T item, unsigned long index)
{
    if (index >= this->size || index < 0)
    {
        throw new ExceptionIndexOutOfRange();
    }
    this->data[index] = item;
}

template <typename T>
void ResizableArray<T>::removeAt(unsigned long index)
{
    if (index >= this->size || index < 0)
    {
        throw new ExceptionIndexOutOfRange();
    }
    // shift elements
    for (unsigned long i = index; i < this->size; ++i)
    {
        this->data[i] = this->data[i + 1];
    }
    // delete last element (i.e., init to zero)
    this->data[this->size - 1] = 0;
    this->size--;
    if (this->size == (unsigned long) ((this->capacity - 1) / 2)) 
    {
		halveCapacity();
	}
}

template <typename T>
unsigned long ResizableArray<T>::getSize() const
{
    return this->size;
}

template <typename T>
unsigned long ResizableArray<T>::getCapacity() const 
{
    return this->capacity;
}

template <typename T>
T ResizableArray<T>::get(unsigned long index) const 
{
    if (index >= this->size || index < 0)
    {
        throw new ExceptionIndexOutOfRange();
    }
    
	T record = data[index];
	return record;
}

template <typename T>
T ResizableArray<T>::operator[](unsigned long index) const
{
    if (index >= this->size || index < 0)
    {
        throw new ExceptionIndexOutOfRange();
    }
    
	T record = data[index];
	return record;
}

template <typename T>
void ResizableArray<T>::doubleCapacity() 
{
    unsigned long newCapacity = this->capacity * 2;
    T *newData = new T[newCapacity];
    if (newData == nullptr)
	{
		throw new ExceptionMemoryNotAvailable();
	}
    for (unsigned long i = 0; i < this->capacity; i++) 
    {
        newData[i] = this->data[i];
    }
    delete[] this->data;
    this->data = newData;
    this->capacity = newCapacity;
}

template <typename T>
void ResizableArray<T>::halveCapacity()
{
    unsigned long newCapacity = (unsigned long) (this->capacity / 2);
    T *newData = new T[newCapacity];
    if (newData == nullptr)
	{
		throw new ExceptionMemoryNotAvailable();
	}
    for (unsigned long i = 0; i < newCapacity; i++) 
    {
        newData[i] = this->data[i];
    }
    delete[] this->data;
    this->data = newData;
    this->capacity = newCapacity;
}

#endif