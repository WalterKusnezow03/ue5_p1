// Fill out your copyright notice in the Description page of Project Settings.


#include "FVectorTouple.h"

FVectorTouple::FVectorTouple(FVector aIn, FVector bIn)
{
    a = aIn;
    b = bIn;
}

FVectorTouple::~FVectorTouple()
{
}

FVector &FVectorTouple::first(){
    return a;
}

FVector &FVectorTouple::second(){
    return b;
}
