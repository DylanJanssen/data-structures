#ifndef MAP_H
#define MAP_H 

/*
    Abstract base class for key-value DS
    Written by Dylan Janssen 
*/

template <typename T> 
class Map 
{
public:
    virtual void insert(int key, const T &value) = 0; 
    virtual void erase(int key) = 0;
    virtual bool find(int key, T &value) = 0;
    virtual ~Map(){};
};

#endif 