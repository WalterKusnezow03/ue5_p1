// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <map>

/**
 * will save assets in a map and provide nessecarry methods
 * E is recommended to be an enum!
 * T must always be a UClass type
 */
template <typename E, typename T>
class P2_API assetManagerGeneric
{
	//static_assert(std::is_enum<E>::value, "must be an enum");
	static_assert(std::is_base_of<UClass, T>::value, "must be an uclass");

public:
	assetManagerGeneric();
	~assetManagerGeneric();

	void addBp(E e, T *t);
	T *getBp(E e);

private:
	std::map<E, T*> map;


};
