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
    void buildTraversalStack(const AVLTree<T>* current);
public:
    AVLTreeEnumerator(const AVLTree<T>* root, AVLTreeOrder order = AVLTreeOrder.inorder);
    virtual ~AVLTreeEnumerator();
    bool hasNext() const;
    T next();                        // throws ExceptionEnumerationBeyondEnd if no next item is available
    T peek() const;                  // throws ExceptionEnumerationBeyondEnd if no next item is available
    AVLTreeOrder getOrder();         // returns the order of this enumerator (preorder, inorder, or postorder)
};

// Add your implementation below this line.



#endif // !AVL_TREE_ENUMERATOR