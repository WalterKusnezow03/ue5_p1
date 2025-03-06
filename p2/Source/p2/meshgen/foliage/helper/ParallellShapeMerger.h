// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class P2_API ParallellShapeMerger
{
public:
	ParallellShapeMerger();
	~ParallellShapeMerger();

	void createParallelSortedShape(
		std::vector<FVector> &baseVectorIn,
		std::vector<FVector> &vectorToAlginAndSort
	);

	void appendTrianglesWith(
		std::vector<FVector> &other
	);

	std::vector<FVector> &baseVectorReference();
	std::vector<FVector> &alignedVectorReference();
	std::vector<FVector> &triangleBufferReference();

private:
	void fillMissingAlignedVertecies(std::vector<FVector> &other);
	void sortIn(std::vector<FVector> &vectorToAlginAndSort);
	void findClosestIndexToAndSave(FVector &vertex);
	void fillAlignedBufferGaps();
	void generateTriangleBuffer();
	bool positionLocked(int index);

	int findClosestIndex(FVector &vertex, bool listenForLock);

	std::vector<FVector> baseVector;
	std::vector<FVector> vectorAligned;
	std::vector<bool> sortedInFlag;

	std::vector<FVector> triangleBuffer;

	const int errorValue = -1;

	//template function
	template <typename T>
    bool isInBounds(std::vector<T>& vec, int index){
		return index >= 0 && index < vec.size();
	}
};
