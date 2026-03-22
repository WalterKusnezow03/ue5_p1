// Copyright Walter Kusnezow All Rights Reserved.
#pragma once

#include "CoreMinimal.h"

#include "MeshDataPlugin/Public/MeshGenBase/MeshData/MeshData.h"


class MESHDATAPLUGIN_API FTriangleClipResult {

public:
    
    void Build(
        const FVector2D &halfBound,
        const FVector &vertex0,
        const FVector &vertex1,
        const FVector &vertex2,
        float depthMaxIn,
        MeshData &dataAppendTo
    ){
        depthMax = depthMaxIn;
        if(BoundsTest(halfBound, vertex0, vertex1, vertex2)){ //any on bound
            Clip(); //if needed
            AppendTo(dataAppendTo);
        }
        
    }

private:
    float depthMax = 50.0f;

    //append to a meshdata object
    //including UV generation
    void AppendTo(MeshData &data);


    bool BoundsTest(
        const FVector2D &halfBound,
        const FVector &vertex0,
        const FVector &vertex1,
        const FVector &vertex2
    );

    //will be done only if needed
    void Clip();



    bool v0InBound = false;
    bool v1InBound = false;
    bool v2InBound = false;

    FVector v0;
    FVector v1;
    FVector v2;

    FVector2D halfBoundsCopied;
    FVector2D fullBounds;

    int CountTrue();
    bool OneVertexInBound();
    bool TwoVerteciesInBound();
    bool AnyInBound();

    void CaseOneVertexInBound();
    void CaseTwoInBound();

    TArray<FVector> generatedTriangleShapedVertexBuffer;
    TArray<FVector2D> generatedTriangleShapedUVBuffer;
    void MakeUVBuffer();

    TArray<FPlane> GenerateBoundingPlanes();
    TArray<FPlane> planeBuffer;

    void ClipOne(const FVector &inBound, const FVector &v1In, const FVector &v2In);
    void ClipTwo(const FVector &inBoundV0, const FVector &inBoundV1, const FVector &v2In);

    FVector2D MakeUV(const FVector &vertex);

    bool IsInBoundAndDepth(
        const FVector2D &halfBound,
        const FVector &vertex
    );
    bool IsInBound(
        const FVector2D &halfBound,
        const FVector &vertex
    );
};