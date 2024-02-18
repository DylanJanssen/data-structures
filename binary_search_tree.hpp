#ifndef BINARY_SEARCH_TREE_H
#define BINARY_SEARCH_TREE_H

/*
    Binary search tree class
    Written by Dylan Janssen 
    Developed as a teaching execise
    Separate Node struct so we can inherit BST for treap class 
*/

#include <iostream>
#include <string>
#include <vector>
#include "map.hpp" 

template <typename T>
struct Node
{
    int key;
    T value;
    Node *left, *right;
    Node(int k, const T &v) : key(k), value(v), left(nullptr), right(nullptr) {}
};

template <typename T, typename node_T = Node<T>>
class BST : public Map<T>
{
protected:
    node_T *root;
    std::vector<std::string> traversals{"Preorder", "Inorder", "Postorder"};

    void insert(node_T *&node, int key, const T &value);
    void erase(node_T *&node, int key);
    bool find(node_T *node, int key, T &value);
    void rotate_left(node_T *&node);
    void rotate_right(node_T *&node);
    void deleteTree(node_T *node);
    node_T *successor(node_T *node);
    void traverse(node_T *node, int type);
    void print(const std::string &prefix, node_T *node, bool is_right);

public:
    BST() : root(nullptr) {}
    void insert(int key, const T &value) override { insert(root, key, value); }
    void erase(int key) override { erase(root, key); }
    bool find(int key, T &value) override { return find(root, key, value); }
    void traverse(int type); // 0=preorder, 1=inorder, 2=postorder
    void print() { print("", root, false); }
    ~BST();
};



template <typename T, typename node_T>
void BST<T, node_T>::print(const std::string &prefix, node_T *node, bool is_right)
{
    if (node == nullptr)
    {
        std::cout << prefix << (is_right ? " ├───X" : " └───X") << std::endl;
        return;
    }
    std::cout << prefix;
    std::cout << (is_right ? " ├── " : " └── ");
    std::cout << node->key << std::endl;
    print(prefix + (is_right ? " │  " : "    "), node->right, true);
    print(prefix + (is_right ? " │  " : "    "), node->left, false);
}

template <typename T, typename node_T>
void BST<T, node_T>::insert(node_T *&node, int key, const T &value)
{
    if (node == nullptr) // make a new node
        node = new node_T(key, value);
    else
    {
        if (node->key == key)
        {
            node->value = value;
            return;
        }
        if (key < node->key)
            insert(node->left, key, value);
        else
            insert(node->right, key, value);
    }
}

template <typename T, typename node_T>
node_T *BST<T, node_T>::successor(node_T *node)
{
    auto curr = node->right;
    while (curr->left != nullptr)
        curr = curr->left;
    return curr;
}

template <typename T, typename node_T>
void BST<T, node_T>::erase(node_T *&node, int key)
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
            auto *succ = successor(node);
            node->key = succ->key;
            node->value = succ->value;
            erase(node->right, succ->key);
        }
    }
}

template <typename T, typename node_T>
void BST<T, node_T>::rotate_left(node_T *&node)
{
    auto temp = node->right;
    node->right = temp->left;
    temp->left = node;
    node = temp;
}

template <typename T, typename node_T>
void BST<T, node_T>::rotate_right(node_T *&node)
{
    auto temp = node->left;
    node->left = temp->right;
    temp->right = node;
    node = temp;
}

template <typename T, typename node_T>
bool BST<T, node_T>::find(node_T *node, int key, T &value)
{
    if (node == nullptr)
        return false;
    if (node->key == key)
    {
        value = node->value;
        return true;
    }
    if (key < node->key)
        return find(node->left, key, value);
    return find(node->right, key, value);
}

template <typename T, typename node_T>
void BST<T, node_T>::traverse(node_T *node, int type)
{
    if (node == nullptr)
        return;
    if (type == 0)
        std::cout << node->key << ' ';
    traverse(node->left, type);
    if (type == 1)
        std::cout << node->key << ' ';
    traverse(node->right, type);
    if (type == 2)
        std::cout << node->key << ' ';
}

template <typename T, typename node_T>
void BST<T, node_T>::traverse(int type)
{
    std::cout << traversals[type] << " : ";
    traverse(root, type);
    std::cout << std::endl;
}

template <typename T, typename node_T>
void BST<T, node_T>::deleteTree(node_T *node)
{
    if (node != nullptr)
    {
        deleteTree(node->left);
        deleteTree(node->right);
        delete node;
    }
}

template <typename T, typename node_T>
BST<T, node_T>::~BST() 
{
    deleteTree(root);
    root = nullptr;
}

#endif 
