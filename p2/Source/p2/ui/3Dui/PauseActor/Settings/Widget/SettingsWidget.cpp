#include "SettingsWidget.h"
#include "customuipluginbase/Dispatcher/ClickDispatcher.h"
#include "p2/ui/3Dui/PauseActor/PauseRoomActor.h"
#include "p2/ui/3Dui/PauseActor/EPauseWidgetEvent.h"

#include "p2/ui/3Dui/PauseActor/Settings/Widget/Option/SettingsOptionWidget.h"


void USettingsWidget::Tick(float DeltaTime){
    exitButtonUtil.Tick(DeltaTime);
    TArray<IBaseUiInterface *> array = GetAllItemsForDispatch();
    for (int i = 0; i < array.Num(); i++){
        if(IBaseUiInterface *current = array[i]){
            current->Tick(DeltaTime);
        }
    }
}

void USettingsWidget::SetParentActor(APauseRoomActor *parent){
    parentActor = parent;
    Init();
}

void USettingsWidget::ResetParentActor(){
    parentActor = nullptr;
}

bool USettingsWidget::InBound(UWidget *widget, const FVector2D &screenPos){
    if(widget){
        ClickDispatcher dispatcher;
        if(dispatcher.InBound(widget, screenPos)){
            return true;
        }
    }
    return false;
}


bool USettingsWidget::dispatchClick(const FVector2D &pos){
    if(parentActor){
        if(exitButtonUtil.dispatchClick(pos)){
            parentActor->Notify(EPauseWidgetEvent::ECloseSettingsWidget);
            return true;
        }
        if(UWidget *widget = GetDebugOptionWidget()){
            if(USettingsOptionWidget *casted = Cast<USettingsOptionWidget>(widget)){
                if(casted->dispatchClick(pos)){
                    bool enabled = casted->IsChecked();

                    ESettingsWidgetEvent event = enabled ? 
                    ESettingsWidgetEvent::EEnableDebugTools : 
                    ESettingsWidgetEvent::EDisbaleDebugTools;
                    parentActor->Notify(event);
                }
            }
        }
    }
    return false;
}

bool USettingsWidget::dispatchHover(const FVector2D &position){
    
    bool resultB = exitButtonUtil.dispatchHover(position);
    bool resultA = DispatchHoverToAllItemsForDispatch(position);

    return resultA || resultB;
}

void USettingsWidget::Init(){
    SetupFromDefaultColors(exitButtonUtil, GetExitButton());
    InitAllOptions();
}

void USettingsWidget::InitAllOptions(){
    TArray<USettingsOptionWidget *> items = GetAllSettingsOptions();
    for (int i = 0; i < items.Num(); i++){
        if(USettingsOptionWidget *current = items[i]){
            current->Init();
        }
    }
}

//remove hover on widget left
void USettingsWidget::removeHover(){
    
    exitButtonUtil.removeHover();
    RemoveHoverFromAllItemsForDispatch();
}

TArray<IBaseUiInterface *> USettingsWidget::GetAllItemsForDispatch(){
    TArray<IBaseUiInterface *> array;
    if(UWidget *widget = GetDebugOptionWidget()){
        if(IBaseUiInterface *casted = Cast<IBaseUiInterface>(widget)){
            array.Add(casted);
        }
    }
    return array;
}

TArray<USettingsOptionWidget *> USettingsWidget::GetAllSettingsOptions(){
    TArray<USettingsOptionWidget *> array;
    if(UWidget *widget = GetDebugOptionWidget()){
        if(USettingsOptionWidget *casted = Cast<USettingsOptionWidget>(widget)){
            array.Add(casted);
        }
    }
    return array;
}



/*
UWidget *GetLoadoutButton();
UWidget *GetExitButton();
UWidget *GetHomeButton();
*/