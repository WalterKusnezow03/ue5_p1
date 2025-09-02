#include "CustomUiSlateWidgetBase.h"



void UCustomUiSlateWidgetBase::SetWidgetScale(FVector2D scalePixels){
    scalePixels.X = std::max(std::abs(scalePixels.X), 1.0);
    scalePixels.Y = std::max(std::abs(scalePixels.Y), 1.0);

    if(baseLayout){
        baseLayout->SetWidthOverride(scalePixels.X);
        baseLayout->SetHeightOverride(scalePixels.Y);
    }
}


void UCustomUiSlateWidgetBase::MakeBaseLayout(){
    if(widget){
        baseLayout = NewObject<USizeBox>();
        baseLayout->AddChild(widget); // Dein Slate-UWidget
    }
    
}

void UCustomUiSlateWidgetBase::MakeWidget(){
    widget = NewObject<UWidgetSlateWrapperBase>();
}


void UCustomUiSlateWidgetBase::Tick(float deltatime){
    if(widget && markedVisible()){
        widget->Tick(deltatime);
    }
}

bool UCustomUiSlateWidgetBase::dispatchClick(){
    if(widget){
        return widget->dispatchClick();
    }
    return false;
}