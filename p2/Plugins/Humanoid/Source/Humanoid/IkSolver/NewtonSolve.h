#pragma once

#include "CoreMinimal.h"
#include "CoreMath/Matrix/MMatrix.h"
#include "Humanoid/Math/Matrix3x3.h"
#include "Humanoid/Bone/Joint.h"

class HUMANOID_API NewtonSolve {

public:
    NewtonSolve();
    ~NewtonSolve();


    FVector SolveForAngularVelocity(
        Joint *joint,   // starting joint
        FVector &target, // local target
        float desiredTime
    );

    bool SolutionWasFound();
    bool solutionIsAlreadyWithinRange(Joint &joint, FVector &target);

private:
    bool solutionFound = false;
    float minDistanceSolutionOk = 5.0f;
    bool logEnabled = false;

    bool solutionIsWithinRange(FVector &a, FVector &b);
    void resetFlag();
    void markSolved();
    void logSolution(FVector &target, FVector &w, FVector &t_w, float dist, int attempts);
    bool increaseNeeded(FVector &a, FVector &b);

    Matrix3x3 computeNumericalJacobian(
        MMatrix &transform,
        Joint *joint,
        const FVector &w,
        float deltatime
    );
    FVector guessAngularVelocity(Joint &joint, FVector &targetLocal);

    void testInverse(Matrix3x3 &a, Matrix3x3 &b);
};