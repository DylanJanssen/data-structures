#ifndef SKIP_LIST_H
#define SKIP_LIST_H 

/*
    Skip list class
    Written by Dylan Janssen 
    Developed as a teaching execise
    Skip lists improve search performance of linked lists by 
    skipping over sections of data to become comparable to binary 
    search trees
*/

#include <string> 
#include <vector> 
#include <iostream> 
#include "map.hpp"


template <typename T> 
class SkipList : public Map<T>
{
private: 
    struct SkipNode 
    {
        int key; 
        T value; 
        std::vector<SkipNode*> forward; 
        SkipNode(int k, const T &v) : key(k), value(v) {} 
    };
    SkipNode *head, *NIL; 
    double probability; 
    int random_level(); 
    int node_level(const std::vector<SkipNode*> &v) const { return v.size() - 1; }
    int sz;
    SkipNode* find(int key, std::vector<SkipNode*> &update); 
public: 
    SkipList(); 
    void insert(int key, const T &value) override; 
    bool find(int key, T &value) override; 
    void erase(int key) override; 
    void display_levels();
    void reconfigure(); 
    int get_highest_level() { return node_level(head->forward); }
    int size() { return sz; }
    ~SkipList(); 
    template <typename U> 
    friend std::ostream& operator<<(std::ostream &os, const SkipList<U> &list); 
};

template <typename T> 
SkipList<T>::SkipList() : probability(0.5), sz(0)
{
    head = new SkipNode(std::numeric_limits<int>::min(), "HEAD");
    NIL  = new SkipNode(std::numeric_limits<int>::max(), "NIL");
    head->forward.emplace_back(NIL); 
    NIL->forward.emplace_back(nullptr); 
}

template <typename T> 
int SkipList<T>::random_level()
{
    auto v = 1; 
    while (double(rand()) / RAND_MAX < probability)
        v++; 
    return v; 
}

template <typename T> 
typename SkipList<T>::SkipNode* SkipList<T>::find(int key, std::vector<SkipList<T>::SkipNode*> &update) 
{
    auto x = head; 
    auto current_maximum = node_level(head->forward); 
    for (int i = current_maximum; i >= 0; i--)
    {
        while (x->forward[i] != nullptr && x->forward[i]->key < key) 
            x = x->forward[i]; 
        update[i] = x; 
    }
    if (x->forward[0]->key == key) 
        return x->forward[0]; 
    return nullptr; 
}

template <typename T> 
bool SkipList<T>::find(int key, T &value)
{
    std::vector<SkipNode*> update(head->forward);
    auto x = find(key, update); 
    if (x != nullptr) 
    {
        value = x->value; 
        return true; 
    }
    return false; 
}

template <typename T> 
void SkipList<T>::insert(int key, const T &value) 
{
    std::vector<SkipNode*> update(head->forward); 
    auto x = find(key, update);
    if (x != nullptr) 
    {
        x->value = value; 
        return; 
    }
    auto new_node_level = random_level(); 
    auto current_level = node_level(update); 
    for (auto i = current_level + 1; i <= new_node_level; i++)
        update.emplace_back(head); 
    x = new SkipNode(key, value); 
    for (auto i = 0; i < new_node_level; i++)
    {
        x->forward.emplace_back(nullptr); 
        if (head->forward.size() <= x->forward.size())
            head->forward.emplace_back(NIL); 
    }
    for (auto i = 0; i < new_node_level; i++)
    {
        x->forward[i] = update[i]->forward[i];
        update[i]->forward[i] = x;  
    }
    sz++;
}

template <typename T> 
void SkipList<T>::erase(int key)
{
    std::vector<SkipNode*> update(head->forward); 
    auto x = find(key, update);
    if (x != nullptr && x->key == key)
    {
        for (int i = 0; i < update.size() && update[i]->forward[i] == x; i++)
            update[i]->forward[i] = x->forward[i]; 
        delete x; 
        while (head->forward[head->forward.size()-2] == NIL)
            head->forward.pop_back(); 
        sz--;
    }
}

template <typename T> 
void SkipList<T>::display_levels()
{
    for (int i = node_level(head->forward); i >= 0; i--)
    {
        auto x = head; 
        while (x->forward[0] != nullptr)
        {
            if (node_level(x->forward) >= i)
                std::cout << '-';
            else 
                std::cout << ' ';
            x = x->forward[0]; 
        }
        std::cout << std::endl; 
    }
}

template <typename T> 
void SkipList<T>::reconfigure() 
{
    // clear all levels except 0 
    auto x = head; 
    while (x->forward[0] != nullptr) 
    {
        x->forward.erase(x->forward.begin()+1, x->forward.end());
        x = x->forward[0]; 
    }
    for (int step = 2; step < sz; step *= 2) 
    {
        x = head; 
        auto prev = head; 
        bool done = false; 
        while (!done) 
        {
            // move x step times forward 
            for (int i = 0; i < step; i++)
            {
                if (x->forward[0] != nullptr)
                    x = x->forward[0]; 
                else 
                    done = true; 
            }
            if (!done) 
            {
                prev->forward.emplace_back(x);
                prev = x; 
            }
        }
        prev->forward.emplace_back(nullptr); 
    }
    head->forward.emplace_back(NIL); 
}

template <typename T> 
SkipList<T>::~SkipList()
{
    auto x = head; 
    while (x != nullptr)
    {
        auto next = x->forward[0]; 
        delete x; 
        x = next; 
    }
}

template <typename U> 
std::ostream& operator<<(std::ostream &os, const SkipList<U> &list) 
{
    auto x = list.head->forward[0]; 
    while (x->key != std::numeric_limits<int>::max())
    {
        os << "Key: " << x->key << " "
           << "Value: " << x->value << " "
           << "Level: " << list.node_level(x->forward) << std::endl;
        x = x->forward[0]; 
    }
    return os; 
}

#endif 
