// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Damageinterface.h"
#include "DrawDebugHelpers.h"

#include "CoreMinimal.h"

/**
 * 
 */
class P2_API EdgeCollector
{
public:
	EdgeCollector();
	~EdgeCollector();

	std::vector<FVector>& getAllEdges(UWorld *World, float minHeight);
	std::vector<FVector> &getReadEdges();

private:
	float edgeMinHeight;

	std::vector<FVector> *readEdges;

	
	void getEdgesFromSingleMesh(UStaticMesh *StaticMesh, FVector debugPos, FTransform LocalToWorldTransform);
	void getEdgesFromSingleMeshComponent(UStaticMeshComponent *MeshComponent);

	bool isVertical(FVector A, FVector B);

	class UWorld *worldIn;

	// Innere Klasse
    class edgeData
    {
		public:
			edgeData(FVector bottomIn, FVector topIn);
			~edgeData();

			FVector top;
			FVector bottom;
	};
	void showPos(UWorld *world, FVector e);
	void showLine(FVector e, FVector g);
	void showEdges(std::vector<edgeData> &edges, UWorld *world);
	void ComputeConvexHull(std::vector<edgeData> &points);
	bool IsClockwise(const edgeData &a, const edgeData &b, const edgeData &c);

	std::vector<edgeData> *edgeDataEdges;
};
