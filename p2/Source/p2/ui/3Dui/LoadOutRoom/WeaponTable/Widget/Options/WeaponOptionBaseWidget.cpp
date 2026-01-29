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
    InitBorderInterface();
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

void UWeaponOptionBaseWidget::InitBorderInterface(){
    SetupFromDefaultColors(
        borderInterface,
        GetBorderWidgetCasted()
    );
    /*
    borderInterface.Init(
        GetBorderWidgetCasted(), 
        colorBase,
        colorChecked,
        colorHover
    );*/
}



void UWeaponOptionBaseWidget::SetVisible(bool flag){
    WidgetHelper::SetVisible(this, flag);
    bMarkedVisible = flag;
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
    borderInterface.SetChecked(flag);
}


void UWeaponOptionBaseWidget::Tick(float DeltaTime){

}

bool UWeaponOptionBaseWidget::dispatchClick(const FVector2D &position){
    return borderInterface.dispatchClick(position);
}

bool UWeaponOptionBaseWidget::dispatchHover(const FVector2D &position){
    return borderInterface.dispatchHover(position);
}
