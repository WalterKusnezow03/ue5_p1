#pragma once

#include "CoreMinimal.h"

#include "PolygonPlugin/Public/Polygons/MeshedPolygon.h"
#include "PolygonPlugin/Public/Polygons/MeshedPolygonExtension/MeshedPolygonDistanceField.h"

class NNCOMMUNICATIONPLUGIN_API NNPathFinderProxy {

public:
    NNPathFinderProxy();


    void DebugSaveImage(
        FVector playerPos,
        float radius
    );
    
    void CollectPolygon(
        FVector playerPos,
        float radius,
        FMeshedPolygon &outData
    );

    void EndSave();

private:
    

    //FMeshedPolygon meshedPolygonStatic;
    FMeshedPolygon meshedPolygonStatic;
    TArray<FMeshedPolygon> cachedImages;

    void DebugSaveMeshedPolygonToStorage(
        FMeshedPolygon &polygon,
        FString nameImage
    );
    
};