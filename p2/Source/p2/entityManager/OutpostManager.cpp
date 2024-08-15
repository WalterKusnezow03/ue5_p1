// Fill out your copyright notice in the Description page of Project Settings.

#include "p2/entityManager/Outpost.h"
#include "p2/entityManager/OutpostManager.h"

OutpostManager::OutpostManager()
{

}

OutpostManager::~OutpostManager()
{

}

//set pointer to none on start
OutpostManager *OutpostManager::instancePointer = nullptr;

OutpostManager * OutpostManager::instance(){
    if(OutpostManager::instancePointer == nullptr){
        OutpostManager::instancePointer = new OutpostManager();
    }
    return OutpostManager::instancePointer;
}


AOutpost *OutpostManager::requestOutpost(UWorld *world, FVector ownLocation){
    if(world != nullptr){

        //iterate foudn outpost, or create
        AOutpost *nearest = nearestOutpostTo(ownLocation);
        if(nearest != nullptr){
            return nearest;
        }else{
            //create outpost
            AOutpost *created = createOutpost(world, ownLocation);
            if(created != nullptr){
                return created;
            }
        }
    }
    return nullptr;
}

/// @brief tries to request an outpost and will subscribe an entity if sucessfull
/// @param world world
/// @param human entity
void OutpostManager::tryRequestOutpost(UWorld *world, AHumanEntityScript *human){
    if(world != nullptr && human != nullptr){
        FVector location = human->GetActorLocation();

        AOutpost *subscribeTo = requestOutpost(world, location);
        if(subscribeTo != nullptr){
            subscribeTo->subscribe(human);
            //human->setOutpost(subscribeTo); //outpost does this him self
        }
    }
}


/// @brief finds the nearest outpost to a location
/// @param location fvector position
/// @return AOutpost or nothing
AOutpost *OutpostManager::nearestOutpostTo(FVector location){
    if(outposts.size() <= 0){
        return nullptr;
    }

    float distanceMax = 100 * 100; //100m

    AOutpost *found = outposts.front();
    float foundDist = FVector::Dist(found->GetActorLocation(), location);

    for (int i = 0; i < outposts.size(); i++){
        AOutpost *current = outposts.at(i);
        if(current != nullptr){
            FVector Cpos = current->GetActorLocation();
            float cDist = FVector::Dist(Cpos, location);
            if(cDist < foundDist){
                foundDist = cDist;
                found = current;
            }
        }
    }

    if(foundDist <= distanceMax){
        return found;
    }
    return nullptr;
}

/// @brief creates an outpost in the given world and location
/// @param world 
/// @param Location 
/// @return 
AOutpost *OutpostManager::createOutpost(UWorld *world, FVector Location){
    if(world != nullptr){
        UClass* OutpostClass = AOutpost::StaticClass();
        if (OutpostClass)
        {
            FActorSpawnParameters SpawnParams;
            AActor* SpawnedOutpost = world->SpawnActor<AActor>(
                OutpostClass, 
                FVector::ZeroVector, 
                FRotator::ZeroRotator, 
                SpawnParams
            );

            AOutpost *p = Cast<AOutpost>(SpawnedOutpost);
            if(p != nullptr){
                return p;
            }
        }
    }
    return nullptr;
}
