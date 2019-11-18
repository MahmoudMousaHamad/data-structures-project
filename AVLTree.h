#ifndef AVL_TREE
#define AVL_TREE

#include "Comparator.h"
#include "AVLTreeEnumerator.h"

template <typename T>
class AVLTree {
    template <typename U>
    friend class AVLTreeEnumerator;
private:
    Comparator<T>* comparator = NULL;               // used to determine tree order and item equality
    unsigned long size = 0;                         // actual number of items currently in tree
    T data{};                                       // data item of any type
    bool empty = true;                              // flag to indicate whether node contains data
    int diff = 0;                                   // height of right minus height of left
    AVLTree<T>* left = NULL;                        // pointer to left subtree
    AVLTree<T>* right = NULL;                       // pointer to right subtree
    void zig();                                     // right rotation
    void zag();                                     // left rotation
    void zigzag();                                  // left rotation on left subtree, followed by right rotation
    void zagzig();                                  // right rotation on right subtree, followed by left rotation
    void rebalance();                               // check for and rebalance this node, if needed
public:
    AVLTree(Comparator<T>* comparator);             // creates empty linked tree with comparator
    virtual ~AVLTree();                             // deletes subtrees

    // if an equivalent item is not already present, insert item in order and return true
    // if an equivalent item is already present, leave tree unchanged and return false
    bool insert(const T item);

    // if an equivalent item is already present, replace item and return true
    // if an equivalent item is not already present, leave tree unchanged and return false
    bool replace(const T item);

    // if an equivalent item is already present, remove item and return true
    // if an equivalent item is not already present, leave tree unchanged and return false
    bool remove(const T item);

    // if an equivalent item is present, return true
    // if an equivalent item is not present, false
    bool contains(T item) const;

    // if an equivalent item is present, return a copy of the item
    // if an equivalent item is not present, throw a new ExceptionLinkedtreeAccess
    T find(const T item) const;

    unsigned long getSize() const;                  // returns the current number of items in the tree

    AVLTreeEnumerator<T> enumerator(AVLTreeOrder order = AVLTreeOrder::inorder) const;    // create an enumerator for this AVL tree
};

// Add your implementation below this line.

template <typename T>
AVLTree<T>::AVLTree(Comparator<T>* comparator)
{
    this->comparator = comparator;
}

template <typename T>
AVLTree<T>::~AVLTree()
{
    delete comparator; comparator = nullptr;
    left = nullptr; right = nullptr;
}

template <typename T>
bool AVLTree<T>::insert(const T item)
{
    if (empty)
    {
        data = item;
        empty = false;
    }
    long result = comparator->compare(item, data);
    int old_diff = 0;
    if (result < 0)
    {
        if (left == nullptr)
        {
            left = new AVLTree<T>(comparator);
            left->data = item;
            size++;
            diff--; // check this
            return true;
        }
        else
        {
            old_diff = left->diff;
            left->insert(item);
            if (old_diff != left->diff && left->diff != 0)
            {
                diff--;
            }
        }
    }
    else if (result > 0)
    {
        if (right == nullptr)
        {
            right = new AVLTree<T>(comparator);
            right->data = item;
            size++;
            diff++; // check this
            return true;
        }
        else
        {
            old_diff = right->diff;
            right->insert(item);
            if (old_diff != right->diff && right->diff != 0)
            {
                diff++;
            }
        }
    }
    rebalance();
    return false;
}

template <typename T>
bool AVLTree<T>::replace(const T item)
{
    if (empty)
    {
        return false;
    }
    long result = comparator->compare(item, data);
    if (result < 0)
    {
        if (left == nullptr)
        {
            return false;
        }
        else
        {
            left->replace(item);
        }
    }
    else if (result > 0)
    {
        if (right == nullptr)
        {
            return false;
        }
        else
        {
            right->replace(item);
        }
    }
    data = item;
    return true;
}

// Check this
template <typename T>
bool AVLTree<T>::remove(const T item)
{
    if (empty)
    {
        return false;
    }

    long result = comparator->compare(item, data);
    int old_diff = 0;
    if (result < 0)
    {
        if (left == nullptr)
        {
            return false;
        }
        else
        {
            old_diff = left->diff;
            left->remove(item);
            if (old_diff != left->diff && left->diff != 0)
            {
                diff++;
            }
        }
    }
    else if (result > 0)
    {
        if (right == nullptr)
        {
            return false;
        }
        else
        {
            old_diff = right->diff;
            right->remove(item);
            if (old_diff != right->diff && right->diff != 0)
            {
                diff--;
            }
        }
    }
    // Leaf node
    if (right == nullptr && left == nullptr)
    {
        delete this;
        this = nullptr;
    }
    // Has left node only, replace it with this
    else if (right == nullptr && left != nullptr)
    {
        data = left->data;
        left = left->left;
        right = left->right;
        delete left;
        diff++;
    }
    // Has right node
    else if (right != nullptr)
    {
        // Right is leaf node
        if (right->right == nullptr && right->left == nullptr)
        {
            data = right->data;
            delete right;
            right = nullptr;
            diff--;
        }
        // Right only has right subtree
        else if (right->right != nullptr && right->left == nullptr)
        {
            data = right->data;
            right = right->right;
            delete right;
            diff--;
        }
        // Right subtree has left subtree
        else
        {
            AVLTree<T>* temp = right;
            // Get leftmost leaf node
            while (temp->left != nullptr)
            {
                if (temp->left->left == nullptr)
                {
                    temp = temp->left;
                    temp->left = nullptr;
                    break;
                }
                temp = temp->left;
                temp->diff++;
            }
            if (right->left == nullptr)
            {
                right = nullptr;
                diff--;
            }
            data = temp->data;
            delete temp;
            temp = nullptr;
        }
    }
    size--;
    return true;
}

template <typename T>
bool AVLTree<T>::contains(T item) const
{
    if (empty)
    {
        return false;
    }

    long result = comparator->compare(item, data);
    if (result < 0)
    {
        if (left == nullptr)
        {
            return false;
        }
        else
        {
            left->contains(item);
        }
    }
    else if (result > 0)
    {
        if (right == nullptr)
        {
            return false;
        }
        else
        {
            right->contains(item);
        }
    }
    return true;
}

template <typename T>
T AVLTree<T>::find(const T item) const
{
    if (empty)
    {
        throw new ExceptionAVLTreeAccess();
    }

    long result = comparator->compare(item, data);
    if (result < 0)
    {
        if (left == nullptr)
        {
            throw new ExceptionAVLTreeAccess();
        }
        else
        {
            left->find(item);
        }
    }
    else if (result > 0)
    {
        if (right == nullptr)
        {
            throw new ExceptionAVLTreeAccess();
        }
        else
        {
            right->find(item);
        }
    }
    return data;
}

template <typename T>
unsigned long AVLTree<T>::getSize() const
{
    return size;
}

template <typename T>
AVLTreeEnumerator<T> AVLTree<T>::enumerator(AVLTreeOrder order) const
{
    return AVLTreeEnumerator<T>(this, order);
}

template <typename T>
void AVLTree<T>::rebalance()
{
    if (diff > -2 && diff < 2)
    {
        return;
    }
    else if (diff < 0 && left->diff < 0) // left-left
    {
        zag();
    }
    else if (diff < 0 && left->diff > 0) // left-right
    {
        zigzag();
    }
    else if (diff > 0 && right->diff < 0) // right-left
    {
        zagzig();
    }
    else if (diff > 0 && right->diff > 0) // right-right
    {
        zag();
    }
}

template <typename T>
void AVLTree<T>::zig()
{
    if (left == nullptr)
    {
        return;
    }
    AVLTree<T> left_temp = left;
    left = left_temp->right;
    left_temp->left = this;
}

template <typename T>
void AVLTree<T>::zag()
{
    if (right == nullptr)
    {
        return;
    }
    AVLTree<T>* right_temp = right;
    right = right_temp->left;
    right_temp->left = this;
}

template <typename T>
void AVLTree<T>::zigzag()
{
    left->zag();
    zig();
}

template <typename T>
void AVLTree<T>::zagzig()
{
    right->zig();
    zag;
}

#endif // !AVL_TREE


