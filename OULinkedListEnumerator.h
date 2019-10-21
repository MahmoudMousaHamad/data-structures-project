#ifndef OU_LINKED_LIST_ENUMERATOR
#define OU_LINKED_LIST_ENUMERATOR

#include "Enumerator.h"
#include "OULink.h"

template <typename T>
class OULinkedListEnumerator : public Enumerator<T>
{
	template <typename U>
    friend class OULinkedList;
    template <typename U>
    friend class OULinkedListEnumerator;
private:
	OULink<T>* current = nullptr;
public:
	OULinkedListEnumerator(OULink<T>* first);
	bool hasNext() const;
	T next();			// throws ExceptionEnumerationBeyondEnd if no next item is available
	T peek() const;		// throws ExceptionEnumerationBeyondEnd if no next item is available
};

// Add your implementation below this line. Do not add or modify anything above this line.

template <typename T>
OULinkedListEnumerator<T>::OULinkedListEnumerator(OULink<T>* first)
{
	current = new OULink<T>(first->data);
	current->next = first;
}

template <typename T>
bool OULinkedListEnumerator<T>::hasNext() const
{
	if (current == nullptr)
	{
		return false;
	}
	else if (current->next == nullptr) 
	{
		return false;
	}
	else
	{
		return true;
	} 
}

template <typename T>
T OULinkedListEnumerator<T>::next()
{
	if (current == nullptr)
	{
		throw new ExceptionEnumerationBeyondEnd();		
	}
	else if (current->next == nullptr)
	{
		throw new ExceptionEnumerationBeyondEnd();
	}
	else 
	{
		OULink<T>* temp = current->next;
		current = current->next;
		return temp->data;
	}
}

template <typename T>
T OULinkedListEnumerator<T>::peek() const
{
	if (current == nullptr)
	{
		throw new ExceptionEnumerationBeyondEnd();		
	}
	else if (current->next == nullptr)
	{
		throw new ExceptionEnumerationBeyondEnd();
	}
	else 
	{
		return current->next->data;
	}
}

#endif // !OU_LINKED_LIST_ENUMERATOR