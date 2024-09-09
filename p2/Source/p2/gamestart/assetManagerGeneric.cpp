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
        if(map.find(e) == map.end()){
            map[e] = t;
        }
    }
}

template<typename E, typename T>
T* assetManagerGeneric<E,T>::getBp(E e){
    if(map.find(e) != map.end()){
        T *pointer = map[e];
        if(pointer != nullptr){
            return pointer;
        }
    }
    return nullptr;
}