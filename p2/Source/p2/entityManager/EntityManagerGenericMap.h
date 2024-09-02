// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <map>


/**
 * enity manager based on a type (aactor) and a plotted enum class, each type will be tracked 
 * in the manager
 */

template <typename E, typename T>
class P2_API EntityManagerGenericMap
{
	static_assert(std::is_enum<E>::value, "must be an enum");
	static_assert(std::is_base_of<AActor, T>::value, "must be an aactor");

public:
	EntityManagerGenericMap();
	~EntityManagerGenericMap();


	void add(E type, T *actor);
	T *getFirstActor(E type);
	bool hasActorsLeft(E type);


private:
	/// @brief saves one entity manager for each enum value
	std::map<E, EntityManagerGeneric<T> *> map;


};
