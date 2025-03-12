// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/VerticalBox.h"
#include "PlayerUi.generated.h"

/**
 * will be the base class for any ui blueprint
 */
UCLASS()
class P2_API UPlayerUi : public UUserWidget
{
	GENERATED_BODY()

public:
	static UPlayerUi *createNewInstance(UWorld *world);
	virtual void init();

private:
	bool isInited = false;
	UCanvasPanel *baseCanvas = nullptr;

	UVerticalBox *baseVerticalBox = nullptr;

	void createBaseBoxForCanvas();
	void createNewText(); //debug
};
