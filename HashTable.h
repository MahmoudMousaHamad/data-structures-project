#ifndef HASH_TABLE
#define HASH_TABLE

#include <math.h>
#include "Exceptions.h"
#include "Comparator.h"
#include "OULinkedList.h"
#include "OULinkedListEnumerator.h"
#include "Hasher.h"

const unsigned int SCHEDULE_SIZE = 25;
const unsigned int SCHEDULE[SCHEDULE_SIZE] = { 1, 2, 5, 11, 23, 53, 107, 223, 449, 907, 1823, 3659, 7309, 14621, 29243, 58511, 117023, 234067, 468157, 936319, 1872667, 3745283, 7490573, 14981147, 29962343 };
const unsigned int DEFAULT_SCHEDULE_INDEX = 3;
const unsigned long DEFAULT_BASE_CAPACITY = SCHEDULE[DEFAULT_SCHEDULE_INDEX];
const float DEFAULT_MAX_LOAD_FACTOR = 0.8f;
const float DEFAULT_MIN_LOAD_FACTOR = 0.3f;
const std::string EXPAND_RESIZE_OPTION = "EXPAND_RESIZE_OPTION";
const std::string SHRINK_RESIZE_OPTION = "SHRINK_RESIZE_OPTION";


template <typename T>
class HashTable {
    template <typename U>
    friend class HashTableEnumerator;
private:
    Comparator<T>* comparator;                                // used to determine item equality
    Hasher<T>* hasher;                                        // used to compute hash value
    unsigned long size = 0;                                    // actual number of items currently in hash table
    float maxLoadFactor = DEFAULT_MAX_LOAD_FACTOR;
    float minLoadFactor = DEFAULT_MIN_LOAD_FACTOR;
    unsigned int scheduleIndex = DEFAULT_SCHEDULE_INDEX;
    unsigned long baseCapacity = DEFAULT_BASE_CAPACITY;        // the size of the array
    unsigned long totalCapacity = baseCapacity;                // the size of the array plus chains of more than one link
    OULinkedList<T>** table = NULL;   
    void initLinkedLists();                             // table will be an array of pointers to OULinkedLists of type T
    void resizeTable(std::string option);
    // you may add additional member variables and functions here to support the operation of your code
public:
    HashTable(Comparator<T>* comparator, Hasher<T>* hasher);            // creates an empty table of DEFAULT_BASE_CAPACITY
    HashTable(Comparator<T>* comparator, Hasher<T>* hasher,
        // if size given, creates empty table with size from schedule of sufficient capacity (considering maxLoadFactor)
        unsigned long size,
        float maxLoadFactor = DEFAULT_MAX_LOAD_FACTOR,
        float minLoadFactor = DEFAULT_MIN_LOAD_FACTOR);
    HashTable(OULinkedList<T>* linkedListToCopy, 
        Hasher<T>* hasher,
        Comparator<T>* comparator);    // converts a linkedlist to a hashtable
    virtual ~HashTable();

    // if an equivalent item is not already present, insert item at proper location and return true
    // if an equivalent item is already present, leave table unchanged and return false
    bool insert(T item);

    // if an equivalent item is already present, replace item and return true
    // if an equivalent item is not already present, leave table unchanged and return false
    bool replace(T item);

    // if an equivalent item is already present, remove item and return true
    // if an equivalent item is not already present, leave table unchanged and return false
    bool remove(T item);

    // if an equivalent item is present, return a copy of the item
    // if an equivalent item is not present, throw a new ExceptionHashTableAccess
    T find(T item) const;

    unsigned long getSize() const;                            // returns the current number of items in the table
    unsigned long getBaseCapacity() const;                    // returns the current base capacity of the table
    unsigned long getTotalCapacity() const;                    // returns the current total capacity of the table
    float getLoadFactor() const;                            // returns the current load factor of the table
    unsigned long getBucketNumber(T item) const;            // returns the current bucket number for an item
};

// Add your implementation below this line.

template <typename T>
HashTable<T>::HashTable(Comparator<T>* comparator, Hasher<T>* hasher)
{
    this->comparator = comparator;
    this->hasher = hasher;
    table = new OULinkedList<T> *[DEFAULT_BASE_CAPACITY];
    if (table == nullptr) throw new ExceptionMemoryNotAvailable();
    for (unsigned long i = 0; i < DEFAULT_BASE_CAPACITY; ++i)
    {
        table[i] = nullptr;
    }
}

template <typename T>
HashTable<T>::HashTable(Comparator<T>* comparator, Hasher<T>* hasher,
    unsigned long _size,
    float maxLoadFactor,
    float minLoadFactor)
{
    this->maxLoadFactor = maxLoadFactor;
    this->minLoadFactor = minLoadFactor;
    this->comparator = comparator;
    this->hasher = hasher;
    if (_size != 0)
    {
        // Get table size from schedule
        for (unsigned int i = 0; i < SCHEDULE_SIZE; ++i)
        {
            if (SCHEDULE[i] > _size)
            {
                if ( ((float) _size / (float) SCHEDULE[i]) > maxLoadFactor) ++i;
                if (((float) _size / (float) SCHEDULE[i]) < minLoadFactor) --i;                
                scheduleIndex = i;
                break;
            }
        }
    }
    else
    {
        scheduleIndex = DEFAULT_SCHEDULE_INDEX;
    }
    this->baseCapacity = SCHEDULE[scheduleIndex];
    this->totalCapacity = baseCapacity;
    table = new OULinkedList<T> *[this->baseCapacity];
    for (unsigned long i = 0; i < DEFAULT_BASE_CAPACITY; ++i)
    {
        table[i] = nullptr;
    }
    if (table == nullptr) throw new ExceptionMemoryNotAvailable();
}

template <typename T>
HashTable<T>::HashTable(OULinkedList<T>* linkedListToCopy, Hasher<T>* hasher, Comparator<T>* comparator)
{
    this->comparator = comparator;
    this->hasher = hasher;
    this->baseCapacity = linkedListToCopy->getSize();
    totalCapacity = baseCapacity;
    size = baseCapacity;
    // Delete previous data
    for (unsigned long i = 0; i < baseCapacity; ++i)
    {
        delete table[i]; table[i] = nullptr;
    }
    delete[] table; table = nullptr;
    // create new table
    table = new OULinkedList<T> *[this->baseCapacity];
    if (table == nullptr) throw new ExceptionMemoryNotAvailable();
    OULinkedListEnumerator<T> enumerator = linkedListToCopy->enumerator();
    while (enumerator.hasNext())
    {
        T item = enumerator.next();
        table[getBucketNumber(item)]->insert(item);
    }
}

template <typename T>
HashTable<T>::~HashTable()
{
    for (unsigned long i = 0; i < baseCapacity; ++i)
    {
        delete table[i]; table[i] = nullptr;
    }
    delete[] table; table = nullptr;
}

template <typename T>
bool HashTable<T>::insert(T item)
{
    bool collision_happened = false;
    unsigned long bucket_number = this->getBucketNumber(item); 
    if (table[bucket_number] == nullptr)
    {
        table[bucket_number] = new OULinkedList<T>(this->comparator);
    }
    if (table[bucket_number]->getSize() == 0) collision_happened = true;
    table[bucket_number]->insert(item);
    totalCapacity++;
    size++;
    if (getLoadFactor() > maxLoadFactor)
    {
        resizeTable(EXPAND_RESIZE_OPTION);
    }
    return collision_happened;
}

template <typename T>
bool HashTable<T>::replace(T item)
{
    return table[this->getBucketNumber(item)]->replace(item);
}

template <typename T>
bool HashTable<T>::remove(T item)
{
    bool item_removed = table[this->getBucketNumber(item)]->remove(item);
    if (item_removed)
    {
        totalCapacity--;
        size--;
        if (getLoadFactor() < minLoadFactor)
        {
            resizeTable(SHRINK_RESIZE_OPTION);
        }
    }
    return item_removed;
}

template <typename T>
T HashTable<T>::find(T item) const
{
    T _item;
    try
    {
        _item = table[this->getBucketNumber(item)]->find(item);
    }
    catch(const ExceptionLinkedListAccess& e)
    {
        throw new ExceptionHashTableAccess();
    }
    return _item;
}

template <typename T>
unsigned long HashTable<T>::getSize() const
{
    return this->size;
}

template <typename T>
unsigned long HashTable<T>::getBaseCapacity() const
{
    return this->baseCapacity;
}

template <typename T>
unsigned long HashTable<T>::getTotalCapacity() const
{
    return this->totalCapacity;
}

template <typename T>
float HashTable<T>::getLoadFactor() const
{
    return ((float) this->size) / ((float) this->totalCapacity);
}

template <typename T>
unsigned long HashTable<T>::getBucketNumber(T item) const
{
    unsigned long item_hash_value = hasher->hash(item);
    unsigned long item_bucket_num = item_hash_value % this->baseCapacity;
    return item_bucket_num;
}

template <typename T>
void HashTable<T>::resizeTable(std::string option)
{
    if (option == EXPAND_RESIZE_OPTION)
    {
        ++scheduleIndex;
    }
    else if (option == SHRINK_RESIZE_OPTION)
    {
        --scheduleIndex;
    }
    else 
    {
        return;
    }
    baseCapacity = SCHEDULE[scheduleIndex];    
    OULinkedList<T>** tempTable = new OULinkedList<T> *[baseCapacity];
    for (unsigned int i = 0; i < size; ++i)
    {
        OULinkedList<T>* tempLinkedList = table[i];
        if (tempLinkedList->getSize() == 0) continue;
        else if (tempLinkedList->getSize() == 1)
        {
            T item = tempLinkedList->getFirst();
            tempTable[getBucketNumber(item)]->insert(item);
        }
        else 
        {
            OULinkedListEnumerator<T> enumerator = tempLinkedList->enumerator();
            while (enumerator.hasNext())
            {
                T item = enumerator.next();
                tempTable[getBucketNumber(item)]->insert(item);
            }
        }
    }
    for (unsigned long i = 0; i < baseCapacity; ++i)
    {
        delete table[i]; table[i] = nullptr;
    }
    delete[] table; table = nullptr;
    table = tempTable;
}
#endif // !HASH_TABLE