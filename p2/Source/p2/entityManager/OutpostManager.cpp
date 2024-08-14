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

    }
    return nullptr;
}

void OutpostManager::tryRequestOutpost(UWorld *world, AHumanEntityScript *human){
    if(world != nullptr && human != nullptr){
        FVector location = human->GetActorLocation();

        //iterate foudn outpost, or create
    }
}



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
