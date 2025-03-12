// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerUi.h"
#include "p2/gameStart/assetManager.h"
#include "Components/CanvasPanel.h"
#include "Components/TextBlock.h"

//instance maker with init!
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



//constructor like
void UPlayerUi::init(){
    if(isInited){
        return;
    }
    isInited = true;


    // Hole das Root-Widget (CanvasPanel oder andere Panel-Elemente)
    UWidget* RootWidget = GetRootWidget();
    if (RootWidget)
    {
        UCanvasPanel* CanvasPanel = Cast<UCanvasPanel>(RootWidget);
        if (CanvasPanel)
        {
            baseCanvas = CanvasPanel;
            // Jetzt kannst du mit dem CanvasPanel arbeiten
            createBaseBoxForCanvas();
            createNewText();
        }
    }
}

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


void UPlayerUi::createNewText(){
    if(baseVerticalBox != nullptr){
        // Erstelle ein UTextBlock-Widget als Beispiel
        UTextBlock* TextBlock1 = NewObject<UTextBlock>(this);
        if (TextBlock1)
        {
            TextBlock1->SetText(FText::FromString(TEXT("Text 1")));
            baseVerticalBox->AddChildToVerticalBox(TextBlock1);
        }
    }
}
