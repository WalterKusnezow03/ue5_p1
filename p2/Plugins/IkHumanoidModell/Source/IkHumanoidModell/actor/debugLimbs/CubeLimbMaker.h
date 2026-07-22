#pragma once
#include "CoreMinimal.h"
#include "GameCore/MeshGenBase/customMeshActorBase.h"
#include "AssetEnumCollection/assetEnums/materialEnum.h"

class IKHUMANOIDMODELL_API CubeLimbMaker {

public:
    static AActor *createLimbPivotAtTop(
        int x,
        int y,
        int height,
        UWorld *world
    );

    static AActor *createLimbPivotAtTop(
        int x,
        int y,
        int height,
        UWorld *world,
        materialEnum typeMaterial
    );


private:
    static AcustomMeshActorBase *spawnAcustomMeshActor(UWorld *world);

    static float min(float a, float b, float c);

    CubeLimbMaker();
    ~CubeLimbMaker();
};