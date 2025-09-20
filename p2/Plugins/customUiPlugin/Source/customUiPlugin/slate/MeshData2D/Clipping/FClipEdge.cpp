#include "FClipEdge.h"

FClipEdge::FClipEdge(const FClipEdge &other){
    if(this != &other){
        *this = other;
    }
}

FClipEdge &FClipEdge::operator=(const FClipEdge &other){
    if(this != &other){
        start = other.start;
        dirSized = other.dirSized;
        normal = other.normal;
    }
    return *this;
}

FClipEdge::FClipEdge(const FVector2D &startIn, const FVector2D &dir){
    start = startIn;
    dirSized = dir;
    end = start + dir;

    //normal to right is swap x and y, -x, +y
    normal = FVector2D(dir.Y, -dir.X);
}

FVector2D FClipEdge::Local(const FVector2D &other){
    return other - start;
}

bool FClipEdge::RightOffLocal(const FVector2D &other){
    float dot = normal.X * other.X + normal.Y * other.Y;
    return dot > 0.0f;
}

bool FClipEdge::RightOffWorld(const FVector2D &other){
    FVector2D aslocal = Local(other);
    return RightOffLocal(aslocal);
}


bool FClipEdge::ClipEndWorld(const FVector2D &a, FVector2D &b){
    if(RightOffWorld(b)){
        FVector OutIntersectionPoint;

        FVector s1 = Make3D(start);
        FVector s2 = Make3D(end);
        FVector t1 = Make3D(a);
        FVector t2 = Make3D(b);


        if (FMath::SegmentIntersection2D(
                s1,
                s2,
                t1,
                t2,
                OutIntersectionPoint
            )
        ){
            b = FVector2D(OutIntersectionPoint.X, OutIntersectionPoint.Y);
            return true;
        }
    }
    return false;
}


FVector FClipEdge::Make3D(const FVector2D &other){
    return FVector(other.X, other.Y, 0.0f);
}