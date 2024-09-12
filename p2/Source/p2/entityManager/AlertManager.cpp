// Fill out your copyright notice in the Description page of Project Settings.


#include "AlertManager.h"
#include "CoreMinimal.h"
#include "p2/entities/EntityScript.h"
#include "p2/interfaces/Damageinterface.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"


AlertManager::AlertManager()
{
}

AlertManager::~AlertManager()
{
}

///@brief alerts all aactors in a given area
///@param world world to get from
///@param location center of radius
///@param SphereRadius collect in radius
void AlertManager::alertInArea(UWorld *world, FVector location, float SphereRadius){
    TArray<AActor*> actors = AlertManager::getAActorsInArea(world, location, SphereRadius);
    // Process the results
    for (AActor* Actor : actors){
        // Notify or process the actor in some way
        if (Actor){
            AEntityScript *entity = Cast<AEntityScript>(Actor);
            if(entity){

                if(entity->isWithinMaxRange(location)){
                    //look at immidiatly
                    entity->alarm();
                }else{
                    entity->alert();
                }
                
            }
        }
    }

}


///@brief alerts all aactors in a given area
///@param world world to get from
///@param location center of radius
///@param SphereRadius collect in radius
void AlertManager::damageAndAlertInArea(
    UWorld *world, 
    FVector location, 
    float SphereRadius, 
    int damage,
    float damageRadius
){

    TArray<AActor*> actors = AlertManager::getAActorsInArea(world, location, SphereRadius);
    // Process the results
    for (AActor* Actor : actors){
        // Notify or process the actor in some way (based on distance)
        if (Actor){
            AEntityScript *entity = Cast<AEntityScript>(Actor);
            if(entity){

                if(entity->isWithinMaxRange(location)){
                    //look at immidiatly
                    entity->alarm();
                }else{
                    entity->alert();
                }
                
            }

            IDamageinterface *damagable = Cast<IDamageinterface>(Actor);
            if(damagable != nullptr){
                
                float distance = FVector::Dist(Actor->GetActorLocation(), location);
                if(distance <= damageRadius){
                    //old
                    //damagable->takedamage(damage);
                    //new
                    damagable->takedamage(damage, location);

                    DebugHelper::showScreenMessage("damagable found, damage ", FColor::Red);
                }
            }
        }
    }
}


///@brief gets all aactors in a given area
///@param world world to get fromworld)

///@param location center of radius
///@param SphereRadius collect in radius
TArray<AActor *> AlertManager::getAActorsInArea(UWorld *world, FVector location, float SphereRadius){
    if(world != nullptr){

        float delta = AlertManager::deltaTime(world);


        // Array to store overlapping actors
        TArray<AActor*> OverlappingActors;

        // Collision query parameters
        TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes; // You can specify what kind of objects to detect
        TArray<AActor*> IgnoreActors; // Actors to ignore in the overlap check
        ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_WorldDynamic)); // Example: looking for Pawns

        // Perform the overlap query
        UKismetSystemLibrary::SphereOverlapActors(
            world,
            location,
            SphereRadius,
            ObjectTypes,
            //AActor::StaticClass(),  // Class of actors to detect, use NULL for all classes
            AActor::StaticClass(),  // Class of actors to detect, use NULL for all classes
            IgnoreActors,
            OverlappingActors
        );

        return OverlappingActors;
    }

    //empty array
    TArray<AActor*> t;
    return t;
}







/**
 * returns the world delta time
 */
float AlertManager::deltaTime(UWorld * world){
    if(world){
        return world->GetDeltaSeconds();
    }
    return 0.0f;
}