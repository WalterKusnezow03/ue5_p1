#pragma once

#include "CoreMinimal.h"


class COREMATH_API SphereCaster {

public:

    //simple
    bool AnyHitInRadius(
        UWorld *world,
        const FVector &location,
        float SphereRadius,
        AActor *excludedActor
    );
    bool AnyHitInRadius(
        UWorld *world,
        const FVector &location,
        float SphereRadius,
        TArray<AActor*> &excludedActors
    );
    //simple




    //template 
    template <typename T>
    T *getClosestTActorInRadius(
        UWorld *world, 
        const FVector &location, 
        float SphereRadius,
        TArray<AActor*> &excludedActors
    ){
        
        TArray<T*> collected = getTActorsInRadius<T>(world, location, SphereRadius, excludedActors);
        if(collected.Num() > 0){
            T *closest = collected[0];
            float distMinCached = DistSquared(location, closest);
            for (int i = 1; i < collected.Num(); i++){
                if(T *current = collected[i]){
                
                    //distMinCached updated inside, and overriden if lower than prev distance
                    if(DistSquaredLower(location, current, distMinCached)){
                        closest = current;
                    }
                }
            }
            return closest;
        }
        return nullptr;
    }

    template <typename T>
    TArray<T *> getTActorsInRadius(
        UWorld *world, 
        const FVector &location, 
        float SphereRadius
    ){
        
        TArray<AActor *> none;
        return getTActorsInRadius<T>(world, location, SphereRadius, none);
    }

    template <typename T>
    TArray<T *> getTActorsInRadius(
        UWorld *world, 
        const FVector &location, 
        float SphereRadius,
        TArray<AActor*> &excludedActors
    ){
        
        TArray<T *> collected;
        if(world){
            TArray<AActor *> rawArray = getAActorsInRadius(world, location, SphereRadius, excludedActors);
            for (int i = 0; i < rawArray.Num(); i++){
                if(AActor *current = rawArray[i]){
                    if(T* casted = Cast<T>(current)){
                        collected.Add(casted);
                    }
                }
            }
        }
        return collected;
    }

    /// ---- BASE FUNCTIONS ----

    //no excluded actors
    TArray<AActor *> getAActorsInRadius(
        UWorld *world, 
        const FVector &location, 
        float SphereRadius
    );
    //no excluded actors


    //with excluded actors
    TArray<AActor *> getAActorsInRadius(
        UWorld *world, 
        const FVector &location, 
        float SphereRadius,
        TArray<AActor*> &excludedActors
    );
    //with excluded actors

    

private:
    //if the distance is lower, the passed distance is updated and true returned.
    bool DistSquaredLower(const FVector &location, AActor *actor, float &distUpdate);
    float DistSquared(const FVector &location, AActor *actor);
};