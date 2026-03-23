// Copyright Walter Kusnezow All Rights Reserved.
#include "WidgetBase.h"
#include "Components/PanelWidget.h"

//called on Event Dispatcher register widget! - very important for animation widget!
// inits all childs recursive!
void UWidgetBase::Init(){
    TArray<UWidgetBase *> array = TGetDirectChildren<UWidgetBase>();
    for(UWidgetBase *current : array){
        if(current){
            current->Init();
        }
    }
}

void UWidgetBase::SetVisible(bool flag){
    UWidgetBase::SetVisible(this, flag);
    bIsSetVisible = flag;
}

TArray<UWidget*> UWidgetBase::GetDirectChildren(){
    TArray<UWidget *> outArray;

    //only this panel if function is aoverriden, is a child. Dont do any
    //thing else.
    if(UPanelWidget *functionIsOverriden = FindPanelWidgetIfNotRoot()){
        TArray<UWidget *> outArrayB = GetDirectChildren(functionIsOverriden);
        for(UWidget *widget : outArrayB){
            if(widget){
                if(outArray.Contains(widget) == false){
                    outArray.Add(widget);
                }
            }
        }
        return outArray;
    }

    //get all since none is designed to be overriden!
    if(UPanelWidget *panel = Cast<UPanelWidget>(GetRootWidget())){
        outArray = GetDirectChildren(panel);
    }
    

    //OK
    //DebugHelper::logMessage("UWidgetBase::GetDirectChildren() ", outArray.Num());

    return outArray;
}


TArray<UWidget*> UWidgetBase::GetDirectChildren(UPanelWidget *panel){
    TArray<UWidget *> outArray;
    if(panel){
        for(int i = 0; i < panel->GetChildrenCount(); i++)
        {
            if(UWidget* child = panel->GetChildAt(i)){
                outArray.Add(child);
            }
        }
    }
    return outArray;
}




void UWidgetBase::TickExternal(float deltatime) {
    if(bIsSetVisible){
        TickAll(deltatime);
    }
}

void UWidgetBase::TickAll(float deltatime){
    TArray<UWidgetBase *> childs = TGetDirectChildren<UWidgetBase>();
    //DebugHelper::showScreenMessage("UWidgetBase::tick!", childs.Num());

    for(UWidgetBase *child : childs){
        if(child){
            child->TickExternal(deltatime);
        }
    }
}

void UWidgetBase::HideAll(){
    TArray<UWidgetBase *> childs = TGetDirectChildren<UWidgetBase>();
    for(UWidgetBase *child : childs){
        if(child){
            child->SetVisible(false);
        }
    }
}