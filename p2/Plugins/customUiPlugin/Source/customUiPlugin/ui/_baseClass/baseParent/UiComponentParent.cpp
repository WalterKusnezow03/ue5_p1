#include "UiComponentParent.h"


/// @brief added child AUTOMATICALLY added to click and visibilty and Tick DISPATCH!
void UUiComponentParent::AddChild(IBaseUiInterface *item){
    if(CanAddChild(item)){
        trackedChildsInterface.Add(item);

        if(UWidget *widget = item->baseLayoutPointer()){
            trackedChildsAsUWidget.Add(widget);
        }
    }
}


bool UUiComponentParent::CanAddChild(IBaseUiInterface *item){
    return item != nullptr && !trackedChildsInterface.Contains(item);
}

void UUiComponentParent::Tick(float deltatime){
    for (int i = 0; i < trackedChildsInterface.Num(); i++){
        if(IBaseUiInterface *current = trackedChildsInterface[i]){
            current->Tick(deltatime);
        }
    }
}
bool UUiComponentParent::dispatchClick(){
    int ignored = 0;
    return dispatchClick(ignored);
}

bool UUiComponentParent::dispatchClick(int &outIndexFirst){
    bool flag = false;
    for (int i = 0; i < trackedChildsInterface.Num(); i++)
    {
        if(IBaseUiInterface *current = trackedChildsInterface[i]){
            if(current->dispatchClick()){
                if(!flag){
                    outIndexFirst = i;
                }
                flag = true;
            }
        }
    }
    return flag;
}




void UUiComponentParent::SetVisible(bool visible){
    for (int i = 0; i < trackedChildsInterface.Num(); i++){
        if(IBaseUiInterface *current = trackedChildsInterface[i]){
            current->SetVisible(visible);
        }
    }
    Super::SetVisible(visible); //hides self base layout pointer if has one.
}


void UUiComponentParent::RemoveChild(IBaseUiInterface *item){
    if(item){
        RemoveChildSwapPopBack(item);
        RemoveChildSwapPopBack(item->baseLayoutPointer());

        /*
        //remove from interface array
        int32 Index = -1;
        if (trackedChildsInterface.Find(item, Index)){
            //swap with back, pop
            int32 lastIndex = trackedChildsInterface.Num() - 1;
            trackedChildsInterface[Index] = trackedChildsInterface[lastIndex];
            trackedChildsInterface.Pop();
        }

        //remove from UWidget array
        if(UWidget *widget = item->baseLayoutPointer()){
            Index = -1;
            if (trackedChildsAsUWidget.Find(widget, Index))
            {
                //swap with back, pop
                int32 lastIndex = trackedChildsAsUWidget.Num() - 1;
                trackedChildsAsUWidget[Index] = trackedChildsAsUWidget[lastIndex];
                trackedChildsAsUWidget.Pop();
            }
        }*/
    }
}

void UUiComponentParent::RemoveChildSwapPopBack(IBaseUiInterface *item){
    if(!item){
        return;
    }
    
    //remove from interface array
    int32 Index = -1;
    if (trackedChildsInterface.Find(item, Index)){ //remove from tick interfaces.
        //swap with back, pop
        int32 lastIndex = trackedChildsInterface.Num() - 1;
        trackedChildsInterface[Index] = trackedChildsInterface[lastIndex];
        trackedChildsInterface.Pop();
    }
}

void UUiComponentParent::RemoveChildSwapPopBack(UWidget *widget){
    if(!widget){
        return;
    }

    int32 Index = -1;
    if (trackedChildsAsUWidget.Find(widget, Index)) //remove from tracked uwidgets.
    {
        //swap with back, pop
        int32 lastIndex = trackedChildsAsUWidget.Num() - 1;
        trackedChildsAsUWidget[Index] = trackedChildsAsUWidget[lastIndex];
        trackedChildsAsUWidget.Pop();
    }
}


void UUiComponentParent::RemoveChild(UWidget *item){
    //remove from interface array
    int32 index = -1;
    IBaseUiInterface *foundParent = nullptr;
    for (int i = 0; i < trackedChildsInterface.Num(); i++)
    {
        IBaseUiInterface *current = trackedChildsInterface[i];
        if(current){
            if(UWidget *base = current->baseLayoutPointer()){
                if(base == item){
                    index = i;
                    foundParent = current;
                    i = trackedChildsInterface.Num();
                }
            }
        }
    }
    //remove parent interface if found
    RemoveChildSwapPopBack(foundParent);

    // remove from UWidget array
    RemoveChildSwapPopBack(item);
}


/// @brief returns nullptr if index invalid!
/// @param i 
/// @return 
IBaseUiInterface *UUiComponentParent::BaseInterfaceAtIndex(int i){
    if(i >= 0 && i < trackedChildsInterface.Num()){
        return trackedChildsInterface[i];
    }
    return nullptr;
}

UWidget *UUiComponentParent::UWidgetAtIndex(int i){
    if(i >= 0 && i < trackedChildsAsUWidget.Num()){
        return trackedChildsAsUWidget[i];
    }
    return nullptr;
}


/// @brief use only for very special cases where this data is needed!
const TArray<IBaseUiInterface *> &UUiComponentParent::AccessInternalItemsTmp() const{
    return trackedChildsInterface;
}