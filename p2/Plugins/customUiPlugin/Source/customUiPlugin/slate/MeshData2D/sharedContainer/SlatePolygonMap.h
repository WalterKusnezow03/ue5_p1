#pragma once

#include "CoreMinimal.h"
#include "customUiPlugin/slate/base/cache/SlateWidgetBoundsCache.h"
#include "customUiPlugin/slate/MeshData2D/SlateMeshDataPolygon.h"
#include <map>




/// @brief Will be stored in UWidget base having Slate Widget,
/// data is synchronized to slate widget - get on draw.
/// Later updates cant be applied to SWidgets, but must be rebuild, thats why
/// the Polygon data is removed from slate!
/// SWIDGET WILL HOLD POINTER TO THIS DATA - This data Will be Stored by value in the UWidget Owning the 
/// SWidget. Very easy architecture.
class CUSTOMUIPLUGIN_API SlatePolygonMap {

public:
    SlatePolygonMap();
    ~SlatePolygonMap();

    void DebugCreatePolygons();

    void Tick(float deltatime);

    void UpdateCursorPosition(FVector2D &pos);

    FVector2D Bounds();

    //allows you to modify the internal mesh data drawn.
    SlateMeshDataPolygon &FindPolygonByLayerInternal(int layer);
    const SlateMeshDataPolygon *FindPolygonByLayerInternalConst(int layer) const;

    TArray<SlateMeshDataPolygon *> allPolygonsSorted();
    
    TArray<SlateMeshDataPolygon *> allPolygonsSortedConst() const;

private:
    
    // --- Draw size cache --- 
    SlateWidgetBoundsCache boundsCache;


    /// @brief stores polygons by layer id
    std::map<int, SlateMeshDataPolygon> polygonMap;

    /// @brief stores layer numbers in ascending order (id from polygonMap)
    TArray<int> layersSorted;
    
    /// @brief sorts the layers of the polygon map into the layers Sorted array for drawing
    void SortLayers();

    bool HasLayer(int layerId) const;

    void UpdateBoundsForSizeCalculation();
    void ForceUpdateBoundsForSizeCalculation();
};