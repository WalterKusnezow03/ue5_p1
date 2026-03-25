// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * Matrix 4x4 (Transform.) Supports funny things like rotations from vectors.
 */
class COREMATH_API MMatrix
{
public:
	MMatrix();
	~MMatrix();

	MMatrix(const FVector &other);
	MMatrix(FRotator &other);
	MMatrix(const FQuat &other);
	MMatrix(const MMatrix &other);
	MMatrix(const FMatrix &mat);
	MMatrix &operator=(const MMatrix &other);

	void setTranslation(const MMatrix &other);
	void setTranslation(const FVector &pos);
	void setTranslation(float x, float y, float z);
	FVector getTranslation() const;

	void operator+=(const FVector &other);
	void operator-=(const FVector &other);

	void operator+=(const MMatrix &other);
	void operator*=(const MMatrix & other);

	FVector operator*(const FVector &other) const;
	MMatrix operator*(const MMatrix &other) const;

	FVector2D operator*(const FVector2D &other) const;

	FString asString();
	FString asStringExtractedTransform();

	void pitchRad(float angle);
	void rollRad(float angle);
	void yawRad(float angle);

	/// @brief add a roll angle, will multiply internally with a new rotation matrix!
	/// @param angle 
	void rollRadAdd(float angle);

	/// @brief add a pitch angle, will multiply internally with a new rotation matrix!
	/// @param angle 
	void pitchRadAdd(float angle);

	/// @brief add a yaw angle, will multiply internally with a new rotation matrix!
	/// @param angle 
	void yawRadAdd(float angle);

	void scaleUniform(float value);
	void scale(float x, float y, float z);

	static float degToRadian(float deg);
	static float radToDegree(float rad);

	static MMatrix createRotatorFromRad(float x, float y, float z);
	static MMatrix createRotatorFromDeg(float x, float y, float z);
	static MMatrix createRotatorFrom(const FRotator &other);
	static MMatrix createRotatorFrom(const FVector &other);
	static MMatrix createRotatorFrom(const FVector &other, FVector2D XAxis, FVector2D ZAxis);

	static MMatrix createRotatorFrom(
		const FVector &other,
		FVector2D XAxis,
		FVector2D ZAxis,
		bool yawConstraint90
	);

	//debug method
	static MMatrix createRotatorFrom(
		const FVector &other,
		FVector2D XAxis,
		FVector2D ZAxis,
		bool yawConstraint90,
		float &outYaw, //debug
		float &outPitch //debug
	);

	//new method for choosing roll instead of yaw
	static MMatrix createRotatorFrom(
		const FVector &other,
		FVector2D XAxis,
		FVector2D ZAxis,
		bool yawConstraint90,
		FVector &localFoward
	);

	void resetRotation();

	void rotate(const MMatrix &other);
	void setRotation(FRotator &other);
	void setRotation(const MMatrix &other);
	void setRotation(const FVector &other);

	void setRotation(const FQuat &quat);

	void setRotation(std::vector<float> &values);

	MMatrix createInverse();

	FRotator extractRotator() const;
	MMatrix extarctRotatorMatrix();
	float extractYawAngleRad();

	void transformFromWorldToLocalCoordinates(FVector &position);

	//transposes the rotationblock for R^T
	void invertRotation();

	static void rotateVectorDeg2D(float angleDeg, FVector2D &vector);
	static void rotateVectorRad2D(float angleRad, FVector2D &vector);

	static float unsignedAngleRadBetween(FVector2D &a, FVector2D &b);
	static float signedAngleRadBetween(FVector2D &a, FVector2D &b);

	void transpose();
	void transposeRotation();
	MMatrix transposedRotation();
	MMatrix invertedTranslation();

	std::vector<float> CopyRotation() const;

private:
	//16 langes array für die 4x4 matrix
	static constexpr int size = 16;
	float array[16];

	float clampDivisionByZero(float value);

	void swapIndices(int a, int b);

	float det(std::vector<float> &matrix);
	std::vector<float> collectExcept(int x, int y, std::vector<float> &matrix);

	static float signForAngle(FVector2D &a, FVector2D &b);

	static bool IsParalellToZAxis(const FVector &check);
	static bool IsParalell(
		const FVector &a,
		const FVector &b
	);

public:
	MMatrix jordanInverse();

	FVector lookDirXForward();

	void makeIdentity();

	FString toString();

private:
	void set(int i, int j, float value);
	float get(int column, int row) const;
	void scaleRow(int row, float scale);
	void minusForRow(int row, int otherRow, float faktor);




	//extraction rotation special
	static bool useRoll(float dotProduct);
	static float DotProduct2D(const FVector &a, const FVector &b);
};
