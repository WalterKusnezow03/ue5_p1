// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanel.h"
#include "p2/ui/screens/PlayerHud.h"
#include "p2/ui/screens/PauseScreen.h"
#include "p2/ui/screens/enum/EScreenEnum.h"
#include "p2/ui/helper/ScreenOpenStack.h"
#include "p2/ui/screens/loadout/LoadoutScreen.h"

#include "PlayerUi.generated.h"

/**
 * will be the base class for any ui, based on a blueprint with a single canvas inside,
 * because i couldnt create it from c++ :)
 * 
 * Will be the only owner of ALL ui screens, 
 * all classes, which you can find here by value, which provide panels, etc.
 * need an instance of THIS class, to bind all ui elements to this
 * class for proper unreal garbage collection
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

	
	void openPauseScreen();

	UFUNCTION()
	void openGameScreen();

	UFUNCTION()
	void openLoadoutScreen();

	UFUNCTION()
	void closeLatestScreen();

	void updateAmmunitionText(int number);
	void updateAmmunitionText(FString message);
	void updateHealthText(int health);
	void updateMissionText(FString messgae);
	void updateMissionTextTimed(FString message);

	UCanvasPanel *canvasPanelPointer();

	void addToCanvas(UWidget *any);

	

protected:

	bool isInited = false;
	UCanvasPanel *baseCanvas = nullptr;
	

	void findBaseCanvasFromBluePrint();
	void createBasePlayerHud();
	void createPauseScreen();
	void createLoadoutScreen();

	void showPlayerCursor(bool show);

	//screens
	bool pauseMenuOpened = false;
	UPlayerHud *playerHud;
	UPauseScreen *pauseScreen;
	ULoadoutScreen *loadoutScreen;

	//todo: aktives element speichern, und alle pointer in einem vektor

	ScreenOpenStack openedScreenStack;
};
