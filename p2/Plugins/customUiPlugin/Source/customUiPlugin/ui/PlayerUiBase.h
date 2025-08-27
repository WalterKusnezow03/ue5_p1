// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanel.h"

#include "customUiPlugin/ui/ScreenStack/ScreenOpenStack.h"

#include "PlayerUiBase.generated.h"

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
class CUSTOMUIPLUGIN_API UPlayerUiBase : public UUserWidget
{
	GENERATED_BODY()

public:

	/// @brief must be overriden but called Super, to add anything to the viewport!
	/// @param world 
	virtual void init(UWorld *world);

	UFUNCTION()
	void closeLatestScreen();


	UCanvasPanel *canvasPanelPointer();

	void addToCanvas(UWidget *any);

	/// @brief TICK THIS FOR MANUAL CLICK DISPATCHER
	void updateClickDispatch();

    /// @brief designed to be overriden to hide the ui / show player HUD
    UFUNCTION()
    virtual void openGameScreen();

    void Tick(float deltatime);

protected:

    void RegisterCursorClick();

	bool isInited = false;
	UCanvasPanel *baseCanvas = nullptr;

	void CreateCanvasPanel();
	virtual void showPlayerCursor(bool show);
	ScreenOpenStack openedScreenStack;

    void createTickActor(UWorld *world);

private:
	bool bMouseLeftIsPressed = false;
};
