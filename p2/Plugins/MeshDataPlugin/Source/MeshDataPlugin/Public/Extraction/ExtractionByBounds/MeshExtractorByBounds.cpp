#include "MeshExtractorByBounds.h"
#include "MeshDataPlugin/Public/Extraction/StaticMeshCopy/MeshExtractorBase.h"
#include "MeshDataPlugin/Public/Extraction/ExtractionByBounds/clipResult/FTriangleClipResult.h"

#include "DebugPlugin/DebugHelper.h"

void MeshExtractorByBounds::ExtractSingleMeshDataByBounds(
    UStaticMeshComponent *componentToExtractFrom, 
    MeshData &meshData,
    UStaticMeshComponent *boundsComponent,
    FVector offset
){
    ExtractSingleMeshDataByBounds(componentToExtractFrom, meshData, boundsComponent);
    MMatrix offsetMat(offset);
    meshData.transformAllVertecies(offsetMat); //move back to original position
}

void MeshExtractorByBounds::ExtractSingleMeshDataByBounds(
    UStaticMeshComponent *componentToExtractFrom, 
    MeshData &meshData,
    UStaticMeshComponent *boundsComponent
){
    MeshExtractorBase extractor;
    MeshData extractedNotClipped;
    extractor.ExtractSingleMeshData(componentToExtractFrom, extractedNotClipped);

    DebugHelper::logMessage(
        "MeshExtractorByBounds::extracted raw", 
        extractedNotClipped.getVerteciesRef().Num()
    );

    
    //generate relative transform, since bounds are BELOW extracted component
    //and inverse
    MMatrix m = GenerateRelativeTransform(boundsComponent);
    MMatrix inverse = GenerateInverseRelativeTransform(boundsComponent);
    
    
    //project to XY plane
    extractedNotClipped.transformAllVertecies(inverse);

    //cut vertecies against z planes
    //uvs generated too!
    CutAgainstBounds(boundsComponent, extractedNotClipped); //now IS ClIPPED

    // 0 HERE!
    DebugHelper::logMessage(
        FString::Printf(
            TEXT(
                "MeshExtractorByBounds::extracted clipped (v %d) (t %d) (n %d) (uv %d)"
            ),
            extractedNotClipped.getVerteciesRef().Num(),
            extractedNotClipped.getTrianglesRef().Num(),
            extractedNotClipped.getNormalsRef().Num(),
            extractedNotClipped.getUV0Ref().Num()
        )
    );


    //move back
    //extractedNotClipped.transformAllVertecies(m); //move back to original position

    extractedNotClipped.transformAllVertecies(m);
    meshData = extractedNotClipped; //override
}

MMatrix MeshExtractorByBounds::GenerateRelativeTransform(
    UStaticMeshComponent *component
){
    //M = T * R * S
    FVector Location = component->GetRelativeLocation();
    FRotator Rotation = component->GetRelativeRotation();
    
    
    MMatrix t(Location);
    MMatrix r(Rotation);

    MMatrix m = t * r; //<-- lese richtung --
    return m;
}

MMatrix MeshExtractorByBounds::GenerateInverseRelativeTransform(
    UStaticMeshComponent *component
){
    FVector Location = component->GetRelativeLocation();
    FRotator Rotation = component->GetRelativeRotation();

    Location *= -1.0f;
    MMatrix t1(Location);
    MMatrix r(Rotation);
    r.transposeRotation();
    //M = T * R
    //M^-1 = R^-1 * T^-1
    MMatrix M1 = r * t1;
    return M1;
}

FVector2D MeshExtractorByBounds::ExtractBounds2D(UStaticMeshComponent *comp){
    FTransform Transform = comp->GetComponentTransform();
    FVector Scale = Transform.GetScale3D();
    FVector2D Scale2D(Scale.X, Scale.Y);
    FVector2D scaleMeters = Scale2D * 100.0f;
    return scaleMeters;
}

void MeshExtractorByBounds::CutAgainstBounds(UStaticMeshComponent *comp, MeshData &projectedData){
    FVector2D bounds2D = ExtractBounds2D(comp);
    FVector2D halfBound = bounds2D * 0.5;
    CutAgainstBounds(halfBound, projectedData);
}

void MeshExtractorByBounds::CutAgainstBounds(const FVector2D &halfBound, MeshData &projectedData){
    DebugHelper::logMessage(
        FString::Printf(
            TEXT("MeshExtractorByBounds::half bound  %.2f %.2f"), 
            halfBound.X, halfBound.Y
        )
    );
    
    
    TArray<int32> &triangleBuffer = projectedData.getTrianglesRef();
    TArray<FVector> &vertexBuffer = projectedData.getVerteciesRef();

    MeshData cutData;


    //splitup
    for (int32 t = 2; t < triangleBuffer.Num(); t += 3){
        int32 i0 = triangleBuffer[t - 2];
        int32 i1 = triangleBuffer[t - 1];
        int32 i2 = triangleBuffer[t - 0];

        const FVector &v0 = vertexBuffer[i0];
        const FVector &v1 = vertexBuffer[i1];
        const FVector &v2 = vertexBuffer[i2];

        FTriangleClipResult result;
        result.Build(
            halfBound,
            v0,
            v1,
            v2,
            cutData
        );
    }

    DebugHelper::logMessage(
        "MeshExtractorByBounds::extracted cut data clipped", 
        cutData.getVerteciesRef().Num()
    );

    cutData.calculateNormals();

    //override
    projectedData = cutData;
}
