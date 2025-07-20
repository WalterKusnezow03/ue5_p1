#include "Vbox.h"

void UVbox::init(){
    initVboxIfNeeded();
}

void UVbox::initVboxIfNeeded(){
    if(baseVBox == nullptr){
        baseVBox = NewObject<UVerticalBox>(this);
    }
}



/// @brief if inited!: adds a item to the vbox and custom visibility changes tracking
/// @param item not nullptr
void UVbox::AddChild(UcustomUiComponentBase *item){
    initVboxIfNeeded(); //if needed for init() skip fallback (not needed to call after construct)
    if(item && baseVBox){
        UWidget *baseOfItem = item->baseLayoutPointer(); //any widget must exist!
        if(baseOfItem){
            Super::AddChild(item); //track in click listeing and visibilty propagation

            //add to hbox
            //baseVBox->AddChildToVerticalBox(baseOfItem);
            AddChild(baseOfItem);
        }
    }
}

/// @brief adds any child to the vbox: NOT Custom click listeing, no custom visibility propagation!
/// @param any 
void UVbox::AddChild(UWidget *any){
    initVboxIfNeeded(); //if needed for init() skip fallback (not needed to call after construct)
    if(any && baseVBox){
        baseVBox->AddChildToVerticalBox(any);
    }
}