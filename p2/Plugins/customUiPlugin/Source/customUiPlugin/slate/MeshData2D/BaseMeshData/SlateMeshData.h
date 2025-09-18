#pragma once

#include "CoreMinimal.h"
#include "customUiPlugin/slate/MeshData2D/BaseMeshData/Color/PairColorPosition.h"
#include "customUiPlugin/slate/MeshData2D/BaseMeshData/bound/FSlateBoundingBox2D.h"
#include "customUiPlugin/slate/MeshData2D/BaseMeshData/Cache/SlateVertexBufferCache.h"
#include "CoreMath/Matrix/2D/MMatrix2D.h"

#include "Engine/Texture2D.h"
#include "Rendering/SlateResourceHandle.h"

//class FResourceHandle;
//class UTexture2D;

/// @brief class to store 2D Mesh Data for Slate Ui Polygons
/// Supports FVector2D and FSlateVertex at the same time.
/// will also hold the texture and provide 
class CUSTOMUIPLUGIN_API SlateMeshData {

    //SlateIndex = uin16, merken
protected:
    //drawing brush
    FSlateBrush textureBrush;
    FSlateResourceHandle emptyHandle;
    UTexture2D *texturePtr = nullptr;

public:
    void SetTexture(UTexture2D *textureIn);
    

    ///@brief returns the rsource handle for this object: nesecarry for drawing the texture on the 
    ///polygon
    const FSlateResourceHandle &drawingHandle() const;

public:
    bool bLogColor = false;

    void ReCalculateAmbientUvColors(); //CALL NEEDED IF ADDED VERTECIES AFTER COLORS!

    SlateMeshData();
    ~SlateMeshData();

    SlateMeshData(const SlateMeshData &other);
    SlateMeshData &operator=(const SlateMeshData &other);

    /// @brief clears the meshdata completly including cache
    void Clear();

    // --- Append ---

    /// @brief adds a triangle to the buffer
    void Append(FVector2D &a, FVector2D &b, FVector2D &c);

    ///@brief appends a quad (2 triangles) to the buffer
    void Append(FVector2D &a, FVector2D &b, FVector2D &c, FVector2D &d);

    /// @brief adds a triangle to the buffer, and splits it recursively if detail > 0
    void AppendEfficent(FVector2D &a, FVector2D &b, FVector2D &c, int detail);

    ///@brief appends a triangle but looks for nearby vertecies
    void AppendEfficent(FVector2D &a, FVector2D &b, FVector2D &c);

    ///@brief appens a quad (2 triangles) but looks for nearby vertecies
    void AppendEfficent(FVector2D &a, FVector2D &b, FVector2D &c, FVector2D &d);

    ///@brief appends a triangle shaped buffer {(v0,v1,v2)(v1,v2,v3)...}
    void AppendEfficentTriangleShapedBuffer(TArray<FVector2D> &triangleShapedBuffer);

    ///@brief appends a triangle shaped buffer {(v0,v1,v2,3)(v0,v1,v2,v3)...}
    void AppendEfficentQuadShapedBuffer(TArray<FVector2D> &quadShapedBuffer);

    ///@brief appends a closed shape, shape MUST be convex like, is in clockwise order!
    void AppendClosedShape(TArray<FVector2D> &shape);

    ///@brief appends a closed shape, shape MUST be convex like, is in clockwise order!, 
    //detail for recursive triangle split
    void AppendClosedShape(TArray<FVector2D> &shape, int detail);

    ///@brief adds a quad between 2 positions
    void AppendQuad(FVector2D &bottomLeft, FVector2D &topRight);

    // --- Data reference for drawing ---
    TArray<FVector2D> &VerteciesRef();
    TArray<SlateIndex> &TrianglesRef();

    const TArray<FVector2D> &VerteciesRefConst() const;
    const TArray<FSlateVertex> &GetSlateVertexBuffer(FSlateRenderTransform &RenderTransform) const;
    const TArray<SlateIndex> &TrianglesRefConst() const;

    TArray<FVector2f> VerteciesAs2f() const; //for line draw


    // --- color dynamic ---
    void UpdateCursorPosition(FVector2D &position, bool bDynamicColoring);
    void UpdateCursorColor(FLinearColor &color);

    ///@brief once a uv color coordinate is added, it is moved to
    ///vertex buffer space, and WILL move if the buffer changes.
    void AddAmbientUvColor(FVector2D uv, FLinearColor color);
    void ClearAmbientColors();

    ///@brief blocks ambient colors
    void SetFullColor(FLinearColor color);
    void ResetFullColor();

    // --- transform ---
    ///@brief applies a transformation matrix to the buffer, changes
    ///not resetable
    void ApplyTransformationImmidiate(const MMatrix2D &other);

    ///@brief centers the widget around the pivot immidiatley
    void CenterAroundPivotImmidiate(FVector2D &pivot);

    ///@brief sets the runtime transformation, applied when a rerender is needed
    ///can be reset. Original vertex buffer is not touched.
    void SetRuntimeTransformation(MMatrix2D &other);
    void ResetRuntimeTransformation();

    FVector2D SizeBounds();
    FVector2D CenterOfMesh()const;



private:
    //track changes for cache remake
    void FlagCacheUpdateNeeded();

    //slate vertex buffer cache to prevent calculating when
    //nothing changed
    SlateVertexBufferCache slateVertexCache;

    //bound
    FSlateBoundingBox2D boundingBox;

    // Mesh data
    float epsilon = 1.0f;
    static constexpr int MaxSizeVertexBuffer = 65535; //vertex number max for uint16
    TArray<FVector2D> Vertecies; //for line draw
    TArray<SlateIndex> Triangles;

    // Color data (const because called on render.)
    bool bCursorColorEnabled = false;
    int CursorHighLightDistanceSquared = 100 * 100;
    FPairColorPosition cursorColorPair;

    bool fullColorEnabled = false;
    FLinearColor fullColor;

    TArray<FPairColorPosition> ambientColorsInvertedSpace;

    ///@brief converts a uv, to inverted uv and then to vertex buffer space, speeds
    ///up color calculation because the scalar is found by one division (less operations)
    FVector2D convertUVInvertedToVertexBufferSpace(const FVector2D &uv);

    ///@brief expects distances the color coordinates, which must be inverted already 
    /// (see method above:convertUVInvertedToVertexBufferSpace)
    void ConvertToScalarValuesNormalized(
        TArray<float> &numbers,
        float totalDistance
    )const;


    //linear colors / gradient
    FLinearColor InterpolatedColorFor(const FVector2D &pos) const;

    //Color data end

    //transformaation
    bool bHasRuntimeTransformation = false;
    /// @brief matrix to apply at runtime to not manipulate the original buffer!
    MMatrix2D transformationRuntimeMatrix;

    void ApplyTransformation(const MMatrix2D &mat, FVector2D &vertex);
    void ApplyTransformationConst(
        const MMatrix2D &mat,
        FVector2f &vertex) const;

    //transformation end



    ///@brief tells if a vertex is present by epsilon distance
    bool HasVertex(FVector2D &a, uint16 &outIndex);

    ///@brief may return -1 if buffer empty
    int ClosestVertex(FVector2D &target);

    ///@brief may return -1 if none found
    int ClosestVertexBelowEpsilon(FVector2D &target);

    bool isValidVertexIndex(int i);

    bool TriangleCanBeAdded();

    
    FVector2D CenterOf(const TArray<FVector2D> &buffer)const;

    ///@brief will make the slate vertex for a given render transform and vertex
    FSlateVertex makeSlateVertex(
        const FVector2D &ref,
        const FSlateRenderTransform &RenderTransform
    ) const;

    ///generates the UV Coordinate clamped based on bounding box
    FVector2f MakeUV(const FVector2D &vertex) const;



    FVector2f ConvertTo2f(const FVector2D &other) const;

    // -- triangle detail increase --

    ///@brief splits all triangles from irginal buffer in half and appends it
    ///in triangle shape to the append buffer, duplicates included!
    void Split(
        TArray<FVector2D> &originalBuffer,
        TArray<FVector2D> &appendToTriangleShapedBuffer
    );

    ///@brief splits a triangle along its longest edge in halt and
    ///appends both triangles the buffer (v0,v1,v2) -> {(new,1,2)(0,new,2)}
    void Split(
        FVector2D &v0,
        FVector2D &v1,
        FVector2D &v2,
        TArray<FVector2D> &appendToTriangleShapedBuffer);
};
