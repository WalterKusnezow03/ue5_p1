// Fill out your copyright notice in the Description page of Project Settings.


#include "DebugHelper.h"

DebugHelper::DebugHelper()
{
}

DebugHelper::~DebugHelper()
{
}

/// @brief shows a scnreen message in green color
/// @param s string to show
void DebugHelper::showScreenMessage(FString s){
    if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, s);
	}
}

/// @brief showsa screen message and a color
/// @param s 
/// @param color 
void DebugHelper::showScreenMessage(FString s, FColor color){
    if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, color, s);
	}
}

/// @brief shows a screen message and a number of choice
/// @param s string
/// @param argument number to concatenate 
void DebugHelper::showScreenMessage(FString s, int argument){
	FString res = s;
	res.Append(FString::Printf(TEXT("%d"), argument));
	showScreenMessage(res, FColor::Green);
}







void DebugHelper::showLineBetween(UWorld *world, FVector Start, FVector End, FColor color){
	if(world != nullptr){
		DrawDebugLine(world, Start, End, color, false, 100.0f, 0, 1.0f);
	}
}

void DebugHelper::showLineBetween(UWorld *world, FVector Start, FVector End){
	showLineBetween(world, Start, End, FColor::Blue);
}