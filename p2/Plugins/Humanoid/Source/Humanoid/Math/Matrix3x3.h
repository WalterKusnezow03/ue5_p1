// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class HUMANOID_API Matrix3x3
{
public:
	Matrix3x3();
	~Matrix3x3();

	Matrix3x3(FVector &other);
	Matrix3x3(FRotator &other);
	Matrix3x3(const Matrix3x3 &other);
	Matrix3x3& operator=(const Matrix3x3 &other);

	void operator+=(Matrix3x3 &other);
	void operator-=(Matrix3x3 &other);
	void operator*=(Matrix3x3 & other);
	void operator*=(float scalar);

	FVector operator*(FVector &other);

	Matrix3x3 operator*(Matrix3x3 &other);
	Matrix3x3 operator-(Matrix3x3 &other);

	//skew operations
	void makeSkew(FVector &omega);
	static Matrix3x3 skew(FVector &omega);
	static Matrix3x3 makeExponentialMap_Skew(FVector &screw, float thetaOrTime);
	static void convertTwistToSE3components(
		FVector &angularVelocity,
		FVector &linearVelocity,
		Matrix3x3 &outRotation,
		FVector &outTranslation,
		float deltatime
	);
	// skew operations end

	void rollRadAdd(float angle);
	void pitchRadAdd(float angle);
	void yawRadAdd(float angle);
	void setRotation(FRotator &other);
	void setRotation(Matrix3x3 &other);

	static float degToRadian(float deg);
	static float radToDegree(float rad);

	void rotate(Matrix3x3 &other);

	FRotator extractRotator();
	
	void transpose();

	std::vector<float> Copy();

private:
	//16 langes array für die 4x4 matrix
	static constexpr int size = 9;
	float array[9];


	void swapIndices(int a, int b);

public:

	void makeIdentity();
	void makeZero();

private:
	void set(int i, int j, float value);
	float get(int column, int row);
	
};
