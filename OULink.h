#ifndef OU_LINK_H
#define OU_LINK_H

template <typename T>
class OULink {
    template <typename T>
    friend class OULinkedList;
    template <typename T>
    friend class OULinkedListEnumerator;
private:
    T data{};                  // data item of any type
    OULink* next = NULL;       // pointer to next link
public:
    OULink(T item);
    virtual ~OULink();
};

// Add your implementation below this line. Do not add or modify anything above this line.

template <typename T>
OULink<T>::OULink(T item)
{
    this.data = item;
}

template <typename T>
OULink<T>::~OULink()
{
    delete next;
}

#endif // !OU_LINK