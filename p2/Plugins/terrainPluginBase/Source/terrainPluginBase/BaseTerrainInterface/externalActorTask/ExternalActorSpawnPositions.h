#pragma once

#include "CoreMinimal.h"

/// - Will allow to collect positions from the terrain to spawn
/// external actors at: used for terrain creator!

class TERRAINPLUGINBASE_API ExternalActorSpawnPositions {

public:
    ExternalActorSpawnPositions();
    ~ExternalActorSpawnPositions();

    ExternalActorSpawnPositions(FString nameIn);
    ExternalActorSpawnPositions(FString nameIn, float onePerCmAreaIn);
    ExternalActorSpawnPositions(FString nameIn, int32 onePerCmAreaIn);


    void SetName(FString name);
    FString GetName() const;

    void AddPosition(FVector posIn);
    const TArray<FVector> &GetSpawnPositions() const;

    virtual void AppendAsBinary(TArray<uint8> &buffer);
    virtual bool LoadFromBinary(
        TArray<uint8> &buffer,
        uint8 *&Ptr // reference to a pointer. Pointer by reference.
    );

    bool NameEqualsIgnoreCase(FString other);

    bool IsValid();
    static bool IsValidName(const FString &name);

    ///returns the desired density (1 per cm)
    ///on one axis
    int GetAxisScalePerCm();

    void SetAxisScalePerCm(int32 cmIn);
    void SetAxisScalePerMeter(int32 meterIn);

private:
    FString nameTagActor;
    TArray<FVector> outPositions;

    int32 onePerCmArea = 1000; // one axis

};