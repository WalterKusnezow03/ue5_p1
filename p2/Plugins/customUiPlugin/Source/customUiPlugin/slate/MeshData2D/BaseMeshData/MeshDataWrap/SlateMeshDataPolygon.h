#pragma once 

#include "CoreMinimal.h"
#include "customUiPlugin/slate/MeshData2D/BaseMeshData/SlateMeshData.h"

class MMatrix2D;

/// @brief will manage a slate mesh data and apply animations to a mesh.
class CUSTOMUIPLUGIN_API SlateMeshDataPolygon {

public:
    SlateMeshDataPolygon();
    ~SlateMeshDataPolygon();

    SlateMeshDataPolygon(const SlateMeshDataPolygon &other);
    SlateMeshDataPolygon &operator=(const SlateMeshDataPolygon &other);

    ///@brief appends a closed shape to the internal Meshdata
    void AppendClosedShape(TArray<FVector2D> &shape);
    void AppendClosedShape(TArray<FVector2D> &shape, int detail);

    //changing
    SlateMeshData &MeshDataRef();

    //rendering
    const SlateMeshData &MeshDataRefConst() const;

    ///@brief ticks the polygon and tells if changes were made
    bool Tick(float deltatime);

    ///@brief expects cursor in local space.
    void UpdateCursorPosition(FVector2D &pos);
    void SetCursorColorEnabled(bool flag);

    ///Call after manipulating mesh data.
    ///@brief overrides the color buffer to the new color
    void SetFullColor(FLinearColor color);

    void SetCursorColor(FLinearColor color);

    void AddAmbientUvColor(FVector2D uv, FLinearColor color);

    void SetRuntimeTransformation(MMatrix2D &other);
    void ApplyTransformImmidiate(MMatrix2D &transform);

    void DebugLogColor(){
        meshData.bLogColor = true;
    }

    /// @brief marks for not filled drawing
    /// USE FOR DEBUG ONLY
    void DrawOutLineOnly();

    /// USE FOR DEBUG ONLY
    bool IsFlaggedDrawOutlineOnly() const;



    //rendering enable and disable
    void EnableRender(bool bFlag);
    bool bIsEnabled() const;


protected:
    /// @brief owned mesh data 
    SlateMeshData meshData;

    //dynamic attributes for drawing
    bool bDynamicCursorColorEnabled = true;
    bool bDrawOutlineOnly = false;


    //animation

    //render block
    bool rendered = true;

};