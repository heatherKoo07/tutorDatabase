/**
    Hash Table ADT
    Created by Heather Koo

    Purpose of HashTable.h: to create Hashtable object to store, search, and delete any data type T.
    When the database is large, it provides efficient search.
    Singly linked list collision resolution is used.
**/

#ifndef PROGRAM_DOCS_HASHTABLE_H
#define PROGRAM_DOCS_HASHTABLE_H

#include <vector>
#include <string>
#include "LinkedList.h"
using namespace std;

/**~*~*
  DefaultEqual structure returns true
  when the left input item is the same as the right.
*~**/
template<class T>
struct DefaultEqual {
    bool operator()(const T& a, const T& b) const {
        return (a == b);
    }
};

/**~*~*
  DefaultKey structure uses the input item as a key of hash table.
*~**/
template<class T>
struct DefaultKey {
    size_t operator()(const T& a) const {
        return a;
    }
};

template <class T, class K=DefaultKey<T>, class E=DefaultEqual<T>>
class HashTable {
private:
    int size;
    vector<LinkedList<T>> hashArr;
    int arrCount;
    int totalCount;
    int hash(const T&) const;

public:
    class iterator {
        friend class HashTable;

    private:
        iterator(ListNode<T>* _cursor): cursor(_cursor) {}

        ListNode<T>* cursor;
    public:
        iterator(): cursor(nullptr) {}
        bool operator==(const iterator& src) const {
            return (cursor == src.cursor);
        }
        bool operator!=(const iterator& src) const {
            return (cursor != src.cursor);
        }

        T& operator*() const {
            return cursor->value;
        }
        //++A
        iterator& operator++() {
            if (!cursor) {
                cursor = nullptr;
                return *this;
            }
            cursor = cursor->next;
            return *this;
        }
        //A++
        iterator& operator++(int) {return operator++();}

    };

    HashTable(int s) {size = s; hashArr.resize(s); arrCount = totalCount = 0;}

    ~HashTable(){};

    iterator find(T key) const {
        size_t idx = hash(key);
        ListNode<T>* cursor = hashArr[idx].getHead();
        return iterator(cursor);
    }
    iterator end() const { return iterator(); }

    void erase(iterator& prev, iterator& cur, const T& targetValue) {
        if (!prev.cursor) {
            hashArr[hash(targetValue)].setHead(cur.cursor->next);
        } else prev.cursor->next = cur.cursor->next;
        delete cur.cursor;

    }

    void hashInsert(const T&);
    bool hashErase(const T&);
    void hashDisplay(void visit(T &)) const;
    bool hashSearch(const T&, T&) const;
    int getCollisionNum() const;
    int getLoadFactor() const;
    int getTotalCount() const {return totalCount;}
    void writeItems(void visit(T &, ostream&), ofstream&) const;
};

//////////////////////////// private functions ////////////////////////////////////////////

/**~*~*
  Member function hash returns the index of the hash array
  by using the mid-square method.
*~**/
template <class T, class K, class E>
int HashTable<T, K, E>::hash(const T& keyObj) const {
    // mid square
    int mid = K()(keyObj)/100%10000;
    return (mid*mid)%size;
}

///////////////////////// public function definitions ///////////////////////////

/**~*~*
  Member function hashInsert inserts a new entry into the corresponding linked list
  at its hash index.
*~**/
template <class T, class K, class E>
void HashTable<T, K, E>::hashInsert(const T& newEntry) {
    int index = hash(newEntry);
    if (hashArr[index].isEmpty()) arrCount++;
    hashArr[index].insertNode(newEntry);
    totalCount++;
}

/**~*~*
  Member function hashDisplay displays all items in the structure.
*~**/
template <class T, class K, class E>
void HashTable<T, K, E>::hashDisplay(void visit(T &)) const {

    for (auto i = hashArr.begin(); i != hashArr.end(); ++i) {
        (*i).display(visit);
    }
}

/**~*~*
  Member function hashSearch returns true if it found the target item,
  which is returned to the caller, and false if not found.
*~**/
template <class T, class K, class E>
bool HashTable<T, K, E>::hashSearch(const T& searchValue, T& returnedValue) const {
    auto i = find(searchValue);
    while (i != end()) {
        if (E()(*i, searchValue)) {
            returnedValue = *i;
            return true;
        }
        ++i;
    }
    return false;

}

/**~*~*
  Member function hashErase returns true if it found and deleted the target item,
  and false if not found.
*~**/
template <class T, class K, class E>
bool HashTable<T, K, E>::hashErase(const T& targetValue) {
    auto i = find(targetValue);
    iterator prev;
    while (i != end()) {
        if (E()(*i, targetValue)) {
            erase(prev, i, targetValue);
            totalCount--;
            if (!totalCount) arrCount--;
            return true;
        }
        prev = i;
        ++i;
    }
    return false;
}

/**~*~*
  Member function getCollisionNum returns the number of collision in the hash table.
*~**/
template <class T, class K, class E>
int HashTable<T, K, E>::getCollisionNum() const {
    return totalCount - arrCount;
}

/**~*~*
  Member function getLoadFactor returns the load factor of the hash table.
*~**/
template <class T, class K, class E>
int HashTable<T, K, E>::getLoadFactor() const {
    return (double)arrCount/size*100;
}

/**~*~*
  Member function writeItems saves all items into the output file.
*~**/
template <class T, class K, class E>
void HashTable<T, K, E>::writeItems(void visit(T &, ostream&), ofstream& outputFile) const {
    for (int i = 0; i < size; i++) {
        hashArr[i].writeItems(visit, outputFile);
    }
}


#endif //PROGRAM_DOCS_HASHTABLE_H
