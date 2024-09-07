#pragma once

#include "CoreMinimal.h"


/**
 * simple vector class, not iterable
 */
template <typename T>
class P2_API TVector{

public:
    TVector();
    TVector(int size);
    ~TVector();

    int size();
    bool empty();
    void clear();
    //void push_back(T t);
    void push_back(const T &t);
    void push_back(const TVector<T> &t);
    void erase(int index);
    T& front();
    T& back();
    T &at(int index);
    void pop_back();

    T &operator[](int index);
    /*
    T & operator[](int index){
        if(index >= 0 && index < size() && vec.size()){
            return vec.at(index);
        }
        throw std::out_of_range("Index out of range");
    }*/

private:
    std::vector<T> vec;

    int sizeIndex;
};