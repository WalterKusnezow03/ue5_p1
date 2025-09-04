#include "WidgetProgressBarBase.h"

#include "customUiPlugin/slate/MeshData2D/SlateMeshData.h"

//constructor.
void UWidgetProgressBarBase::ConstructWidget(){
    //Super::ConstructWidget(); //debug
    CreateBar();
    UiDebugHelper::logMessage("slate: UWidgetProgressBarBase Constructed widget!"); // is called
}

SlateMeshDataPolygon *UWidgetProgressBarBase::PolygonBackground(){
    return FindFromSlateWidget(layerBackground);
}

SlateMeshDataPolygon *UWidgetProgressBarBase::PolygonForeGround(){
    return FindFromSlateWidget(layerForeGround);
}


void UWidgetProgressBarBase::CreateBar(){
    FVector2D a(0, 0);
    FVector2D b(200, 50);

    SlateMeshDataPolygon *background = PolygonBackground();
    if(background){
        SlateMeshData &meshData = background->MeshDataRef();
        meshData.AppendQuad(a, b);
        UiDebugHelper::logMessage("slate: UWidgetProgressBarBase create quad!"); 
    }

    SlateMeshDataPolygon *foreGround = PolygonForeGround();
    if (foreGround)
    {
        SlateMeshData &meshData = foreGround->MeshDataRef();
        meshData.AppendQuad(a, b);
        UiDebugHelper::logMessage("slate: UWidgetProgressBarBase create quad 2!");
    }

    SetColorForeground(FLinearColor::Green);
    SetColorBackground(FLinearColor::Black);
}

void UWidgetProgressBarBase::SetColorBackground(FLinearColor color){
    if(SlateMeshDataPolygon *polygon = PolygonBackground()){
        polygon->SetColor(color);
    }
}

void UWidgetProgressBarBase::SetColorForeground(FLinearColor color){
    if(SlateMeshDataPolygon *polygon = PolygonForeGround()){
        polygon->SetColor(color);
    }
}



void UWidgetProgressBarBase::SetProgress(float num){
    num = ClampProgress(num);
    if (SlateMeshDataPolygon *polygon = PolygonForeGround()){
        MMatrix2D scale = MMatrix2D::MakeScaleMatrix(num, 1.0f); //scaled on X axis for now.
        SlateMeshData &meshData = polygon->MeshDataRef();
        meshData.SetRuntimeTransformation(scale);
    }
}

void UWidgetProgressBarBase::ResetProgress(){
    if (SlateMeshDataPolygon *polygon = PolygonForeGround()){
        SlateMeshData &meshData = polygon->MeshDataRef();
        //reset matrix instead of rescaling to 1.0f, useless calculation made internally.
        meshData.ResetRuntimeTransformation();
    }
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

    //debug

    debugTime += deltatime;
    if(debugTime >= 3.0f){
        debugTime = 0.0f;
    }

    float scalar = debugTime / 3.0f; //scalar = distTarget / distAll
    SetProgress(scalar);
}
