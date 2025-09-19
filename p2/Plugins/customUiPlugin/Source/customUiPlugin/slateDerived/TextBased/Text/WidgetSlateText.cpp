#include "WidgetSlateText.h"
#include "CoreMath/Matrix/2D/MMatrix2D.h"


void UWidgetSlateText::ConstructWidget(){
    Super::ConstructWidget();
    CreateTextBox();

    //debug
    SetCursorColorEnabled(true);
}

SlateMeshDataPolygon &UWidgetSlateText::PolygonAtTextLayer(){
    return FindFromMap(textLayer);
}

FVector2D UWidgetSlateText::setupScale(){
    int widthWidget = 120;
    int heightWidtet = 40;
    return FVector2D(widthWidget, heightWidtet);
}

FVector2D UWidgetSlateText::MaxTextSizeDefault(){
    FVector2D bound = setupScale();
    return MaxTextSize(bound);
}

FVector2D UWidgetSlateText::MaxTextSize(FVector2D &bound){
    return bound * 0.8f;
}



void UWidgetSlateText::CreateTextBox(){
    SlateMeshDataPolygon &polygon = PolygonAtTextLayer();
    polygon.SetFullColor(UiColors::backgroundBlackTransculent);

    //create some mesh default mesh
    SlateMeshData &meshdata = polygon.MeshDataRef();
    FVector2D tl(0, 0);
    FVector2D br = setupScale();
    int detail = 9; //recursion split
    meshdata.AppendQuad(tl, br, detail);

    SlateTextBase &text = polygon.GetSlateText();
    //text always fits inside bounds set, shrinks.
    FVector2D maxBoundText = MaxTextSizeDefault();
    text.SetFitMaxSize(maxBoundText);
    
    text.SetColor(FColor::White);
    text.SetCenteredInWidget(true);

    
}


void UWidgetSlateText::SetText(FString text){
    SlateMeshDataPolygon &p = PolygonAtTextLayer();
    p.SetText(text);
}


FString UWidgetSlateText::GetText(){
    SlateMeshDataPolygon &p = PolygonAtTextLayer();
    const SlateTextBase &internalText = p.GetSlateTextConst();
    return internalText.GetText();
}


void UWidgetSlateText::OnHover(){
    //nothing in here yet.
}

/*
void UWidgetSlateText::SetResolution(FVector2D res){
    Super::SetResolution(res);
    UpdateTextHeightFromBounds(res);
}

void UWidgetSlateText::UpdateTextHeightFromBounds(FVector2D &bound){

    //update text
    SlateTextBase &text = polygon.GetSlateText();

    //text always fits inside bounds set, shrinks.
    FVector2D maxBoundText = MaxTextSize(bound);
    text.SetFitMaxSize(maxBoundText);
   
}*/