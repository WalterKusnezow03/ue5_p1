// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class P2_API abstractKinematicFunctions
{
public:
	abstractKinematicFunctions();
	~abstractKinematicFunctions();


	
	//constants
	static constexpr float dampSwing = 0.3f;


	static constexpr float c6 = 0.02f;
	static constexpr float c7 = 0.0f;

	static constexpr float c8 = 0.3f;
	static constexpr float c9 = 0.12f;

	static constexpr float ct0 = 0;
	static constexpr float ct1 = 2.3876f;
	static constexpr float c10 = 0.17f;
	static constexpr float c11 = 0.12f;

	static constexpr float velocity = 2.0f;

	//functions
	static float legLift(float t);
	static float pitchlegSwing(float t);

	static float legSwingPitch(float t);
};
