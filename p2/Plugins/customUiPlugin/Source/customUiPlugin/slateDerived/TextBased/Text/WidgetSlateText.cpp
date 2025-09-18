#include "WidgetSlateText.h"
#include "CoreMath/Matrix/2D/MMatrix2D.h"


void UWidgetSlateText::ConstructWidget(){
    Super::ConstructWidget();
    CreateTextBox();
}



SlateMeshDataPolygon &UWidgetSlateText::PolygonAtTextLayer(){
    return FindFromMap(textLayer);
}



void UWidgetSlateText::CreateTextBox(){
    SlateMeshDataPolygon &polygon = PolygonAtTextLayer();
    polygon.SetFullColor(UiColors::backgroundBlackTransculent);

    //create some mesh default mesh
    int widthWidget = 120;
    int heightWidtet = 40;
    SlateMeshData &meshdata = polygon.MeshDataRef();
    FVector2D tl(0, 0);
    FVector2D br(widthWidget, heightWidtet);
    meshdata.AppendQuad(tl, br);

    SlateTextBase &text = polygon.GetSlateText();
    text.SetColor(FColor::White);
    text.SetCenteredInWidget(true);

    //text always fits inside bounds set, shrinks.
    FVector2D maxBoundText = br * 0.8f;
    text.SetFitMaxSize(maxBoundText);
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