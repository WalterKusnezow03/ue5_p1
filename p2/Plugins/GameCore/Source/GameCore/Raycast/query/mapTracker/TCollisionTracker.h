#pragma once

#include "CoreMinimal.h"
#include "GameCore/Raycast/query/CollisionTracker.h"

/// @brief tracks actors by type T for collision trackings
/// @tparam T 
template <typename T>
class GAMECORE_API TCollisionTracker {

public:
    TCollisionTracker(){

    }

    ~TCollisionTracker(){

    }

    void AddIgnoredActor(T type, AActor *actor){
        CollisionTracker &ref = find(type);
        ref.AddIgnoredActor(actor);
    }

    void RemoveIgnoredActor(T type, AActor *actor){
        CollisionTracker &ref = find(type);
        ref.RemoveIgnoredActor(actor);
    }

    FCollisionQueryParams getCollisonParams(T type){
        CollisionTracker &ref = find(type);
        return ref.getCollisonParams();
    }

    FCollisionQueryParams getAllCollisonParams(){
        TArray<AActor *> allActors;
        for(auto &pair : collisionTrackerMap){
            CollisionTracker &current = pair.second;
            TArray<AActor *> currentPtrs = current.copyTracked();
            for (int i = 0; i < currentPtrs.Num(); i++){
                AActor *actor = currentPtrs[i];
                if(actor && !allActors.Contains(actor)){
                    allActors.Add(actor);
                }
            }
        }
        return CollisionTracker::makeParams(allActors);
    }

private:

    CollisionTracker &find(T type){
        if(collisionTrackerMap.find(type) == collisionTrackerMap.end()){
            collisionTrackerMap[type] = CollisionTracker();
        }
        return collisionTrackerMap[type];
    }

    std::map<T, CollisionTracker> collisionTrackerMap;
};