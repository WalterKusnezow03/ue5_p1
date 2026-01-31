#pragma once

#include "CoreMinimal.h"
#include "CoreMath/Matrix/MMatrix.h"
#include <map>
#include "p2/ui/3Dui/HUD/Widget/MinimapWidgetData/EMarkerType.h"

class P2_API MiniMapData {

public:
    MiniMapData();
    ~MiniMapData();

    void UpdatePlayerTransform(AActor *player);


    std::map<EMarkerType, TArray<FVector2D>> &MapFromCollectMarkersCanvasSpace(
        const FVector2D &canvasScale
    );

    //adds and removes marked actors from minimap
    void AddMarker(EMarkerType type, AActor *actor);
    void RemoveMarker(AActor *actor);

private:
    MMatrix playerRotation;
    MMatrix playerLocation;

    MMatrix PlayerTransformInverse;
    void MakePlayerInverse();

    


    float maxRadiusMap = 5000.0f; // 50m

    void UpdateMarkersCanvasSpace(const FVector2D &canvasScale);

    void CollectMarkersCanvasSpace(
        EMarkerType type, 
        TArray<FVector2D> &outMarkers,
        const FVector2D &canvasScale,
        const FVector2D &canvasHalfScale
    );


    void CollectMarkersWorld(
        TArray<FVector2D> &outMarkers,
        EMarkerType type
    );

    void MoveToCanvasSpace(
        TArray<FVector2D> &array, 
        const FVector2D &canvasScale, 
        const FVector2D &canvasHalfScale
    );
    void MoveToCanvasSpace(
        FVector2D &pos, 
        const FVector2D &canvasScale, 
        const FVector2D &canvasHalfScale
    );

    // TRACKED ACTORS
    std::map<EMarkerType, TArray<AActor *>> typeMap;

    // CACHE
    std::map<EMarkerType, TArray<FVector2D>> visibleMarkerMap;
    // CACHE
   

    TArray<AActor *> &Find(EMarkerType type);

    FVector2D LocationInPlayerRelativeSpace(AActor *actor);
    FVector2D LocationInPlayerRelativeSpace(FVector location);
    bool InRange(FVector2D &location);
};