#pragma once 

#include "CoreMinimal.h"

///@brief will fit a set of polygons into a dynamic polygon, given by an outline
///cannot make sure all of them are fit.
///-> will move along the outline and try to fit a polygon at some outline point.
/// (polygon is transformed to position and rotated until can fit with a fixed tries number.)
class COREMATH_API GreedyPolygonFitter {


public:
    GreedyPolygonFitter();
    ~GreedyPolygonFitter();

    /// @brief sets the outline shape of the polygon, the edges. Expects it to be sorted,
    /// vertecies are connected over the array, 
    /// Vector 0 and 1 form an egde, Vector n and 0 form an edge, size array = n + 1.
    /// @param shape 
    void SetOutlineShape(const TArray<FVector2D> &shapeIn);

private:
    TArray<FVector2D> outlineShape;
};