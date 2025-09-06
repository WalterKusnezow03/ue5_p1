#pragma once 

#include "CoreMinimal.h"
#include "Components/Widget.h"

class CUSTOMUIPLUGIN_API WidgetHelper {

public:
    
    static void SetVisible(UWidget *any, bool visible){
        if(any != nullptr){
            //ESlateVisibility newStatus = visible ? ESlateVisibility::Visible : ESlateVisibility::Hidden;

            ESlateVisibility newStatus = visible ? ESlateVisibility::Visible : ESlateVisibility::Collapsed;
            // ESlateVisibility::Hidden;
            any->SetVisibility(newStatus);

            //test
            any->SetIsEnabled(visible);

        }
    }




};