#include "InteractWidget.h"
#include "p2/ui/3Dui/GamePlayWidgets/InteractWidget/InteractWidgetActor.h"

#include "customUiPlugin/slateDerived/progress/circle/WidgetProgressQuadCircular.h"


void UInteractWidget::SetParentActor(AInteractWidgetActor *parent){
    parentActorWidget = parent;
    Init();
}

void UInteractWidget::Init(){
    SetupFromDefaultColors(borderInterface, GetBorderWidget());

    float defaultTime = 3.0f;
    bool resetsItself = false;
    interactTimer.Begin(defaultTime, resetsItself);
}

bool UInteractWidget::dispatchClick(const FVector2D &pos){
    return borderInterface.dispatchClick(pos);
}

bool UInteractWidget::dispatchHover(const FVector2D &pos){
    return borderInterface.dispatchHover(pos);
    // return false;
}

//remove hover on widget left
void UInteractWidget::removeHover(){
    /*
    //TODO
    virtual bool dispatchClick(const FVector2D &position);
    bool dispatchHover(const FVector2D &position);
    
    */
    borderInterface.removeHover();
}

void UInteractWidget::Tick(float DeltaTime){
    //TODO
    TickCircleWidgetBase(DeltaTime);
    TryTickCircleProgress(DeltaTime);
}

void UInteractWidget::UpdateInteractKeyHoldState(bool holdDownTrue){
    ResetCircleProgressIfStateChanged(holdDownTrue);
    interactionKeyIsHoldDown = holdDownTrue;
}


void UInteractWidget::ResetCircleProgressIfStateChanged(bool holdDown){
    if(!holdDown){
        if(holdDown != interactionKeyIsHoldDown){
            SetCircleProgress(0.0f);

            /// ---- TODO: RESET TIME ----
            interactTimer.Reset();
        }
    }
}


//tick needed -> ticking slate widget wrapper base for resize widget operations.
void UInteractWidget::TickCircleWidgetBase(float deltaTime){
    if(UWidgetProgressQuadCircular *widget = GetProgressWidgetCasted()){
        widget->Tick(deltaTime);
    }
}



void UInteractWidget::TryTickCircleProgress(float deltatime){
    if(interactionKeyIsHoldDown){
        interactTimer.Tick(deltatime);
        SetCircleProgressFromTimer();

        if (interactTimer.timesUp())
        {

            //NOTFIY PARENT
            if(parentActorWidget){
                parentActorWidget->Notify();
            }
        }
    }
}


void UInteractWidget::SetCircleProgressFromTimer(){
    float scalar = interactTimer.scalar();
    SetCircleProgress(scalar);
}

void UInteractWidget::SetCircleProgress(float scalar){
    if(UWidgetProgressQuadCircular *circle = GetProgressWidgetCasted()){
        circle->SetProgress(scalar);
    }
}

UWidgetProgressQuadCircular *UInteractWidget::GetProgressWidgetCasted(){
    if(UWidget *raw = GetProgressWidget()){
        return Cast<UWidgetProgressQuadCircular>(raw);
    }
    return nullptr;
}