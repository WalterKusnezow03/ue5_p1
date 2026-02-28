#include "BorderInterfaceUtil.h"
#include "customuipluginbase/Dispatcher/ClickDispatcher.h"
#include "DebugPlugin/DebugHelper.h"

BorderInterfaceUtil::BorderInterfaceUtil(){

}

BorderInterfaceUtil::~BorderInterfaceUtil(){

}



void BorderInterfaceUtil::Init(UWidget *widget){
    if(widget){
        rawWidget = widget;
        if (UBorder *casted = Cast<UBorder>(widget))
        {
            borderWidget = casted;
        }
    }
    SetChecked(false);
    SetHovered(false);
    SetDefault();
}

void BorderInterfaceUtil::Init(
    UWidget *borderWidgetIn, 
    FLinearColor colorBaseIn,
    FLinearColor colorCheckedIn,
    FLinearColor colorHoverIn
){
    colorBase = colorBaseIn;
    colorChecked = colorCheckedIn;
    colorHover = colorHoverIn;
    Init(borderWidgetIn);
}

void BorderInterfaceUtil::SetChecked(bool flag){
    if(borderWidget){
        bIsChecked = flag;
        FLinearColor color = bIsChecked ? colorChecked : colorBase;
        borderWidget->SetBrushColor(color);
    }
}

void BorderInterfaceUtil::SetHovered(bool flag){
    if(borderWidget){
        bIsHovered = flag;

        FLinearColor colorPressed = bIsChecked ? colorChecked : colorBase;
        FLinearColor color = bIsHovered ? colorHover : colorPressed;

        borderWidget->SetBrushColor(color);
    }
}

void BorderInterfaceUtil::removeHover(){
    SetHovered(false);
}

void BorderInterfaceUtil::SetDefault(){
    if(borderWidget){
        borderWidget->SetBrushColor(colorBase);
        DebugHelper::logMessage("BorderInterfaceUtil::SetDefault");
    }
}

void BorderInterfaceUtil::ResetCheckedState(){
    SetChecked(false);
}

bool BorderInterfaceUtil::dispatchClick(const FVector2D &position){
    bool checked = InBound(position);
    SetChecked(checked);
    dispatchHover(position);
    return checked;
}

bool BorderInterfaceUtil::dispatchHover(const FVector2D &position){
    bool result = InBound(position);
    SetHovered(result);
    return result;
}

bool BorderInterfaceUtil::InBound(const FVector2D &position){
    ClickDispatcher dispatcher;
    return dispatcher.InBound(rawWidget, position);
}


