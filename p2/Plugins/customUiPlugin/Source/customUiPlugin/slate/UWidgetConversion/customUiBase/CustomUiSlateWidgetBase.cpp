#include "CustomUiSlateWidgetBase.h"

void UCustomUiSlateWidgetBase::init(){
    if(WAS_INIT_FLAG){
        return;
    }
    Super::init();
    MakeWidget();
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