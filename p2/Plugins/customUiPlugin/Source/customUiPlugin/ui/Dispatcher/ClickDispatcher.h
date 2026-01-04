#pragma once

#include "CoreMinimal.h"
#include "customuipluginbase/UserInputTracking/UserInput.h"

/// @brief testing needed!
class CUSTOMUIPLUGIN_API ClickDispatcher {

public:
    ClickDispatcher() {};
    ~ClickDispatcher() {};

    bool Dispatch(UWidget *widget);
    bool Dispatch(UWidget *widget, FVector2D &mousePos);

    UWidget* DispatchWithResult(UWidget *widget, FVector2D &mousePos);


private:
    bool Dispatch(TArray<UWidget *> &widgets, FVector2D &mousePos);
    UWidget* DispatchWithResult(TArray<UWidget *> &widgets, FVector2D &mousePos);
    bool InBound(UWidget *widget, FVector2D &mousePos);
    bool IsVisible(UWidget *widget);
};