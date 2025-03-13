// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerUi.h"
#include "p2/gameStart/assetManager.h"
#include "Components/CanvasPanel.h"
#include "Components/TextBlock.h"
#include "p2/ui/makeUWidgets/TextAndImage.h"

//instance maker with init call!
UPlayerUi* UPlayerUi::createNewInstance(UWorld *world){
    if(world != nullptr){
        assetManager *pointer = assetManager::instance();
        if(pointer != nullptr){
            UClass *blueprint = pointer->uiBp();

            if(blueprint != nullptr){

                UPlayerUi* newUiInstance = CreateWidget<UPlayerUi>(world, blueprint);
                if (newUiInstance != nullptr)
                {
                    newUiInstance->init();
                    return newUiInstance;
                }
            }
        }
    }
    return nullptr;
}


void UPlayerUi::addSelfToVerticalBox(UWidget *any){
    if(any != nullptr && baseVerticalBox != nullptr){
        baseVerticalBox->AddChildToVerticalBox(any);
    }
}



//constructor like
void UPlayerUi::init(){
    if(isInited){
        return;
    }
    isInited = true;


    //the root widget must be a canvas panel in bp!
    findBaseCanvasFromBluePrint();

    // Jetzt kannst du mit dem CanvasPanel arbeiten
    createBaseBoxForCanvas();
    createNewText();
    createAmmoShower();
}

void UPlayerUi::findBaseCanvasFromBluePrint(){
    UWidget* RootWidget = GetRootWidget();
    if (RootWidget)
    {
        UCanvasPanel* CanvasPanel = Cast<UCanvasPanel>(RootWidget);
        if (CanvasPanel)
        {
            baseCanvas = CanvasPanel;
        }
    }
}

/*
void UPlayerUi::addToBaseCanvas(UWidget *widget){
    if(widget != nullptr){
        if(baseCanvas != nullptr){
            baseCanvas->AddChild(widget);
        }
    }
}*/


void UPlayerUi::createBaseBoxForCanvas(){
    if(baseCanvas != nullptr){
        // Erstelle ein UVerticalBox-Widget
        baseVerticalBox = NewObject<UVerticalBox>(this);
        if (baseVerticalBox)
        {
            // Füge das VerticalBox zum CanvasPanel hinzu
            baseCanvas->AddChild(baseVerticalBox);
        }
    }
}


//DEBUG

void UPlayerUi::createNewText(){
    if(baseVerticalBox != nullptr){
        // Erstelle ein UTextBlock-Widget als Beispiel
        UTextBlock* TextBlock1 = NewObject<UTextBlock>(this);
        if (TextBlock1)
        {
            TextBlock1->SetText(FText::FromString(TEXT("ui text")));
            baseVerticalBox->AddChildToVerticalBox(TextBlock1);
        }
    }
}




void UPlayerUi::createAmmoShower(){
    if(baseVerticalBox != nullptr){
        ammunitionTextAndImage = TextAndImage(*this);
        updateAmmunitionText(100);
        
    }
}

void UPlayerUi::updateAmmunitionText(int number){
    FString message = FString::Printf(TEXT("%d"), number);
    ammunitionTextAndImage.setText(message);
}

