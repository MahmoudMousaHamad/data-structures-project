#ifndef AVL_TREE_ENUMERATOR
#define AVL_TREE_ENUMERATOR

#include <stack>
#include "Enumerator.h"
#include "AVLTreeOrder.h"

template <typename S>
class AVLTree;

template <typename T>
class AVLTreeEnumerator : public Enumerator<T>
{
private:
    AVLTreeOrder order;
    std::stack<const AVLTree<T>*> traversalStack;
    void buildTraversalStack(const AVLTree<T>* current);        // What does this function do?
public:
    AVLTreeEnumerator(const AVLTree<T>* root, AVLTreeOrder order = AVLTreeOrder::inorder);
    virtual ~AVLTreeEnumerator();
    bool hasNext() const;
    T next();                        // throws ExceptionEnumerationBeyondEnd if no next item is available
    T peek() const;                  // throws ExceptionEnumerationBeyondEnd if no next item is available
    AVLTreeOrder getOrder();         // returns the order of this enumerator (preorder, inorder, or postorder)
};


// Add your implementation below this line.

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
			traversalStack.push(current);
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
				tempStack.push(current);
				current = current->left;
			}
			traversalStack.push(current);
			current = tempStack.pop();
			current = current->right;
		}
    }
    else if (order == AVLTreeOrder::preorder)
    {
		while (current != nullptr || !tempStack.empty())
		{
			// get left-most node
			while (current != nullptr)
			{
				tempStack.push(current);
				traversalStack.push(current);
				current = current->left;
			}
			current = tempStack.pop();
			current = current->right;
		}
    }
}

template <typename T>
bool AVLTreeEnumerator<T>::hasNext() const
{
    return traversalStack.size() > 0;
}

template <typename T>
T AVLTreeEnumerator<T>::next()
{
	if (traversalStack.size() == 0)
	{
		throw new ExceptionEnumerationBeyondEnd();
	}
	return traversalStack.pop();
}

template <typename T>
T AVLTreeEnumerator<T>::peek() const
{
	if (traversalStack.size() == 0)
	{
		throw new ExceptionEnumerationBeyondEnd();
	}
	return traversalStack.peek();
}

template <typename T>
AVLTreeOrder AVLTreeEnumerator<T>::getOrder()
{
	return order;
}


#endif // !AVL_TREE_ENUMERATOR
