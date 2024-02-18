/*
    Benchmarking the data structures written as teaching execises 
    Written by Dylan Janssen 
*/

#include <iostream> 
#include <vector> 
#include <numeric> 
#include <map> 
#include <set> 
#include <random> 
#include <string> 

// Include data structures 
#include "map.hpp"
#include "linked_list.hpp"
#include "skip_list.hpp"
#include "treap.hpp"
#include "binary_search_tree.hpp"


void benchmark_map(std::map<int, std::string> &s, const std::vector<int> &keys) 
{
    std::string value; 
    auto start = clock();
    for (auto k : keys) 
        s.insert({k, std::to_string(k)}); 
    auto end = clock(); 
    auto duration = double(end - start) / CLOCKS_PER_SEC; 
    std::cout << "   Insert time: " << duration << std::endl; 

    start = clock();
    for (auto k : keys) 
        if (s.find(k) == s.end())
            std::cout << "Not found" << std::endl; 
    end = clock(); 
    duration = double(end - start) / CLOCKS_PER_SEC; 
    std::cout << "   Search time: " << duration << std::endl; 
}

template <typename T>
void benchmark(Map<T> &ds, const std::vector<int> &keys)
{
    std::string value; 
    auto start = clock();
    for (auto k : keys) 
        ds.insert(k, std::to_string(k));
    auto end = clock();
    auto duration = double(end - start) / CLOCKS_PER_SEC; 
    std::cout << "    Insert time: " << duration << std::endl; 
    
    start = clock();
    for (auto k : keys) 
        if (!ds.find(k, value))
            std::cout << "Not found" << std::endl; 
    end = clock();  
    duration = double(end - start) / CLOCKS_PER_SEC;  
    std::cout << "    Search time: " << duration << std::endl; 
}

void benchmark_datastructures(const std::vector<int> keys, const std::string type)
{
    std::cout << type << std::endl; 

    LinkedList<std::string> linked_list; 
    SkipList<std::string> skip_list; 
    BST<std::string> bst; 
    Treap<std::string> treap; 
    std::map<int, std::string> map; 
    
    
    std::cout << "  Linked List" << std::endl; 
    benchmark(linked_list, keys); 
    std::cout << "  Skip List" << std::endl; 
    benchmark(skip_list, keys); 
    std::cout << "  BST" << std::endl; 
    benchmark(bst, keys); 
    std::cout << "  Treap" << std::endl; 
    benchmark(treap, keys); 
    std::cout << "  STL std::map (red-black tree)" << std::endl; 
    benchmark_map(map, keys);
    std::cout << std::endl; 
} 

int main(int argc, char **argv)
{
    if (argc != 2) 
    {
        std::cout << "USAGE: ./program_name #number_of_keys" << std::endl;
        exit(1); 
    }

    const int size = std::stoi(argv[1]);
    std::vector<int> keys(size);
    iota(keys.begin(), keys.end(), 0); 
    benchmark_datastructures(keys, "ordered data");
    
    std::vector<int> rev(keys.rbegin(), keys.rend());
    benchmark_datastructures(keys, "reversed data");

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(keys.begin(), keys.end(), std::default_random_engine(seed));
    benchmark_datastructures(keys, "shuffled data");

    return 0; 
}