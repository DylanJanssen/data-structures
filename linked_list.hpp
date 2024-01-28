#ifndef LINKED_LIST_H
#define LINKED_LIST_H 

/*
    Linked list class
    Written by Dylan Janssen 
    Developed as a teaching execise
    Includes an iterator class to demonstrate writing our own iterators 
*/

#include <iostream> 
#include <string> 

template <typename T> 
class LinkedList 
{
private: 
    struct Node 
    {
        int key; 
        T value; 
        Node *next; 
        Node(int k, const T &v) : key(k), value(v), next(nullptr) {}
    };
    Node *head; 
    int sz; 
public: 
    LinkedList() : head(nullptr), sz(0) {} 
    LinkedList(const LinkedList &list);
    LinkedList(LinkedList &&list); 
    ~LinkedList(); 
    int size() const { return sz; }
    bool find(int key, T &value) const; 
    void insert(int key, const T &value); 
    void erase(int key); 
    LinkedList<T> operator+(const LinkedList<T> &rhs); 
    LinkedList<T> operator-(const LinkedList<T> &rhs); 
    template <typename U> 
    friend std::ostream& operator<<(std::ostream &os, const LinkedList<U> &list); 
    // Iterator Class 
    class Iterator
    {
    private: 
        Node *curr; 
    public: 
        Iterator(Node *ptr) : curr(ptr) {} 
        // prefix ++ // increments and returns incremented value 
        Iterator& operator++();
        // postfix ++ // increments and returns preincremented value 
        Iterator operator++(int);
        std::pair<int, T> operator*() const { return std::pair<int, T>(curr->key, curr->value); } // dereference 
        bool operator==(const Iterator &rhs) { return curr == rhs.curr; }
        bool operator!=(const Iterator &rhs) { return curr != rhs.curr; }
    };

    // LinkedList Iterator Methods 
    Iterator begin() const { return Iterator(head); }
    Iterator end() const { return Iterator(nullptr); }
};


template <typename T> 
LinkedList<T>::LinkedList(const LinkedList &list) : LinkedList()
{
    for (auto [key, value] : list) 
        insert(key, value);
}

template <typename T> 
LinkedList<T>::LinkedList(LinkedList &&list) : LinkedList() 
{
    swap(head, list.head); 
    swap(sz, list.sz); 
}

template <typename T> 
LinkedList<T>::~LinkedList()
{
    auto x = head; 
    while (x != nullptr)
    {
        auto next = x->next; 
        delete x; 
        x = next; 
    }
}

template <typename T> 
bool LinkedList<T>::find(int key, T &value) const
{
    auto x = head; 
    while (x != nullptr) 
    {
        if (x->key == key)
        {
            value = x->value; 
            return true; 
        }
        if (x->key > key) 
            return false; 
        x = x->next; 
    }
    return false; 
}

template <typename T> 
void LinkedList<T>::insert(int key, const T &value)
{
    auto x = head; 
    Node *prev = nullptr; 
    while (x != nullptr) 
    {
        if (x->key == key)
        {
            x->value = value; 
            return; 
        }
        if (x->key > key)
            break;
        prev = x; 
        x = x->next; 
    }
    auto *node = new Node(key, value); 
    if (prev != nullptr) // insert between prev and x 
    {
        node->next = prev->next; 
        prev->next = node; 
    }
    else // insert at head 
    {
        node->next = head; 
        head = node; 
    }
    sz++; 
}

template <typename T> 
void LinkedList<T>::erase(int key)
{
    auto x = head; 
    Node *prev = nullptr; 
    while (x != nullptr) 
    {
        if (x->key == key) // delete x 
        {
            if (prev != nullptr)
                prev->next = x->next; 
            else 
                head = x->next; 
            delete x; 
            sz--;
            return; 
        }
        if (x->key > key) 
            return; 
        prev = x; 
        x = x->next; 
    }
}

// Overload the + operator to merge two lists, removing duplicates
template <typename T> 
LinkedList<T> LinkedList<T>::operator+(const LinkedList<T> &rhs)
{
    LinkedList<T> result; 
    auto lhs_ptr = head; 
    auto rhs_ptr = rhs.head; 
    while (lhs_ptr != nullptr && rhs_ptr != nullptr) 
    {
        if (lhs_ptr->key < rhs_ptr->key)
        {
            result.insert(lhs_ptr->key, lhs_ptr->value);
            lhs_ptr = lhs_ptr->next; 
        }
        else if (rhs_ptr->key < lhs_ptr->key) 
        {
            result.insert(rhs_ptr->key, rhs_ptr->value);
            rhs_ptr = rhs_ptr->next; 
        }
        else 
        {
            result.insert(lhs_ptr->key, lhs_ptr->value);
            lhs_ptr = lhs_ptr->next; 
            rhs_ptr = rhs_ptr->next; 
        }
    }
    while (lhs_ptr != nullptr)
    {
        result.insert(lhs_ptr->key, lhs_ptr->value);
        lhs_ptr = lhs_ptr->next;
    }
    while (rhs_ptr != nullptr)
    {
        result.insert(rhs_ptr->key, rhs_ptr->value);
        rhs_ptr = rhs_ptr->next; 
    }
    return result; 
}

template <typename T> 
LinkedList<T> LinkedList<T>::operator-(const LinkedList<T> &rhs) 
{
    LinkedList<std::string> result(*this); 
    for (auto [key, value] : rhs)
        result.erase(key); 
    return result; 
}

template <typename T> 
typename LinkedList<T>::Iterator& LinkedList<T>::Iterator::operator++()
{
    curr = curr->next; 
    return *this; 
}

template <typename T> 
typename LinkedList<T>::Iterator LinkedList<T>::Iterator::operator++(int)
{
    LinkedList<T>::Iterator temp = *this; 
    curr = curr->next; 
    return temp; 
}

template <typename U> 
std::ostream& operator<<(std::ostream &os, const LinkedList<U> &list)
{
    auto x = list.head; 
    while (x != nullptr) 
    {
        os << x->key << ":" << x->value << " "; 
        x = x->next; 
    }
    return os; 
}
#endif 
