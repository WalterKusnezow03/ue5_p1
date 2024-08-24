// Fill out your copyright notice in the Description page of Project Settings.


#include "EntityManagerGeneric.h"

template <typename T>
EntityManagerGeneric<T>::EntityManagerGeneric()
{

}

template <typename T>
EntityManagerGeneric<T>::~EntityManagerGeneric(){
    if(hasActorsLeft()){
        while(hasActorsLeft()){
            //erase elements but dont call destructor. Not the owner of the entites!
            actorVector.erase(actorVector.end() + 0); 
        }
    }

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
    actorVector.push_back(actor);
}

/// @brief returns an actor from the list if possible
/// @tparam T type
/// @return actor or nullptr returned
template <typename T>
T * EntityManagerGeneric<T>::getFirstActor(){
    if(hasActorsLeft()){
        T *actor = actorVector.back(); //get last elements, first would do shifting elements(bad)
        actorVector.pop_back();
        return actor;
    }
    return nullptr;
}
