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
            int zScaleRaw = (int)(Extent.Z * 2);
            int zScale = (int)(Extent.Z * 2) / ONE_METER;

            //setting the largest zscale found for now
            if(zScaleRaw > zWallHeight){
                zWallHeight = zScaleRaw;
            }

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





/// @brief creates a layout based on a size inout of indices of meters
/// @param world world to spawn in
/// @param x in meters
/// @param y in meters
void RoomManager::createALayout(UWorld* world, FVector &location, int x, int y){
    

    //default method part
    int height = 200; // 200;
    //height = zWallHeight; //new

    int staircasesPerLayer = 2;
    std::vector<roomBounds> copyStairs;
    std::vector<TTouple<int, int>> reverseBlock;
    // std::vector<roomBounds> copy;
    for (int i = 0; i < 2; i++)
    {
        layoutCreator l(this); // all the data will be destroyed when it goes out of scope, remember.
        if(copyStairs.size() > 0){
            l.createRooms(x, y, copyStairs, true, reverseBlock);
        }else{
            l.createRooms(x, y, staircasesPerLayer);
            reverseBlock = l.getInverseBlockList();

        }

        std::vector<roomBounds> copy = l.copyData();
        copyStairs = l.copyStaircaseData();
        FVector offset(0, 0, height * i);
        offset += location; //apply location properly

        //process layer and add up new height from the just created layer
        processLayer(world, copy, offset);
    }
}




/// @brief will process a vector of rooms and apply an offset to them
/// @param world world to spawn in
/// @param vec vector of rooms
/// @param offset offset to apply as total for the layout from origin (0,0,0)
void RoomManager::processLayer(
    UWorld* world, 
    std::vector<roomBounds> &vec, 
    FVector offset
){
    
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

            FVector position(xpos, ypos, 50); //z position must be aligned too some how (later in terrain integrate)
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
                std::vector<FVector> &windowPositions = roomToCreate->readRelativeWindowPositions();

                std::vector<FVector> doorPositionsConverted;
                std::vector<FVector> windowPositionsConverted;

                for (int j = 0; j < doorPositions.size(); j++){
                    //convert door
                    FVector adjusted = doorPositions.at(j);
                    convertScaleToMeterFVector(adjusted);
                    doorPositionsConverted.push_back(adjusted);
                }

                for (int j = 0; j < windowPositions.size(); j++){
                    //convert window
                    FVector adjusted = windowPositions.at(j);
                    convertScaleToMeterFVector(adjusted);
                    windowPositionsConverted.push_back(adjusted);
                }

                //create the walls first!
                if(assetManager *a = assetManager::instance()){
                    //spawn all walls
                    aroom->spawnWalls(a->findBp(roomAssetEnum::wallEnum));

                    //the room will process the postions and enable walls and doors accordingly
                    aroom->processPositionVectorsAndReplaceWall(
                        doorPositionsConverted, 
                        a->findBp(roomAssetEnum::doorEnum)
                    );
                    //process windows too but after doors
                    aroom->processPositionVectorsAndReplaceWall(
                        windowPositionsConverted, 
                        a->findBp(roomAssetEnum::windowEnum)
                    );
                }
                


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

