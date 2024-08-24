// Fill out your copyright notice in the Description page of Project Settings.


#include "DebugHelper.h"

DebugHelper::DebugHelper()
{
}

DebugHelper::~DebugHelper()
{
}


void DebugHelper::showScreenMessage(FString s){
    if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, s);
	}
}

void DebugHelper::showScreenMessage(FString s, FColor color){
    if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, color, s);
	}
}



void DebugHelper::showLineBetween(UWorld *world, FVector Start, FVector End, FColor color){
	if(world != nullptr){
		DrawDebugLine(world, Start, End, color, false, 100.0f, 0, 1.0f);
	}
}

void DebugHelper::showLineBetween(UWorld *world, FVector Start, FVector End){
	showLineBetween(world, Start, End, FColor::Blue);
}