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
        //debug tools enable / disable
        /*if(UWidget *widget = GetDebugOptionWidget()){
            if(USettingsOptionWidget *casted = Cast<USettingsOptionWidget>(widget)){
                if(casted->dispatchClick(pos)){
                    bool enabled = casted->IsChecked();

                    ESettingsWidgetEvent event = enabled ? 
                    ESettingsWidgetEvent::EEnableDebugTools : 
                    ESettingsWidgetEvent::EDisbaleDebugTools;
                    parentActor->Notify(event);
                }
            }
        }*/

        bool checkedDebugOption = false;
        if(dispatchClickToSettingsOption(pos, GetDebugOptionWidget(), checkedDebugOption)){
            ESettingsWidgetEvent event = checkedDebugOption ? 
            ESettingsWidgetEvent::EEnableDebugTools : 
            ESettingsWidgetEvent::EDisbaleDebugTools;
            parentActor->Notify(event);
        }

        bool checkedUVsOnWidgetsDebug = false;
        if(dispatchClickToSettingsOption(pos, GetDebugWidgetUVOptionWidget(), checkedUVsOnWidgetsDebug)){
            ESettingsWidgetEvent event = checkedUVsOnWidgetsDebug ? 
            ESettingsWidgetEvent::EEnableDebugAnyMeshWidgetUVTools : 
            ESettingsWidgetEvent::EDisbaleDebugAnyMeshWidgetUVTools;
            parentActor->Notify(event);
        }





    }
    return false;
}

///dispatches click to a usettings option widget and returns the checked state, if dispatch was
/// sucessfull.
bool USettingsWidget::dispatchClickToSettingsOption(const FVector2D &pos, UWidget *widget, bool &checkedResult){
    if(widget){
        if(USettingsOptionWidget *casted = Cast<USettingsOptionWidget>(widget)){
            if(casted->dispatchClick(pos)){
                checkedResult = casted->IsChecked();
                return true;
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

    if(UWidget *widget = GetDebugOptionWidget()){
        if(USettingsOptionWidget *casted = Cast<USettingsOptionWidget>(widget)){
            casted->Init();
            casted->SetChecked(DebugHelper::DebugToolsEnabled);
        }
    }
    if(UWidget *widget = GetDebugWidgetUVOptionWidget()){
        if(USettingsOptionWidget *casted = Cast<USettingsOptionWidget>(widget)){
            casted->Init();
            casted->SetChecked(ASharedAnyMeshWidgetComponentSettings::BShowColoredUVMap());
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
    if(UWidget *widget = GetDebugWidgetUVOptionWidget()){
        if(IBaseUiInterface *casted = Cast<IBaseUiInterface>(widget)){
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