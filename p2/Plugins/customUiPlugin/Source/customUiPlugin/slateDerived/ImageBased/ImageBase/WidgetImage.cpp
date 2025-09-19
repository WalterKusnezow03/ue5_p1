#include "WidgetImage.h"


void UWidgetImage::ConstructWidget(){
    Super::ConstructWidget();
    CreateImageBase();
}

void UWidgetImage::CreateImageBase(){

    SlateMeshDataPolygon &polygon = PolygonAtImageLayer();
    SlateMeshData &meshdata = polygon.MeshDataRef();

    SlateMeshDataPolygon &polygonBackground = PolygonAtBackgroundLayer();
    SlateMeshData &meshdataBackground = polygonBackground.MeshDataRef();

    //setup quad
    FVector2D tl(0, 0);
    FVector2D br(1, 1);
    meshdata.AppendQuad(tl, br);
    polygon.SetFullColor(FLinearColor(1.0f,1.0f,1.0f,1.0f));

    meshdataBackground.AppendQuad(tl, br);
    meshdataBackground.SetFullColor(UiColors::backgroundBlackTransculent);

}

SlateMeshDataPolygon &UWidgetImage::PolygonAtImageLayer(){
    return FindFromMap(imageLayer);
}

SlateMeshDataPolygon &UWidgetImage::PolygonAtBackgroundLayer(){
    return FindFromMap(imageLayer - 1);
}


/// @brief set image with auto resize widget
/// @param image 
void UWidgetImage::SetImage(UTexture2D *image){
    if(image){
        //get poylgon data
        //get mesh data
        //set image
        SlateMeshDataPolygon &polygon = PolygonAtImageLayer();
        SlateMeshData &meshdata = polygon.MeshDataRef();
        meshdata.SetTexture(image);
    }
}

void UWidgetImage::SetImage(UTexture2D *image, FVector2D fixedRes){
    

    if(image){
        SetImage(image);
        SetResolution(validateResolution(fixedRes));
    }
}

FVector2D UWidgetImage::validateResolution(FVector2D &other){
    FVector2D out(
        std::max(other.X, 1.0),
        std::max(other.Y, 1.0)
    );
    return out;
}


void UWidgetImage::Tick(float deltatime){
    Super::Tick(deltatime);
    UiDebugHelper::logMessage(
        FString::Printf(TEXT("UWidgetImage tick, res %s"), *GetResolution().ToString())
    );
}
