#include "GameLaunchScreen.h"

void UGameLaunchScreen::init(UPlayerUiBase &ref){
    Super::init(ref);

    createBackgroundBlurAndDefaultColor(); //Super
    createBaseLayout();
    createMenu();
}


void UGameLaunchScreen::createBaseLayout(){
    baseHbox = NewObject<UHbox>(this);

    if(baseHbox){

        //TRACK IN CUTSOM CLICK DISPATCHING AND VISIBLITY
        //OWNS ALL ELEMENTS
        baseHbox->init(); //VERY IMPORTANT
        AddChild(baseHbox);
        


    }
}

void UGameLaunchScreen::createMenu(){
    if(baseHbox){
        menuVbox = NewObject<UVbox>(this);
        if(menuVbox){
            baseHbox->AddChild(menuVbox);

            //create buttons


        }
    }
}