// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <map>

/**
 * will save assets in a map and provide nessecarry methods
 * THIS CLASS WILL ONLY RETURN POINTERS AND IS DESIGNED TO HOLD THESE
 * FOR EXAMPLE FOR UCLASS* or UMATERIAL*
 * E is recommended to be an enum!
 * T is recommended to be a UClass type!
 * 
 * T must always be a UClass type
 */
template <typename E, typename T>
class P2_API assetManagerGeneric
{
	static_assert(std::is_enum<E>::value, "must be an enum");
	static_assert(std::is_base_of<UObject, T>::value, "must be an UObject");

public:
	assetManagerGeneric();
	~assetManagerGeneric();

	void addBp(E e, T *t);
	T *getBp(E e);

private:
	//std::map<E, T*> map;

	UPROPERTY()
	TMap<E, T *> map;
};
