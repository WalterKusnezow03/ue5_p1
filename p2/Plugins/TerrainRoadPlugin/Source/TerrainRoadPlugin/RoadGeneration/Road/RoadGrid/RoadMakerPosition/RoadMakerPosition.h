#pragma once

#include "CoreMinimal.h"

class TerrainInterfaceBase;

//will store the quad and its neigbors, as well as a used flag
//will help with building area determination
class TERRAINROADPLUGIN_API FRoadMakerPosition {

public:
    FRoadMakerPosition();
    ~FRoadMakerPosition();

    FRoadMakerPosition(const FRoadMakerPosition &other);
    FRoadMakerPosition &operator=(const FRoadMakerPosition &other);

    FRoadMakerPosition(
        const FVector &pos
    );

    void UpdateHeightAndMarkInBoundStatus(TerrainInterfaceBase *creator, float offset);
    FVector &GetPosition(){
        return position;
    }

    bool IsInsideTerrain(){
        return isInsideTerrain;
    }

protected:
    

    FVector position;
    bool isInsideTerrain = true;
};