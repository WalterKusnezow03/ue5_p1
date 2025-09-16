#pragma once 

#include "CoreMinimal.h"

class SlateMeshDataPolygon;

///@brief Will compute the total bounding box of a SlateMeshDataPolygon Array and cache
///it. Can be flagged for update, manual update needed
///will cache the size in const context for safe Desired 2D size call from SSlateWidgetBase
class CUSTOMUIPLUGIN_API SlateWidgetBoundsCache {

public:
    SlateWidgetBoundsCache();
    ~SlateWidgetBoundsCache();

    void SetUpdateNeededTrue();
    bool UpdateNeeded() const;

    void Recreate(TArray<SlateMeshDataPolygon*> &polygons);

    FVector2D DesiredSize(float scalar) const;

    FVector2D Size() const;


    void RemoveExtension();
    void SetExtension(int size);
    

private:
    FSlateBoundingBox2D boundsInternal;
    bool updateNeededFlag = true;

    //due to racing conditions, a counter is needed
    int countModified = 0;


    //size extension of bounds
    bool hasExtension = false;
    int prevSize = 0;
    FVector2D extension;
};