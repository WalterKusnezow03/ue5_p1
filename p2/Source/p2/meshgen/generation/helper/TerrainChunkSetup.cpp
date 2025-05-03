

#include "TerrainChunkSetup.h"
#include "p2/entityManager/OutPost/Outpost.h"
#include "p2/_world/worldLevel.h"
#include "p2/entityManager/OutPost/OutpostManager.h"

TerrainChunkSetup::TerrainChunkSetup(
    std::vector<std::vector<FVector>> &refIn,
    ETerrainType typeIn,
    bool createOutpostIn,
    FVector &outpostLocationIn,
    bool blockTreesIn
){
    setMapReference(refIn);
    savedTerrainType = typeIn;
    createOutpost = createOutpostIn;
    blockTrees = blockTreesIn;
    outpostLocation = outpostLocationIn;
}

TerrainChunkSetup::TerrainChunkSetup(TerrainChunkSetup &other){
    if(this != &other){
        *this = other;
    }
}

TerrainChunkSetup &TerrainChunkSetup::operator=(TerrainChunkSetup &other){
    if(this != &other){
        map2D = other.map2D;
        savedTerrainType = other.savedTerrainType;
        createOutpost = other.createOutpost;
        blockTrees = other.blockTrees;
    }
    return *this;
}

TerrainChunkSetup::~TerrainChunkSetup(){
    map2D = nullptr;
}


float TerrainChunkSetup::treeDensitySkalar(){
    if(createTrees()){
        /*
        EForest,
        ETropical,
        EDesert,
        EDesertForest,
        EOcean,
        ESnowHill
        */
        if(savedTerrainType == ETerrainType::EForest) return 0.1f;
        if(savedTerrainType == ETerrainType::ETropical) return 0.1f;
        if(savedTerrainType == ETerrainType::EDesert) return 0.05f;
        if(savedTerrainType == ETerrainType::EDesertForest) return 0.2f;
        if(savedTerrainType == ETerrainType::EOcean) return 0.05f;
        if(savedTerrainType == ETerrainType::ESnowHill) return 0.05f;

    }
    return 0.0f;
}


std::vector<std::vector<FVector>> &TerrainChunkSetup::mapReference(){
    if(map2D != nullptr){
        return *map2D;
    }
    return none;
}

/// @brief reference must NOT be TEMPORARY, STORED AS POINTER! 
/// @param refIn
void TerrainChunkSetup::setMapReference(std::vector<std::vector<FVector>> &refIn){
    if(&refIn != nullptr){
        map2D = &refIn;
    }
}


bool TerrainChunkSetup::createTrees(){
    return !blockTrees;
}

ETerrainType TerrainChunkSetup::getTerrainType(){
    return savedTerrainType;
}



void TerrainChunkSetup::createOutPostIfFlagged(UWorld *world){
    if(world && createOutpost){

        if(OutpostManager *ptr = worldLevel::outpostManager()){
            AOutpost *outpost = ptr->requestOutpost(world, outpostLocation);

            if(outpost){
                outpost->createAlarmPolesIfNeeded();
            }
        }

    }
}