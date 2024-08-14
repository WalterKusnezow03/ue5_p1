// Fill out your copyright notice in the Description page of Project Settings.


#include "AlertManager.h"
#include "CoreMinimal.h"
#include "p2/entities/EntityScript.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"


AlertManager::AlertManager()
{
}

AlertManager::~AlertManager()
{
}



void AlertManager::alertInArea(UWorld *world, FVector location, float SphereRadius){
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

        // Process the results
        for (AActor* Actor : OverlappingActors){
            // Notify or process the actor in some way
            if (Actor){
                AEntityScript * entity = Cast<AEntityScript>(Actor);
                if(entity){
                    entity->alert();
                    //entity->alarm();
                }
            }
        }
    }

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