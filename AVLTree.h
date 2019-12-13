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
	bool _insert(AVLTree<T>* node, const T item);
	bool _remove(const T item);
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

	bool isEmpty() const;

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
    // delete comparator; comparator = nullptr;
    if (left != nullptr)
    {
        delete left;
    }
    if (right != nullptr)
    {
        delete right;
    }
    left = nullptr; right = nullptr;
}

template <typename T>
bool AVLTree<T>::_insert(AVLTree<T>* node, const T item)
{
	bool insert_successful = false;
	long result = comparator->compare(item, data);
	int old_diff = 0;
	if (result < 0)
	{
		if (left == nullptr)
		{
			left = new AVLTree<T>(comparator);
			left->data = item;
			left->empty = false;
			left->diff = 0;
			diff--;
			node->size++;
			insert_successful = true;
		}
		else
		{
			old_diff = left->diff;
			insert_successful = left->_insert(node, item);
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
			right->empty = false;
			right->diff = 0;
			diff++;
			node->size++;
			insert_successful = true;
		}
		else
		{
			old_diff = right->diff;
			insert_successful = right->_insert(node, item);
			if (old_diff != right->diff && right->diff != 0)
			{
				diff++;
			}
		}
	}
	if (insert_successful) this->rebalance();
	return insert_successful;
}

template <typename T>
bool AVLTree<T>::insert(const T item)
{
	if (empty)
	{
		data = item;
		empty = false;
		size++;
		return true;
	}
	return _insert(this, item);
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
bool AVLTree<T>::_remove(const T item)
{
    if (empty)
    {
        return false;
    }
    long result = comparator->compare(item, data);
    int old_diff = 0;
	// go left
    if (result < 0)
    {
        if (left == nullptr || left->empty)
        {
			return false;
        }
        else
        {
            old_diff = left->diff;
			if (left->_remove(item))
			{
				if (old_diff != left->diff && left->diff != 0)
				{
					diff++;
				}
				if (left->empty)
				{
					delete left;
					left = nullptr;
				}
			}
        }
    }
	// go right
    else if (result > 0)
    {
        if (right == nullptr || right->empty)
        {
			return false;
        }
        else
        {
            old_diff = right->diff;
			if (right->_remove(item))
			{
				if (old_diff != right->diff && right->diff != 0)
				{
					diff--;
				}
				if (right->empty)
				{
					delete right;
					right = nullptr;
				}
			}
        }
    }
	else
	{
		// one or no child
		if (right == nullptr)
		{
			if (left != nullptr)
			{
				data = left->data;
				delete left;
				left = nullptr;
			}
			else
			{
				this->empty = true;
			}

		}
		else if (left == nullptr)
		{
			if (right != nullptr)
			{
				data = right->data;
				delete right;
				right = nullptr;
			}
			else
			{
				this->empty = true;
			}
		}
		else
		{
			// find inorder successor
			AVLTree<T>* successor = right;
			while (successor->left != nullptr)
			{
				successor = successor->left;
			}
			data = successor->data;
			old_diff = right->diff;
			right->_remove(successor->data);
			if (right->empty || (right->diff != old_diff && right->diff == 0))
			{
				diff--;
			}
		}
	}

	rebalance();
    return true;
}

template<typename T>
bool AVLTree<T>::remove(const T item)
{
	if (empty)
	{
		return false;
	}
	int result = comparator->compare(this->data, item);
	if (result == 0)
	{
		if ((left == nullptr || left->empty) && (right == nullptr || right->empty))
		{
			empty = true;
		}
		else if ((left == nullptr || left->empty) && (right != nullptr && !right->empty))
		{
			data = right->data;
			delete right;
			right = nullptr;
			diff--;
		}
		else if ((right == nullptr || right->empty) && (left != nullptr && !left->empty))
		{
			data = left->data;
			delete left;
			left = nullptr;
			diff++;
		}
		else //if ((right == nullptr || right->empty) && (left == nullptr && left->empty))
		{
			AVLTree<T>* farthestLeft = right;
			int oldRightDiff = right->diff;
			while (farthestLeft->left != nullptr)
			{
				farthestLeft = farthestLeft->left;
			}
			data = farthestLeft->data;
			right->remove(farthestLeft->data);
			if (oldRightDiff != right->diff && right->diff == 0)
			{
				diff--;
			}

		}

	}
	if (comparator->compare(this->data, item) > 0)
	{
		if (left == nullptr)
		{
			return false;
		}
		else
		{
			int oldLeftDiff = left->diff;
			if (left->remove(item))
			{
				if (oldLeftDiff != left->diff && left->diff == 0)
				{
					diff++;
				}
				// if left child is empty, delete it
				if (left->empty)
				{
					delete left;
					left = nullptr;
				}
			}
		}
	}
	if (comparator->compare(this->data, item) < 0)
	{
		if (right == nullptr)
		{
			return false;
		}
		else
		{
			int oldRightDiff = right->diff;
			if (right->remove(item))
			{
				if (oldRightDiff != right->diff && right->diff == 0)
				{
					diff--;
				}
				// if left child is empty, delete it
				if (right->empty)
				{
					delete right;
					right = nullptr;
				}
			}
		}
	}

	rebalance();
	size--;
	return true;
}
/*
template <typename T>
bool AVLTree<T>::remove(const T item)
{
	if (empty)
	{
		return false;
	}
	long result = comparator->compare(item, data);
	int old_diff = 0;
	// go left
	if (result < 0)
	{
		if (left == nullptr || left->empty)
		{
			return false;
		}
		else
		{
			old_diff = left->diff;
			if (left->_remove(item))
			{
				if (old_diff != left->diff && left->diff != 0)
				{
					diff++;
				}
				if (left->empty)
				{
					delete left;
					left = nullptr;
				}
			}
		}
	}
	// go right
	else if (result > 0)
	{
		if (right == nullptr || right->empty)
		{
			return false;
		}
		else
		{
			old_diff = right->diff;
			if (right->_remove(item))
			{
				if (old_diff != right->diff && right->diff != 0)
				{
					diff--;
				}
				if (right->empty)
				{
					delete right;
					right = nullptr;
				}
			}
		}
	}
	else
	{
		// one or no child
		if (right == nullptr)
		{
			if (left != nullptr)
			{
				data = left->data;
				delete left;
				left = nullptr;
				diff++;
			}
			else
			{
				this->empty = true;
			}
		}
		else if (left == nullptr)
		{
			if (right != nullptr)
			{
				data = right->data;
				delete right;
				right = nullptr;
				diff--;
			}
			else
			{
				this->empty = true;
			}
		}
		else
		{
			// find inorder successor
			AVLTree<T>* successor = right;
			while (successor->left != nullptr)
			{
				successor = successor->left;
			}
			data = successor->data;
			old_diff = right->diff;
			right->_remove(successor->data);
			if (right->diff != old_diff && right->diff != 0)
			{
				diff--;
			}
		}
	}

	size--;
	rebalance();
	return true;
}
*/

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
    AVLTreeEnumerator<T> enumerator(this, order);
	return enumerator;
}

template <typename T>
bool AVLTree<T>::isEmpty() const
{
	return this->empty;
}

template <typename T>
void AVLTree<T>::rebalance()
{
	if (left == nullptr && right == nullptr)
	{
		return;
	}
    if (diff >= -1 && diff <= 1)
    {
        return;
    }
    else if (diff < 0 && left->diff <= 0) // left-left
    {
        zig();
    }
    else if (diff < 0 && left->diff > 0) // left-right
    {
        zigzag();
    }
    else if (diff > 0 && right->diff <= 0) // right-left
    {
        this->zagzig();
    }
    else if (diff > 0 && right->diff >= 0) // right-right
    {
        this->zag();
    }
}

template <typename T>
void AVLTree<T>::zig()
{
    if (left == nullptr)
    {
        return;
    }

	int gdiff = diff; 
	int pdiff = left->diff;

	AVLTree<T>* left_temp = left;
	left = left_temp->left;
	left_temp->left = left_temp->right;
	left_temp->right = right;
	right = left_temp;

	// Swap data
	T temp_data = data;
	data = left_temp->data;
	left_temp->data = temp_data;

	if (pdiff < 0) 
	{ 
		diff = gdiff + 2;
		right->diff = (gdiff - pdiff) + 1;
	}
	else 
	{ 
		diff = 1 + pdiff; 
		right->diff = 1 + gdiff; 
	}
}

template <typename T>
void AVLTree<T>::zag()
{
    if (this->right == nullptr)
    {
        return;
	}

	int gdiff = diff;
	int pdiff = right->diff;

    AVLTree<T>* right_temp = right;
    right = right_temp->right;
    right_temp->right = right_temp->left;
	right_temp->left = left;
	left = right_temp;

	// Swap data
	T temp_data = data;
	data = right_temp->data;
	right_temp->data = temp_data;

	if (pdiff > 0)
	{
		diff = gdiff - 2;
		left->diff = (gdiff - pdiff) - 1;
	}
	else
	{
		diff = pdiff - 1;
		left->diff = gdiff - 1;
	}
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
    zag();
}

#endif // !AVL_TREE


