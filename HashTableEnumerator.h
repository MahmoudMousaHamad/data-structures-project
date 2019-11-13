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
    if (hashTable->getSize() == 0)
    {
        throw new ExceptionHashTableAccess();
    }
    for (unsigned long i = 0; i < hashTable->getBaseCapacity(); ++i)
    {
        if (hashTable->table[i] != nullptr)
        {
            if (hashTable->table[i]->getSize() > 0)
            {
                chainEnumerator = new OULinkedListEnumerator<T>(hashTable->table[i]->enumerator());
                bucket = i;
                break;
            }
        }
    }
    
}

template <typename T>
HashTableEnumerator<T>::~HashTableEnumerator()
{
    delete hashTable; hashTable = nullptr;
}

template <typename T>
bool HashTableEnumerator<T>::hasNext() const
{
  if (chainEnumerator->hasNext())
  {
    return true;
  }
    for (unsigned long i = bucket + 1; i < hashTable->getBaseCapacity(); ++i)
    {
        OULinkedList<T>* linkedList = hashTable->table[i];
        if (linkedList == nullptr) continue;
        if (linkedList->enumerator().hasNext())
        {
            return true;
        }
        else
        {
            continue;
        }
    }
    return false;
}

template <typename T>
T HashTableEnumerator<T>::next()
{
  if (chainEnumerator->hasNext())
  {
    return chainEnumerator->next();
  }
    for (unsigned long i = bucket+1; i < hashTable->getBaseCapacity(); ++i)
    {
        OULinkedList<T>* linkedList = hashTable->table[i];
        if (linkedList == nullptr) continue;
        if (linkedList->enumerator().hasNext())
        {
          bucket = i;
          chainEnumerator = new OULinkedListEnumerator<T>(linkedList->enumerator());
          return chainEnumerator->next();
        }
    }
    throw new ExceptionEnumerationBeyondEnd();
}

template <typename T>
T HashTableEnumerator<T>::peek() const
{
    if (chainEnumerator->hasNext())
    {
        return chainEnumerator->peek();
    }
    for (unsigned long i = bucket+1; i < hashTable->getBaseCapacity(); ++i)
    {
        OULinkedList<T>* linkedList = hashTable->table[i];
        // std::cout << "Bucket: " << bucket << "\n";
        if (linkedList == nullptr) continue;
        if (linkedList->enumerator().hasNext())
        {
          return linkedList->enumerator().peek();
        }
    }
    throw new ExceptionEnumerationBeyondEnd();
}

#endif // !HASH_TABLE_ENUMERATOR