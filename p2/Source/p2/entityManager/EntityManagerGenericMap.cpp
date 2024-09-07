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




/// @brief adds an actor to the proper entity manager
/// @tparam E enum type
/// @tparam T actor to save
/// @param type enum type
/// @param actor enum type
template <typename E, typename T>
void EntityManagerGenericMap<E,T>::add(E type, T *actor){
    //DebugHelper::showScreenMessage("GENERIC MAP ADD", FColor::Blue);

    if(map.find(type) != map.end()){
        EntityManagerGeneric<T> *fromMap = map[type];
        fromMap->add(actor);
    }else{
        EntityManagerGeneric<T> *newManager = new EntityManagerGeneric<T>();
        newManager->add(actor);
        map[type] = newManager;
    }
}

/// @brief returns an actor from the proper entity manager based on the enum value
/// @tparam E enum type to access
/// @tparam T actor type to save
/// @param type returns the first actor bound to the type 
/// @return actor based in type or nullptr if couldnt get the actor
template <typename E, typename T>
T* EntityManagerGenericMap<E,T>::getFirstActor(E type){

    //DebugHelper::showScreenMessage("GENERIC MAP REQUEST", FColor::Blue);
    if (map.find(type) != map.end())
    {
        EntityManagerGeneric<T> *fromMap = map[type];
        if(fromMap != nullptr && fromMap->hasActorsLeft()){
            //DebugHelper::showScreenMessage("GENERIC MAP REQUEST SUCCESS", FColor::Blue);
            return fromMap->getFirstActor();
        }
    }

    return nullptr;
}

/// @brief returns if any actor is left in the proper entity manager based on the enum value
/// @tparam E enum type to access
/// @tparam T actor type 
/// @param type enum type to check for
/// @return bool hasleft or not, can get any or not
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



/// @brief tries to remove an item from the entity manager
/// @tparam E type to erase
/// @tparam T type of actors
/// @param actor element to find erase
template <typename E, typename T>
void EntityManagerGenericMap<E,T>::erase(E type, T * actor){
    if(map.find(type) != map.end() && actor != nullptr){
        EntityManagerGeneric<T> *pointer = map[type];
        if(pointer != nullptr){
            pointer->erase(actor);
        }
    }
}