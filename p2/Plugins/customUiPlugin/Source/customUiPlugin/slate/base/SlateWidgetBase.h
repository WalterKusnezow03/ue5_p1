#pragma once

#include "CoreMinimal.h"
#include "customUiPlugin/slate/MeshData2D/SlateMeshDataPolygon.h"
#include "customUiPlugin/slate/base/cache/SlateWidgetBoundsCache.h"
#include "Widgets/SCompoundWidget.h"
#include <map>

class CUSTOMUIPLUGIN_API SSlateWidgetBase : public SCompoundWidget
{
public:
    
    void DebugCreatePolygons();

private:
    void createLayer(int i);


public:
    //SLATE_BEGIN_ARGS(SSlateWidgetBase) {}
    //SLATE_END_ARGS()

    SLATE_BEGIN_ARGS(SSlateWidgetBase) 
        : _NumLayers(1) // Defaultwert hier korrekt
    {}
        SLATE_ARGUMENT(int32, NumLayers)
    SLATE_END_ARGS()


    

    //might be overriden to add text (unclear.)
    virtual void Construct(const FArguments& InArgs);

    /// MUST BE OVERRIDEN HERE
    //Called every frame
    virtual int32 OnPaint(const FPaintArgs& Args,
                          const FGeometry& AllottedGeometry,
                          const FSlateRect& MyCullingRect,
                          FSlateWindowElementList& OutDrawElements,
                          int32 LayerId,
                          const FWidgetStyle& InWidgetStyle,
                          bool bParentEnabled) const override;

    

    /// @brief must be triggered manually if not ticked after mesh data modfication!
    void UpdateBoundsForSizeCalculation();
    void ForceUpdateBoundsForSizeCalculation();

    FVector2D Bounds();

    //allows you to modify the internal mesh data drawn.
    SlateMeshDataPolygon &FindPolygonByLayerInternal(int layer);


    bool HasLayer(int layer) const;

    /// @brief if you change something, call this method to redraw the widget
    void MarkForRedraw();

    using SCompoundWidget::Tick;
    /// @brief widget can be ticked from UcustomUiComponent context owning this slate widget!
    /// ticks all polygons.
    virtual void Tick(float deltatime);

    ///@brief tells if the cursor is inside this geometry
    bool dispatchClick();

protected:

    //draw polygons
    void DrawAllPolygons(
        FSlateWindowElementList &OutDrawElements,
        int32 LayerId, //pass from OnPaint, dont change, polygons drawn later will overlap previous ones by default.
        FSlateRenderTransform &RenderTransform
    ) const;

private:
    void DrawPolygon(
        int layerInternal,
        FSlateWindowElementList &OutDrawElements,
        int32 LayerId, // pass from OnPaint
        FSlateRenderTransform &RenderTransform
    ) const;

    void DrawPolygon(
        const SlateMeshDataPolygon &polygon,
        FSlateWindowElementList &OutDrawElements,
        int32 LayerId, // pass from OnPaint
        FSlateRenderTransform &RenderTransform
    ) const;
    //draw polygons end



    bool bDebugDrawBox = false;
    bool bDebugLog = true;

    /// @brief stores polygons by layer id
    std::map<int, SlateMeshDataPolygon> polygonMap;

    /// @brief stores layer numbers in ascending order (id from polygonMap)
    TArray<int> layersSorted;
    
    /// @brief sorts the layers of the polygon map into the layers Sorted array for drawing
    void SortLayers();

    TArray<SlateMeshDataPolygon *> allPolygonsSorted();

    FVector2D CursorPositionScreenSpace();
    FVector2D CursorPositionLocalSpace();

private:
    const SlateMeshDataPolygon *FindPolygonByLayerInternalConst(int layer) const;



    // --- Draw size cache --- 
    SlateWidgetBoundsCache boundsCache;
};