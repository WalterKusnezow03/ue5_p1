// Fill out your copyright notice in the Description page of Project Settings.
#include "terrainCreator.h"

#include <cmath>
#include "GameCore/util/FVectorUtil.h"
#include "Algo/Sort.h"  // Include the necessary header
#include "terrainPlugin/meshgen/generation/bezierCurve.h"
#include "GameCore/util/TVector.h"
#include "HAL/PlatformTime.h"
#include <algorithm>
#include <set>
#include "GameCore/MeshGenBase/foliage/ETerrainType.h"
#include "CoreMath/Matrix/MMatrix.h"

#include "GameCore/EntityGC/EntityManagerBase.h"
#include "GameCore/MeshGenBase/MeshData/BoundingBox/BoundingShapeXY.h"

#include "terrainPlugin/meshgen/rooms/roomActor/roomProcedural.h"
#include "terrainPlugin/meshgen/water/customWaterActor.h"
#include "terrainPlugin/meshgen/foliage/helper/FVectorShape.h"

#include "terrainConstants.h"



terrainCreator::terrainCreator()
{
}

terrainCreator::~terrainCreator()
{

}




/***
 * 
 * ---- TERRAIN METHODS -----
 * 
 */

/// @brief will always create a qudratic terrain
/// @param world world to spawn in / debug draw in
/// @param meters size in meters
void terrainCreator::createTerrain(UWorld *world, int meters){
    std::vector<terrainHillSetup> none;
    createTerrain(world, meters, none);
}

/// @brief will always create a qudratic terrain
/// @param world world to spawn in / debug draw in
/// @param meters size in meters
/// @param predefinedHillDataVecFlatArea chunks to keep a certain height forced, may vary because of
/// bezier smoothening.
void terrainCreator::createTerrain(
    UWorld *world,
    int meters,
    std::vector<terrainHillSetup> &predefinedHillDataVecFlatArea //flat area
){
    worldPointer = world;

    int chunks = floor(meters / terrainConstants::CHUNKSIZE); //to chunks
    //int detail = terrainConstants::CHUNKSIZE; // 1 by 1 detail

    //fill map
    map.reserve(chunks);
    for (int i = 0; i < chunks; i++){
        std::vector<chunk> vec;
        vec.reserve(chunks);
        for (int j = 0; j < chunks; j++){
            chunk c(i,j);
            vec.push_back(c);
        }
        map.push_back(vec);
    }


    //random height and smooth
    int layers = 12; //20 (12 waren auf 10x10)

    int layersPerTen = 12;
    layers = (chunks / 10.0f) * layersPerTen;

    createRandomHeightMapChunkWide(layers);
    smooth3dMap();


    flattenChunksForHillData(predefinedHillDataVecFlatArea); //override after smooth height, clamp upper limit
}











/// @brief scales the height for all chunks (designed to upscale before bezier and downscale later)
/// creates more detailed interpolation on Z axis (maybe)
/// @param scale sclae to set
void terrainCreator::scaleHeightForAll(float scale){
    for (int i = 0; i < map.size(); i++){
        for (int j = 0; j < map.at(i).size(); j++){
            map.at(i).at(j).scaleheightForAll(scale);
        }
    }
}





/**
 * ----- 3D SMOOTH MAP SECTION -----
 */

/// @brief will smooth out all chunks rows and columns and merge them together to the map
void terrainCreator::smooth3dMap(){
    FVector a(0, 0, 0);
    int max = map.size() * terrainConstants::ONEMETER * terrainConstants::CHUNKSIZE;
    FVector b(max, max, 0);

    int iterations = 3;
    smooth3dMap(a, b, iterations);
    //smooth3dMap(a, b, 1);
}

/// @brief will smooth out all chunks rows and columns and merge them together to the map
void terrainCreator::smooth3dMap(FVector &a, FVector &b, int iterations){

    //calculate enclosed bounds, works as expected
    int fromX = a.X < b.X ? a.X : b.X;
    int fromY = a.Y < b.Y ? a.Y : b.Y;
    int toX = a.X > b.X ? a.X : b.X;
    int toY = a.Y > b.Y ? a.Y : b.Y;
    fromX = clampIndex(cmToChunkIndex(fromX));
    fromY = clampIndex(cmToChunkIndex(fromY));
    toX = clampIndex(cmToChunkIndex(toX));
    toY = clampIndex(cmToChunkIndex(toY));


    // get all x and y axis and smooth them.
    bezierCurve curve;

    //define this vector out side loop for preventing unesecarry wiping from stack until complete
    //TVector data gets overriden anyway / internal garbage collector
    int scalePrediction = terrainConstants::CHUNKSIZE * map.size(); //one meter gaps obviously.
    TVector<FVector2D> output(scalePrediction); // use only one custom tvector for efficency

    for (int it = 0; it < iterations; it++){

        // all x columns
        int xcount = 0;
        //for (int i = 0; i < map.size(); i++)
        for (int i = fromX; i <= toX; i++)
        {   
            
            for (int innerX = 0; innerX < terrainConstants::CHUNKSIZE; innerX++)
            {
                std::vector<FVector2D> column;
                for (int j = fromY; j <= toY; j++){
                    column.push_back(map.at(i).at(j).getFirstXColumnAnchor(innerX));
                }
                output.clear();
                curve.calculatecurve(column, output, terrainConstants::ONEMETER);

                //trying writing immidately
                applyColumnOrRow(xcount, output, true);
                xcount++;
            }
        }
        



        //then all y rows
        int ycount = 0;
        for (int cY = fromY; cY <= toY; cY++){
            for (int innerY = 0; innerY < terrainConstants::CHUNKSIZE; innerY++)
            {
                std::vector<FVector2D> row;
                //über ganz x laufen und einsammeln
                for (int cX = fromX; cX <= toX; cX++){
                    row.push_back(map.at(cX).at(cY).getFirstYRowAnchor(innerY));
                }

                output.clear();
                curve.calculatecurve(row, output, terrainConstants::ONEMETER);
                
                
                applyColumnOrRow(ycount, output, false);

                ycount++;
            }
        }


    }

}






void terrainCreator::applyColumnOrRow(
    int index, 
    TVector<FVector2D> &data,
    bool isColumn
){
    float newHeight = 0;
    int i_InMeter = index;
    int i_InCm = i_InMeter * terrainConstants::ONEMETER;

    int i_InChunk = cmToChunkIndex(i_InCm); //x chunk index in map
    if(i_InChunk > map.size() - 1){
        return; //issue
    }

    int other_InChunk = 0;

    FVector current;
    FVector prev;

    FVector current_build_vectordebug;

    // convert y as cm to chunk
    for (int i = 0; i < data.size(); i++){

        FVector2D now = data.at(i);
        newHeight = now.Y;
        float other_InCm = std::floor(now.X); //test
        other_InChunk = cmToChunkIndex(other_InCm);
        
        if(verifyIndex(other_InChunk)){

            chunk *c = nullptr;
            int i_InnerIndex = 0;
            int other_InnerIndex = 0;
            if (isColumn){
                c = &map.at(i_InChunk).at(other_InChunk);
                i_InnerIndex = cmToInnerChunkIndex(i_InCm);
                other_InnerIndex = cmToInnerChunkIndex(other_InCm);

                current = FVector(i_InCm, other_InCm, newHeight);

                current_build_vectordebug = FVector(
                    i_InnerIndex * terrainConstants::ONEMETER + i_InChunk * terrainConstants::CHUNKSIZE * terrainConstants::ONEMETER,
                    other_InnerIndex * terrainConstants::ONEMETER + other_InChunk * terrainConstants::CHUNKSIZE * terrainConstants::ONEMETER,
                    newHeight
                );
            }
            else{
                c = &map.at(other_InChunk).at(i_InChunk);
                i_InnerIndex = cmToInnerChunkIndex(other_InCm);
                other_InnerIndex = cmToInnerChunkIndex(i_InCm);

                current = FVector(other_InCm, i_InCm, newHeight);

                //the same because the coords are flipped above 
                current_build_vectordebug = FVector(
                    i_InnerIndex * terrainConstants::ONEMETER + i_InChunk * terrainConstants::CHUNKSIZE * terrainConstants::ONEMETER,
                    other_InnerIndex * terrainConstants::ONEMETER + other_InChunk * terrainConstants::CHUNKSIZE * terrainConstants::ONEMETER,
                    newHeight
                );
            }

            


            if(c != nullptr){
                c->applyIndivualVertexIndexBased(
                    i_InnerIndex,
                    other_InnerIndex,
                    newHeight,
                    //true,
                    isColumn
                );
                bool draw = false;
                if(draw){
                    DebugHelper::showLineBetween(worldPointer, prev, current, FColor::Orange);
                    DebugHelper::showLineBetween(worldPointer, current, current + FVector(0,0,-100), FColor::Red);
                    
                    //HIER WIRD DER FEHLER SICHTBAR (wenn er auftritt, was er nicht mehr tut.)
                    DebugHelper::showLineBetween(
                        worldPointer,
                        current + FVector(-10, 0, -100),
                        current_build_vectordebug + FVector(-50, 0, -100),
                        FColor::Cyan
                    );

                    prev = current;
                }
            }
        
        }
    }




}











/**
 * 
 * --- EMBED ROOMS ---
 * 
 */

/// @brief will set the are to a certain height provided in the location vector, which should be 
/// the bottom left corner of the rooms created, will smooth the terrain data afterwards
/// @param location location and Z offset to set
/// @param sizeMetersX size in meters X
/// @param sizeMetersY size in meters Y
void terrainCreator::setFlatArea(FVector &location, int sizeMetersX, int sizeMetersY){
    //convert to meters ue5 world scale just as usual
    sizeMetersX *= 100;
    sizeMetersY *= 100;

    //get map bounds with little upscaling
    int fromX = clampIndex(cmToChunkIndex(location.X) - 1);
    int fromY = clampIndex(cmToChunkIndex(location.Y) - 1);
    int toX = clampIndex(fromX + cmToChunkIndex(sizeMetersX) + 1);
    int toY = clampIndex(fromY + cmToChunkIndex(sizeMetersY) + 1);

    int heightToSet = location.Z;

    //iterate over map and smooth
    for (int i = fromX; i <= toX; i++){
        for (int j = fromY; j <= toY; j++){
            chunk *c = &map.at(i).at(j);
            if(c != nullptr){
                c->setheightForAll(heightToSet);
                c->setTreesBlocked(true);
            }
        }
    }

    //finally also smooth the map
    FVector buffer = FVector(terrainConstants::CHUNKSIZE, terrainConstants::CHUNKSIZE, 0);
    FVector a = location - buffer;
    FVector b(
        sizeMetersX, 
        sizeMetersY,
        0
    );
    b += (a + buffer * 2);

    int iterations = 2;
    smooth3dMap(a, b, iterations); // disabled for debugging
}

/**
 * 
 * --- SCALE CONVERSION SECTION ---
 * 
 */


int terrainCreator::cmToMeter(int a){
    float af = a;
    float result = a / (terrainConstants::ONEMETER * 1.0f);
    // return a / terrainConstants::ONEMETER;
    return std::round(result); //works better!!
}
int terrainCreator::meterToInnerChunkIndex(int a){
    return a % terrainConstants::CHUNKSIZE;
}
/// @brief creates the inner chunk index from cm
/// @param a cm
/// @return index
int terrainCreator::cmToInnerChunkIndex(int a){
    a = cmToMeter(a);
    return meterToInnerChunkIndex(a);
}

/// @brief creates the outer chunk index inside the map for a cm value
/// @param a cm in
/// @return chunk index outer
int terrainCreator::cmToChunkIndex(int a){
    //int cmForOneChunk = terrainConstants::CHUNKSIZE * terrainConstants::ONEMETER;
    int aToMeter = cmToMeter(a);
    int aToChunk = aToMeter / terrainConstants::CHUNKSIZE;
    return aToChunk;
}

/// @brief checks if the index is within the map bounds
/// @param a index
/// @return true false map bounds kept
bool terrainCreator::verifyIndex(int a){
    return a >= 0 && a < map.size();
}


/// @brief validates the index on either x or y axis
/// @return clamped value valid to acces the 2D vector map
int terrainCreator::clampIndex(int a){
    if(a < 0){
        a = 0;
    }
    if(a >= map.size()){
        a = map.size() - 1;
    }
    return a;
}







/**
 * 
 * plotting
 *
 */


/// @brief debug plotting
/// @param world 
void terrainCreator::plotAllChunks(UWorld * world){
    if (world != nullptr && (terrainCreator::PLOTTING_ENABLED))
    {

        for (int i = 0; i < map.size(); i++){
            for (int j = 0; j < map.at(i).size(); j++){
                map.at(i).at(j).plot(world);
            }
        }
    }
}

/**
 * --- VIRTUAL RAYCAST ---
 * DEBUG NEEDED, ENTETIES YEET IN THE AIR
 */

/// @brief instead of raycasting the z height can be got from the generated mesh data
/// @param position position to find (only x y important)
/// @return return z for the x y position
float terrainCreator::getHeightFor(FVector &position){

    


    //create pane at world vertecies and perform hittest with FMath.
    TArray<FVector> positionIndices = {
        FVector(position.X, position.Y, 0.0f),
        FVector(position.X + terrainConstants::ONEMETER, position.Y, 0.0f),
        FVector(position.X, position.Y + terrainConstants::ONEMETER, 0.0f)
        //,FVector(position.X + ONEMETER, position.Y + ONEMETER, 0.0f)
    };

    //NEW Collect vertecies
    TArray<FVector> worldVertecies;
    TArray<chunk *> chunks = chunksAt(positionIndices);
    for (int i = 0; i < chunks.Num(); i++){
        FVector newPos;
        chunk *current = chunks[i];
        if(current && i < positionIndices.Num()){
            if(current->NextWorldVertexAt(positionIndices[i], newPos)){
                worldVertecies.Add(newPos); //closest vertex added
            }
        }
    }

    /*
    FPlane(va,vb,vc)

    template<typename FReal>  
    static UE::Math::TVector < FReal > LinePlaneIntersection  
    (  
        const UE::Math::TVector < FReal > & Point1,  
        const UE::Math::TVector < FReal > & Point2,  
        const UE::Math::TPlane < FReal > & Plane  
    )
    */

    //is tested works very well
    if(worldVertecies.Num() == 3){
        FVector start = position + FVector(0, 0, 10000);
        FVector end = position - FVector(0, 0, 10000);
        FPlane plane(
            worldVertecies[0],
            worldVertecies[1],
            worldVertecies[2]
        );
        //DebugHelper::showScreenMessage("plane test", FColor::Orange);
        FVector hit;
        if (FMath::SegmentPlaneIntersection(start, end, plane, hit))
        {
            //DebugHelper::showScreenMessage("plane test hit", FColor::Orange);
            //DebugHelper::showLineBetween(worldPointer, hit, hit + FVector(0, 0, 100), FColor::Red, 0.5f);
            return hit.Z;
        }
    }
    return 0.0f;

}



/// @brief returns the count of the inner chunks in total
/// @return count
int terrainCreator::chunkNum(){
    //is used from the entity manager to pass the mesh actors
    if(map.size() > 0){
        return map.size() * map.at(0).size();
    }
    return 0;
}



/**
 * ---- APPLY DATA TO MESH ACTORS ----
 */


/// @brief will create all meshes and populate the data
/// will not hold on to the reference when goes out of scope, only applies the data
/// where it can and doesnt go out of bounds
void terrainCreator::applyTerrainDataToMeshActors(){

    int x = 0;
    int y = 0;
    //inclusive index limit
    int xLimit = map.size();
    int yLimit = map.size();

    applyTerrainDataToMeshActors(x, xLimit, y, yLimit);

}

void terrainCreator::applyTerrainDataToMeshActors(
    int lowerX,
    int xLimit,
    int lowerY,
    int yLimit
){
    int x = lowerX;
    int y = lowerY;
    int actorIndex = 0;
    int limit = (yLimit - lowerY) * (xLimit - lowerX);
    while (actorIndex < limit)
    {
        actorIndex++;
        createChunkAtIfNotCreatedYet(x, y); //verifies the index automatically!
        x++;
        //top corner reached, return
        if(y >= yLimit && x >= xLimit){
            return;
        }
        //next row
        if(x >= xLimit){
            x = 0;
            y++;
        }
    }
}




void terrainCreator::createChunkAtIfNotCreatedYet(int x, int y){

    chunk *currentChunk = chunkAt(x,y);
    if(
        (currentChunk != nullptr) && 
        (currentChunk->wasAlreadyCreated() == false)
    ){
        currentChunk->setWasCreatedTrue();

        int xLimit = map.size();
        int yLimit = map.size();

        AcustomMeshActor *currentActor = getNewMeshActor();
        /*
        deprecated! 
        EntityManager *entityManagerPointer = worldLevel::entityManager();
        if(entityManagerPointer != nullptr){
            FVector location(0, 0, 0);
            currentActor = entityManagerPointer->spawnAcustomMeshActor(worldPointer, location);
        }*/
        if(currentActor == nullptr){
            return;
        }

        

        // apply position
        FVector newPos = currentChunk->positionPivotBottomLeft();
        currentActor->SetActorLocation(newPos);

        //apply data
        //readAndMerge (connect to next in map)
        chunk *top = nullptr;
        chunk *right = nullptr;
        chunk *topright = nullptr;

        
        if(y + 1 < yLimit){
            top = &map.at(x).at(y+1);
        }
        if(x + 1 < xLimit){
            right = &map.at(x+1).at(y);
        }
        if(x + 1 < xLimit && y + 1 < yLimit){
            topright = &map.at(x+1).at(y+1);
        }

        TerrainChunkSetup package = currentChunk->makeSetupPackage(top, right, topright);
        ETerrainType terrainType = package.getTerrainType();
        currentActor->createTerrainFrom2DMap(package);



        if(terrainType == ETerrainType::EOcean){
            newPos.Z = terrainConstants::HEIGHT_MAX_OCEAN * 0.8f;
            createWaterPaneAt(newPos);
        }

        DebugHelper::showScreenMessage("CREATED NEW CHUNK", FColor::Purple);
    }
}


void terrainCreator::createWaterPaneAt(FVector &location){
    if(worldPointer != nullptr){
        int scaleCm = terrainConstants::CHUNKSIZE * terrainConstants::ONEMETER;

        AcustomWaterActor::createWaterPane(
            worldPointer,
            location,
            scaleCm
        );
    }
}














/**
 * 
 * 
 * --- create height maps ---
 * 
 * 
 */


/// @brief will create a random height map chunk wide, then to be smoothed
void terrainCreator::createRandomHeightMapChunkWide(int layers){

    layers = std::abs(layers);
    for (int i = 0; i < layers; i++)
    {
        terrainHillSetup newHill = createRandomHillData();
        applyHillData(newHill);
    }

    if(terrainConstants::MINCHUNK_LARGEHILL < map.size()){
        int layersLarge = layers / 4.0f;
        for (int i = 0; i < layersLarge; i++){
            terrainHillSetup newHillLarge = createRandomHillDataLargeScale();
            applyHillData(newHillLarge);
        }
    }
    
    
}


terrainHillSetup terrainCreator::createRandomHillData(){
    int scaleX = FVectorUtil::randomNumber(terrainConstants::MINCHUNK_HILL, map.size()); //random hardcoded for now.
    int scaleY = FVectorUtil::randomNumber(terrainConstants::MINCHUNK_HILL, map.size());
    int heightMin = terrainConstants::ONEMETER / 2.0f;
    return createRandomHillData(scaleX, scaleY,heightMin);
}

terrainHillSetup terrainCreator::createRandomHillDataLargeScale(){
    
    int scaleX = FVectorUtil::randomNumber(terrainConstants::MINCHUNK_LARGEHILL, map.size()); //random hardcoded for now.
    int scaleY = FVectorUtil::randomNumber(terrainConstants::MINCHUNK_LARGEHILL, map.size());
    int heightMin = terrainConstants::ONEMETER * 2.0f;
    return createRandomHillData(scaleX, scaleY, heightMin);
}

terrainHillSetup terrainCreator::createRandomHillData(
    int sizeX, 
    int sizeY,
    int heightMin
){
    sizeX = std::abs(sizeX);
    sizeY = std::abs(sizeY);
    if(sizeX <= 0){
        sizeX = 1;
    }
    if(sizeY <= 0){
        sizeY = 1;
    }


    int startX = clampIndex(FVectorUtil::randomNumber(1, map.size() - sizeX));
    int startY = clampIndex(FVectorUtil::randomNumber(1, map.size() - sizeY));
    int heightMax = heightMin * 3; //2

    return terrainHillSetup(
        startX,
        startY,
        sizeX,
        sizeY,
        heightMin,
        heightMax
    );
}

void terrainCreator::applyHillData(std::vector<terrainHillSetup> &hillDataVec){
    for (int i = 0; i < hillDataVec.size(); i++){
        applyHillData(hillDataVec[i]);
    }
}


/// @brief will enheight the map based on the passed hilldata in size X, size Y and height add
/// @param hillData 
void terrainCreator::applyHillData(terrainHillSetup &hillData){
    for (int i = clampIndex(hillData.xPosCopy()); i < clampIndex(hillData.xTargetCopy()); i++){
        for (int j = clampIndex(hillData.yPosCopy()); j < clampIndex(hillData.yTargetCopy()); j++){
            if(verifyIndex(i) && verifyIndex(j)){
                map.at(i).at(j).addheightForAll(hillData.getHeightIfSetOrRandomHeight());
            }
        }
    }
}




void terrainCreator::flattenChunksForHillData(std::vector<terrainHillSetup> &hillDataVec){
    for (int i = 0; i < hillDataVec.size(); i++){
        flattenChunksForHillData(hillDataVec[i]);
    }
}

///@brief clamps an area to a max height defined by the passed hilldata object
void terrainCreator::flattenChunksForHillData(terrainHillSetup &hillData){
    for (int i = clampIndex(hillData.xPosCopy()); i < clampIndex(hillData.xTargetCopy()); i++){
        for (int j = clampIndex(hillData.yPosCopy()); j < clampIndex(hillData.yTargetCopy()); j++){
            if(verifyIndex(i) && verifyIndex(j)){
                //map.at(i).at(j).clampheightForAllUpperLimit(hillData.getForcedSetHeight());
                //map.at(i).at(j).clampheightForAllUpperLimitByOwnAverageHeight();
                map.at(i).at(j).setheightForAllToAverage();

                //disable trees for rooms
                map.at(i).at(j).setTreesBlocked(true);
            }
        }
    }
}







/// @brief randomizes terrain types by enclosing bezier curves
/// @param world 
void terrainCreator::randomizeTerrainTypes(UWorld *world){
    
    int sizeOfShape = 10; //Chunks
    int step = 1;
    FVectorShape shape;

    int shapeCount = map.size();

    std::vector<ETerrainType> terraintypesVector = createRandomTerrainTypes(shapeCount);

    for (int i = 0; i < shapeCount; i++){

        shape.createRandomNewSmoothedShapeClamped(sizeOfShape, step);
        shape.floorAllCoordinateValues(); //macht es quasi eckig

        //DEBUG
        
        if(false){
            std::vector<FVector> vertecies = shape.vectorCopy();
            MMatrix scaleMat;
            scaleMat.scale(100, 100, 1);
            MMatrix rot;
            rot.pitchRadAdd(MMatrix::degToRadian(90.0f));
            MMatrix translate;
            translate.setTranslation(0, 0, 300);

            MMatrix result = rot * scaleMat;
            result = translate * result;

            for (int j = 0; j < vertecies.size(); j++){
                vertecies[j] = result * vertecies[j];
            }
            DebugHelper::showLine(world, vertecies, FColor::Blue);
        }
        
        //DEBUG END



        //random offset into map
        MMatrix moveMatrix;
        moveMatrix.setTranslation(
            FVectorUtil::randomNumber(0, map.size() - sizeOfShape),
            FVectorUtil::randomNumber(0, map.size() - sizeOfShape),
            0
        );
        if(false)
            shape.moveVerteciesWith(moveMatrix); //debug remove

        

        shape.sortVerteciesOnXAxis();
        std::vector<FVector> vertecies = shape.vectorCopy();
        
        if(vertecies.size() > 0){
            
            FVector &chunkAt = vertecies[0];
            int x = clampIndex(chunkAt.X);
            int y = clampIndex(chunkAt.Y);

            for (int vertex = 1; vertex < vertecies.size(); vertex++){
                FVector &prevVertex = vertecies[vertex - 1];
                FVector &currentVertex = vertecies[vertex];

                applyTerrainTypeBetween(prevVertex, currentVertex, terraintypesVector[i]);

                //DebugHelper::logMessage("terrainDebug apply terrain ");
            }
        }

    }
}

///@brief applies a terrain type betweem two coordinates of chunks, where X component
/// is SAME! 
void terrainCreator::applyTerrainTypeBetween(FVector &a, FVector &b, ETerrainType typeIn){
    if(a.X == b.X){
        int xIndex = clampIndex(a.X);

        FVector &smaller = a.Y < b.Y ? a : b;
        FVector &bigger = a.Y > b.Y ? a : b;

        for (int i = smaller.Y; i <= bigger.Y; i++){
            int yIndex = clampIndex(i);

            chunk *currentChunk = chunkAt(xIndex, yIndex);
            if(currentChunk != nullptr){
                currentChunk->updateTerraintype(typeIn);
            }
        }
    }
}

///@brief creates a random terrain vector in passed count
///@param count of terrain types
std::vector<ETerrainType> terrainCreator::createRandomTerrainTypes(int count){
    std::vector<ETerrainType> outterrain;
    ETerrainType prev = ETerrainType::ETropical;
    for (int i = 0; i < count; i++){
        outterrain.push_back(selectTerrainTypeExcluding(prev));
        prev = outterrain.back();
    }
    return outterrain;
}

///@brief selects a terrain type excluding a target terrain
ETerrainType terrainCreator::selectTerrainTypeExcluding(ETerrainType typeToExclude){
    std::vector<ETerrainType> vector = //AcustomMeshActorBase::terrainVector();
    {
        ETerrainType::EDesert,
        ETerrainType::EDesertForest,
        ETerrainType::ETropical
        //,ETerrainType::ESnowHill
    };

    ETerrainType terraintypeRandom = ETerrainType::ETropical;
    int randomIndex = FVectorUtil::randomNumber(0, vector.size());
    randomIndex %= vector.size();
    terraintypeRandom = vector[randomIndex];
    if(terraintypeRandom == typeToExclude){
        randomIndex = (randomIndex + 1) % vector.size();
        terraintypeRandom = vector[randomIndex];
    }

    return terraintypeRandom;
}

/// @brief returns a chunk by pointer or nullptr if the index was invalid
/// @param x 
/// @param y 
/// @return 
chunk *terrainCreator::chunkAt(int x, int y){
    if(verifyIndex(x) && verifyIndex(y)){
        return &map[x][y];
    }
    return nullptr;
}

TArray<chunk *> terrainCreator::chunksAt(
    TArray<FVector> &positionsWorld
){
    TArray<chunk *> outputArray;
    for (int i = 0; i < positionsWorld.Num(); i++)
    {
        FVector &current = positionsWorld[i];
        chunk *ptr = chunkAt(
            cmToChunkIndex(current.X),
            cmToChunkIndex(current.Y)
        );
        if(ptr != nullptr){
            outputArray.Add(ptr);
        }
    }
    return outputArray;
}

/// @brief applies the ESnowhill terrain type to chunks matching the minheight requirement
void terrainCreator::applySpecialTerrainTypesByHeight(){
    for (int i = 0; i < map.size(); i++){
        for (int j = 0; j < map[i].size(); j++){
            chunk *currentChunkPointer = chunkAt(i, j);
            if(currentChunkPointer != nullptr){
                currentChunkPointer->updateTerrainTypeBySpecialHeights();
            }
        }
    }
}


/**
 * 
 * 
 * ---- player tick creation ----
 * 
 * 
 */

///@brief will create surrounding chunks if not created yet
void terrainCreator::Tick(FVector &playerLocation){
    //player to chunkindex
    int x = cmToChunkIndex(playerLocation.X);
    int y = cmToChunkIndex(playerLocation.Y);

    FVector location(x, y, 0);
    //DebugHelper::showScreenMessage("terrain tick chunk", location, FColor::Green);

    //25 mesh actors

    int half = terrainConstants::CHUNKSTOCREATEATONCE / 2;
    half = 2;

    //debug
    half = map.size();

    applyTerrainDataToMeshActors(
        x - terrainConstants::CHUNKSTOCREATEATONCE,
        x + terrainConstants::CHUNKSTOCREATEATONCE,
        y - terrainConstants::CHUNKSTOCREATEATONCE,
        y + terrainConstants::CHUNKSTOCREATEATONCE
    );
}

/**
 * 
 * --- Example method for creating the terrain ---
 * 
 */
void terrainCreator::debugCreateTerrain(UWorld *world){
    //createTerrainAndSpawnMeshActors(world, 200);

    //new with player tick needed
    //createTerrainAndCreateBuildings(world, 200);
    debugCreateTerrain(world, 200);
}

void terrainCreator::debugCreateTerrain(UWorld *world, int meters){
    //createTerrainAndSpawnMeshActors(world, 200); //old

    meters = std::abs(meters);
    meters = std::max(meters, terrainConstants::CHUNKSIZE * 2);
    createTerrainAndCreateBuildings(world, meters); //new
}

/// @brief creates a terrain and brand new mesh actors without using the entity manager
/// @param world world to spawn in, must not be nullptr
/// @param meters meters xy of terrain
void terrainCreator::createTerrainAndSpawnMeshActors(
    UWorld *world, int meters
){
    meters = std::abs(meters);
    if(meters < 100){
        meters = 100;
    }

    if(world != nullptr){
        FVector location(0, 0, 0);
        createTerrain(world, meters);

        int numberCreated = chunkNum();

        randomizeTerrainTypes(world);
        applySpecialTerrainTypesByHeight();
        applyTerrainDataToMeshActors();
    }
}



/**
 * create outposts section / buildings
 */

///@brief creates the buildings and the terrain, but only will spawn terrain if the player is
///near enough on tick
void terrainCreator::createTerrainAndCreateBuildings(
    UWorld *world, int meters
){
    int chunkRange = meters / terrainConstants::CHUNKSIZE;

    DebugHelper::logMessage("debugterrain METERS ", meters);
    DebugHelper::logMessage("debugterrain CHUNKS ", chunkRange);

    int count = 3;
    int minsizeChunks = 1;
    int maxsizeChunks = 3;

    std::vector<terrainHillSetup> predefinedHillDataVecFlatArea;
    createFlatAreas(count, minsizeChunks, maxsizeChunks, chunkRange, predefinedHillDataVecFlatArea);
    createTerrain(world, meters, predefinedHillDataVecFlatArea);

    DebugHelper::logMessage("debugterrain hill data size ", predefinedHillDataVecFlatArea.size());

    randomizeTerrainTypes(world);
    applySpecialTerrainTypesByHeight();
    
    //recursion issue ? 
    //use this data to create the buildings
    //predefinedHillDataVecFlatArea
    //based also on terrain type!
    std::vector<chunk *> chunksFound;
    findChunksEnclosedBy(
        predefinedHillDataVecFlatArea,
        chunksFound
    );
    DebugHelper::logMessage("debugterrain chunks found", chunksFound.size());
    
    for (int i = 0; i < chunksFound.size(); i++){
        chunk *currentPointer = chunksFound[i];
        if(currentPointer != nullptr){
            FVector posPivot = currentPointer->positionPivotBottomLeft();
            posPivot.Z = currentPointer->maxHeight();

            //create building there.
            int sizeMaxMeters = terrainConstants::CHUNKSIZE;
            sizeMaxMeters -= 3;
            AroomProcedural::generate(world, sizeMaxMeters, sizeMaxMeters, posPivot); //in size is METERS

        }
    }

    //spawn all - managed via tick(?).
    //applyTerrainDataToMeshActors();

    markCreateOutpostsAt(predefinedHillDataVecFlatArea);
    createRoads(world);
}

/// @brief creates a output vector of terrainHillsetup in chunk index boundign boxes
/// @param count 
/// @param minsizeChunks 
/// @param maxsizeChunks 
/// @param chunkRange 
/// @param output 
void terrainCreator::createFlatAreas(
    int count, 
    int minsizeChunks, 
    int maxsizeChunks,
    int chunkRange,
    std::vector<terrainHillSetup> &output
){
    for (int i = 0; i < count; i++){
        createFlatArea(minsizeChunks, maxsizeChunks, chunkRange, output);
    }
}


/// @brief creates a terrainHillsetup in chunk index boundign box space
/// @param minsizeChunks 
/// @param maxsizeChunks 
/// @param chunkRange 
/// @param output 
void terrainCreator::createFlatArea(
    int minsizeChunks, 
    int maxsizeChunks,
    int chunkRange,
    std::vector<terrainHillSetup> &output
){
    int scaleX = 0;
    int scaleY = 0;
    int startX = 0;
    int startY = 0;
    int i = 1000;
    while(i > 0){

        scaleX = std::abs(FVectorUtil::randomNumber(minsizeChunks, maxsizeChunks));
        scaleY = std::abs(FVectorUtil::randomNumber(minsizeChunks, maxsizeChunks));

        int min = 3;
        startX = std::abs(FVectorUtil::randomNumber(min, chunkRange - scaleX));
        startY = std::abs(FVectorUtil::randomNumber(min, chunkRange - scaleY));

        bool overlapFound = false;
        for (int t = 0; t < output.size(); t++)
        {
            if(output[t].doesOverlapArea(startX, startY, scaleX, scaleY)){
                overlapFound = true;
                t = output.size();
            }
        }
        if(!overlapFound){
            break;
        }

        i--;
    }

    int heightMin = 0;
    int heightMax = 0;

    terrainHillSetup newHillSetup(
        startX,
        startY,
        scaleX,
        scaleY,
        heightMin,
        heightMax
    );
    newHillSetup.forceSetHeight(100); //debug

    //terrainHillSetup newHillSetup = createRandomHillData(scaleX, scaleY);

    

    output.push_back(newHillSetup);
}





///@brief helps to find all chunks enclosed by the hill setup passed
//use only temporary, pointers might get invalid if you change the map!
void terrainCreator::findChunksEnclosedBy(
    std::vector<terrainHillSetup> &hills,
    std::vector<chunk *> &output
){
    std::set<chunk *> outset;
    for (int i = 0; i < hills.size(); i++){
        findChunksEnclosedBy(hills[i], outset);
    }

    for(auto &current : outset){
        output.push_back(current);
    }
}


///@brief helps to find all chunks enclosed by the hill setup passed
//use only temporary, pointers might get invalid if you change the map!
void terrainCreator::findChunksEnclosedBy(
    terrainHillSetup &hillData,
    std::set<chunk *> &output
){
    for (int i = clampIndex(hillData.xPosCopy()); i < clampIndex(hillData.xTargetCopy()); i++){
        for (int j = clampIndex(hillData.yPosCopy()); j < clampIndex(hillData.yTargetCopy()); j++){
            if(verifyIndex(i) && verifyIndex(j)){

                //if not contained yet, add
                chunk *ptr = chunkAt(i,j);
                if(ptr != nullptr){
                    output.insert(ptr);
                }
                
            }
        }
    }
}




void terrainCreator::markCreateOutpostsAt(
    std::vector<terrainHillSetup> &predefinedHillDataVecFlatArea
){
    if(worldPointer != nullptr){
        float scaleUpFactor = terrainConstants::CHUNKSIZE * terrainConstants::ONEMETER;

        std::vector<FVector> worldCoordinates;
        for (int i = 0; i < predefinedHillDataVecFlatArea.size(); i++){
            terrainHillSetup &current = predefinedHillDataVecFlatArea[i];

            FVector center = current.center();
            int x = center.X;
            int y = center.Y;

            if(verifyIndex(x) && verifyIndex(y)){

                //if not contained yet, add
                chunk *ptr = chunkAt(x,y);
                if(ptr != nullptr){
                    ptr->markCreateOutpostTrue();
                }
            }
        }
    }
}




AcustomMeshActor *terrainCreator::getNewMeshActor(){
    AcustomMeshActor *meshActor = nullptr;
    EntityManagerBase *entityManagerPointer = EntityManagerBase::instanceBase();
    if(entityManagerPointer){
        meshActor = entityManagerPointer->requestByEnum<AcustomMeshActor>(
            ETrackedActors::EMeshActor,
            worldPointer
        );
    }
    return meshActor;
}

float terrainCreator::getHeightFor(FVector2D &pos){
    FVector pos3d(pos.X, pos.Y, 0.0f);
    return getHeightFor(pos3d);
    /*
    int x = cmToChunkIndex(pos.X);
    int y = cmToChunkIndex(pos.Y);
    chunk *ptr = chunkAt(x,y);
    if(ptr != nullptr){
        FVector pos3d(pos.X, pos.Y, 0.0f);
        int height = ptr->getHeightFor(pos3d);
        return height;
    }
    return 0.0f;*/
}




void terrainCreator::createRoads(UWorld* world){
    //roadmaker.createRoads(this, world, map.size());
}