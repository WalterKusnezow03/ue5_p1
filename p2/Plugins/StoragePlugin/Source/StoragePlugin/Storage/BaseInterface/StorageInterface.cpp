#include "StorageInterface.h"
#include "Misc/FileHelper.h"
#include "StoragePlugin/Storage/VertexData/TerrainVertex.h"
#include "StoragePlugin/Storage/VertexData/TerrainNormal.h"
#include "StoragePlugin/Storage/VertexData/TerrainUv.h"
#include "StoragePlugin/Storage/VertexData/TerrainVertexIndex.h"
#include "GameCore/DebugHelper.h"


//-------- SAVING --------

bool StorageInterface::SaveBinaryData(
    FString Path,
    TArray<uint8> &Bytes
){
    // Load
    return FFileHelper::SaveArrayToFile(Bytes, *Path);
}


//-------- LOADING --------



//load binary
bool StorageInterface::LoadBinaryData(
    FString Path,
    TArray<uint8> &Bytes
){
    // Load
    return FFileHelper::LoadFileToArray(Bytes, *Path);
}



// ------ PATH -------

/// @brief creates a path as "/ProjectDir/worldLevelName/", "/" at end, append inner path.
/// @param worldLevelName 
/// @return 
FString StorageInterface::BaseDir(FString worldLevelName){
    FString preString = BaseDir(); //ends with a single "/"
    preString += FString::Printf(TEXT("%s/"), *worldLevelName); //also ends with a single "/"
    return preString;
}


FString StorageInterface::BaseDir(){
    return FPaths::ProjectSavedDir();
}


// ------ HELPERS -------

void StorageInterface::PrintBinary(TArray<uint8>&bytes, FString message){
    FString byteString = FString::Printf(TEXT("Storage Interface %s bin: "), *message);
    for (int i = 0; i < bytes.Num(); i++){
        byteString += FString::FromInt((int32)bytes[i]);
    }

    DebugHelper::logMessage(byteString);
}