#include "ChunkParserStorageInterface.h"
#include "GameCore/MeshGenBase/materialHelper/MaterialEnumHelper.h"




ChunkParserStorageInterface::ChunkParserStorageInterface(){

}

ChunkParserStorageInterface::~ChunkParserStorageInterface(){

}

void ChunkParserStorageInterface::Save(
    FString worldLevelname, 
    ChunkParser &chunkData
){
    //for all lods
        //for all materials
            //load with mesh data interface

    TArray<uint8> bytes;








}

bool ChunkParserStorageInterface::Load(
    FString worldLevelname, 
    ChunkParser &chunkData
){
    
    //for all lods
        //for all materials
            //load with mesh data interface

    return true;
}

int ChunkParserStorageInterface::materialToId(materialEnum type){
    return MaterialEnumHelper::indexFor(type);
}