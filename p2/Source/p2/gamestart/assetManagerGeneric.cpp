// Fill out your copyright notice in the Description page of Project Settings.


#include "assetManagerGeneric.h"

template <typename E, typename T>
assetManagerGeneric<E,T>::assetManagerGeneric()
{
}

template <typename E, typename T>
assetManagerGeneric<E,T>::~assetManagerGeneric()
{
}

template <typename E, typename T>
void assetManagerGeneric<E,T>::addBp(E e, T *t){
    if(t != nullptr){
        //Find returns a pointer to the value (which is a pointer -> making a pointer to a pointer)
        T **found = map.Find(e);
        if(found == nullptr){
            map.Add(e, t);
        }
        /*if(map.find(e) == map.end()){
            map[e] = t;
        }*/
    }
}

/// @brief will try to find the type from the map
/// @tparam E key
/// @tparam T 
/// @param e enum type
/// @return returned value pointer
template<typename E, typename T>
T* assetManagerGeneric<E,T>::getBp(E e){
    T **found = map.Find(e); //Find returns a pointer to the value (which is a pointer -> making a pointer to a pointer)
    if(found != nullptr){
        T *pt = *found;
        if(pt != nullptr){
            return pt;
        }
    }
    /*
    if(map.find(e) != map.end()){
        T *pointer = map[e];
        if(pointer != nullptr){
            return pointer;
        }
    }*/
    return nullptr;
}