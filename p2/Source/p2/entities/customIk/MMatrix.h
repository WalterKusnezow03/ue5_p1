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

	void operator+=(FVector &other);
	void operator-=(FVector &other);

	void operator+=(MMatrix &other);
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

	void rollRadAdd(float angle);
	void pitchRadAdd(float angle);
	void yawRadAdd(float angle);
	


	static float degToRadian(float deg);
	static float radToDegree(float rad);

	static MMatrix createRotatorFromRad(float x, float y, float z);
	static MMatrix createRotatorFromDeg(float x, float y, float z);

	void resetRotation();

	void rotate(MMatrix &other);


	MMatrix createInverse();

	FRotator extractRotator();

	void transformFromWorldToLocalCoordinates(FVector &position);

private:
	



	//16 langes array für die 4x4 matrix
	static constexpr int size = 16;
	float array[16];


	
	void invertRotation();
	void swapIndices(int a, int b);



	

	void transpose();



	float det(std::vector<float> &matrix);
	std::vector<float> collectExcept(int x, int y, std::vector<float> &matrix);




public:
	MMatrix jordanInverse();
private:
	void set(int i, int j, float value);
	float get(int column, int row);
	void scaleRow(int row, float scale);
	void minusForRow(int row, int otherRow, float faktor);
};
