#include "WidgetProgressBarBase.h"

#include "customUiPlugin/slate/MeshData2D/BaseMeshData/SlateMeshData.h"

#include "customUiPlugin/slate/MeshData2D/Text/SlateTextBase.h"

//constructor.
void UWidgetProgressBarBase::ConstructWidget(){
    //bDebugPolygon = false; //from super
    //Super::ConstructWidget(); // debug
    CreateBar();

    //debug create text
    if(true){
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

    SetColorForeground(FLinearColor::Green);
    SetColorBackground(FLinearColor::White);
    SetResolution(FVector2D(150, 20));
}

void UWidgetProgressBarBase::SetColorBackground(FLinearColor color){
    SlateMeshDataPolygon &background = PolygonBackground();
    background.SetFullColor(color);
}

void UWidgetProgressBarBase::SetColorForeground(FLinearColor color){
    SlateMeshDataPolygon &polygon = PolygonForeGround();
    //polygon.SetColor(color);

    //debug
    polygon.AddAmbientUvColor(FVector2D(0, 0), color);
    polygon.AddAmbientUvColor(FVector2D(0, 1), color);
    polygon.AddAmbientUvColor(FVector2D(1, 0), color);
    polygon.AddAmbientUvColor(FVector2D(1, 1), color);
    polygon.AddAmbientUvColor(FVector2D(0.5, 0.5), color);
    //polygon.DebugLogColor();
}






void UWidgetProgressBarBase::SetProgress(float num){
    // scale for foreground by scalar
    num = ClampProgress(num);
    progressCopy = num;
    MMatrix2D scale = MMatrix2D::MakeScaleMatrix(num, 1.0f); // scaled on X axis for now.

    SlateMeshDataPolygon &polygon = PolygonForeGround();
    SlateMeshData &meshData = polygon.MeshDataRef();
    meshData.SetRuntimeTransformation(scale);
    
}

void UWidgetProgressBarBase::ResetProgress(){
    SetProgress(1.0f);
}


float UWidgetProgressBarBase::ClampProgress(float num){
    num = std::max(num, 0.0f);
    num = std::min(num, 1.0f);
    return num;
}



/// --- Click, Tick and Animation ---
bool UWidgetProgressBarBase::dispatchClick(){
    if(Super::dispatchClick()){
        //start some animation for debugging

        return true;
    }
    return false;
}

void UWidgetProgressBarBase::Tick(float deltatime){
    Super::Tick(deltatime);

    if(TickAllowed()){
        //debug

        debugTime += deltatime;
        if(debugTime >= 3.0f){
            debugTime = 0.0f;
        }

        float scalar = debugTime / 3.0f; //scalar = distTarget / distAll
        SetProgress(scalar);
    }
    
}
