// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <map>

/**
 * Pointers are added to root, and tracked until this object is killed.
 * 
 * 
 * will save assets in a map and provide nessecarry methods
 * THIS CLASS WILL ONLY RETURN POINTERS AND IS DESIGNED TO HOLD THESE
 * FOR EXAMPLE FOR UCLASS* or UMATERIAL*, cannot mark as uproperty, because unreal doesnt support
 * template uobjects!
 * E is recommended to be an enum!
 * T is recommended to be a UClass type!
 * 
 * T must always be a UClass type
 */
template <typename E, typename T>
class ASSETPLUGIN_API assetManagerGeneric
{
	static_assert(std::is_enum<E>::value, "must be an enum");
	static_assert(std::is_base_of<UObject, T>::value, "must be an UObject");

protected:
	void removeFromGC(T *t){
		if(t){
			t->AddToRoot();
		}
	}

	void addToGC(T *t){
		if(t){
			t->RemoveFromRoot();
		}
	}

public:
	assetManagerGeneric(){

	}
	~assetManagerGeneric(){

		//remove all Assets from root
		for(auto& Pair : map){
			E Key = Pair.Key;
			T* Value = Pair.Value;
			if (Value){
				addToGC(Value); // add back to GC
			}
		}


	}

	



	// --- ADDING / READING ASSETS ---

	void addBp(E e, T *t){
		if(t != nullptr){
			//Find returns a pointer to the value (which is a pointer -> making a pointer to a pointer)
			T **found = map.Find(e);
			if(found == nullptr){
				map.Add(e, t);

				//remove from Unreal GC
				removeFromGC(t);
			}
		}
	}

	/// @brief will try to find the type from the map
	/// @tparam E key
	/// @tparam T 
	/// @param e enum type
	T *getBp(E e){
		T **found = map.Find(e); //Find returns a pointer to the value (which is a pointer -> making a pointer to a pointer)
		if(found != nullptr){
			T *pt = *found;
			if(pt != nullptr){
				return pt;
			}
		}
		return nullptr;
	}

private:
	//std::map<E, T*> map;

	UPROPERTY()
	TMap<E, T *> map;
};
