// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "p2/interfaces/Interactinterface.h"
#include "p2/rooms/roomParts/jumpableActor.h"
#include "p2/interfaces/Damageinterface.h"
#include "window.generated.h"

UCLASS()
class P2_API Awindow : public AjumpableActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	Awindow();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;



private:

	

};
