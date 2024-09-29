// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class P2_API MMatrix
{
public:
	MMatrix();
	~MMatrix();

	MMatrix(MMatrix &other);
	MMatrix& operator=(MMatrix &other);

	void setTranslation(FVector &pos);
	void setTranslation(float x, float y, float z);
	FVector getTranslation();

	void operator+=(MMatrix & other);
	void operator*=(MMatrix & other);

	FVector operator*(FVector &other);
	MMatrix operator*(MMatrix &other);

	FString asString();

	void roll(float angle);
	void pitch(float angle);
	void yaw(float angle);

	void rollRad(float angle);
	void pitchRad(float angle);
	void yawRad(float angle);

	static float degToRadian(float deg);

private:
	//16 langes array für die 4x4 matrix
	static constexpr int size = 16;
	float array[16];

	
};
