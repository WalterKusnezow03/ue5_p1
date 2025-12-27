#pragma once
#include "CoreMinimal.h"
#include "GameCore/MeshGenBase/customMeshActorBase.h"

class IKHUMANOIDMODELL_API CubeLimbMaker {

public:
    static AActor *createLimbPivotAtTop(
        int x,
        int y,
        int height,
        UWorld *world
    );


private:
    static AcustomMeshActorBase *spawnAcustomMeshActor(UWorld *world);

    static float min(float a, float b, float c);

    CubeLimbMaker();
    ~CubeLimbMaker();
};