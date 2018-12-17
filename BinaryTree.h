// Binary tree abstract base class
// Created by Habin Cho and Heather Koo
// Purpose of BinaryTree.h : a parent class of BinarySearchTree class
 
#ifndef _BINARY_TREE
#define _BINARY_TREE

#include "BinaryNode.h"
#include <iomanip>

template<class ItemType>
class BinaryTree
{
protected:
	BinaryNode<ItemType>* rootPtr;		// ptr to root node
	int count;							// number of nodes in tree

public:
    // "admin" functions
 	BinaryTree() {rootPtr = 0; count = 0;}
	BinaryTree(const BinaryTree<ItemType> & tree);
	virtual ~BinaryTree() {destroyTree(rootPtr); rootPtr = 0; count = 0;}
	BinaryTree & operator = (const BinaryTree & sourceTree);
   
	// common functions for all binary trees
 	bool isEmpty() const	{return count == 0;}
	int size() const	    {return count;}
	void clear()			{rootPtr = nullptr; count = 0;}
	void preOrder(void visit(ItemType &)) const {_preorder(visit, rootPtr);}
	void inOrder(void visit(ItemType &)) const  {_inorder(visit, rootPtr);}
	void postOrder(void visit(ItemType &)) const{_postorder(visit, rootPtr);}

	void preOrder_iter(void visit(ItemType &)) const;
    void inOrder_iter(void visit(ItemType &)) const;
    void postOrder_iter(void visit(ItemType &)) const;

    void breadthFirstTraversal(void visit(ItemType &)) const;
    void printIndentedList(void visit(ItemType &)) const;

	// abstract functions to be implemented by derived class
	virtual bool insert(const ItemType & newData) = 0; 
	virtual bool remove(const ItemType & data) = 0; 
	virtual bool getEntry(const ItemType & anEntry, ItemType & returnedItem) const = 0;

private:   
	// delete all nodes from the tree
	void destroyTree(BinaryNode<ItemType>* nodePtr);

	// copy from the tree rooted at nodePtr and returns a pointer to the copy
	BinaryNode<ItemType>* copyTree(const BinaryNode<ItemType>* nodePtr);

	// internal traverse
	void _preorder(void visit(ItemType &), BinaryNode<ItemType>* nodePtr) const;
	void _inorder(void visit(ItemType &), BinaryNode<ItemType>* nodePtr) const;
	void _postorder(void visit(ItemType &), BinaryNode<ItemType>* nodePtr) const;
    void _printIndentedList(void visit(ItemType &), BinaryNode<ItemType>* nodePtr, int level) const;
};

///////////////////////// public function definitions ///////////////////////////

/**~*~*
   Copy constructor (Deep copy)
*~**/
template<class ItemType>
BinaryTree<ItemType>::BinaryTree(const BinaryTree<ItemType> & tree) {
    rootPtr = copyTree(tree.rootPtr);
    count = tree.count;
}

/**~*~*
   Overloaded assignment operator (Shallow copy)
*~**/

template<class ItemType>
BinaryTree<ItemType> & BinaryTree<ItemType>::operator=(const BinaryTree<ItemType> & sourceTree) {
    this->rootPtr = sourceTree.rootPtr;
    this->count = sourceTree.count;
    return *this;
}

/**~*~*
  Member function printIndentedList calls the private function
  to display a tree as an indented list with level numbers.
*~**/
template<class ItemType>
void BinaryTree<ItemType>::printIndentedList(void visit(ItemType &)) const {
    int level = 0;
    _printIndentedList(visit, rootPtr, level);
}


//////////////////////////// private functions ////////////////////////////////////////////

/**~*~*
   Member function copyTree copies from the tree rooted at nodePtr
   and returns a pointer to the copy
*~**/
template<class ItemType>
BinaryNode<ItemType>* BinaryTree<ItemType>::copyTree(const BinaryNode<ItemType>* nodePtr)
{
    if (!nodePtr) return nullptr;
    BinaryNode<ItemType>* newNodePtr = new BinaryNode<ItemType>(nodePtr->getItem());

    newNodePtr->setLeftPtr(copyTree(nodePtr->getLeftPtr()));
    newNodePtr->setRightPtr(copyTree(nodePtr->getRightPtr()));

    return newNodePtr;
}

/**~*~*
   Member function destroyTree frees allocated memory of all nodes from the tree
*~**/
template<class ItemType>
void BinaryTree<ItemType>::destroyTree(BinaryNode<ItemType>* nodePtr)
{
    if (nodePtr) {
        destroyTree(nodePtr->getLeftPtr());
        destroyTree(nodePtr->getRightPtr());
        delete nodePtr;
    }

}

/**~*~*
  Member function _preorder traverses a tree in depth-first pre-order recursively,
  and displays values using a function pointer.
*~**/
template<class ItemType>
void BinaryTree<ItemType>::_preorder(void visit(ItemType &), BinaryNode<ItemType>* nodePtr) const
{
    if (nodePtr != 0)
    {
        ItemType anItem = nodePtr->getItem();
        visit(anItem);
        _preorder(visit, nodePtr->getLeftPtr());
        _preorder(visit, nodePtr->getRightPtr());
    }
}

/**~*~*
  Member function _inorder traverses a tree in depth-first in-order recursively,
  and displays values using a function pointer.
*~**/
template<class ItemType>
void BinaryTree<ItemType>::_inorder(void visit(ItemType &), BinaryNode<ItemType>* nodePtr) const
{
    if (nodePtr) {
        ItemType anItem = nodePtr->getItem();
        _inorder(visit, nodePtr->getLeftPtr());
        visit(anItem);
        _inorder(visit, nodePtr->getRightPtr());
    }
}

/**~*~*
  Member function _postorder traverses a tree in depth-first post-order recursively,
  and displays values using a function pointer.
*~**/
template<class ItemType>
void BinaryTree<ItemType>::_postorder(void visit(ItemType &), BinaryNode<ItemType>* nodePtr) const
{
    if (nodePtr) {
        ItemType anItem = nodePtr->getItem();
        _postorder(visit, nodePtr->getLeftPtr());
        _postorder(visit, nodePtr->getRightPtr());
        visit(anItem);
    }
}

/**~*~*
  Member function _printIndentedList traverses a tree in mirror order recursively,
  and displays it as an indented list with level numbers.
*~**/
template<class ItemType>
void BinaryTree<ItemType>::_printIndentedList(void visit(ItemType &), BinaryNode<ItemType>* nodePtr, int level) const {
    level++;
    if (nodePtr) {
        ItemType anItem = nodePtr->getItem();
        _printIndentedList(visit, nodePtr->getRightPtr(), level);
        std::cout << std::setw((level-1)*7) << level << ". ";
        visit(anItem);
        _printIndentedList(visit, nodePtr->getLeftPtr(), level);
    }
}


#endif

