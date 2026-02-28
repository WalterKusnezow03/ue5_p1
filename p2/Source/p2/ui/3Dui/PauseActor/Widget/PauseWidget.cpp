#include "PauseWidget.h"
#include "customuipluginbase/Dispatcher/ClickDispatcher.h"
#include "p2/ui/3Dui/PauseActor/PauseRoomActor.h"
#include "p2/ui/3Dui/PauseActor/EPauseWidgetEvent.h"



void UPauseWidget::Tick(float DeltaTime){
    loadoutButtonUtil.Tick(DeltaTime);
    exitButtonUtil.Tick(DeltaTime);
    homeButtonUtil.Tick(DeltaTime);
}


void UPauseWidget::SetParentActor(APauseRoomActor *parent){
    parentActor = parent;
    Init();
}

void UPauseWidget::ResetParentActor(){
    parentActor = nullptr;
}

bool UPauseWidget::InBound(UWidget *widget, const FVector2D &screenPos){
    if(widget){
        ClickDispatcher dispatcher;
        if(dispatcher.InBound(widget, screenPos)){
            return true;
        }
    }
    return false;
}


bool UPauseWidget::dispatchClick(const FVector2D &pos){
    if(parentActor){

        if(loadoutButtonUtil.dispatchClick(pos)){
            parentActor->Notify(EPauseWidgetEvent::EOpenLoadoutRoom);
            return true;
        }
        if(homeButtonUtil.dispatchClick(pos)){
            parentActor->Notify(EPauseWidgetEvent::EOpenGameStartRoom);
            return true;
        }
        if(exitButtonUtil.dispatchClick(pos)){
            parentActor->Notify(EPauseWidgetEvent::EExitPauseRoom);
            return true;
        }
    }
    return false;
}

bool UPauseWidget::dispatchHover(const FVector2D &position){
    bool resultA = loadoutButtonUtil.dispatchHover(position);
    bool resultB = exitButtonUtil.dispatchHover(position);
    bool resultC = homeButtonUtil.dispatchHover(position);

    return resultA || resultB || resultC;
}

void UPauseWidget::Init(){
    SetupFromDefaultColors(loadoutButtonUtil, GetLoadoutButton());
    SetupFromDefaultColors(exitButtonUtil, GetExitButton());
    SetupFromDefaultColors(homeButtonUtil, GetHomeButton());
}

//remove hover on widget left
void UPauseWidget::removeHover(){
    loadoutButtonUtil.removeHover();
    exitButtonUtil.removeHover();
    homeButtonUtil.removeHover();
}


/*
UWidget *GetLoadoutButton();
UWidget *GetExitButton();
UWidget *GetHomeButton();
*/