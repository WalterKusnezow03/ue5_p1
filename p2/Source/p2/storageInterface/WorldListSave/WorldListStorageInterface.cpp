#include "WorldListStorageInterface.h"
#include "Containers/Set.h"


WorldListStorageInterface::WorldListStorageInterface(){

}

WorldListStorageInterface::~WorldListStorageInterface(){

}
void WorldListStorageInterface::Save(TArray<FString> &worldNames){
    TArray<FString> filtered = makeUniqueNonEmpty(worldNames);
    StorageInterfaceStringArray::Save(makePath(), filtered);
}

bool WorldListStorageInterface::Load(TArray<FString> &worldNamesOut){
    return StorageInterfaceStringArray::Load(makePath(), worldNamesOut);
}

FString WorldListStorageInterface::makePath(){
    FString innerPath = TEXT("worldList/worldList.bin");
    FString path = BaseDir() + innerPath;
    return path;
}