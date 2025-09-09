#include "GameLaunchScreen.h"
#include "GameCore/DebugHelper.h"
#include "AssetPlugin/gamestart/assetEnums/textureEnum.h"

#include "customUiPlugin/ui/Widgets/buttons/subtypes/TextButton.h"
#include "customUiPlugin/ui/Widgets/autoContainer/Hbox.h"
#include "customUiPlugin/ui/Widgets/autoContainer/Vbox.h"

#include "p2/ui/Widgets/buttons/subtypes/ImageOverlayedButton.h"
#include "p2/ui/screens/gameLaunch/WorldList/WorldList.h"

#include "customUiPlugin/ui/Widgets/autoContainer/sizing/FixedSizeBox.h"
#include "p2/_world/worldLevel.h"

//debug
#include "customUiPlugin/slateDerived/bar/WidgetProgressBarBase.h"

#include "customUiPlugin/slate/UWidgetConversion/UWigetBase/WidgetSlateWrapperBase.h"



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
        
        AddChild(
            menuVbox,
            FVector2D(0.5f, 0.1f), // pivot locationScalar
            FVector2D(0.5f, 0.0f)  // privot weight scalar
        );                     // to top center, align center

        createTypeFieldForWorldCreation();

        //world list
        worldListPicker = NewObject<UWorldList>(this);
        worldListPicker->init(this);
        menuVbox->AddChild(worldListPicker);


        FVector2D pad(10, 10);
        menuVbox->SetPadding(pad);
        
        //menuVbox->SetItemsFillHorizontal();



        //debug custom slate widgets
        if(true){
            


            UWidgetSlateWrapperBase *widget = NewObject<UWidgetSlateWrapperBase>(this);
            menuVbox->AddChild((IBaseUiInterface*) widget);
        }
        

        if(true){

            UWidgetProgressBarBase *p = NewObject<UWidgetProgressBarBase>(this);
            menuVbox->AddChild((IBaseUiInterface*) p);

        }

        
    }
    
}

void UGameLaunchScreen::createTypeFieldForWorldCreation(){
    if(!menuVbox){
        //must be created at this point
        return;
    }

    //create textbox for new world.

    //hbox containing it
    createBarHbox = NewObject<UHbox>(this);
    createBarHbox->init();
    menuVbox->AddChild(createBarHbox);

    //type field 
    typeFieldWorld = NewObject<UTextBoxBase>(this);
    typeFieldWorld->init();
    typeFieldWorld->SetHintText("Type a World Name");
    createBarHbox->AddChild(typeFieldWorld);

    typeFieldConfirmButton = NewObject<UImageOverlayedButton>(this);
    typeFieldConfirmButton->init();
    typeFieldConfirmButton->setImage(textureEnum::healthIcon);
    typeFieldConfirmButton->SetCallBack(
        FSimpleDelegate::CreateUObject(this, &UGameLaunchScreen::createAndLaunchWorldFromTypeField)
    );
    createBarHbox->AddChild(typeFieldConfirmButton);

    //FSimpleDelegate::CreateUObject(uclassInstance*, &<classname>::<methodname>)
    /*FSimpleDelegate::CreateLambda([uclassInstance*, SomeValue]()
    {
        uclassInstance->SomeUFunction(SomeValue);
    }*/

}





void UGameLaunchScreen::createAndLaunchWorldFromTypeField(){
    if(worldListPicker && typeFieldWorld){
        FString name = typeFieldWorld->GetText();
        if(worldListPicker->CanAddWorld(name)){
            worldListPicker->AddWorld(name); //update ui, save name to storage
            launchWorld(name);
        }
    }
}

void UGameLaunchScreen::launchWorld(FString worldName){
    FString message = FString::Printf(TEXT("UGameLaunchScreen Launch World: %s"), *worldName);

    DebugHelper::logMessage(message);
    DebugHelper::showScreenMessage(message, FColor::Purple);


    //NEW: using instance ptr, -> objective: remove any parent references
    //as its not needed. Try to refacture for ui base must be instances 
    //to create ANY Widgets.
    
    if(UPlayerUi *instance = UPlayerUi::currentInstance()){
        AworldLevel::gameStateManager.UpdateGameState(EGameState::EGamePlay);

        AworldLevel::initWorld(worldName);

        //instance->openGameScreen(); //close all
    }
}