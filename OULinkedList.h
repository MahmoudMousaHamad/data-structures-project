#ifndef OU_LINKED_LIST
#define OU_LINKED_LIST

#include "OULink.h"
#include "Comparator.h"
#include "OULinkedListEnumerator.h"

// OULinkedList stands for Ordered, Unique Linked List. It is a linked list that is always maintained in
// order (based on the comparator provided to it when the list is created) and that only contains unique
// items (that is, duplicates are not allowed)
template <typename T>
class OULinkedList {
    template <typename U>
    friend class OULinkedListEnumerator;
private:
    Comparator<T>* comparator = NULL;                // used to determine list order and item equality
    unsigned long size = 0;                             // actual number of items currently in list
    OULink<T>* first = NULL;                         // pointer to first link in list
    OULink<T>* last = NULL;                          // pointer to last link in list
public:
    OULinkedList(Comparator<T>* comparator);        // creates empty linked list with comparator
    virtual ~OULinkedList();                        // deletes all links and their data items

    // if an equivalent item is not already present, insert item in order and return true
    // if an equivalent item is already present, leave list unchanged and return false
    bool insert(T item);

    // if item is greater than item at last, append item at end and return true
    // if item is less than or equal to item at last, leave list unchanged and return false
    bool append(T item);

    // if an equivalent item is already present, replace item and return true
    // if an equivalent item is not already present, leave list unchanged and return false
    bool replace(T item);

    // if an equivalent item is already present, remove item and return true
    // if an equivalent item is not already present, leave list unchanged and return false
    bool remove(T item);

    // if any items are present, return a copy of the first item
    // if no items are present, throw new ExceptionLinkedListAccess
    T getFirst() const;

    // if any items are present, return a copy of the first item, remove it from list
    // if no items are present, throw new ExceptionLinkedListAccess
    T pullFirst();

    // if any items are present, remove the first item and return true
    // if no items are present, leave list unchanged and return false
    bool removeFirst();

    // if an equivalent item is present, return true
    // if an equivalent item is not present, false
    bool contains(T item) const;

    // if an equivalent item is present, return a copy of that item
    // if an equivalent item is not present, throw a new ExceptionLinkedListAccess
    T find(T item) const;

    void clear();                                     // deletes all links in the list, resets size to 0

    unsigned long getSize() const;                    // returns the current number of items in the list

    OULinkedListEnumerator<T> enumerator() const;     // create an enumerator for this linked list
};

// Add your implementation below this line. Do not add or modify anything above this line.

template <typename T>
OULinkedList<T>::OULinkedList(Comparator<T>* comparator)
{
    this->comparator = comparator;
	this->size = 0;
}

template <typename T>
OULinkedList<T>::~OULinkedList()
{
    delete this->first;
    first = nullptr;
}

template <typename T>
bool OULinkedList<T>::insert(T item)
{
    if (first == NULL)
    {
        first = new OULink<T>(item);
        if (first == nullptr)
        {
            throw new ExceptionMemoryNotAvailable();
        }
        last = first;
        this->size++;
        return true;
    }
    OULink<T>* previous = NULL;
    OULink<T>* current = first;
    while (current != NULL)
    {
        long result = comparator->compare(item, current->data);
        if (result < 0)
        {
            OULink<T>* newLink = new OULink<T>(item);
            if (newLink == nullptr)
            {
                throw new ExceptionMemoryNotAvailable();
            }
            if (first == current)
            {
                newLink->next = first;
                first = newLink;
            }
            else
            {
                newLink->next = current;
                previous->next = newLink;
            }
            size++;
            return true;
        }
        else if (result == 0)
        {
            return false;
        }
        else
        {
            previous = current;
            current = current->next;
        }
    }
    OULink<T>* new_link = new OULink<T>(item);
    if (new_link == nullptr) throw new ExceptionMemoryNotAvailable();
    last->next = new_link;
    last = new_link;
    size++;
    return true;
}

template <typename T>
bool OULinkedList<T>::append(T item)
{
    OULink<T> *newLink = new OULink<T>(item);
    if (newLink == nullptr) throw new ExceptionMemoryNotAvailable();
    if (first == NULL)
    {
        first = newLink;
        last = newLink;
        size++;
        return true;
    }
    long result = comparator->compare(item, last->data);
    if (result > 0)
    {
        last->next = newLink;
        last = newLink;
        size++;
        return true;
    }
    return false;
}

template <typename T>
bool OULinkedList<T>::replace(T item)
{
    if (first == NULL) return false;
    OULink<T>* current = first;
    do 
    {
        long result = comparator->compare(item, current->data);
        if (result < 0)
        {
            return false;
        }
        else if (result == 0)
        {
            current->data = item;
            return true;
        }
        current = current->next;
    } 
    while (current->next != NULL);
    return false;
}

template <typename T>
bool OULinkedList<T>::remove(T item)
{
    if (first == NULL) return false;
    if (comparator->compare(item, first->data) == 0)
    {
        if (first->next == NULL)
        {
            delete first;
            first = NULL;
            last = NULL;
        }
        else
        {
            first->data = first->next->data;
            first->next = first->next->next;
        }
        size--;
        return true;
    }
    OULink<T>* previous = first;
    while (previous->next != NULL && comparator->compare(item, previous->next->data) != 0)
    {
        previous = previous->next;
    }
    if (previous->next == NULL)
    {
        return false;
    }
    previous->next = previous->next->next;
    return true;
}

template <typename T>
T OULinkedList<T>::getFirst() const
{
    if (first == NULL)
    {
        throw new ExceptionLinkedListAccess();
    }
    else 
    {
        return first->data;
    }
}

template <typename T>
T OULinkedList<T>::pullFirst()
{
    if (first == NULL)
    {
        throw new ExceptionLinkedListAccess();
    }
    else 
    {
        OULink<T>* tempLink = first;
        if (first->next != NULL)
        {
            first = first->next;
        }
        else
        {
            first = NULL;
        }
        T tempItem = tempLink->data;
        delete tempLink;
        tempLink = nullptr;
        size--;
        return tempItem;
    }
}

template <typename T>
bool OULinkedList<T>::removeFirst()
{
    if (first == NULL)
    {
        throw new ExceptionLinkedListAccess();
    }
    else 
    {
        OULink<T>* tempLink = first;
        if (first->next != NULL)
        {
            first = first->next;
        }
        else
        {
            first = NULL;
        }
        delete tempLink;
        tempLink = nullptr;
        size--;
        return true;
    }
    return false;
}

template <typename T>
bool OULinkedList<T>::contains(T item) const
{
    if (first == NULL)
    {
        return false;
    }
    OULink<T>* current = first;
    do
    {
        long result = comparator->compare(item, current->data);
        if (result < 0)
        {
            return false;
        }
        else if (result == 0)
        {
            return true;
        }
        current = current->next();
    }
    while (current->next != NULL);
    return false;
}

template <typename T>
T OULinkedList<T>::find(T item) const
{
    if (first == NULL)
    {
      throw new ExceptionLinkedListAccess();
    }
    OULink<T>* current = first;
    do
    {
        long result = comparator->compare(item, current->data);
        if (result < 0)
        {
            break;
        }
        else if (result == 0)
        {
            return current->data;
        }
        current = current->next;
    }
    while (current->next != NULL);
    throw new ExceptionLinkedListAccess();
}

template <typename T>
void OULinkedList<T>::clear()
{
    delete first;
    first = nullptr;
    this->size = 0;
}

template <typename T>
unsigned long OULinkedList<T>::getSize() const
{
    return size;
}

template <typename T>
OULinkedListEnumerator<T> OULinkedList<T>::enumerator() const
{
    OULinkedListEnumerator<T> enumerator(first);
    return enumerator;
}

#endif // !OU_LINKED_LIST