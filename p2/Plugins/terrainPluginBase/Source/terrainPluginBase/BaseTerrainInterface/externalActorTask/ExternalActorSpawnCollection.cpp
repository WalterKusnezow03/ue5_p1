#include "ExternalActorSpawnCollection.h"
#include "StoragePlugin/Storage/Template/TemplateBufferStorageInterface.h"
#include "terrainPluginBase/BaseTerrainInterface/externalActorTask/storageInterface/ExternalActorSpawnCollectionStorageInterface.h"
#include "GameCore/DataAssetCollection/ActorAssetByString.h"

ExternalActorSpawnCollection::ExternalActorSpawnCollection(){

}

ExternalActorSpawnCollection::~ExternalActorSpawnCollection(){

}

ExternalActorSpawnCollection::ExternalActorSpawnCollection(const ExternalActorSpawnCollection &other){
    if(this != &other){
        *this = other;
    }
}

ExternalActorSpawnCollection &ExternalActorSpawnCollection::operator=(const ExternalActorSpawnCollection &other){
    if(this != &other){
        collection = other.collection;
        worldName = other.worldName;
    }
    return *this;
}

void ExternalActorSpawnCollection::AddCollection(FString name){
    if(ExternalActorSpawnPositions::IsValidName(name)){
        collection.SetNum(collection.Num() + 1);
        ExternalActorSpawnPositions &last = collection.Last();
        last.SetName(name);
    }
}

void ExternalActorSpawnCollection::AddCollection(FActorAssetByString *dataFromAsset){
    if(dataFromAsset){
        ExternalActorSpawnPositions created(
            dataFromAsset->nameOfAsset, 
            dataFromAsset->spawnOnePerCmQuad
        );
        AddCollection(created);
    }
}

void ExternalActorSpawnCollection::AddCollection(ExternalActorSpawnPositions &dataIn){
    if(dataIn.IsValid()){
        collection.Add(dataIn);
    }
}

const ExternalActorSpawnPositions &ExternalActorSpawnCollection::FindCollection(FString name){
    ExternalActorSpawnPositions *ptr = HasCollection(name);
    if(ptr != nullptr){
        return *ptr;
    }
    return fallback;
}

ExternalActorSpawnPositions* ExternalActorSpawnCollection::HasCollection(FString name){
    for (int i = 0; i < collection.Num(); i++){
        ExternalActorSpawnPositions &current = collection[i];
        if(current.NameEqualsIgnoreCase(name)){
            return &current;
        }
    }
    return nullptr;
}


void ExternalActorSpawnCollection::AppendAsBinary(TArray<uint8> &buffer){
    TemplateBufferStorageInterface::AppendCount(collection.Num(), buffer);
    for (int i = 0; i < collection.Num(); i++){
        ExternalActorSpawnPositions &current = collection[i];
        current.AppendAsBinary(buffer);
    }
}

bool ExternalActorSpawnCollection::LoadFromBinary(
    TArray<uint8> &buffer,
    uint8 *&Ptr // reference to a pointer. Pointer by reference.
){
    if(TemplateBufferStorageInterface::EndReached(Ptr, buffer)){
        return false;
    }
    int32 countLoaded = 0;
    TemplateBufferStorageInterface::LoadCount(countLoaded, Ptr);

    for (int i = 0; i < countLoaded; i++){
        if(TemplateBufferStorageInterface::EndReached(Ptr, buffer)){
            return false;
        }
        ExternalActorSpawnPositions created;
        if(created.LoadFromBinary(buffer, Ptr)){
            AddCollection(created);
        }/*else{
            return false;
        }*/
    }

    return true;
}


bool ExternalActorSpawnCollection::WorldNameValid(){
    return worldName.Len() > 0;
}

void ExternalActorSpawnCollection::Save(){
    if(WorldNameValid()){
        ExternalActorSpawnCollectionStorageInterface interface;
        interface.Save(worldName, *this);
    }
}

bool ExternalActorSpawnCollection::Load(FString worldNameIn){
    SetWorldName(worldNameIn);
    if(WorldNameValid()){
        ExternalActorSpawnCollectionStorageInterface interface;
        return interface.Load(worldName, *this);
    }
    return false;
}

void ExternalActorSpawnCollection::SetWorldName(FString worldNameIn){
    worldName = worldNameIn;
}

TArray<ExternalActorSpawnPositions> &ExternalActorSpawnCollection::GetCollection(){
    return collection;
}

const TArray<ExternalActorSpawnPositions> &ExternalActorSpawnCollection::GetCollectionConst() const {
    return collection;
}

void ExternalActorSpawnCollection::Clear(){
    SetWorldName("");
    collection.Empty();
}