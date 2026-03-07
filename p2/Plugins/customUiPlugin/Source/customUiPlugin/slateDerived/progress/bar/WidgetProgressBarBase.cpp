#include "WidgetProgressBarBase.h"

#include "customUiPlugin/slate/MeshData2D/BaseMeshData/SlateMeshData.h"

#include "customUiPlugin/slate/MeshData2D/Text/SlateTextBase.h"

//constructor.
void UWidgetProgressBarBase::ConstructWidget(){
    //bDebugPolygon = false; //from super
    //Super::ConstructWidget(); // debug
    CreateBar();

    //debug create text
    if(false){
        SlateMeshDataPolygon &p = PolygonBackground();
        SlateTextBase &text = p.GetSlateText();
    }


    if(false)
        UiDebugHelper::logMessage("slate: UWidgetProgressBarBase Constructed widget!"); // is called
}

//temporary reference, use one at a time!
SlateMeshDataPolygon &UWidgetProgressBarBase::PolygonBackground(){
    return FindFromMap(layerBackground);
}

//temporary reference, use one at a time!
SlateMeshDataPolygon &UWidgetProgressBarBase::PolygonForeGround(){
    return FindFromMap(layerForeGround);
}


void UWidgetProgressBarBase::CreateBar(){
    FVector2D a(0, 0);
    //FVector2D b(200, 50);
    FVector2D b(1, 1);

    SlateMeshDataPolygon &background = PolygonBackground();
    SlateMeshData &meshData = background.MeshDataRef();
    meshData.AppendQuad(a, b);
    

    SlateMeshDataPolygon &foreGround = PolygonForeGround();
    SlateMeshData &meshData1 = foreGround.MeshDataRef();
    meshData1.AppendQuad(a, b);

    SetColorForeground(colorForeground);
    SetColorBackground(colorBackground);
}

void UWidgetProgressBarBase::SetColorBackground(FLinearColor color){
    SlateMeshDataPolygon &background = PolygonBackground();
    background.SetFullColor(color);
}

void UWidgetProgressBarBase::SetColorForeground(FLinearColor color){
    SlateMeshDataPolygon &polygon = PolygonForeGround();
    polygon.SetFullColor(color);

    //debug
    /*polygon.AddAmbientUvColor(FVector2D(0, 0), color);
    polygon.AddAmbientUvColor(FVector2D(0, 1), color);
    polygon.AddAmbientUvColor(FVector2D(1, 0), color);
    polygon.AddAmbientUvColor(FVector2D(1, 1), color);
    polygon.AddAmbientUvColor(FVector2D(0.5, 0.5), color);*/
    //polygon.DebugLogColor();
}






void UWidgetProgressBarBase::SetProgress(float scalar){
    Super::SetProgress(scalar);

    MMatrix2D scale = MMatrix2D::MakeScaleMatrix(progressScalar, 1.0f); // scaled on X axis for now.

    SlateMeshDataPolygon &polygon = PolygonForeGround();
    SlateMeshData &meshData = polygon.MeshDataRef();
    meshData.SetRuntimeTransformation(scale);
    
}


