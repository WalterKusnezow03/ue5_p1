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