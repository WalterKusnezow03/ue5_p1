// Fill out your copyright notice in the Description page of Project Settings.


#include "AlertManager.h"
#include "CoreMinimal.h"
#include "p2/entities/EntityScript.h"
#include "GameCore/interfaces/Damageinterface.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "CoreMath/Matrix/MMatrix.h"
#include "DebugPlugin/DebugHelper.h"

std::vector<AEntityScript *> AlertManager::subscribedToAlert;

AlertManager::AlertManager()
{
}

AlertManager::~AlertManager()
{
}

void AlertManager::EndPlay(){
    subscribedToAlert.clear();
}

///@brief alerts all aactors in a given area
///@param world world to get from
///@param location center of radius
///@param SphereRadius collect in radius
void AlertManager::alertInArea(UWorld *world, FVector location, float SphereRadius){
    
    std::vector<int> indexArray = subscribedActorsInAreaByIndex(location, SphereRadius);
    //DebugHelper::showScreenMessage("ALERT MANAGER: ", indexArray.size(), FColor::Orange);
    for (int i = 0; i < indexArray.size(); i++){
        int currentIndex = indexArray[i];
        
        AEntityScript *entity = subscribedToAlert[currentIndex];
        if(entity){
            //DebugHelper::showScreenMessage("ALERT ", FColor::Orange);
            if(entity->isWithinMaxRange(location)){
                //look at immidiatly
                entity->alarm();
            }

            entity->alert(location);
        }
    }
    
    
    /*
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
    }*/

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

    std::vector<int> indexArray = subscribedActorsInAreaByIndex(location, SphereRadius);
    for (int i = 0; i < indexArray.size(); i++){
        int currentIndex = indexArray[i];
        AEntityScript *entity = subscribedToAlert[currentIndex];
        if(entity){
            if(entity->isWithinMaxRange(location)){
                //look at immidiatly
                entity->alarm();
            }else{
                entity->alert(location);
            }
        }
    }

    //OLD SPHERE CAST
    
    TArray<AActor*> actors = AlertManager::getAActorsInArea(world, location, SphereRadius);
    // Process the results
    for (AActor* Actor : actors){
        // Notify or process the actor in some way (based on distance)
        if (Actor){
            /*
            AEntityScript *entity = Cast<AEntityScript>(Actor);
            if(entity){

                if(entity->isWithinMaxRange(location)){
                    //look at immidiatly
                    entity->alarm();
                }else{
                    entity->alert();
                }   
            }*/

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


///@brief gets all entity aactors in a given area
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
 * subscription
 */
std::vector<int> AlertManager::subscribedActorsInAreaByIndex(FVector &location, float SphereRadius){
    std::vector<int> entitiesInAreaByIndex;
    for (int i = 0; i < subscribedToAlert.size(); i++)
    {
        AEntityScript *ptr = subscribedToAlert[i];
        if(ptr){
            float dist = FVector::Dist(location, ptr->GetActorLocation());
            if(dist <= SphereRadius){
                entitiesInAreaByIndex.push_back(i);
            }
        }
    }
    return entitiesInAreaByIndex;
}


void AlertManager::subscribeToAlert(AEntityScript *pointer){
    if(pointer != nullptr){
        if(findIndex(pointer) == -1){
            subscribedToAlert.push_back(pointer);
        }
    }
}


void AlertManager::unSubscribeFromAlert(AEntityScript *pointer){
    if(subscribedToAlert.size() > 0){
        int index = findIndex(pointer);
        if(index != -1){
            //swap with end
            subscribedToAlert[index] = subscribedToAlert[subscribedToAlert.size() - 1];
            subscribedToAlert.pop_back();
        }
    }
    
}

int AlertManager::findIndex(AEntityScript *pointer){
    int index = -1;
    if(pointer){
        for (int i = 0; i < subscribedToAlert.size(); i++){
            if(subscribedToAlert[i] == pointer){
                return i;
            }
        }
    }
    return index;
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



/**
 * api for minimap
 */
void AlertManager::EntitiesInRadius(
    FVector &pos,
    float radius,
    TArray<FVector> &outputPositions
){
    for(int i = 0; i < subscribedToAlert.size(); i++){
        AEntityScript *ptr = subscribedToAlert[i];
        if(ptr != nullptr){
            FVector comparePos = ptr->GetActorLocation();
            float distance = FVector::Dist(comparePos, pos);
            if(distance < radius){
                outputPositions.Add(comparePos);
            }
        }
    }
}

bool AlertManager::AnyEntitesInRadius(
    FVector &pos,
    float radius
){
    float distSquared = radius * radius;
    for(int i = 0; i < subscribedToAlert.size(); i++){
        AEntityScript *ptr = subscribedToAlert[i];
        if(ptr != nullptr){
            FVector comparePos = ptr->GetActorLocation();
            float distance = FVector::DistSquared(comparePos, pos);
            if(distance < distSquared){
                return true;
            }
        }
    }
    return false;
}

void AlertManager::EntitiesInRadiusAsTransform(
    FVector &pos,
    float radius,
    TArray<MMatrix> &outputMatrices
){
    for(int i = 0; i < subscribedToAlert.size(); i++){
        AEntityScript *ptr = subscribedToAlert[i];
        if(ptr != nullptr){
            FVector comparePos = ptr->GetActorLocation();
            float distance = FVector::Dist(comparePos, pos);
            if(distance < radius){
                FRotator rot = ptr->GetActorRotation();
                
                MMatrix r(rot);
                MMatrix t(comparePos);
                
                MMatrix mat = t * r;//M = T * R
                
                outputMatrices.Add(mat);
            }
        }
    }
}