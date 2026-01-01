#include "GameLaunchScreen.h"
#include "DebugPlugin/DebugHelper.h"
#include "AssetEnumCollection/assetEnums/textureEnum.h"

#include "customUiPlugin/ui/Widgets/buttons/subtypes/TextButton.h"
#include "customUiPlugin/ui/Widgets/autoContainer/Hbox.h"
#include "customUiPlugin/ui/Widgets/autoContainer/Vbox.h"

#include "p2/ui/screens/gameLaunch/WorldList/WorldList.h"

#include "customUiPlugin/ui/Widgets/autoContainer/sizing/FixedSizeBox.h"
#include "p2/_world/worldLevel.h"
#include "customUiPlugin/slateDerived/TextBased/TextInputSupport/WidgetSlateEditableText.h"


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
    AddDebugElements();

    DebugHelper::logMessage("UGameLaunchScreen init");
}

void UGameLaunchScreen::createTopBar(){
    topHbox = NewWidgetInitialized<UHbox>(this);

    if(topHbox){
        AddChild(topHbox);
        UTextBlock *text = NewObject<UTextBlock>(this);
        text->SetText(FText::FromString(TEXT("Game Launch Screen")));
        topHbox->AddChild(text);
    }
}

void UGameLaunchScreen::createMenu(){
    if(!menuVbox){
        menuVbox = NewWidgetInitialized<UVbox>(this);
        if (menuVbox)
        {
            //add to canvas
            AddChild(
                menuVbox,
                FVector2D(0.5f, 0.1f), // pivot locationScalar
                FVector2D(0.5f, 0.0f)  // privot weight scalar
            );                     // to top center, align center

            createTypeFieldForWorldCreation();
            menuVbox->CreateSpacer(30);
            createWorldList();
        }
    }
}

void UGameLaunchScreen::createTypeFieldForWorldCreation(){
    if(!menuVbox){
        //must be created at this point
        return;
    }

    //create info text

    //scaling
    int UniFormButtonScale = HeightTopBar;
    int WidthTypeBox = WidthOfLayout - UniFormButtonScale;
    FVector2D resTypeBox(WidthTypeBox, HeightTopBar);
    FVector2D resTypeButton(UniFormButtonScale, UniFormButtonScale);

    //create hbox containing it
    createBarHbox = NewWidgetInitialized<UHbox>(this);
    menuVbox->AddChild(createBarHbox);

    //type field 
    typeFieldWorld = NewObject<UWidgetSlateEditableText>(this);
    typeFieldWorld->SetHintText("Create new world");
    typeFieldWorld->SetResolution(resTypeBox);

    typeFieldConfirmButton = NewObject<UWidgetImageExtended>(this);
    typeFieldConfirmButton->SetImage(textureEnum::healthIcon, FVector2D(50, 50));
    typeFieldConfirmButton->SetResolution(resTypeButton);

    createBarHbox->AddChild((IBaseUiInterface*) typeFieldWorld);
    createBarHbox->AddChild((IBaseUiInterface*) typeFieldConfirmButton);
}

void UGameLaunchScreen::createWorldList(){
    //world list
    worldListPicker = NewObject<UWorldList>(this);
    worldListPicker->init(this);
    worldListPicker->SetWidth(WidthOfLayout);
    menuVbox->AddChild(worldListPicker);
}





bool UGameLaunchScreen::dispatchClick(){
    bool result = Super::dispatchClick();
    bool resultB = false;
    if (typeFieldConfirmButton)
    {
        if(typeFieldConfirmButton->dispatchClick()){
            createAndLaunchWorldFromTypeField();
            resultB = true;
        }
    }
    return result || resultB;
}

void UGameLaunchScreen::createAndLaunchWorldFromTypeField(){
    DebugHelper::logMessage("UGameLaunchScreen try create world");
    if (worldListPicker && typeFieldWorld)
    {
        FString name = typeFieldWorld->GetText();
        if(name.Len() <= 0){
            //Invalid
            DebugHelper::showScreenMessage("world name invalid!");
            return;
        }

        DebugHelper::logMessage("UGameLaunchScreen try create world: ", name);
        if(worldListPicker->CanAddWorld(name)){
            worldListPicker->AddWorld(name); //update ui, save name to storage
            launchWorld(name);
        }else{
            DebugHelper::logMessage("UGameLaunchScreen could not create new world. Exsisting!");
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






// ----- debug ------
#include "p2/ui/algorithm/DebugUi/GreedyFitWidget.h"
#include "customUiPlugin/ui/Widgets/autoContainer/DropDownBase/VerticalDropDownBase.h"
#include "customUiPlugin/slateDerived/TextBased/TextInputSupport/WidgetSlateEditableText.h"
void UGameLaunchScreen::AddDebugElements(){
    if(!menuVbox){
        return;
    }

    //return;
    //menuVbox->CreateSpacer(200);
    

    //debug custom slate widgets
    if(false){
        UWidgetSlateWrapperBase *widget = NewObject<UWidgetSlateWrapperBase>(this);
        menuVbox->AddChild((IBaseUiInterface*) widget);
    }
    if(false){
        UWidgetProgressBarBase *p = NewObject<UWidgetProgressBarBase>(this);
        menuVbox->AddChild((IBaseUiInterface*) p);
    }
    if(false){
        UGreedyFitWidget *widget = NewObject<UGreedyFitWidget>(this);
        DebugHelper::logMessage("created greedy fit widget");
        menuVbox->AddChild((IBaseUiInterface *)widget);
    }
    

    if(false){
        UVerticalDropDownBase *dropDownDebug = NewWidgetInitialized<UVerticalDropDownBase>(this);
        menuVbox->AddChild(dropDownDebug);
        dropDownDebug->Debug();
    }

    if(false){
        UWidgetSlateEditableText *editable = NewObject<UWidgetSlateEditableText>(this);
        menuVbox->AddChild((IBaseUiInterface*) editable);
    }
}