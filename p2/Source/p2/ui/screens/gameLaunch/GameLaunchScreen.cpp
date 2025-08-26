#include "GameLaunchScreen.h"
#include "GameCore/DebugHelper.h"

void UGameLaunchScreen::init(UPlayerUiBase &ref){
    if(WAS_INIT_FLAG){
        return;
    }
    Super::init(ref);

    createBackgroundBlurAndDefaultColor(); //Super
    createBaseLayout();
    createMenu();

    DebugHelper::logMessage("UGameLaunchScreen init");
}

void UGameLaunchScreen::createBaseLayout(){
    baseHbox = NewObject<UHbox>(this);

    if(baseHbox){

        //TRACK IN CUTSOM CLICK DISPATCHING AND VISIBLITY
        //OWNS ALL ELEMENTS
        baseHbox->init(); //VERY IMPORTANT
        AddChild(baseHbox);

        UTextBlock *text = NewObject<UTextBlock>(this);

        text->SetText(FText::FromString(TEXT("Game Launch Screen")));
        baseHbox->AddChild(text);
    }
}

void UGameLaunchScreen::createMenu(){
    if(baseHbox){
        menuVbox = NewObject<UVbox>(this);
        menuVbox->init();
        if (menuVbox)
        {
            baseHbox->AddChild(menuVbox);

            //create textbox for new world.

            //world list
            worldListPicker = NewObject<UWorldList>(this);
            worldListPicker->init(this);
            menuVbox->AddChild(worldListPicker);
        }
    }
}


void UGameLaunchScreen::launchWorld(FString world){
    FString message = FString::Printf(TEXT("UGameLaunchScreen Launch World: %s"), *world);

    DebugHelper::logMessage(message);
    DebugHelper::showScreenMessage(message, FColor::Green);
}