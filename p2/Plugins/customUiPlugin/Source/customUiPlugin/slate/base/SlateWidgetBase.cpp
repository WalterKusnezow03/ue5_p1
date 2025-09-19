#include "SlateWidgetBase.h"
#include "Rendering/DrawElements.h"
#include "Fonts/SlateFontInfo.h"
#include "customUiPlugin/Private/Debug/UiDebugHelper.h"


void SSlateWidgetBase::Construct(const FArguments& InArgs)
{   
    
    // Falls du Inhalte (Text etc.) einfügen willst
    ChildSlot
    [
        SNullWidget::NullWidget //manual bounding box needed because no child widget is present.
    ];
    

    PolygonMapExternal = InArgs._PolyGonMapPtr;
}

// ---- external Tick ----
void SSlateWidgetBase::Tick(float deltatime){
    FVector2D cursorLocalSpace = CursorPositionLocalSpace();
    if(PolygonMapExternal.IsValid()){
        PolygonMapExternal->UpdateCursorPosition(cursorLocalSpace);
    }


    if(bDebugLog){
        UiDebugHelper::showScreenMessage(FString::Printf(
            TEXT("Cursor Local (%.1f, %.1f)"),
            cursorLocalSpace.X,
            cursorLocalSpace.Y),
            FColor::Red
        );
    }

}






// ---- PAINTING -----

int32 SSlateWidgetBase::OnPaint(const FPaintArgs& Args,
                         const FGeometry& AllottedGeometry,
                         const FSlateRect& MyCullingRect,
                         FSlateWindowElementList& OutDrawElements,
                         int32 LayerId,
                         const FWidgetStyle& InWidgetStyle,
                         bool bParentEnabled) const
{
    //use transform to transform the vertex buffer of the polygons,
    //needed each time. Otherwise the Polygon is drawn from root(0,0)
    FSlateRenderTransform RenderTransform = AllottedGeometry.GetAccumulatedRenderTransform();

    //draw debug box / background red
    if(bDebugDrawBox){
        FSlateDrawElement::MakeBox(
            OutDrawElements,
            LayerId,
            AllottedGeometry.ToPaintGeometry(),
            FCoreStyle::Get().GetBrush("WhiteBrush"), // einfacher weißer Brush
            ESlateDrawEffect::None,
            FLinearColor::Red // Farbe des Vierecks
        );
    }

    DrawAllPolygons(
        OutDrawElements,
        LayerId,
        RenderTransform, //for polygon transformation 
        AllottedGeometry
    );

    return LayerId + 1;
}





void SSlateWidgetBase::DrawAllPolygons(
    FSlateWindowElementList& OutDrawElements,
    int32 &LayerId, //dont change, polygons drawn later will overlap previous ones by default.
    FSlateRenderTransform &RenderTransform,
    const FGeometry& AllottedGeometry
)const{

    if(PolygonMapExternal.IsValid()){
        TArray<SlateMeshDataPolygon *> array = PolygonMapExternal->allPolygonsSortedConst();
        for (int i = 0; i < array.Num(); i++){
            const SlateMeshDataPolygon *current = array[i];
            if(current){
                DrawPolygon(*current, OutDrawElements, LayerId, RenderTransform, AllottedGeometry);
            }   
        }
    }
}



void SSlateWidgetBase::DrawPolygon(
    const SlateMeshDataPolygon &polygon,
    FSlateWindowElementList& OutDrawElements,
    int32 &LayerId, //dont change, polygons drawn later will overlap previous ones by default.
    FSlateRenderTransform &RenderTransform,
    const FGeometry& AllottedGeometry
)const{
    if(!polygon.bIsEnabled()){
        return; //rendering disabled.
    }

    //DEFAULT DRAW
    if(polygon.IsFlaggedDrawOutlineOnly()){ 
        DrawPolygonOutlineOnly(polygon, OutDrawElements, LayerId, AllottedGeometry);
    }else{
        DrawPolygonFilled(polygon, OutDrawElements, LayerId, RenderTransform, AllottedGeometry);
    }
    LayerId++;

    //draw text ontop
    DrawText(
        polygon,
        OutDrawElements,
        LayerId + 100, // (higher index to force text to be ontop, 1 is not enough!) 
        //dont change, polygons drawn later will overlap previous ones by default.
        AllottedGeometry
    );
}

void SSlateWidgetBase::DrawPolygonFilled(
    const SlateMeshDataPolygon &polygon,
    FSlateWindowElementList& OutDrawElements,
    int32 LayerId, //dont change, polygons drawn later will overlap previous ones by default.
    FSlateRenderTransform &RenderTransform,
    const FGeometry& AllottedGeometry
)const {
    const TArray<FSlateVertex> &vertecies = 
        polygon
        .MeshDataRefConst()
        .GetSlateVertexBuffer(RenderTransform); //const context internally escaped, buffer is cached.
    const TArray<SlateIndex> &triangles = polygon.MeshDataRefConst().TrianglesRefConst();

    //FSlateResourceHandle ResourceHandle; // leer

    FSlateResourceHandle ResourceHandle = polygon.MeshDataRefConst().drawingHandle();

    FSlateDrawElement::MakeCustomVerts(
        OutDrawElements,
        LayerId,
        ResourceHandle,
        vertecies,
        triangles,
        nullptr, //unclear what this is
        0,       //unclear what this is
        1,       //unclear what this is
        ESlateDrawEffect::None //unclear what this is
    );
}


// --- not in use anyway ---
void SSlateWidgetBase::DrawPolygonOutlineOnly(
    const SlateMeshDataPolygon &polygon,
    FSlateWindowElementList& OutDrawElements,
    int32 LayerId, //dont change, polygons drawn later will overlap previous ones by default.
    const FGeometry& AllottedGeometry
)const {
    /*
    static void MakeLines  
    (  
        FSlateWindowElementList & ElementList,  
        uint32 InLayer,  
        const FPaintGeometry & PaintGeometry,  
        TArray < FVector2f > Points,  
        ESlateDrawEffect InDrawEffects,  
        const FLinearColor & InTint,  
        bool bAntialias,  
        float Thickness  
    )
    */
   const TArray<FVector2f> &twoFBuffer =
   polygon
       .MeshDataRefConst()
       .VerteciesAs2f();

    //draw only outline
    FSlateDrawElement::MakeLines(
    OutDrawElements,
    LayerId,
    AllottedGeometry.ToPaintGeometry(),
    twoFBuffer,
    ESlateDrawEffect::None,
    FLinearColor::Red,
    true, // geschlossene Linie
    2.0f   // Linienstärke
    );
}




// --- TESTING NEEDED ! ---

void SSlateWidgetBase::DrawText(
    const SlateMeshDataPolygon &polygon,
    FSlateWindowElementList& OutDrawElements,
    int32 LayerId, //dont change, polygons drawn later will overlap previous ones by default.
    const FGeometry& AllottedGeometry
) const {
    if(polygon.bHasText()){
        
        const SlateTextBase &text = polygon.GetSlateTextConst();
        FVector2f pivot = polygon.SlateTextPivot2f();
        const FSlateFontInfo &fontInfo = text.FontInfo();
        FColor color = text.Color();
        FString stringToDisplay = text.GetText();


        FVector2f LocalSize = FVector2f(AllottedGeometry.GetLocalSize());
        FSlateLayoutTransform LayoutTransform(pivot); // pivot ist FVector2f

        FSlateDrawElement::MakeText(
            OutDrawElements,
            LayerId,
            AllottedGeometry.ToPaintGeometry(LocalSize, LayoutTransform),
            FText::FromString(stringToDisplay),
            fontInfo,
            ESlateDrawEffect::None,
            color
        );




        /*
        FSlateDrawElement::
        static void MakeText  
        (  
            FSlateWindowElementList & ElementList,  
            uint32 InLayer,  
            const FPaintGeometry & PaintGeometry,  
            const FString & InText,  
            const int32 StartIndex,  
            const int32 EndIndex,  
            const FSlateFontInfo & InFontInfo,  
            ESlateDrawEffect InDrawEffects,  
            const FLinearColor & InTint  
        ) 
        */
    }
}









// --- click dispatch ---
bool SSlateWidgetBase::dispatchClick(){
    FGeometry Geometry = GetCachedGeometry();
    FVector2D cursorPos = CursorPositionScreenSpace();

    FVector2D topLeft = Geometry.LocalToAbsolute(FVector2D(0, 0));
    FVector2D bottomRight = Geometry.LocalToAbsolute(Geometry.GetLocalSize());

    FVector2D size = bottomRight - topLeft; //AB = B - A
    topLeft -= size * 0.1f;
    bottomRight += size * 0.1f;

    bool inBoundsX = cursorPos.X >= topLeft.X && cursorPos.X <= bottomRight.X;
    bool inBoundsY = cursorPos.Y >= topLeft.Y && cursorPos.Y <= bottomRight.Y;

    return inBoundsX && inBoundsY;
}

FVector2D SSlateWidgetBase::CursorPositionScreenSpace(){
    return FSlateApplication::Get().GetCursorPos();
}

FVector2D SSlateWidgetBase::CursorPositionLocalSpace(){
    FVector2D screenSpace = CursorPositionScreenSpace();
    FGeometry Geometry = GetCachedGeometry();
    return Geometry.AbsoluteToLocal(screenSpace);
    
    //FVector2D topLeft = Geometry.LocalToAbsolute(FVector2D(0, 0));
    //return screenSpace - topLeft; //AB = B - A
}
