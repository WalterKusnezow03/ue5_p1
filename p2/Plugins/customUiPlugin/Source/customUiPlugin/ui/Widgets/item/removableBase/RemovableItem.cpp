#include "RemovableItem.h"


void URemovableItem::init(){
    if(WAS_INIT_FLAG){
        return;
    }
    Super::init();
    createLayout();
    createRemoveButton();
}

bool URemovableItem::dispatchClick(){
    return baseHBox != nullptr && baseHBox->dispatchClick();
}


void URemovableItem::SetVisible(bool visible){
    if(baseHBox){
        baseHBox->SetVisible(visible);
    }
}


void URemovableItem::createLayout(){
    baseHBox = NewObject<UHbox>(this);
    baseHBox->init();

    hboxLeft = NewObject<UHbox>(this);
    hboxLeft->init();

    hboxRight = NewObject<UHbox>(this);
    hboxRight->init();

    baseHBox->AddChild(hboxLeft);
    baseHBox->AddChild(hboxRight);

    
}

void URemovableItem::createRemoveButton(){
    if(!removeButton && hboxRight){
        removeButton = NewObject<UTextButton>(this);
        removeButton->init();
        removeButton->SetText(TEXT("Remove"));
        hboxRight->AddChild(removeButton);
    }
}

void URemovableItem::AddChild(UcustomUiComponentBase *widget){
    if(widget && hboxLeft){
        hboxLeft->AddChild(widget);
    }
}

void URemovableItem::SetRemoveCallback(FSimpleDelegate delegate){
    if(removeButton){
        removeButton->SetCallBack(delegate);
    }
}