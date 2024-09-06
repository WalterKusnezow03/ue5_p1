// Fill out your copyright notice in the Description page of Project Settings.


#include <string>
#include <map>
#include "p2/rooms/layoutCreator/layoutCreator.h"
#include "p2/rooms/layoutCreator/roomBounds.h"
#include "p2/rooms/room.h"
#include "RoomManager.h"




RoomManager::RoomManager()
{
    //fill vectormap
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
            roomtypeEnum readType = roomtypeEnum::room; //default value

            Aroom *room = Cast<Aroom>(TempActor);
            if(room != nullptr){
                readType = room->readType();
            }

            //new map
            sizeData scaleDataNew(
                xScale,
                yScale,
                readType,
                uclass
            );
            if(vectorMap.find(readType) != vectorMap.end()){
                std::vector<RoomManager::sizeData> &ref = vectorMap[readType];
                ref.push_back(scaleDataNew);
            }else{
                std::vector<RoomManager::sizeData> newVec;
                newVec.push_back(scaleDataNew);
                vectorMap[readType] = newVec;
            }


            //delete again
            TempActor->Destroy();
        }



    }
}

void RoomManager::addDoor(UClass *uclass){
    if(uclass != nullptr){
        door1 = uclass;
    }
}






/// @brief creates a layout based on a size inout of indices of meters
/// @param world world to spawn in
/// @param x in meters
/// @param y in meters
void RoomManager::createALayout(UWorld* world, FVector &location, int x, int y){
    //showKeys(); //debug
    //showLog(); //shows previous log
    /*
    int staircasesPerLayer = 2;
    layoutCreator l(this); // all the data will be destroyed when it goes out of scope, remember.
    l.createRooms(x, y, staircasesPerLayer);

    //copy data
    std::vector<roomBounds> copy = l.copyData();
    std::vector<roomBounds> copyStairs = l.copyStaircaseData();

    //next layer
    l.createRooms(x, y, copyStairs, true); //sollte in ordnung sein da deepcopy und clean methode
    std::vector<roomBounds> copy1 = l.copyData();

    std::vector<std::vector<roomBounds> *> allLayers;
    allLayers.push_back(&copy);
    allLayers.push_back(&copy1);
    int height = 200;


    for (int i = 0; i < allLayers.size(); i++)
    {
        FVector offset(0, 0, i * height);
        offset += location; //apply location properly

        std::vector<roomBounds> &vec = *allLayers.at(i);
        processLayer(world, vec, offset);
    }*/

    int height = 200;
    int staircasesPerLayer = 2;
    std::vector<roomBounds> copyStairs;
    for (int i = 0; i < 2; i++)
    {
        layoutCreator l(this); // all the data will be destroyed when it goes out of scope, remember.
        if(copyStairs.size() > 0){
            l.createRooms(x, y, copyStairs, true);
        }else{
            l.createRooms(x, y, staircasesPerLayer);
        }
        std::vector<roomBounds> copy = l.copyData();
        copyStairs = l.copyStaircaseData();
        FVector offset(0, 0, i * height);
        offset += location; //apply location properly

        processLayer(world, copy, offset);
    }
}




/// @brief will process a vector of rooms and apply an offset to them
/// @param world world to spawn in
/// @param vec vector of rooms
/// @param offset offset to apply as total for the layout from origin (0,0,0)
void RoomManager::processLayer(UWorld* world, std::vector<roomBounds> &vec, FVector offset){
    //continue with instantiating objects
    if(EntityManager *e = EntityManager::instance()){
        for (int i = 0; i < vec.size(); i++){ //itertae all rooms to create

            roomBounds *roomToCreate = &vec.at(i);
            //UClass *uclass = getBpFor(roomToCreate->xscale(), roomToCreate->yscale(), roomToCreate->readType());
            UClass *uclass = roomToCreate->readBp();
            if(uclass == nullptr){ //skip any nullptr which indeed can be the case (for gaps on purpose)
                continue;
            }

            int xposInGrid = roomToCreate->xpos(); //must be converted
            int yposInGrid = roomToCreate->ypos();

            //convert positions
            int xpos = convertScaleToMeter(xposInGrid);
            int ypos = convertScaleToMeter(yposInGrid);

            FVector position(xpos, ypos, 50); //z position must be aligned too some how
            //position += location;
            position += offset;

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





RoomManager::sizeData* RoomManager::getAny(roomtypeEnum type){
    if (vectorMap.find(type) != vectorMap.end())
    {
        std::vector<RoomManager::sizeData> &ref = vectorMap[type];
        if(ref.size() > 0){
            int random = FVectorUtil::randomNumber(0, ref.size());
            random %= ref.size();
            RoomManager::sizeData *point = &ref.at(random);
            return point;
        }
        
    }
    return nullptr;
}

// ---- size data ----
RoomManager::sizeData::sizeData(int x, int y, roomtypeEnum typeIn, UClass *uclassIn){
    xsize = x;
    ysize = y;
    type = typeIn;
    uclassBp = uclassIn;
}

RoomManager::sizeData::~sizeData(){
    uclassBp = nullptr;
}

int RoomManager::sizeData::xSize(){
    return xsize;
}
int RoomManager::sizeData::ySize(){
    return ysize;
}

UClass *RoomManager::sizeData::getBp(){
    return uclassBp;
}

