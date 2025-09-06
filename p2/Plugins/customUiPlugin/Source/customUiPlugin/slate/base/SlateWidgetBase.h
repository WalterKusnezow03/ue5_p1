#pragma once

#include "CoreMinimal.h"
#include "customUiPlugin/slate/MeshData2D/SlateMeshDataPolygon.h"

#include "customUiPlugin/slate/MeshData2D/sharedContainer/SlatePolygonMap.h"
#include "Widgets/SCompoundWidget.h"
#include <map>

class CUSTOMUIPLUGIN_API SSlateWidgetBase : public SCompoundWidget
{
public:
    
    

private:

    bool bDebugDrawBox = false;
    bool bDebugLog = false;

    //SlatePolygonMap *PolygonMapExternal = nullptr;
    TSharedPtr<SlatePolygonMap> PolygonMapExternal;

public:
    //SLATE_BEGIN_ARGS(SSlateWidgetBase) {}
    //SLATE_END_ARGS()

    /*SLATE_BEGIN_ARGS(SSlateWidgetBase) 
        : _PolyGonMapPtr(nullptr) // Defaultwert hier korrekt
    {}
        SLATE_ARGUMENT(SlatePolygonMap*, PolyGonMapPtr)
    SLATE_END_ARGS()*/

    SLATE_BEGIN_ARGS(SSlateWidgetBase) 
        : _PolyGonMapPtr(nullptr) // Defaultwert hier korrekt
    {}
        SLATE_ARGUMENT(TSharedPtr<SlatePolygonMap>, PolyGonMapPtr)
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

    


    FVector2D Bounds();



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



    


    FVector2D CursorPositionScreenSpace();
    FVector2D CursorPositionLocalSpace();




};