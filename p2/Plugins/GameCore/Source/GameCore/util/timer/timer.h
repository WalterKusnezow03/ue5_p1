// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class GAMECORE_API timer
{
public:
	timer();
	timer(float time, bool resetsItselfIn);
	virtual ~timer();

	void Begin(float time);
	void Begin(float time, bool resetsItselfIn);
	void Tick(float deltaTime);
	
	virtual bool timesUp();
	virtual void TickWithTimesUpReset(float deltatime);

	float currentTimeLeft(){
		return timeLeft;
	}

	virtual float scalar();

private:
	float initialTime;
	float timeLeft;
	bool resetsItself = false;
};
