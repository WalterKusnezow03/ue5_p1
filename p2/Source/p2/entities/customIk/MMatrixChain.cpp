// Fill out your copyright notice in the Description page of Project Settings.


#include "p2/entities/customIk/MMatrixChain.h"

/**
 * Man betrachtet immer seine matrix im vergleich zum vorgänger, 
 * zuerst die rotation anwenden
 * dann alle matrizen in der kette multiplizieren, mal einem vektor
 * um den vektor in das koordinaten system des ersten matrix knoten zu bringen
 */


MMatrixChain::MMatrixChain()
{
}


MMatrixChain::~MMatrixChain()
{
    //dont delete any pointers!
}

void MMatrixChain::add(MMatrix *add){
    if(add != nullptr){
        matrizen.push_back(add);
    }
    
}

void MMatrixChain::setRoll(float x1, float x2){
    std::vector<float> rolling = {x1, x2};
    for (int i = 0; i < rolling.size(); i++){
        if (i < matrizen.size()){
            matrizen[i]->roll(rolling[i]);
        }
    }
}




void MMatrixChain::build(UWorld *world, FVector toBuildFromlastAnchor, FColor color){
    if(matrizen.size() < 1){
        return;
    }
    std::vector<FVector> resultDraw;
    MMatrix result = *matrizen[0];
    resultDraw.push_back(result.getTranslation());
    for (int i = 1; i < matrizen.size(); i++){
        result *= *matrizen[i];
        resultDraw.push_back(result.getTranslation()); //translation will be now in result space always
    }


    FVector outputVec = result * toBuildFromlastAnchor;
    resultDraw.push_back(outputVec);

    if(DRAW && world != nullptr){
        int scale = 100;
        DebugHelper::showLine(world, resultDraw, color, scale);

        FVector copy = resultDraw[0];
        FVector copy1 = copy + FVector(0, 0, 100);
        DebugHelper::showLineBetween(world, copy, copy1, FColor::Cyan);
    }
}

