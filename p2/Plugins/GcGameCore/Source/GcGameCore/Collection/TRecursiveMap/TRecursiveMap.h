#pragma once 

#include "CoreMinimal.h"
#include <map>
#include "GcGameCore/Collection/interface/CollectionProperties.h"

/*
UObjectBase
 └─ UObject
      └─ AActor
*/
template <typename T>
class GCGAMECORE_API TRecursiveMap{
	//static_assert(std::is_base_of<AActor, T>::value, "T must be derived from AActor");

private:
    bool bDebugLog = true;
    bool AddRootSafety = true;

public:
    TRecursiveMap(){
        //FString message = FString::Printf(TEXT("Collector: TRecursiveMap construct"));
        //UE_LOG(LogTemp, Log, TEXT("%s"), *message);
    }
	~TRecursiveMap(){
        Clear();
    }

    void Clear(){
        //expose actors tracked to root!!
        for (int i = 0; i < actorVector.size(); i++){
            T *actor = actorVector[i];
            if(actor){
                UObject *casted = Cast<UObject>(actor);
                if(casted && AddRootSafety){
                    casted->RemoveFromRoot(); //unhide from gc!
                }
            }
        }
        actorVector.clear();
        
        for(auto &pair : mapInner){
            TRecursiveMap *current = pair.second;
            delete current;
            pair.second = nullptr;
        }
        mapInner.clear();
    }

    void Add(
        T *someObject, 
        int keyIndex,
        CollectionProperties &properties
    ){
        //properties.ShowKey("Collector: try add to t recursive map");
        if(properties.HasNextKey(keyIndex)){

            if(bDebugLog){
                properties.ShowKey("Collector has next key: ", keyIndex);
            }
            
            FString currentKey = properties.GetKey(keyIndex);
            MakeMapIfNeeded(currentKey);
            TRecursiveMap *ptr = mapInner[currentKey];
            if(ptr){
                //properties.ShowKey("Collector new map: ", keyIndex);
                keyIndex++;
                ptr->Add(someObject, keyIndex, properties);
            }
        }else{
            //arrived at lowest level
            add(someObject);
            //hide from gc

            UObject *casted = Cast<UObject>(someObject);
            if(casted && AddRootSafety){
                casted->AddToRoot();
            }
            if(bDebugLog){
                properties.ShowKey("Collector: ---ADDED--- trecursivemap ADDED!");
            }
        }
    }

    T* Pop(
        int keyIndex,
        CollectionProperties &properties
    ){
        if(properties.HasNextKey(keyIndex)){
            FString currentKey = properties.GetKey(keyIndex);
            if(mapInner.find(currentKey) != mapInner.end()){
                //properties.ShowKey("Collector has deeper map pop: ", keyIndex);
                keyIndex++;
                return mapInner[currentKey]->Pop(keyIndex, properties);
            }
        }else{
            T* found = getFirstActor();
            if(found){
                if(bDebugLog){
                    properties.ShowKey("Collector ---FOUND--- map pop ");
                }
                
                UObject *casted = Cast<UObject>(found);
                if(casted && AddRootSafety){
                    casted->RemoveFromRoot();//expose to gc
                }
            }
            return found;
        }
        return nullptr;
    }

private:
    std::map<FString, TRecursiveMap *> mapInner;

    void MakeMapIfNeeded(FString key){
        if(mapInner.find(key) == mapInner.end()){
            mapInner[key] = new TRecursiveMap<T>();
        }
    }

	
	void add(T *actor){
		if(actor != nullptr){

			for (int i = 0; i < actorVector.size(); i++){
				if(actorVector.at(i) == actor){
					return; //duplicate found
				}
			}
			actorVector.push_back(actor);
	
			//DebugHelper::showScreenMessage("released an entity! ", FColor::Yellow);
		}
	}


	T *getFirstActor(){
		if(hasActorsLeft()){
			T *actor = actorVector.back(); //get last elements, first would do shifting elements(bad)
			actorVector.pop_back();
			//DebugHelper::showScreenMessage("popped an entity! ", FColor::Yellow);
			return actor;
		}
		return nullptr;
	}

	bool hasActorsLeft(){
		return actorVector.size() > 0;
	}

private:
	/// @brief ector vector T* pointers
	std::vector<T *> actorVector;


};
