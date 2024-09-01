// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class P2_API shapeCreator
{
public:

	static void createShape(std::vector<FVector2D> &output);

private:
	shapeCreator();
	~shapeCreator();
	static std::vector<FVector2D> shape0();
	static std::vector<FVector2D> shape1();
};
