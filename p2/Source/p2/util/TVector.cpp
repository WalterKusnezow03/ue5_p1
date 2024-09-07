

#include "TVector.h"


template <typename T>
TVector<T>::TVector(){
    sizeIndex = 0;
}

template <typename T>
TVector<T>::TVector(int size){
    sizeIndex = 0;
    vec.reserve(size);
}


template <typename T>
TVector<T>::~TVector(){
    //vec.clear();
}


template <typename T>
T & TVector<T>::operator[](int index){
    if(index >= 0 && index < size() && index < vec.size()){
        return vec.at(index);
    }
    throw std::out_of_range("Index out of range");
}



template <typename T>
int TVector<T>::size(){
    return sizeIndex;
}

template <typename T>
bool TVector<T>::empty(){
    return sizeIndex <= 0;
}

template <typename T>
void TVector<T>::clear(){
    sizeIndex = 0;
}

template <typename T>
void TVector<T>::erase(int index){
    if(index >= 0 && index < sizeIndex && vec.size() > 0){
        vec.erase(vec.begin() + index);
        sizeIndex--;
        if(sizeIndex < 0){
            sizeIndex = 0;
        }
        return;
    }
    throw std::out_of_range("Index out of range");
}


/// @brief pushes one element in the vector
/// @tparam T type of the vector
/// @param t item to push
template <typename T>
void TVector<T>::push_back(const T &t){ //its passing a reference to what ever t is, even a pointer or a value, no matter
    if(sizeIndex >= 0 && sizeIndex < vec.size()){
        vec[sizeIndex] = t;
        sizeIndex++;
    }else{
        vec.push_back(t);
        sizeIndex = vec.size();
    }
}

/// @brief push back another TVectors data into this vector
/// @tparam T 
/// @param t 
template <typename T>
void TVector<T>::push_back(const TVector<T> &t){
    for (int i = 0; i < t.size(); i++){
        push_back(t.at(i));
    }
}


template <typename T>
T& TVector<T>::front(){
    if(vec.size() > 0){
        return vec.front();
    }
    throw std::out_of_range("Index out of range");
}

template <typename T>
T& TVector<T>::back(){
    if(vec.size() > 0){
        return vec.back();
    }
    throw std::out_of_range("Index out of range");
}

template <typename T>
void TVector<T>::pop_back(){
    if(sizeIndex == vec.size() && vec.size() > 0){
        vec.pop_back();
        sizeIndex = vec.size();
    }else{
        sizeIndex--;
        if(sizeIndex < 0){
            sizeIndex = 0;
        }
    }
    throw std::out_of_range("Index out of range");
}


template <typename T>
T &TVector<T>::at(int index){
    if(index >= 0 && index < size() && index < vec.size()){
        return vec.at(index);
    }
    throw std::out_of_range("Index out of range");
}