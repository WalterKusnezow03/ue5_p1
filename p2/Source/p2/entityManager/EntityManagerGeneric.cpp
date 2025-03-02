// Fill out your copyright notice in the Description page of Project Settings.


#include "EntityManagerGeneric.h"
#include "p2/DebugHelper.h"
/*
template <typename T>
EntityManagerGeneric<T>::EntityManagerGeneric()
{

}

template <typename T>
EntityManagerGeneric<T>::~EntityManagerGeneric(){
    actorVector.clear();
}

/// @brief returns if any actors are left in the list
/// @tparam T 
/// @return any elements left in the list
template <typename T>
bool EntityManagerGeneric<T>::hasActorsLeft(){
    return actorVector.size() > 0;
}

/// @brief add a new actor to the list
/// @tparam T type of actor
/// @param actor actor to be added
template <typename T>
void EntityManagerGeneric<T>::add(T *actor){
    //will not support duplicates
    if(actor != nullptr){
        for (int i = 0; i < actorVector.size(); i++){
            if(actorVector.at(i) == actor){
                return; //duplicate found
            }
        }
        actorVector.push_back(actor);

        //DebugHelper::showScreenMessage("released an entity! ", FColor::Yellow);
    }

    //actorVector.push_back(actor);
}

/// @brief returns an actor from the list if possible and removes it!
/// @tparam T type
/// @return actor or nullptr returned
template <typename T>
T * EntityManagerGeneric<T>::getFirstActor(){
    if(hasActorsLeft()){
        T *actor = actorVector.back(); //get last elements, first would do shifting elements(bad)
        actorVector.pop_back();
        //DebugHelper::showScreenMessage("popped an entity! ", FColor::Yellow);
        return actor;
    }
    return nullptr;
}


/// @brief tries to remove an item from the entity manager
/// @tparam T type
/// @param actor element to find erase
template <typename T>
void EntityManagerGeneric<T>::erase(T * actor){
    if(actor != nullptr){
        for (int i = 0; i < actorVector.size(); i++){
            if(actorVector.at(i) == actor){
                actorVector.erase(actorVector.begin() + i);
                return;
            }
        }
    }
    
}*/