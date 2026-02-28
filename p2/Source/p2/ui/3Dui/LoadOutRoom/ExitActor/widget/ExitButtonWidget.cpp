#include "ExitButtonWidget.h"
#include "customuipluginbase/Dispatcher/ClickDispatcher.h"
#include "DebugPlugin/DebugHelper.h"

void UExitButtonWidget::SetParentActor(ALoadoutRoomExitButtonActor *parent){
    parentActorWidget = parent;
    Init();
}

bool UExitButtonWidget::dispatchClick(const FVector2D &pos){
    DebugHelper::logMessage("UExitButtonWidget::DispatchClick");

    if(borderInterface.dispatchClick(pos)){
        if(parentActorWidget){
            parentActorWidget->NotifyExit();
            return true;
        }
    }
    return false;
}

bool UExitButtonWidget::dispatchHover(const FVector2D &pos){
    return borderInterface.dispatchHover(pos);
}



void UExitButtonWidget::removeHover(){
    borderInterface.removeHover();
}

void UExitButtonWidget::Init(){
    SetupFromDefaultColors(borderInterface, GetBorderWidget());
}

void UExitButtonWidget::Tick(float deltatime){
    borderInterface.Tick(deltatime);
}