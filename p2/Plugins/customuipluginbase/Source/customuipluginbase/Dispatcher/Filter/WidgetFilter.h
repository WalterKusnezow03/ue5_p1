#pragma once

#include "CoreMinimal.h"
#include "customuipluginbase/UserInputTracking/UserInput.h"
#include "DebugPlugin/DebugHelper.h"
#include "Components/PanelWidget.h"

/// @brief testing needed!
class CUSTOMUIPLUGINBASE_API WidgetFilter {

public:
    WidgetFilter() {};
    ~WidgetFilter() {};

    //NOT Recursive
    template <typename T>
    TArray<T*> FindDirectChildrenFromPanel(UWidget *panelRaw){
        TArray<T *> castedArray;
        if (panelRaw)
        {
            if(UPanelWidget *panel = Cast<UPanelWidget>(panelRaw)){
                TArray<UWidget *> widgets = panel->GetAllChildren();
                for (int i = 0; i < widgets.Num(); i++){
                    if(UWidget *current = widgets[i]){
                        if(T *casted = Cast<T>(current)){
                            castedArray.Add(casted);
                        }
                    }
                }
            }                
        }
        return castedArray;
    }

};