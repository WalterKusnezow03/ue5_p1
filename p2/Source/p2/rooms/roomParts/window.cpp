// Fill out your copyright notice in the Description page of Project Settings.


#include "window.h"

// Sets default values
Awindow::Awindow()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void Awindow::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void Awindow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

