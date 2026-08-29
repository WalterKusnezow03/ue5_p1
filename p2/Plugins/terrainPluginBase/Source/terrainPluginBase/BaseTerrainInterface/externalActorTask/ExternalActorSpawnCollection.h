#pragma once

#include "CoreMinimal.h"
#include "ExternalActorSpawnPositions.h"

/// - Will allow to collect positions from the terrain to spawn
/// external actors at: used for terrain creator!

/// - Will be stored as binary to disk

/// - needed to respawn actors on terrain load from disk!

struct FActorAssetByString;

class TERRAINPLUGINBASE_API ExternalActorSpawnCollection {

public:
    ExternalActorSpawnCollection();
    ~ExternalActorSpawnCollection();
    ExternalActorSpawnCollection(const ExternalActorSpawnCollection &other);
    ExternalActorSpawnCollection &operator=(const ExternalActorSpawnCollection &other);

    

    void Clear();

    void AddCollection(FString name);
    void AddCollection(FActorAssetByString *collectionFromAsset);
    void AddCollection(ExternalActorSpawnPositions &dataIn);
    const ExternalActorSpawnPositions &FindCollection(FString name);

    //storage interface
    void Save();
    bool Load(FString worldNameIn);
    void SetWorldName(FString worldNameIn);
    // stroage interface

    TArray<ExternalActorSpawnPositions> &GetCollection();
    const TArray<ExternalActorSpawnPositions> &GetCollectionConst() const;

    
    virtual void AppendAsBinary(TArray<uint8> &buffer);
    virtual bool LoadFromBinary(
        TArray<uint8> &buffer,
        uint8 *&Ptr // reference to a pointer. Pointer by reference.
    );

private:
    TArray<ExternalActorSpawnPositions> collection;
    ExternalActorSpawnPositions *HasCollection(FString name);

    ExternalActorSpawnPositions fallback;
    FString worldName;
    bool WorldNameValid();
};