// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "p2/entities/customIk/MMatrix.h"

/**
 * 
 */
class P2_API MatrixTree
{
public:
	MatrixTree();
	~MatrixTree();

	void generate(int height, int perCmMatrix);

private:
	/** 
	 * Alle matrizen sollten in einem vektor festgehalten werden
	 * Die chains sollten mit indices erstellt werden
	 */
	std::vector<MMatrix> matrices;

	bool indexIsValid(int index);
	MMatrix &matrixByIndex(int index);


	/**
	 * sollte hier auch das mesh selber drin gespeichert werden?
	 */
};
