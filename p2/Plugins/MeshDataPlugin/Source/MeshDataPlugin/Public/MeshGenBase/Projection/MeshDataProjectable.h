#pragma once


#include "CoreMinimal.h"
#include "MeshDataPlugin/Public/MeshGenBase/MeshData/MeshData.h"

class MESHDATAPLUGIN_API MeshDataProjectable : public MeshData {

public:

    void ProjectIntoDirection(
        const FVector &actorOffset,
        FVector &direction,
        float sizeRaycast,
        UWorld *world
    );

    void CreateGrid(float sizeX, float sizeY, float detail);

private:


};