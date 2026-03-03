// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CoreMath/Matrix/MMatrix.h"

/**
 * 
 */
class PLUECKERCORE_API Matrix3x3
{
public:
	Matrix3x3();
	~Matrix3x3();

	Matrix3x3(FVector &other);
	Matrix3x3(FRotator &other);
	Matrix3x3(const Matrix3x3 &other);
	Matrix3x3& operator=(const Matrix3x3 &other);

	void operator+=(const Matrix3x3 &other);
	void operator-=(const Matrix3x3 &other);
	void operator*=(const Matrix3x3 & other);
	void operator*=(float scalar);

	FVector operator*(const FVector &other) const;

	Matrix3x3 operator*(const Matrix3x3 &other) const;
	Matrix3x3 operator-(const Matrix3x3 &other) const;

	//skew operations
	void makeSkew(FVector &omega);
	static Matrix3x3 skew(FVector &omega);
	static Matrix3x3 makeExponentialMap_Skew(FVector &screw, float thetaOrTime);
	static void convertPlueckerToSE3components(
		FVector &angularVelocity,
		FVector &linearVelocity,
		Matrix3x3 &outRotation,
		FVector &outTranslation,
		float deltatime
	);

	void scaleUniform(float s);
	void scale(float x, float y, float z);

	// skew operations end

	void rollRadAdd(float angle);
	void pitchRadAdd(float angle);
	void yawRadAdd(float angle);
	void setRotation(FRotator other);
	void setRotation(Matrix3x3 &other);
	void setRotation(MMatrix &other);

	static float degToRadian(float deg);
	static float radToDegree(float rad);

	void rotate(Matrix3x3 &other);

	FRotator extractRotator();
	
	void transpose();

	std::vector<float> Copy() const;
	void Override(std::vector<float> &values);

	void setColumn(FVector &column, int i);

	Matrix3x3 jordanInverse() const;

	FString asString();
	static void testInverse();

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
	float getRowMajor(int rowY, int columnX);




	//for inverse
	float clampDivisionByZero(float other) const;
	void scaleRow(int row, float scale);
	void minusForRow(int row, int otherRow, float faktor);
};
