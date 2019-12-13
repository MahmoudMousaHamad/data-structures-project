#ifndef AVL_TREE_ENUMERATOR
#define AVL_TREE_ENUMERATOR

#include <queue>
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


// Add your implementation below this line.

template <typename T>
AVLTreeEnumerator<T>::AVLTreeEnumerator(const AVLTree<T>* root, AVLTreeOrder order)
{
	if (root->empty)
	{
		throw new ExceptionAVLTreeAccess();
	}
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
	if (current == nullptr)
	{
		return;
	}
    std::stack<const AVLTree<T>*> tempStack;
	// left
	// parent
	// right
    if (order == AVLTreeOrder::inorder)
    {
        while ((current != nullptr && !current->empty) || !tempStack.empty())
        {
            // get left-most node
            while (current != nullptr)
            {
                tempStack.push(current);
                current = current->left;
            }
            current = tempStack.top();
			tempStack.pop();
			traversalQueue.push(current);
            current = current->right;
        }
    }
	// parent
	// left
	// right
	else if (order == AVLTreeOrder::preorder)
	{
		while (current != nullptr || !tempStack.empty())
		{
			// get left-most node
			while (current != nullptr)
			{
				tempStack.push(current);
				traversalQueue.push(current);
				current = current->left;
			}
			current = tempStack.top();
			tempStack.pop();
			current = current->right;
		}
	}
	// left
	// right
	// parent
    else if (order == AVLTreeOrder::postorder)
    {
		buildTraversalStack(current->left);
		buildTraversalStack(current->right);
		traversalQueue.push(current);

		//while (current != nullptr || !tempStack.empty())
		//{
		//	// get left-most node
		//	while (current->left != nullptr)
		//	{
		//		tempStack.push(current->right);
		//		tempStack.push(current);
		//		current = current->left;
		//	}
		//	if (current->right != nullptr)
		//	{
		//		current = current->right;
		//		continue;
		//	}
		//	traversalQueue.push(current);
		//	current = tempStack.top();
		//	tempStack.pop();
		//	current = current->right;
		//}
    }
}

template <typename T>
bool AVLTreeEnumerator<T>::hasNext() const
{
    return traversalQueue.size() > 0;
}

template <typename T>
T AVLTreeEnumerator<T>::next()
{
	if (traversalQueue.size() == 0)
	{
		throw new ExceptionEnumerationBeyondEnd();
	}
	const AVLTree<T>* node = traversalQueue.front();
	traversalQueue.pop();
	return node->data;
}

template <typename T>
T AVLTreeEnumerator<T>::peek() const
{
	if (traversalQueue.size() == 0)
	{
		throw new ExceptionEnumerationBeyondEnd();
	}
	const AVLTree<T>* node = traversalQueue.front();
	return node->data;
}

template <typename T>
AVLTreeOrder AVLTreeEnumerator<T>::getOrder()
{
	return order;
}


#endif // !AVL_TREE_ENUMERATOR