// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class P2_API timer
{
public:
	timer();
	timer(float time, bool resetsItselfIn);
	~timer();

	void Begin(float time);
	void Begin(float time, bool resetsItselfIn);
	void Tick(float deltaTime);
	bool timesUp();
	

private:
	float initialTime;
	float timeLeft;
	bool resetsItself = false;
};
