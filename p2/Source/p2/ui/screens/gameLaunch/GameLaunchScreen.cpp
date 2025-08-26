#include "GameLaunchScreen.h"
#include "GameCore/DebugHelper.h"

void UGameLaunchScreen::init(UPlayerUiBase &ref){
    if(WAS_INIT_FLAG){
        return;
    }
    Super::init(ref);

    createBackgroundBlurAndDefaultColor(); //Super
    createTopBar();
    createMenu();

    DebugHelper::logMessage("UGameLaunchScreen init");
}

void UGameLaunchScreen::createTopBar(){
    topHbox = NewObject<UHbox>(this);

    if(topHbox){

        //TRACK IN CUTSOM CLICK DISPATCHING AND VISIBLITY
        //OWNS ALL ELEMENTS
        topHbox->init(); //VERY IMPORTANT
        AddChild(topHbox);

        UTextBlock *text = NewObject<UTextBlock>(this);

        text->SetText(FText::FromString(TEXT("Game Launch Screen")));
        topHbox->AddChild(text);
    }
}

void UGameLaunchScreen::createMenu(){



    //new method with pivot and gravity
    /*
    void UCanvasScreen::AddChild(
        UcustomUiComponentBase *item,
        FVector2D screenAnchor, //corner top left (0,0), bottom right (1,1)
        FVector2D alignment //gravity / pivot of item (0,0.5), make centered on y
    )*/
    
    menuVbox = NewObject<UVbox>(this);
    menuVbox->init();
    if (menuVbox)
    {
        //baseHbox->AddChild(menuVbox);
        AddChild(menuVbox, FVector2D(0.5f, 0.0f), FVector2D(0.5f, 0.0f)); //to top center, align center

        //create textbox for new world.

        //world list
        worldListPicker = NewObject<UWorldList>(this);
        worldListPicker->init(this);
        menuVbox->AddChild(worldListPicker);
    }
    
}


void UGameLaunchScreen::launchWorld(FString world){
    FString message = FString::Printf(TEXT("UGameLaunchScreen Launch World: %s"), *world);

    DebugHelper::logMessage(message);
    DebugHelper::showScreenMessage(message, FColor::Green);
}