// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class EVENTWIDGETSYSTEMPLUGIN_API AnimationTimer
{
public:
	AnimationTimer();
	AnimationTimer(float time, bool resetsItselfIn);
	virtual ~AnimationTimer();

	void Begin(float time);
	void Begin(float time, bool resetsItselfIn);
	void Reset();
	virtual void Tick(float deltaTime);

	virtual bool timesUp();
	virtual bool TickWithTimesUpReset(float deltatime);

	float currentTimeLeft(){
		return timeLeft;
	}

	//between 0 and 1, reached or not
	virtual float scalar();

	//between 1 and 0, 0 if no time left
	float InvertedScalar();

private:
	float initialTime;
	float timeLeft;
	bool resetsItself = false;
};
