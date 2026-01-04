#include "ClickDispatcher.h"
#include "customuipluginbase/baseInterface/BaseUiInterface.h"



UWidget* ClickDispatcher::DispatchWithResult(UWidget *widget, FVector2D &mousePos){
    if(widget){
        if(widget){
            DebugHelper::logMessage("ClickDispatcher:: Try Dispatch B");
            if(IsVisible(widget)){
                DebugHelper::logMessage("ClickDispatcher:: Try Dispatch C");
                if(InBound(widget, mousePos)){
                    //has children
                    bool found = false;
                    UWidget *foundWidget = nullptr;
                    if (UPanelWidget *panel = Cast<UPanelWidget>(widget))
                    {
                        TArray<UWidget *> widgets = panel->GetAllChildren();
                        if(UWidget *result = DispatchWithResult(widgets, mousePos)){
                            found = true;
                            foundWidget = result;
                        }
                    }

                    //dispatch since no child hit
                    if(!found){
                        if(IBaseUiInterface *casted = Cast<IBaseUiInterface>(widget)){
                            casted->dispatchClick(mousePos);
                            DebugHelper::logMessage("ClickDispatcher:: Self Dispatch");
                            return widget;
                        }
                    }else{
                        return foundWidget;
                    }
                }
            }
        }
    }
    return nullptr;
}

















bool ClickDispatcher::Dispatch(UWidget *widget){
    FVector2D mousePos = FSlateApplication::Get().GetCursorPos();
    return Dispatch(widget, mousePos);
}

bool ClickDispatcher::Dispatch(UWidget *widget, FVector2D &mousePos){
    DebugHelper::logMessage("ClickDispatcher:: Try Dispatch A");

    return DispatchWithResult(widget, mousePos) != nullptr;
}

bool ClickDispatcher::IsVisible(UWidget *widget){
    return widget && widget->GetVisibility() == ESlateVisibility::Visible;
}



UWidget* ClickDispatcher::DispatchWithResult(TArray<UWidget *> &widgets, FVector2D &mousePos){
    UWidget *result = nullptr;
    for (int i = 0; i < widgets.Num(); i++)
    {
        UWidget *current = widgets[i];
        if(UWidget *foundWidget = DispatchWithResult(current, mousePos)){
            result = foundWidget;
        }
    }
    return result;
}

bool ClickDispatcher::Dispatch(TArray<UWidget*> &widgets, FVector2D &mousePos){

    return DispatchWithResult(widgets, mousePos) != nullptr;
}


bool ClickDispatcher::InBound(UWidget* widget, FVector2D &mousePos){
    if(widget){
        const FGeometry& Geo = widget->GetCachedGeometry();

        FVector2D LocalPos = Geo.AbsoluteToLocal(mousePos);

        if (LocalPos.X >= 0 && LocalPos.Y >= 0 &&
            LocalPos.X <= Geo.GetLocalSize().X &&
            LocalPos.Y <= Geo.GetLocalSize().Y)
        {
            // Hit

            return true;
        }
    }
    return false;
}