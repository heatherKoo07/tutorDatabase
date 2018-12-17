/**
    Binary Search Tree ADT
    Created by Heather Koo

    Purpose of BST for this project:
    We used two binary search trees in this project. The first binary search tree is sorted by tutor ID,
    and the second binary search is sorted by tutor name. The main purpose of the first BST is to sort the data in ascending order by ID.
    And the main purpose of the second BST is not only to sort the data in ascending order by name, but also to allow the users
    to search the data by name in the database.
 **/


#ifndef _BINARY_SEARCH_TREE
#define _BINARY_SEARCH_TREE

#include "BinaryTree.h"

/**~*~*
  DefaultLess structure returns true
  when the left input item is less than the right.
*~**/

template<class ItemType>
struct DefaultLess {
    bool operator()(const ItemType& a, const ItemType& b) const {
        return (a < b);
    }
};


template<class ItemType,
         class Less = DefaultLess<ItemType> >
class BinarySearchTree : public BinaryTree<ItemType>
{   
private:
	// internal insert node: insert newNode in nodePtr subtree
	BinaryNode<ItemType>* _insert(BinaryNode<ItemType>* nodePtr, BinaryNode<ItemType>* newNode);
   
	// internal remove node: locate and delete target node under nodePtr subtree
	BinaryNode<ItemType>* _remove(BinaryNode<ItemType>* nodePtr, const ItemType target, bool & success);
   
	// delete target node from tree, called by internal remove node
	BinaryNode<ItemType>* deleteNode(BinaryNode<ItemType>* targetNodePtr);
   
	// remove the leftmost node in the left subtree of nodePtr
	BinaryNode<ItemType>* removeLeftmostNode(BinaryNode<ItemType>* nodePtr, ItemType & successor);
   
	// search for target node
	BinaryNode<ItemType>* findNode(BinaryNode<ItemType>* treePtr, const ItemType & target) const;

public:
	// insert a node at the correct location
    bool insert(const ItemType & newEntry);
	// remove a node if found
	bool remove(const ItemType & anEntry);
	// find a target node
	bool getEntry(const ItemType & target, ItemType & returnedItem) const;
	void printSameEntry(const ItemType& anEntry, void visit(ItemType &)) const;
	int getEntryNum(const ItemType& anEntry) const;

};


///////////////////////// public function definitions ///////////////////////////

template<class ItemType, class Less>
bool BinarySearchTree<ItemType, Less>::insert(const ItemType & newEntry)
{
	BinaryNode<ItemType>* newNodePtr = new BinaryNode<ItemType>(newEntry);
	this->rootPtr = _insert(this->rootPtr, newNodePtr);
	this->count++;
	return true;
}

template<class ItemType, class Less>
bool BinarySearchTree<ItemType, Less>::remove(const ItemType & target)
{
	bool isSuccessful = false;
	this->rootPtr = _remove(this->rootPtr, target, isSuccessful);
	if (isSuccessful) this->count--;
	return isSuccessful; 
}

/**~*~*
  Member function getEntry returns true if it found the target item,
  which is returned to the caller, and false if not found.
*~**/
template<class ItemType, class Less>
bool BinarySearchTree<ItemType, Less>::getEntry(const ItemType& anEntry, ItemType & returnedItem) const
{
    BinaryNode<ItemType>* targetNodePtr = findNode(this->rootPtr, anEntry);
    if (!targetNodePtr) return false;
    returnedItem = targetNodePtr->getItem();
    return true;
}

/**~*~*
  Member function getEntryNum returns the number of keys
  which are the same as input item.
*~**/
template<class ItemType, class Less>
int BinarySearchTree<ItemType, Less>::getEntryNum(const ItemType& anEntry) const
{
	int entryCount = 0;
	BinaryNode<ItemType>* targetNodePtr = findNode(this->rootPtr, anEntry);
	while (targetNodePtr) {
		entryCount++;
		targetNodePtr = findNode(targetNodePtr->getRightPtr(), anEntry);
	}
	return entryCount;
}

/**~*~*
  Member function printSameEntry displays all same keys as the input item.
*~**/
template<class ItemType, class Less>
void BinarySearchTree<ItemType, Less>::printSameEntry(const ItemType& anEntry, void visit(ItemType &)) const
{
    BinaryNode<ItemType>* targetNodePtr = findNode(this->rootPtr, anEntry);
    if (!targetNodePtr) {
		std::cout << "The tutor ID could not be identified!\n";
		return;
    }
	std::cout <<  getEntryNum(anEntry) << " FOUND\n";
    while (targetNodePtr) {
        ItemType returnedItem = targetNodePtr->getItem();
        visit(returnedItem);
        targetNodePtr = findNode(targetNodePtr->getRightPtr(), anEntry);
    }
}


//////////////////////////// private functions ////////////////////////////////////////////

/**~*~*
  Member function _insert inserts new Node in nodePtr binary search subtree recursively,
  and returns the nodePtr.
*~**/
template<class ItemType, class Less>
BinaryNode<ItemType>* BinarySearchTree<ItemType, Less>::_insert(BinaryNode<ItemType>* nodePtr,
                                                          BinaryNode<ItemType>* newNodePtr)
{
    if (!nodePtr) nodePtr = newNodePtr;
    else if (Less()(newNodePtr->getItem(), nodePtr->getItem())) nodePtr->setLeftPtr(_insert(nodePtr->getLeftPtr(), newNodePtr));
    else nodePtr->setRightPtr(_insert(nodePtr->getRightPtr(), newNodePtr));

    return nodePtr;
}

template<class ItemType, class Less>
BinaryNode<ItemType>* BinarySearchTree<ItemType, Less>::_remove(BinaryNode<ItemType>* nodePtr,
                                                          const ItemType target,
                                                          bool & success)

{
	if (nodePtr == 0)                   
	{
		success = false;
		return 0;
	}
	if (Less()(nodePtr->getItem(), target)) nodePtr->setRightPtr(_remove(nodePtr->getRightPtr(), target, success));
	else if (!Less()(nodePtr->getItem(), target) && !Less()(target, nodePtr->getItem())) {
		if (nodePtr->getItem() != target) {
            nodePtr->setRightPtr(_remove(nodePtr->getRightPtr(), target, success));
		} else {
            nodePtr = deleteNode(nodePtr);
            success = true;
		}
	} else nodePtr->setLeftPtr(_remove(nodePtr->getLeftPtr(), target, success));

	return nodePtr;   
}

template<class ItemType, class Less>
BinaryNode<ItemType>* BinarySearchTree<ItemType, Less>::deleteNode(BinaryNode<ItemType>* nodePtr)
{
	if (nodePtr->isLeaf())				
	{
		delete nodePtr;
		nodePtr = 0;
		return nodePtr;
	}
	else if (nodePtr->getLeftPtr() == 0)  
	{
		BinaryNode<ItemType>* nodeToConnectPtr = nodePtr->getRightPtr();
		delete nodePtr;
		nodePtr = 0;
		return nodeToConnectPtr;
	}
	else if (nodePtr->getRightPtr() == 0) 
	{
		BinaryNode<ItemType>* nodeToConnectPtr = nodePtr->getLeftPtr();
		delete nodePtr;
		nodePtr = 0;
		return nodeToConnectPtr;      
	}
	else                                  
	{
		ItemType newNodeValue;
		nodePtr->setRightPtr(removeLeftmostNode(nodePtr->getRightPtr(), newNodeValue));
		nodePtr->setItem(newNodeValue);
		return nodePtr;
	}  
}

template<class ItemType, class Less>
BinaryNode<ItemType>* BinarySearchTree<ItemType, Less>::removeLeftmostNode(BinaryNode<ItemType>* nodePtr,
                                                                     ItemType & successor)
{
	if (nodePtr->getLeftPtr() == 0)
	{
		successor = nodePtr->getItem();
		return deleteNode(nodePtr);
	}
	else 
	{
		nodePtr->setLeftPtr(removeLeftmostNode(nodePtr->getLeftPtr(), successor));
		return nodePtr;
	}       
}

/**~*~*
  Member function findNode searches for the target node recursively,
  and returns the node pointer if found.
*~**/
template<class ItemType, class Less>
BinaryNode<ItemType>* BinarySearchTree<ItemType, Less>::findNode(BinaryNode<ItemType>* nodePtr,
                                                           const ItemType & target) const 
{
    if (!nodePtr) return nullptr;

    if (!Less()(nodePtr->getItem(), target) && !Less()(target, nodePtr->getItem())) return nodePtr;
    else if (Less()(nodePtr->getItem(), target)) return findNode(nodePtr->getRightPtr(), target);
    else return findNode(nodePtr->getLeftPtr(), target);
}  


#endif
