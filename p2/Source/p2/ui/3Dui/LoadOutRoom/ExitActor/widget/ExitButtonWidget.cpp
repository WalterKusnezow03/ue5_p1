#include "ExitButtonWidget.h"
#include "customuipluginbase/Dispatcher/ClickDispatcher.h"
#include "DebugPlugin/DebugHelper.h"

void UExitButtonWidget::SetParentActor(ALoadoutRoomExitButtonActor *parent){
    parentActorWidget = parent;
}

bool UExitButtonWidget::dispatchClick(const FVector2D &pos){
    DebugHelper::logMessage("UExitButtonWidget::DispatchClick");

    ClickDispatcher dispatcher;
    if(dispatcher.InBound(this, pos)){
        if(parentActorWidget){
            parentActorWidget->NotifyExit();
        }
        return true;
    }
    return false;
}
