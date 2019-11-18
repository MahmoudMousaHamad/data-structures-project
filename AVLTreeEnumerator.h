#ifndef AVL_TREE_ENUMERATOR
#define AVL_TREE_ENUMERATOR

#include <queue>
#include "Enumerator.h"
#include "AVLTreeOrder.h"

template <typename S>
class AVLTree;

template <typename T>
class AVLTreeEnumerator : public Enumerator<T>
{
private:
    AVLTreeOrder order;
    std::queue<const AVLTree<T>*> traversalQueue;
    void buildTraversalStack(const AVLTree<T>* current);        // What does this function do?
public:
    AVLTreeEnumerator(const AVLTree<T>* root, AVLTreeOrder order = AVLTreeOrder::inorder);
    virtual ~AVLTreeEnumerator();
    bool hasNext() const;
    T next();                        // throws ExceptionEnumerationBeyondEnd if no next item is available
    T peek() const;                  // throws ExceptionEnumerationBeyondEnd if no next item is available
    AVLTreeOrder getOrder();         // returns the order of this enumerator (preorder, inorder, or postorder)
};

template <typename T>
AVLTreeEnumerator<T>::AVLTreeEnumerator(const AVLTree<T>* root, AVLTreeOrder order)
{
    this->order = order;
    this->buildTraversalStack(root);
}

template <typename T>
AVLTreeEnumerator<T>::~AVLTreeEnumerator()
{

}


template <typename T>
void AVLTreeEnumerator<T>::buildTraversalStack(const AVLTree<T>* current)
{
    std::stack<const AVLTree<T>*> tempStack;

    if (order == AVLTreeOrder::inorder)
    {
        while (current != nullptr || !tempStack.empty())
        {
            // get left-most node
            while (current != nullptr)
            {
                tempStack.push(current);
                current = current->left;
            }
            current = tempStack.pop();
            traversalQueue.push(current);
            current = current->right;
        }
    }
    else if (order == AVLTreeOrder::postorder)
    {
        while (current != nullptr || !tempStack.empty())
        {
            // get left-most node
            while (current != nullptr)
            {
                if (current->right)
                {
                    tempStack.push(current->right);
                }
                tempStack.push(current);
                current = current->left;
            }
            current = tempStack.pop();
            if (current->right && tempStack.top() == current->right)
            {
                tempStack.pop();
                tempStack.push(current);
                current = current->right;
            }
            else
            {
                traversalQueue.push(current);
            }
        }
    }
    else if (order == AVLTreeOrder::preorder)
    {
        
    }
}

template <typename T>
bool AVLTreeEnumerator<T>::hasNext()
{
    bool has_next = false;
    
    return false;
}

// Add your implementation below this line.



#endif // !AVL_TREE_ENUMERATOR