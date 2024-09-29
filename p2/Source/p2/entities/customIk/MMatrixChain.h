// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MMatrix.h"

/**
 * 
 */
class P2_API MMatrixChain
{
public:
	MMatrixChain();
	~MMatrixChain();

	void add(MMatrix *add);
	void setRoll(float x1, float x2); //debug function

	void build(UWorld *world, FVector toBuildFromlastAnchor, FColor color);

private:
	std::vector<MMatrix*> matrizen;

	static constexpr bool DRAW = true;
};
