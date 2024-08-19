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
	~timer();

	/// @brief decrement the time
	/// @param deltaTime 
	void Tick(float deltaTime);

	/// @brief timer is running or not
	/// @return timer <= 0
	bool timesUp();

	void Begin(float time);

private:
	float timeLeft;



};
