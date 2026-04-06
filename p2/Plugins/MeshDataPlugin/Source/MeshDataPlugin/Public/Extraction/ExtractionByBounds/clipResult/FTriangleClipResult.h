// Copyright Walter Kusnezow All Rights Reserved.
#pragma once

#include "CoreMinimal.h"

#include "MeshDataPlugin/Public/Extraction/ExtractionByBounds/clipResult/TriangleJacobianColorizer.h"
#include "MeshDataPlugin/Public/MeshGenBase/MeshData/MeshData.h"

/// @brief clips a triangle against XY bounds
/// it is expected that the triangle is in XY Projected Space
/// UVs for textures (AnyMeshWidgetPlugin!) 
/// and HeatMap Colors for Distorting metrics are calculated too.
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

    void ClipBestCut(const FVector &inBound, FVector &v1In);

    TArray<FVector> generatedTriangleShapedVertexBuffer;
    TArray<FVector2D> generatedTriangleShapedUVBuffer;
    TArray<FColor> generatedTriangleShapedColorBuffer;
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



    //analytical Jacobian distortion metric
    TriangleJacobianColorizer jacobianAnalyticTool;
    bool VertexAndUVBufferValid();
    void MakeColorBuffer();
};