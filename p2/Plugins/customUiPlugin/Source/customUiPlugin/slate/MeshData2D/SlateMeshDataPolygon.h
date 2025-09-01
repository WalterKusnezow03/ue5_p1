#pragma once 

#include "CoreMinimal.h"
#include "SlateMeshData.h"

class CUSTOMUIPLUGIN_API SlateMeshDataPolygon {

public:
    SlateMeshDataPolygon();
    ~SlateMeshDataPolygon();

    ///@brief appends a closed shape to the internal Meshdata
    void AppendClosedShape(TArray<FVector2D> &shape);

    SlateMeshData &MeshDataRef();

    const SlateMeshData &MeshDataRefConst() const;

protected:
    SlateMeshData meshData;
};