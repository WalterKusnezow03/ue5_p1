#include "QuadIntersectFrame.h"


FQuadIntersectFrame::FQuadIntersectFrame(){

}

FQuadIntersectFrame::~FQuadIntersectFrame(){

}

FQuadIntersectFrame::FQuadIntersectFrame(const FQuadIntersectFrame &other){
    if(this != &other){
        *this = other;
    }
}
FQuadIntersectFrame &FQuadIntersectFrame::operator=(const FQuadIntersectFrame &other){
    if(this != &other){
        frame0 = other.frame0;
        frame1 = other.frame1;
    }
    return *this;
}

void FQuadIntersectFrame::Setup(
    const FVector &v0,
    const FVector &v1,
    const FVector &v2,
    const FVector &v3
){
    frame0.Setup(v0, v1, v2);
    frame1.Setup(v0, v2, v3);
}


bool FQuadIntersectFrame::DoesIntersect(
    const FVector &start,
    const FVector &dir,
    FVector &outIntersectionPoint
){
    if(frame0.DoesIntersect(start, dir, outIntersectionPoint)){
        return true;
    }
    return frame1.DoesIntersect(start, dir, outIntersectionPoint);
}