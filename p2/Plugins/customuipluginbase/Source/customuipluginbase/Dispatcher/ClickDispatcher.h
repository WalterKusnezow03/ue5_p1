#pragma once

#include "CoreMinimal.h"
#include "customuipluginbase/UserInputTracking/UserInput.h"
#include "DebugPlugin/DebugHelper.h"
#include "Components/PanelWidget.h"

/// @brief testing needed!
class CUSTOMUIPLUGINBASE_API ClickDispatcher {

public:
    ClickDispatcher() {};
    ~ClickDispatcher() {};

    bool Dispatch(UWidget *widget);
    bool Dispatch(UWidget *widget, const FVector2D &mousePos);

    //is recursive!
    UWidget* DispatchWithResult(UWidget *widget, const FVector2D &mousePos);

    //NOT Recursive
    template <typename T>
    T* DispatchWithResultFromPanel(UWidget *panelRaw, const FVector2D &mousePos){
        if (panelRaw)
        {
            if(UPanelWidget *panel = Cast<UPanelWidget>(panelRaw)){
                TArray<UWidget *> widgets = panel->GetAllChildren();
                for (int i = 0; i < widgets.Num(); i++){
                    if(UWidget *current = widgets[i]){
                        if(T *casted = Cast<T>(current)){
                            if(InBound(current, mousePos)){
                                return casted;
                            }
                        }
                    }
                }
            }                
        }
        return nullptr;
    }

    bool InBound(UWidget *widget, const FVector2D &mousePos);

private:
    bool Dispatch(TArray<UWidget *> &widgets, const FVector2D &mousePos);
    UWidget* DispatchWithResult(TArray<UWidget *> &widgets, const FVector2D &mousePos);
    bool IsVisible(UWidget *widget);
};