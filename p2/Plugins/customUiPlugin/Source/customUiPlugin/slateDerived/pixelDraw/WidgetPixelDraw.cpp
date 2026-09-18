#include "WidgetPixelDraw.h"

#include "customUiPlugin/slate/MeshData2D/BaseMeshData/SlateMeshData.h"


//constructor.
void UWidgetPixelDraw::ConstructWidget(){
    //bDebugPolygon = false; //from super
    //Super::ConstructWidget(); // debug
    
    //CreatePixels();
    //Clear();

    if(true)
        UiDebugHelper::logMessage("slate: UWidgetPixelDraw Constructed widget!"); // is called
}

SlateMeshDataPolygon &UWidgetPixelDraw::FindData(int x, int y){
    int index = y * width + x;
    return FindFromMap(index);
}





void UWidgetPixelDraw::CreatePixels(){
    for (int i = 0; i < width; i++){
        for (int j = 0; j < height; j++){
            CreatePixel(i, j);
        }
    }    
}

void UWidgetPixelDraw::CreatePixel(int x, int y){
    FVector2D a(0, 0);
    FVector2D b(1, 1);
    FindData(x,y).MeshDataRef().AppendQuad(a, b);
}

void UWidgetPixelDraw::Clear(){
    //FindData().MeshDataRef().ClearAmbientColors();
}

void UWidgetPixelDraw::SetColorPixel(int x, int y, FLinearColor color){
    FindData(x,y).SetFullColor(color);
}

void UWidgetPixelDraw::SetColorPixel(int x, int y, FColor color){
    FLinearColor LinearColorFromFColor = FLinearColor::FromSRGBColor(color);
    SetColorPixel(x, y, LinearColorFromFColor);
}

#include "CoreMath/Matrix/2D/MMatrix2D.h"
void UWidgetPixelDraw::SetScale(float s){
    MMatrix2D scaleMat;
    scaleMat.Scale(s);

    for (int i = 0; i < width; i++){
        for(int j = 0; j < height; j++){
            FindData(i,j).MeshDataRef().SetRuntimeTransformation(scaleMat);
        }
    }

        
}





