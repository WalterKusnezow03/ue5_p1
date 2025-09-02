#pragma once 

#include "CoreMinimal.h"
#include "SlateMeshData.h"

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

    ///@brief overrides the color buffer to the new color
    void SetColor(FLinearColor color);

    void SetCursorColor(FLinearColor color);

protected:
    SlateMeshData meshData;

    bool bDynamicCursorColorEnabled = true;
};