// Fill out your copyright notice in the Description page of Project Settings.


#include "AlertManager.h"
#include "CoreMinimal.h"
#include "p2/entities/EntityScript.h"
#include "GameCore/interfaces/DamageInterface/Damageinterface.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "CoreMath/Matrix/MMatrix.h"
#include "DebugPlugin/DebugHelper.h"
#include "CoreMath/util/SphereCaster.h"


AAlertManager *AAlertManager::instancePtr = nullptr;

AAlertManager* AAlertManager::Instance(UWorld* World)
{
    if(instancePtr){
        return instancePtr;
    }
    if (World){
        FActorSpawnParameters SpawnParams;
        SpawnParams.Name = TEXT("ASharedAnyMeshWidgetComponentSettings");
        FVector Location = FVector::ZeroVector;
        FRotator Rotation = FRotator::ZeroRotator;
        AAlertManager* Spawned = World->SpawnActor<AAlertManager>(
            AAlertManager::StaticClass(), Location, Rotation, SpawnParams
        );   
        instancePtr = Spawned;
        return Spawned;
    }
    return nullptr;
}
void AAlertManager::EndPlay(const EEndPlayReason::Type EndPlayReason){
    instancePtr = nullptr;
    EndPlayClear();
    Super::EndPlay(EndPlayReason);
}

void AAlertManager::EndPlayClear(){
    subscribedToAlert.clear();
}

///@brief alerts all aactors in a given area
///@param world world to get from
///@param location center of radius
///@param SphereRadius collect in radius
void AAlertManager::alertInArea(FVector location, float SphereRadius){
    
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
void AAlertManager::damageAndAlertInArea(
    UWorld *world, 
    FVector location, 
    float SphereRadius, 
    int damage,
    float damageRadius
){

    /// ------ testing needed ! ------
    AlertSubscribedActorsInRange(location, SphereRadius);

    //OLD SPHERE CAST -- needed for terrain --
    TArray<IDamageinterface*> inRangeInterfaces;
    getDamagableActorsInAreaBySphereCast(
        world, 
        location, 
        SphereRadius,
        inRangeInterfaces
    );

    //DebugHelper::logMessage("AlertManager::damageAndAlertInArea found Interfaces ", inRangeInterfaces.Num());

    for (int i = 0; i < inRangeInterfaces.Num(); i++){
        if(IDamageinterface *current = inRangeInterfaces[i]){
            //check needed here?
            if(current->IsInRange(location, damageRadius)){
                FCustomHitResult result;
                result.SetupHitResult(location, damage);
                result.SetMessage("Explosive!");
                current->takedamage(result);
            }
        }
    }

    /*
    TArray<AActor*> actors = AlertManager::getAActorsInArea(world, location, SphereRadius);
    // Process the results
    for (AActor* Actor : actors){
        // Notify or process the actor in some way (based on distance)
        if (Actor){
            
            IDamageinterface *damagable = Cast<IDamageinterface>(Actor);
            if(damagable != nullptr){

                if(damagable->IsInRange(location, damageRadius)){

                }



                //deprecated!
                float distance = FVector::Dist(Actor->GetActorLocation(), location);
                if(distance <= damageRadius){
                    
                    //new
                    FCustomHitResult result;
                    result.SetupHitResult(location, damage);
                    damagable->takedamage(result);

                    DebugHelper::logMessage(
                        FString::Printf(TEXT("AlertManager::damageAndAlertInArea::DAMAGE %d"), damage)
                    );

                    DebugHelper::showScreenMessage("damagable found, damage ", FColor::Red);
                }
            }
        }
    }*/
}

void AAlertManager::getDamagableActorsInAreaBySphereCast(
    UWorld *world, 
    const FVector &location, 
    float SphereRadius,
    TArray<IDamageinterface*> &outArray
){

    SphereCaster sphereCaster;
    TArray<IDamageinterface *> interfaces = 
        sphereCaster.getTActorsInRadius<IDamageinterface>(world, location, SphereRadius);
    for(int i = 0; i < interfaces.Num(); i++){
        if(IDamageinterface *damagable = interfaces[i]){
            
            //DebugHelper::logMessage("AlertManager::damagebleInRange Test", damagable->GetName());
            //check needed here or immidiate add?
            if(damagable->IsInRange(location, SphereRadius)){
                outArray.Add(damagable);

                //DebugHelper::logMessage("AlertManager::damagebleInRange ", damagable->GetName());
            }
        }
    }

    /*
    //deprecated!
    TArray<AActor*> actors = AlertManager::getAActorsInArea(world, location, SphereRadius);
    for(int i = 0; i < actors.Num(); i++){
        if(AActor *current = actors[i]){
            if(IDamageinterface *damagable = Cast<IDamageinterface>(current)){
                
                DebugHelper::logMessage("AlertManager::damagebleInRange Test", current->GetName());
                //check needed here or immidiate add?
                if(damagable->IsInRange(location, SphereRadius)){
                    outArray.Add(damagable);

                    DebugHelper::logMessage("AlertManager::damagebleInRange ", current->GetName());
                }
            }
        }
    }*/

}

void AAlertManager::AlertSubscribedActorsInRange(
    const FVector &location, float SphereRadius
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
}

    






///@brief gets all entity aactors in a given area
///@param world world to get fromworld)

///@param location center of radius
///@param SphereRadius collect in radius
TArray<AActor *> AAlertManager::getAActorsInArea(UWorld *world, FVector location, float SphereRadius){
    if(world != nullptr){


        // Array to store overlapping actors
        TArray<AActor*> OverlappingActors;

        // Collision query parameters
        TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes; // You can specify what kind of objects to detect
        TArray<AActor*> IgnoreActors; // Actors to ignore in the overlap check
        ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_WorldDynamic));
        ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_WorldStatic));

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
std::vector<int> AAlertManager::subscribedActorsInAreaByIndex(const FVector &location, float SphereRadius){
    std::vector<int> entitiesInAreaByIndex;
    for (int i = 0; i < subscribedToAlert.size(); i++){
        if(AEntityScript *ptr = subscribedToAlert[i]){
            float dist = FVector::Dist(location, ptr->GetActorLocation());
            if(dist <= SphereRadius){
                entitiesInAreaByIndex.push_back(i);
            }
        }
    }
    return entitiesInAreaByIndex;
}


void AAlertManager::subscribeToAlert(AEntityScript *pointer){
    if(pointer != nullptr){
        if(findIndex(pointer) == -1){
            subscribedToAlert.push_back(pointer);
        }
    }
}


void AAlertManager::unSubscribeFromAlert(AEntityScript *pointer){
    if(subscribedToAlert.size() > 0){
        int index = findIndex(pointer);
        if(index != -1){
            //swap with end
            subscribedToAlert[index] = subscribedToAlert[subscribedToAlert.size() - 1];
            subscribedToAlert.pop_back();
        }
    }
    
}

int AAlertManager::findIndex(AEntityScript *pointer){
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
 * api for minimap
 */
void AAlertManager::EntitiesInRadius(
    FVector &pos,
    float radius,
    TArray<FVector> &outputPositions
){
    TArray<AEntityScript *> inRadius = EntitiesInRadiusProtected(pos, radius);
    for(int i = 0; i < inRadius.Num(); i++){
        AEntityScript *ptr = inRadius[i];
        if(ptr != nullptr){
            outputPositions.Add(ptr->GetActorLocation());
        }
    }
}

TArray<AEntityScript*> AAlertManager::EntitiesInRadiusProtected(
    FVector &pos,
    float radius
){
    TArray<AEntityScript *> outArray;
    float radius2 = radius * radius;
    for(int i = 0; i < subscribedToAlert.size(); i++){
        AEntityScript *ptr = subscribedToAlert[i];
        if(ptr != nullptr){
            FVector comparePos = ptr->GetActorLocation();
            float distance = FVector::DistSquared(comparePos, pos);
            if(distance < radius2){
                outArray.Add(ptr);
            }
        }
    }
    return outArray;
}

void AAlertManager::EntitiesInRadiusFootPositions(
    FVector &pos,
    float radius,
    TArray<FVector> &outputPositions
){
    TArray<AEntityScript *> inRadius = EntitiesInRadiusProtected(pos, radius);
    for(int i = 0; i < inRadius.Num(); i++){
        AEntityScript *ptr = inRadius[i];
        if(ptr != nullptr){
            ptr->AppendFootPositions(outputPositions);
        }
    }
}



bool AAlertManager::AnyEntitesInRadius(
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

void AAlertManager::EntitiesInRadiusAsTransform(
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