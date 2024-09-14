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

        int xScaleRaw = 0;
        int yScaleRaw = 0;
        int zScaleRaw = 0;

        AActor *TempActor = nullptr;
        if(EntityManager *e = EntityManager::instance()){
            FVector l(0, 0, 0);
            TempActor = e->spawnAactor(world, uclass, l);
        }
        if(TempActor == nullptr){
            return;
        }

        AActorUtil::calculateActorBounds(TempActor, xScaleRaw, yScaleRaw, zScaleRaw);

        int xScale = (int) xScaleRaw / ONE_METER;
        int yScale = (int) yScaleRaw / ONE_METER;
        int zScale = (int) zScaleRaw/ ONE_METER;

        // single tile is saved seperatly
        if (xScale == 1 && yScale == 1)
        {
            singleTileBp = uclass;
            TempActor->Destroy(); //löschen nicht vergessen
            TempActor = nullptr;
            return;
        }

        //DEFAULT ADDING

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


        //finally destroy
        TempActor->Destroy();
        TempActor = nullptr;
    }
}





/// @brief creates a layout based on a size inout of indices of meters
/// @param world world to spawn in
/// @param x in meters
/// @param y in meters
void RoomManager::createALayout(UWorld* world, FVector &location, int x, int y){
    

    //default method part
    int height = 200; // 200; //---> height must be get from created room!!!
    

    int staircasesPerLayer = 1;
    std::vector<roomBounds> copyStairs;
    std::vector<TTouple<int, int>> reverseBlock;
    int layers = 2;

    for (int i = 0; i < layers; i++)
    {
        layoutCreator l(this); // all the data will be destroyed when it goes out of scope, remember.
        if(copyStairs.size() > 0){
            //if last layer: update bp for staircase topper
            if(i == layers -1){
                //read all stair sizes and get the proper stair topper
                for (int s = 0; s < copyStairs.size(); s++)
                {
                    //the staircase topper mus have the same bounds as the staircase it self on xy pane
                    UClass *stairtopper = getBpFor(
                        copyStairs.at(s).xscale(),
                        copyStairs.at(s).yscale(),
                        roomtypeEnum::staircaseTopper
                    );
                    if(stairtopper != nullptr){
                        copyStairs.at(s).updateBp(stairtopper);
                    }
                }
            }

            bool leaveGap = false;
            l.createRooms(x, y, copyStairs, leaveGap, reverseBlock);
            reverseBlock = l.getInverseBlockList();
        }else{
            //default layer
            l.createRooms(x, y, staircasesPerLayer);
            reverseBlock = l.getInverseBlockList();

        }

        std::vector<roomBounds> copy = l.copyData();
        copyStairs = l.copyStaircaseData();
        FVector offset(0, 0, height * i);
        offset += location; //apply location properly

        
        // process layer and add up new height from the just created layer
        processLayer(world, copy, offset, true);

    }
}




/// @brief will process a vector of rooms and apply an offset to them
/// @param world world to spawn in
/// @param vec vector of rooms
/// @param offset offset to apply as total for the layout from origin (0,0,0)
/// @param createWalls spawn nesecarry walls, doors and windows or not
void RoomManager::processLayer(
    UWorld* world, 
    std::vector<roomBounds> &vec, 
    FVector offset,
    bool createWalls
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

            //create walls, doors and windows. Or not.
            Aroom *aroom = Cast<Aroom>(actor);
            if(aroom != nullptr && createWalls){

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

                //lastly spawn the roof
                aroom->spawnRoof();
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



//gets the proper blueprint from size data and type
UClass *RoomManager::getBpFor(int xSizeIn, int ySizeIn, roomtypeEnum type){
    if (vectorMap.find(type) != vectorMap.end())
    {
        std::vector<RoomManager::sizeData> &ref = vectorMap[type];
        if(ref.size() > 0){
            for (int i = 0; i < ref.size(); i++){
                RoomManager::sizeData *pointer = &ref.at(i);
                int xcopy = pointer->xSize();
                int ycopy = pointer->ySize();
                if(xcopy == xSizeIn && ycopy == ySizeIn){
                    return pointer->getBp();
                }
            }
        }
    }
    return nullptr;
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

