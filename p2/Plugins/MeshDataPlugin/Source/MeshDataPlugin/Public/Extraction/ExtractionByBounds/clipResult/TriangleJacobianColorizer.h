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

};