// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * Entity Manager -> this class is an individual object pool for a type parameter <T>
 * T must be derived from AActor
 */
template <typename T>
class P2_API EntityManagerGeneric{
	static_assert(std::is_base_of<AActor, T>::value, "T must be derived from AActor");

public:
	EntityManagerGeneric();
	~EntityManagerGeneric();

	
	void add(T *actor);

	void erase(T *actor);

	T *getFirstActor();

	bool hasActorsLeft();

private:
	/// @brief ector vector T* pointers
	std::vector<T *> actorVector;


};
