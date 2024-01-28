#ifndef TREAP_H
#define TREAP_H

/*
    Treap class
    Written by Dylan Janssen 
    Developed as a teaching execise
    Treap class that hides the heap values, heap values are 
    randomly generated to attempt to automatically balance 
    a binary search tree. 
*/

#include "binary_search_tree.hpp"

template <typename T>
struct TreapNode
{
    int key;
    T value;
    TreapNode *left, *right;
    int priority;
    TreapNode(int k, T v, int p) : key(k), value(v), left(nullptr), right(nullptr), priority(p) {}
};


template <typename T, typename node_T = TreapNode<T>>
class Treap : public BST<T, node_T>
{
private:
    void insert(node_T *&node, int key, T &value);
    void erase(node_T *&node, int key);

public:
    void insert(int key, T value) { insert(this->root, key, value); }
    void erase(int key) { erase(this->root, key); }
};

template <typename T, typename node_T>
void Treap<T, node_T>::insert(node_T *&node, int key, T &value)
{
    if (node == nullptr) // make a new node
        node = new node_T(key, value, rand());
    else
    {
        if (node->key == key)
        {
            node->value = value;
            return;
        }
        if (key < node->key)
        {
            insert(node->left, key, value);
            if (node->left->priority > node->priority)
                BST<T, node_T>::rotate_right(node);
        }
        else
        {
            insert(node->right, key, value);
            if (node->right->priority > node->priority)
                BST<T, node_T>::rotate_left(node);
        }
    }
}

template <typename T, typename node_T>
void Treap<T, node_T>::erase(node_T *&node, int key)
{
    if (node == nullptr)
        return;
    if (key > node->key)
        erase(node->right, key);
    else if (key < node->key)
        erase(node->left, key);
    else
    {
        if (node->left == nullptr && node->right == nullptr)
        {
            delete node;
            node = nullptr;
        }
        else if (node->left == nullptr || node->right == nullptr)
        {
            auto temp = node;
            node = (node->left == nullptr) ? node->right : node->left;
            delete temp;
        }
        else
        {
            if (node->left->priority < node->right->priority)
            {
                rotate_left(node);
                erase(node->left, key);
            }
            else
            {
                rotate_right(node);
                erase(node->right, key);
            }
        }
    }
}

#endif 
