#ifndef HASH_TABLE_ENUMERATOR
#define HASH_TABLE_ENUMERATOR

#include "Enumerator.h"
#include "Exceptions.h"
#include "HashTable.h"
#include "OULinkedList.h"
#include "OULinkedListEnumerator.h"

template <typename T>
class HashTableEnumerator : public Enumerator<T>
{
private:
    unsigned long bucket = 0;
    OULinkedListEnumerator<T>* chainEnumerator = NULL;
    HashTable<T>* hashTable;
public:
    HashTableEnumerator(HashTable<T>* hashTable);
    virtual ~HashTableEnumerator();
    bool hasNext() const;
    T next();              // throws ExceptionEnumerationBeyondEnd if no next item is available
    T peek() const;        // throws ExceptionEnumerationBeyondEnd if no next item is available
};

// Add your implementation below this line.

template <typename T>
HashTableEnumerator<T>::HashTableEnumerator(HashTable<T>* hashTable)
{
    this->hashTable = hashTable;
}

template <typename T>
HashTableEnumerator<T>::~HashTableEnumerator()
{
    delete hashTable; hashTable = nullptr;
    delete chainEnumerator; chainEnumerator = nullptr;
}

template <typename T>
bool HashTableEnumerator<T>::hasNext() const
{
    for (unsigned long i = bucket; i < hashTable->getBaseCapacity(); ++i)
    {
        if (hashTable->table[i]->getSize() > 0)
        {
            return true;
        }
    }
    return false;
}

template <typename T>
T HashTableEnumerator<T>::next()
{
    for (unsigned long i = bucket; i < hashTable->getBaseCapacity(); ++i)
    {
        OULinkedList<T>* linkedList = hashTable->table[i];
        if (linkedList->getSize() > 0)
        {
            chainEnumerator = linkedList->enumerator();
            while (chainEnumerator->hasNext())
            {
                return chainEnumerator->next();
            }
            ++bucket;
        }
    }
}

template <typename T>
T HashTableEnumerator<T>::peek() const
{
    for (unsigned long i = bucket; i < hashTable->getBaseCapacity(); ++i)
    {
        OULinkedList<T>* linkedList = hashTable->table[i];
        if (linkedList->getSize() > 0)
        {
            chainEnumerator = linkedList->enumerator();
            while (chainEnumerator->hasNext())
            {
                return chainEnumerator->peek();
            }
        }
    }
}

#endif // !HASH_TABLE_ENUMERATOR