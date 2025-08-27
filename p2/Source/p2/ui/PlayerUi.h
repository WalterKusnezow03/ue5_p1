// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "customUiPlugin/ui/PlayerUiBase.h"

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanel.h"
#include "p2/ui/screens/PlayerHud.h"
#include "p2/ui/screens/PauseScreen.h"
#include "p2/ui/screens/enum/EScreenEnum.h"
#include "customUiPlugin/ui/ScreenStack/ScreenOpenStack.h"
#include "p2/ui/screens/loadout/LoadoutScreen.h"
#include "p2/ui/screens/gameLaunch/GameLaunchScreen.h"

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
class P2_API UPlayerUi : public UPlayerUiBase
{
	GENERATED_BODY()

private:
	//one ui there anyway, simplifies launching the game
	//by moving away ui code from player.
	static UPlayerUi *instancePtr;  //must be inited as nullptr and cleaned on game end!

public:
	virtual void BeginDestroy() override;

	static UPlayerUi *currentInstance();

	static UPlayerUi *createNewInstance(UWorld *world);
	virtual void init(UWorld *world) override;

	UFUNCTION()
	void openPauseScreen();

	/// @brief important to override! - hides menu for play, shows HUD
	virtual void openGameScreen() override;

	/// @brief opens the loadout screen.
	UFUNCTION()
	void openLoadoutScreen();

	/// @brief opens the game launch screen
	UFUNCTION()
	void openGameLaunchScreen();

	//UFUNCTION()
	//void closeLatestScreen();

	void updateAmmunitionText(int number);
	void updateAmmunitionText(FString message);
	void updateHealthText(int health);
	void updateMissionText(FString messgae);
	void updateMissionTextTimed(FString message);


protected:
	bool screenSwitchAllowed();

	void createBasePlayerHud();
	void createPauseScreen();
	void createLoadoutScreen();
	void createGameLaunchScreen();

	virtual void showPlayerCursor(bool show) override;

	//screens
	bool pauseMenuOpened = false;
	UPlayerHud *playerHud;
	UPauseScreen *pauseScreen;
	ULoadoutScreen *loadoutScreen;
	UGameLaunchScreen *gameLaunchScreen;



};
