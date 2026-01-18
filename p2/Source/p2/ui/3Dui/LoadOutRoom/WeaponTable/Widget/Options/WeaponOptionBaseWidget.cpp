#include "WeaponOptionBaseWidget.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"
#include "customuipluginbase/baseInterface/WidgetHelper.h"
#include "DebugPlugin/DebugHelper.h"


void UWeaponOptionBaseWidget::SetText(FString textIn){
    if(UWidget *found = GetTextWidget()){
        if(UTextBlock *casted = Cast<UTextBlock>(found)){
            casted->SetText(FText::FromString(textIn));
        }
    }
}


void UWeaponOptionBaseWidget::Init(){
    UpdateTextFromType();
    UpdatePadding();
    SetVisible(true);
    SetChecked(false);
}

void UWeaponOptionBaseWidget::UpdateTextFromType(){
    SetText("None");
}

void UWeaponOptionBaseWidget::UpdatePadding(){
    if(UBorder *casted = GetBorderWidgetCasted()){
        casted->SetPadding(FMargin(
            desiredPadding, 
            desiredPadding, 
            desiredPadding, 
            desiredPadding
        ));
    }
}

void UWeaponOptionBaseWidget::SetVisible(bool flag){
    WidgetHelper::SetVisible(this, flag);
}

UBorder *UWeaponOptionBaseWidget::GetBorderWidgetCasted(){
    if(UWidget *border = GetBorderWidget()){
        if(UBorder *casted = Cast<UBorder>(border)){
            return casted;
        }
    }
    return nullptr;
}

void UWeaponOptionBaseWidget::SetChecked(bool flag){
    
    if(UBorder *casted = GetBorderWidgetCasted()){
        bIsChecked = flag;
        FLinearColor color = bIsChecked ? colorChecked : colorBase;
        casted->SetBrushColor(color);
        DebugHelper::logMessage("UWeaponOptionBaseWidget Update Color");
    }
    
}