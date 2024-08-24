// Fill out your copyright notice in the Description page of Project Settings.


#include <string>
#include <map>
#include "p2/entityManager/RoomManager.h"

RoomManager::RoomManager()
{
}

RoomManager::~RoomManager()
{
}

void RoomManager::add(UWorld *world, UClass *uclass){
    if(uclass != nullptr && world != nullptr){
        //get bounding box for size?

        //UClass* MyBlueprintClass = ...; // Your UClass reference
        AActor* TempActor = world->SpawnActor<AActor>(uclass);

        if (TempActor)
        {
            FBox BoundingBox = TempActor->GetComponentsBoundingBox(true);
            FVector Origin = BoundingBox.GetCenter();
            FVector Extent = BoundingBox.GetExtent();

            int xScale = (int) (Extent.X * 2) % ONE_METER;
            int yScale = (int) (Extent.Y * 2) % ONE_METER;
            int zScale = (int) (Extent.Z * 2) % ONE_METER;


            std::string key = createKey(xScale, yScale);
            if(!contains(key)){
                map[key] = uclass;
            }

            //std::string newKey = createKey(xScale, yScale);

            TempActor->Destroy();
        }



    }
}



/// @brief returns a blueprint or nullptr for a given size targeted
/// @param x 
/// @param y 
/// @return uclass to instantiate
UClass *RoomManager::getBpFor(int x, int y){

    std::string key = createKey(x, y);
    UClass *fromMap = map[key];
    if(fromMap != nullptr){
        return fromMap;
    }
    return nullptr;
}



std::string RoomManager::createKey(int x, int y){
    return (std::to_string(x) + "-" + std::to_string(y));
}


bool RoomManager::contains(int x, int y){
    std::string key = createKey(x, y);
    return contains(key); //man darf keine rückgabe typen 
}

bool RoomManager::contains(std::string &key){
    UClass *read = map[key];
    return read != nullptr;
}