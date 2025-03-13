// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/VerticalBox.h"
#include "p2/ui/makeUWidgets/TextAndImage.h"
#include "p2/ui/alignmentPresets/PresetCornersLayout.h"
#include "PlayerUi.generated.h"

/**
 * will be the base class for any ui, based on a blueprint with a single canvas inside,
 * because i couldnt create it from c++ :)
 * 
 * 
 * Derivative hierachy
 * 
 * 	UObject
	└── UVisual (abstrakte Klasse, Basisklasse für Widgets)
		└── UWidget (Basisklasse aller UMG-Widgets)
			├── UPanelWidget (Basisklasse für Container-Widgets)
			│   ├── UCanvasPanel
			│   ├── UGridPanel
			│   ├── UHorizontalBox
			│   ├── UVerticalBox
			│   ├── UOverlay
			│   ├── UScrollBox
			│   ├── UWrapBox
			│   ├── USizeBox
			│   └── ...
			├── UButton
			├── UTextBlock
			├── UImage
			├── UProgressBar
			├── USlider
			├── UCheckBox
			├── UUserWidget (für komplexe UI-Logik)
			└── ...
 * 
 * 
 * 
 */

UCLASS()
class P2_API UPlayerUi : public UUserWidget
{
	GENERATED_BODY()

public:
	static UPlayerUi *createNewInstance(UWorld *world);
	virtual void init();

	

	void updateAmmunitionText(int number);
	void updateAmmunitionText(FString message);

	void updateHealthText(int health);

	UCanvasPanel *canvasPanelPointer();

protected:
	bool isInited = false;
	UCanvasPanel *baseCanvas = nullptr;

	

	void findBaseCanvasFromBluePrint();
	


	//HUD SECTION
	PresetCornersLayout playerHudCornerLayout;
	TextAndImage ammunitionTextAndImage;
	TextAndImage healthTextAndImage;

	void createBasePlayerHud();
	void createAmmunitionHudElement();
	void createHealthHudElement();


};
