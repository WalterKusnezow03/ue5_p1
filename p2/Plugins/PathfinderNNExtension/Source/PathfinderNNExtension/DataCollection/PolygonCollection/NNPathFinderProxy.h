#pragma once

#include "CoreMinimal.h"

#include "PolygonPlugin/Public/Polygons/MeshedPolygon.h"
#include "PolygonPlugin/Public/Polygons/MeshedPolygonExtension/MeshedPolygonDistanceField.h"


class PATHFINDERNNEXTENSION_API NNPathFinderProxy {

public:
    NNPathFinderProxy();


    
    void CollectPolygon(
        FVector playerPos,
        float radius,
        FMeshedPolygon &outData
    );

    void EndSave();

private:
    bool saveOnEnd = false;

    TArray<FMeshedPolygon> cachedImages;

    void DebugSaveMeshedPolygonToStorage(
        FMeshedPolygon &polygon,
        FString nameImage
    );
    
};