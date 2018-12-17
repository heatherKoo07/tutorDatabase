/**
    Linked List ADT
    Created by Heather Koo

    Purpose of LinkedList for this project:
    to be used in Hashtable collision resolution.(each index in hash array contains a linked list.)

 **/

#ifndef PROGRAM_DOCS_LINKEDLIST_H
#define PROGRAM_DOCS_LINKEDLIST_H


template <class T>
class ListNode {
public:
    T value;
    ListNode<T>* next;

    ListNode(T nodeValue) {value = nodeValue; next = nullptr;}
};

template <class T>
class LinkedList {
private:
    ListNode<T>* head;
    int count;

public:
    LinkedList() {head = nullptr; count = 0;}
    ~LinkedList();
    ListNode<T>* getHead() const {return head;}
    bool isEmpty() const {return count == 0;}
    void setHead(ListNode<T>* h) {head = h;}
    void insertNode(T newValue);
    int getCount() const {return count;}
    void display(void visit(T &)) const;
    void writeItems(void visit(T &, std::ostream&), std::ofstream& outputFile) const;
};

/**~*~*
   Destructor
   This function deletes every node in the list.
*~**/
template <class T>
LinkedList<T>::~LinkedList() {
    if (head) {
        ListNode<T>* pCur = head;
        while (pCur) {
            ListNode<T>* pNext = pCur->next;
            delete pCur;
            pCur = pNext;
        }
    }
}

/**~*~*
   The insertNode function inserts a new node
   into the front of the linked list.
*~**/
template <class T>
void LinkedList<T>::insertNode(T newValue) {
    if (!head) {
        head = new ListNode<T>(newValue);
    } else {
        ListNode<T>* newNode = new ListNode<T>(newValue);
        newNode->next = head;
        head = newNode;
    }
    count++;
}

/**~*~*
   The display function displays all node values
   in the linked list.
*~**/
template <class T>
void LinkedList<T>::display(void visit(T &)) const {
    if (!head) return;
    ListNode<T>* pCur = head;
    while (pCur) {
        visit(pCur->value);
        pCur = pCur->next;
    }
}

/**~*~*
  The writeItems function saves all items into the output file.
*~**/

template <class T>
void LinkedList<T>::writeItems(void visit(T &, std::ostream &), std::ofstream& outputFile) const {
    ListNode<T>* pCur = head;
    while (pCur) {
        visit(pCur->value, outputFile);
        pCur = pCur->next;
    }
}

#endif //PROGRAM_DOCS_LINKEDLIST_H
