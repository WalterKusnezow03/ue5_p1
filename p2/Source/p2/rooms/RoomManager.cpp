// Fill out your copyright notice in the Description page of Project Settings.


#include <string>
#include <map>
#include "p2/rooms/layoutCreator.h"
#include "p2/rooms/room.h"
#include "RoomManager.h"




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
            /**
             * from doc:
             *  AActor::GetActorBounds(...){}
             * 
                virtual void GetActorBounds  
                &40;  
                    bool bOnlyCollidingComponents,  
                    FVector & Origin,  
                    FVector & BoxExtent,  
                    bool bIncludeFromChildActors  
                &41; const  

             */
            FVector Origin;
            FVector Extent;
            TempActor->GetActorBounds(true, Origin, Extent); //leg mir das da rein prinzip

            /*
            FBox BoundingBox = TempActor->GetComponentsBoundingBox(true);
            FVector Origin = BoundingBox.GetCenter();
            FVector Extent = BoundingBox.GetExtent();*/

            int xScale = (int)(Extent.X * 2) / ONE_METER;
            int yScale = (int)(Extent.Y * 2) / ONE_METER;
            int zScale = (int)(Extent.Z * 2) / ONE_METER;

            //create the key
            std::string key;
            Aroom *room = Cast<Aroom>(TempActor);
            if(room != nullptr){
                roomtypeEnum type = room->readType();
                key = createKey(xScale, yScale, type);

            }

            //add the key to the map
            if(!contains(key)){
                map[key] = uclass;

                
                FString s(UTF8_TO_TCHAR(key.c_str()));
                s.Append(FString::Printf(TEXT(" room added ")));
                DebugHelper::showScreenMessage(s);

                addLog(s);
            }

            //std::string newKey = createKey(xScale, yScale);

            TempActor->Destroy();
        }



    }
}

void RoomManager::addDoor(UClass *uclass){
    if(uclass != nullptr){
        door1 = uclass;
    }
}



/// @brief returns a blueprint or nullptr for a given size targeted
/// @param x 
/// @param y 
/// @return uclass to instantiate
UClass *RoomManager::getBpFor(int x, int y, roomtypeEnum type){

    std::string key = createKey(x, y, type);
    if(contains(key)){
        UClass *fromMap = map[key];
        if(fromMap != nullptr){
            return fromMap;
        }
    }
    
    return nullptr;
}



/// @brief creates a key from 2 values
/// @param x int first
/// @param y int second
/// @return key from x and y
std::string RoomManager::createKey(int x, int y, roomtypeEnum type){
    std::string front = std::to_string(x) + "-" + std::to_string(y);
    switch(type){
        case roomtypeEnum::room:
            front += "roomkey";
            return front;
        case roomtypeEnum::staircase:
            front += "strairkey";
            return front;
        }
    return front;
}




bool RoomManager::contains(int x, int y, roomtypeEnum type){
    if(type == roomtypeEnum::staircase){
        DebugHelper::showScreenMessage("ask for strairkey", FColor::Purple);
    }
    std::string key = createKey(x, y, type);
    return contains(key); //man darf keine rückgabe typen 
}

/// @brief returns whether they key is in the map or not
/// @param key key to find
/// @return found or not
bool RoomManager::contains(std::string &key){
    if(map.find(key) != map.end()){
        return true;
    }
    return false;
    //UClass *read = map[key];
    //return read != nullptr;
}




/// @brief creates a layout based on a size inout of indices of meters
/// @param world world to spawn in
/// @param x in meters
/// @param y in meters
void RoomManager::createALayout(UWorld* world, FVector &location, int x, int y){
    //showKeys(); //debug
    //showLog(); //shows previous log

    int staircasesPerLayer = 2;
    layoutCreator l(this); // all the data will be destroyed when it goes out of scope, remember.
    l.createRooms(x, y, staircasesPerLayer);




    //copy data
    std::vector<layoutCreator::roomBounds> copy = l.copyData();

    //continue with instantiating objects
    if(EntityManager *e = EntityManager::instance()){
        for (int i = 0; i < copy.size(); i++){

            layoutCreator::roomBounds *roomToCreate = &copy.at(i);
            UClass *uclass = getBpFor(roomToCreate->xscale(), roomToCreate->yscale(), roomToCreate->readType());

            int xposInGrid = roomToCreate->xpos(); //must be converted
            int yposInGrid = roomToCreate->ypos();

            //convert positions
            int xpos = convertScaleToMeter(xposInGrid);
            int ypos = convertScaleToMeter(yposInGrid);

            FVector position(xpos, ypos, 50); //z position must be aligned too some how
            position += location;

            //create rooms
            AActor *actor = e->spawnAactor(world, uclass, position);
            actor->SetActorLocation(position);

            Aroom *aroom = Cast<Aroom>(actor);
            if(aroom != nullptr){
                //and doors / added later or on constructor. Think about it needed.

                //relative door positions (relative to bottom left corner of a room)
                std::vector<FVector> &doorPositions = roomToCreate->readRelativeDoorPositions();

                std::vector<FVector> doorPositionsConverted;

                for (int j = 0; j < doorPositions.size(); j++){
                    //convert
                    FVector adjusted = doorPositions.at(j);
                    convertScaleToMeterFVector(adjusted);

                    doorPositionsConverted.push_back(adjusted);
                }

                if(door1 == nullptr){
                    //DebugHelper::showScreenMessage("door was nullptr! - room manager");
                }

                //the room will process the postions and enable walls and doors accordingly
                aroom->processDoorPositionVectors(doorPositionsConverted, door1);
            }
        }
    }


}

/// @brief converts a unreal engine scale (cm) to m as int. Will introduce conversion loss!
/// @param a cm
/// @return a in meter, value floored
int RoomManager::convertMeterToIndex(int a){
    return (int)(a / ONE_METER);
}

/// @brief converts an meter index to cm
/// @param a meter index
/// @return as cm position / unreal position scale
int RoomManager::convertScaleToMeter(int a){
    return a * ONE_METER;
}
void RoomManager::convertScaleToMeterFVector(FVector &vector){
    vector.X = convertScaleToMeter(vector.X);
    vector.Y = convertScaleToMeter(vector.Y);
    //vector.Z = convertScaleToMeter(vector.Z);
}



//debug logging

void RoomManager::showKeys(){

    FString result = FString(TEXT("keys: "));
    FString n = FString(TEXT("\n"));

    for (auto& pair : map)
            {
                FString s = FString(pair.first.c_str());
                result += s;
                result += n;
            }
    DebugHelper::showScreenMessage(result);
}



void RoomManager::addLog(FString s){
    logResult += s;
    FString r = FString(TEXT("\n"));
    logResult += r;
}

void RoomManager::showLog(){
    DebugHelper::showScreenMessage(logResult);
}