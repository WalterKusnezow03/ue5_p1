#include "SlateWidgetBase.h"
#include "Rendering/DrawElements.h"
#include "customUiPlugin/Private/Debug/UiDebugHelper.h"


void SSlateWidgetBase::Construct(const FArguments& InArgs)
{   
    
    // Falls du Inhalte (Text etc.) einfügen willst
    ChildSlot
    [
        SNullWidget::NullWidget
    ];

    if(bDebugCreatePolygons){
        DebugCreatePolygonsOnConstruct();
    }
}

void SSlateWidgetBase::DebugCreatePolygonsOnConstruct(){

    SlateMeshDataPolygon &polygon = FindPolygonByLayerInternal(0);

    TArray<FVector2D> shape = {
        FVector2D(0, 0),
        FVector2D(100, 0),
        FVector2D(200, 0),
        FVector2D(300, 300),
        FVector2D(100, 200)
    };
    polygon.AppendClosedShape(shape, 10); //20 freeze, 5 to low!

    //polygon.SetColor(FLinearColor::Blue);
    polygon.SetCursorColor(FLinearColor::White);

    SlateMeshData &meshdata = polygon.MeshDataRef();
    meshdata.AddAmbientUvColor(FVector2D(0, 0), FLinearColor::Red);
    meshdata.AddAmbientUvColor(FVector2D(1, 1), FLinearColor::Blue);

}

// ---- external Tick ----
void SSlateWidgetBase::Tick(float deltatime){
    FVector2D cursorLocalSpace = CursorPositionLocalSpace();

    if(bDebugLog){
        UiDebugHelper::showScreenMessage(FString::Printf(
            TEXT("Cursor Local (%.1f, %.1f)"),
            cursorLocalSpace.X,
            cursorLocalSpace.Y),
            FColor::Red
        );
    }

    bool changesRegistered = false;
    TArray<SlateMeshDataPolygon *> polygons = allPolygonsSorted();
    for (int i = 0; i < polygons.Num(); i++){
        
        if(SlateMeshDataPolygon *current = polygons[i]){
            //A & w = A, A & f = f
            //A | w = w, A | f = A
            changesRegistered = changesRegistered || current->Tick(deltatime);
            current->UpdateCursorPosition(cursorLocalSpace);
        }
    }
    //not needed, redrawn every frame.
    if(false && changesRegistered){
        MarkForRedraw();
    }
}

// ---- Redraw request -----
void SSlateWidgetBase::MarkForRedraw(){
    Invalidate(EInvalidateWidgetReason::Paint);
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
        RenderTransform //for polygon transformation 
    );

    return LayerId + 1;
}





void SSlateWidgetBase::DrawAllPolygons(
    FSlateWindowElementList& OutDrawElements,
    int32 LayerId, //dont change, polygons drawn later will overlap previous ones by default.
    FSlateRenderTransform &RenderTransform
)const{
    for (int i = 0; i < layersSorted.Num(); i++){
        int layer = layersSorted[i];
        DrawPolygon(layer, OutDrawElements, LayerId, RenderTransform);
    }
}


void SSlateWidgetBase::DrawPolygon(
    int layerInternal,
    FSlateWindowElementList& OutDrawElements,
    int32 LayerId, //dont change, polygons drawn later will overlap previous ones by default.
    FSlateRenderTransform &RenderTransform
)const{
    if(HasLayer(layerInternal)){

        auto it = polygonMap.find(layerInternal);
        if (it != polygonMap.end()){
            const SlateMeshDataPolygon& ref = it->second;
            // Zugriff auf ref
        
            DrawPolygon(
                ref,
                OutDrawElements,
                LayerId,
                RenderTransform
            );
        }

    }
}

void SSlateWidgetBase::DrawPolygon(
    const SlateMeshDataPolygon &polygon,
    FSlateWindowElementList& OutDrawElements,
    int32 LayerId, //dont change, polygons drawn later will overlap previous ones by default.
    FSlateRenderTransform &RenderTransform
)const{

    const TArray<FSlateVertex> &vertecies = 
        polygon
        .MeshDataRefConst()
        .GetSlateVertexBuffer(RenderTransform); //const context internally escaped, buffer is cached.
    const TArray<SlateIndex> &triangles = polygon.MeshDataRefConst().TrianglesRefConst();

    TArray<FColor> colors;

    //const FSlateBrush* Brush = FCoreStyle::Get().GetBrush("WhiteBrush");
    //FSlateResourceHandle ResourceHandle(Brush);

    FSlateResourceHandle ResourceHandle; // leer

    FSlateDrawElement::MakeCustomVerts(
        OutDrawElements,
        LayerId,
        ResourceHandle,
        vertecies,
        triangles,
        nullptr, // InInstanceData
        0,       // InInstanceOffset
        1,       // InNumInstances
        ESlateDrawEffect::None
    );


    //FString message = FString::Printf(TEXT("SlateWidgetBase: Draw Polygon (%d)"), vertecies.Num());
    //UiDebugHelper::logMessage(message);

    /*
    FSlateDrawElement::MakeCustomVerts(
        OutDrawElements,
        LayerId,
        ResourceHandle,
        vertecies,
        triangles,
        nullptr, // optional: UVs
        colors, // optional: Colors
        ESlateDrawEffect::None
    );*/
}







/// --- DATA MANAGEMENT ---
TArray<SlateMeshDataPolygon *> SSlateWidgetBase::allPolygonsSorted(){
    TArray<SlateMeshDataPolygon *> outArray;
    for (int i = 0; i < layersSorted.Num(); i++){
        int layer = layersSorted[i];
        if (HasLayer(layer))
        {
            outArray.Add(&FindPolygonByLayerInternal(layer));
        }
    }
    return outArray;
}

void SSlateWidgetBase::SortLayers(){
    TArray<int> allLayers;
    for(auto &pair : polygonMap){
        allLayers.Add(pair.first);
    }
    allLayers.Sort();
    layersSorted = allLayers;
}

SlateMeshDataPolygon &SSlateWidgetBase::FindPolygonByLayerInternal(int layerId){
    if(!HasLayer(layerId)){
        polygonMap[layerId] = SlateMeshDataPolygon();
        SortLayers(); //sort layers again.
    }
    return polygonMap[layerId];
}

bool SSlateWidgetBase::HasLayer(int layerId) const {
    return polygonMap.find(layerId) != polygonMap.end();
}

/*
outline polygon

TArray<FVector2D> Vertices;
Vertices.Add(FVector2D(0,0));
Vertices.Add(FVector2D(100,0));
Vertices.Add(FVector2D(100,100));
Vertices.Add(FVector2D(0,100));
Vertices.Add(FVector2D(0,0)); // letztes Vertex = Startpunkt, um Polygon zu schließen

FSlateDrawElement::MakeLines(
    OutDrawElements,
    LayerId,
    AllottedGeometry.ToPaintGeometry(),
    Vertices,
    ESlateDrawEffect::None,
    FLinearColor::Red,
    true, // geschlossene Linie
    2.f   // Linienstärke
);


*/

/*
Filled polygon

TArray<FSlateVertex> Vertices;
TArray<uint16> Indices;

// Beispiel: Dreieck
Vertices.Add(FSlateVertex::Make<FVector2D>(FVector2D(0,0), FLinearColor::Red));
Vertices.Add(FSlateVertex::Make<FVector2D>(FVector2D(100,0), FLinearColor::Red));
Vertices.Add(FSlateVertex::Make<FVector2D>(FVector2D(50,100), FLinearColor::Red));

Indices.Add(0);
Indices.Add(1);
Indices.Add(2);

FSlateDrawElement::MakeCustomVerts(
    OutDrawElements,
    LayerId,
    FCoreStyle::Get().GetBrush("WhiteBrush"),
    Vertices,
    Indices,
    nullptr, // optional: UVs
    nullptr, // optional: Colors
    ESlateDrawEffect::None
);


*/




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
