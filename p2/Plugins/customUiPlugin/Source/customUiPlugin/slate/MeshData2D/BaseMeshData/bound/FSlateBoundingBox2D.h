#pragma once

#include "CoreMinimal.h"
#include "CoreMath/vector/bound/FBoundingBox2D.h"

class SlateMeshData;
class SlateMeshDataPolygon;


/// @brief bounding box for 2D Slate mesh data
/*
(tl)
0,0------ 1,0
|          |
0,1-------1,1(br)
*/
struct CUSTOMUIPLUGIN_API FSlateBoundingBox2D : public FBoundingBox2D{

    FSlateBoundingBox2D();
    virtual ~FSlateBoundingBox2D();

    FSlateBoundingBox2D(const FSlateBoundingBox2D & other);
    FSlateBoundingBox2D &operator=(const FSlateBoundingBox2D & other);

    //update from polygon and meshdata
    using FBoundingBox2D::Update;
    void Update(const SlateMeshData &ref);
    void Update(const SlateMeshDataPolygon &ref);

    
};