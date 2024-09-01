// Fill out your copyright notice in the Description page of Project Settings.



#include "p2/entityManager/EntityManagerGenericMap.h"
#include "p2/entityManager/EntityManagerGeneric.h"

template <typename E, typename T>
EntityManagerGenericMap<E,T>::EntityManagerGenericMap()
{

}

template <typename E, typename T>
EntityManagerGenericMap<E,T>::~EntityManagerGenericMap()
{
    for(auto &pair : map){
        EntityManagerGeneric<T> *val = pair.second;
        if(val != nullptr){
            delete val; //delete the created entity manager which wont be accessed anyway from outside. 
        }
    }
}





template <typename E, typename T>
void EntityManagerGenericMap<E,T>::add(E type, T *actor){
    DebugHelper::showScreenMessage("GENERIC MAP ADD", FColor::Blue);

    if(map.find(type) != map.end()){
        EntityManagerGeneric<T> *fromMap = map[type];
        fromMap->add(actor);
    }else{
        EntityManagerGeneric<T> *newManager = new EntityManagerGeneric<T>();
        newManager->add(actor);
        map[type] = newManager;
    }
}

template <typename E, typename T>
T* EntityManagerGenericMap<E,T>::getFirstActor(E type){

    DebugHelper::showScreenMessage("GENERIC MAP REQUEST", FColor::Blue);
    if (map.find(type) != map.end())
    {
        EntityManagerGeneric<T> *fromMap = map[type];
        if(fromMap != nullptr && fromMap->hasActorsLeft()){
            DebugHelper::showScreenMessage("GENERIC MAP REQUEST SUCCESS", FColor::Blue);
            return fromMap->getFirstActor();
        }
    }

    return nullptr;
}

template <typename E, typename T>
bool EntityManagerGenericMap<E,T>::hasActorsLeft(E type){
    if(map.find(type) != map.end()){
        EntityManagerGeneric<T> *fromMap = map[type];
        if(fromMap != nullptr && fromMap->hasActorsLeft()){
            return true;
        }
    }
    return false;
}