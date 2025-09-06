#pragma once 

#include "CoreMinimal.h"
#include "SlateMeshData.h"

class MMatrix2D;

class CUSTOMUIPLUGIN_API SlateMeshDataPolygon {

public:
    SlateMeshDataPolygon();
    ~SlateMeshDataPolygon();

    ///@brief appends a closed shape to the internal Meshdata
    void AppendClosedShape(TArray<FVector2D> &shape);
    void AppendClosedShape(TArray<FVector2D> &shape, int detail);

    SlateMeshData &MeshDataRef();

    const SlateMeshData &MeshDataRefConst() const;

    ///@brief ticks the polygon and tells if changes were made
    bool Tick(float deltatime);

    ///@brief expects cursor in local space.
    void UpdateCursorPosition(FVector2D &pos);

    ///Call after manipulating mesh data.
    ///@brief overrides the color buffer to the new color
    void SetColor(FLinearColor color);

    void SetCursorColor(FLinearColor color);

    void AddAmbientUvColor(FVector2D uv, FLinearColor color);

    void ApplyTransformImmidiate(MMatrix2D &transform);

    void DebugLogColor(){
        meshData.bLogColor = true;
    }

protected:
    SlateMeshData meshData;

    bool bDynamicCursorColorEnabled = true;
};