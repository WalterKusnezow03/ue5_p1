#include "HudUiWidget.h"
#include "Components/TextBlock.h"


void UHudUiWidget::UpdateWidget(FPlayerStatus &playerStatus){

    UpdateHealthText(playerStatus.HealthString());
    UpdateAmmunitionText(playerStatus.AmmunitionString());

}

void UHudUiWidget::UpdateHealthText(FString text){
    UpdateHealthText(FText::FromString(*text));
}

void UHudUiWidget::UpdateHealthText(FText text){
    if(UWidget *widget = GetWidgetHealthText()){
        if(UTextBlock *casted = Cast<UTextBlock>(widget)){
            casted->SetText(text);
        }
    }
}

void UHudUiWidget::UpdateAmmunitionText(FString text){
    UpdateAmmunitionText(FText::FromString(*text));
}

void UHudUiWidget::UpdateAmmunitionText(FText text){
    if(UWidget *widget = GetWidgetAmmunitionText()){
        if(UTextBlock *casted = Cast<UTextBlock>(widget)){
            casted->SetText(text);
        }
    }
}


