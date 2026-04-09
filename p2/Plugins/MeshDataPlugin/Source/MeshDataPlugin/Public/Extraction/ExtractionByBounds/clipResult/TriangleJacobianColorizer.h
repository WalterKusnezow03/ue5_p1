// Copyright Walter Kusnezow All Rights Reserved.
#pragma once

#include "CoreMinimal.h"


class MESHDATAPLUGIN_API TriangleJacobianColorizer {

public:
    TriangleJacobianColorizer();
    ~TriangleJacobianColorizer();
    
        

    FColor DistortionColorFor(
        const FVector &vertex,
        const FVector &v1In,
        const FVector &v2In,
        const FVector2D &uvVertex,
        const FVector2D &uv1,
        const FVector2D &uv2,
        const FColor &distortColorNone,
        const FColor &distortColorFull
    );

private:


    FColor MetrikColor(
        const FColor &distortColorNone,
        const FColor &distortColorFull,
        float scalar
    );

    //computed values
    float DetJTJ = 0.0f;
    float sinTheta = 0.0f;

    float Area = 0.0f;

    float singularValueLamda1 = 0.0f;
    float singularValueLamda2 = 0.0f;


    void FindDeterminantJTJ(
        const FVector &JacobianU,
        const FVector &JacobianV
    );
    void FindArea(
        const FVector &JacobianU,
        const FVector &JacobianV
    );
    void BuildSingularValues(
        const FVector &JacobianU,
        const FVector &JacobianV
    );
};