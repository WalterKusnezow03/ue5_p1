#include "SettingsOptionWidget.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"
#include "customuipluginbase/baseInterface/WidgetHelper.h"
#include "DebugPlugin/DebugHelper.h"


void USettingsOptionWidget::SetText(FString textIn){
    if(UWidget *found = GetTextWidget()){
        if(UTextBlock *casted = Cast<UTextBlock>(found)){
            casted->SetText(FText::FromString(textIn));
        }
    }
}


void USettingsOptionWidget::Init(){
    UpdatePadding();
    InitBorderInterface();

    //copy static state
    SetChecked(DebugHelper::DebugToolsEnabled);
}

void USettingsOptionWidget::UpdatePadding(){
    if(UBorder *casted = GetBorderWidgetCastedEnabled()){
        casted->SetPadding(FMargin(
            desiredPadding, 
            desiredPadding, 
            desiredPadding, 
            desiredPadding
        ));
    }
    if(UBorder *casted = GetBorderWidgetCastedDisabled()){
        casted->SetPadding(FMargin(
            desiredPadding, 
            desiredPadding, 
            desiredPadding, 
            desiredPadding
        ));
    }
}

void USettingsOptionWidget::InitBorderInterface(){
    SetupFromDefaultColors(
        borderInterfaceEnabled,
        GetBorderWidgetCastedEnabled()
    );
    SetupFromDefaultColors(
        borderInterfaceDisabled,
        GetBorderWidgetCastedDisabled()
    );
    /*
    borderInterface.Init(
        GetBorderWidgetCasted(), 
        colorBase,
        colorChecked,
        colorHover
    );*/
}



void USettingsOptionWidget::SetVisible(bool flag){
    WidgetHelper::SetVisible(this, flag);
    bMarkedVisible = flag;
}

UBorder *USettingsOptionWidget::GetBorderWidgetCastedEnabled(){
    if(UWidget *border = GetBorderWidgetEnabled()){
        if(UBorder *casted = Cast<UBorder>(border)){
            return casted;
        }
    }
    return nullptr;
}

UBorder *USettingsOptionWidget::GetBorderWidgetCastedDisabled(){
    if(UWidget *border = GetBorderWidgetDisabled()){
        if(UBorder *casted = Cast<UBorder>(border)){
            return casted;
        }
    }
    return nullptr;
}




void USettingsOptionWidget::SetChecked(bool flag){
    borderInterfaceEnabled.SetChecked(flag);
    borderInterfaceDisabled.SetChecked(!flag);
    bIsChecked = flag;
}

void USettingsOptionWidget::Tick(float DeltaTime){
    
}

bool USettingsOptionWidget::dispatchClick(const FVector2D &position){
    if(borderInterfaceEnabled.dispatchClick(position)){
        SetChecked(true);
        return true;
    }
    if(borderInterfaceDisabled.dispatchClick(position)){
        SetChecked(false);
        return true;
    }
    return false;
}

bool USettingsOptionWidget::dispatchHover(const FVector2D &position){
    bool b1 = borderInterfaceEnabled.dispatchHover(position);
    bool b2 = borderInterfaceDisabled.dispatchHover(position);
    return b1 || b2;
}

//remove hover on widget left
void USettingsOptionWidget::removeHover(){
    borderInterfaceEnabled.removeHover();
    borderInterfaceDisabled.removeHover();
}