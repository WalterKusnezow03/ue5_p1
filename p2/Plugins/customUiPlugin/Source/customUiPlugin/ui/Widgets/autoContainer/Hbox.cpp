#include "Hbox.h"


//#include "Components/VerticalBox.h"

void UHbox::init(){
    initHboxIfNeeded();
}

void UHbox::initHboxIfNeeded(){
    if(baseHBox == nullptr){
        baseHBox = NewObject<UHorizontalBox>(this);
    }
}



/// @brief if inited!: adds a item to the hbox and custom visibility changes tracking
/// @param item not nullptr
void UHbox::AddChild(UcustomUiComponentBase *item){
    initHboxIfNeeded(); // if needed for init() skip fallback (not needed to call after construct)
    if(item && baseHBox){
        UWidget *baseOfItem = item->baseLayoutPointer(); //any widget must exist!
        if(baseOfItem){
            Super::AddChild(item); //track in click listeing and visibilty propagation

            //add to hbox
            //baseHBox->AddChildToHorizontalBox(baseOfItem);
            AddChild(baseOfItem);
        }
    }
}

/// @brief adds any child to the hbox: NOT Custom click listeing, no custom visibility propagation!
/// @param any 
void UHbox::AddChild(UWidget *any){
    initHboxIfNeeded(); // if needed for init() skip fallback (not needed to call after construct)
    if (any && baseHBox){
        baseHBox->AddChildToHorizontalBox(any);
    }
}



///@brief removes a child from click listening
void UHbox::RemoveChild(UcustomUiComponentBase *item){
    if(item){
        Super::RemoveChild(item);
        RemoveChild(item->baseLayoutPointer());
    }
}

///@brief must be overriden
void UHbox::RemoveChild(UWidget *any){
    if(any && baseHBox){
        baseHBox->RemoveChild(any);
    }
}