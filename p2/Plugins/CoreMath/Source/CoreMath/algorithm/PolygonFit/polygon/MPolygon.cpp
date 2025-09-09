#include "MPolygon.h"
#include "Math/UnrealMathUtility.h"


MPolygon::MPolygon(){

}

MPolygon::~MPolygon(){

}

void MPolygon::SetShape(const TArray<FVector2D> &shapeIn){
    shapeOriginal = shapeIn;
    UpdateTransformedShape();
}

void MPolygon::SetTranslation(const MMatrix2D &matrix){
    myTranslation = matrix;
    UpdateTransformedShape();
}

void MPolygon::SetRotation(const MMatrix2D &matrix){
    myRotation = matrix;
    UpdateTransformedShape();
}

void MPolygon::UpdateTransformedShape(){
    if(shapeOriginal.Num() <= 0){
        return;
    }

    //M = T * R <-- lese richtung
    //rotiert um 0,0, nicht zentrum.
    MMatrix2D transform = myTranslation * myRotation;
    shapeTransformed.SetNum(shapeOriginal.Num());
    for (int i = 0; i < shapeOriginal.Num(); i++){
        FVector2D copy = shapeOriginal[i];
        shapeTransformed[i] = transform * copy;
    }
}

bool MPolygon::DoesIntersectClockwiseShape(TArray<FVector2D> &shapeToFit){
    if(shapeToFit.Num() <= 0 || shapeTransformed.Num() <= 0){
        return false;
    }

    for (int i = 0; i < shapeToFit.Num(); i++){
        int j = (i + 1) % shapeToFit.Num();
        if(DoesIntersect(shapeToFit[i], shapeToFit[j])){
            return true;
        }
    }
    return false;
}




bool MPolygon::DoesIntersect(
    FVector2D &aWorld, 
    FVector2D &bWorld
){
    if(shapeTransformed.Num() <= 0){
        return false;
    }

    for (int i = 0; i < shapeTransformed.Num(); i++){
        int j = (i + 1) % shapeTransformed.Num();
        if(DoesIntersect(
            aWorld, bWorld,
            shapeTransformed[i], shapeTransformed[j]
        )){
            return true;
        }
    }
    return false;
}

bool MPolygon::DoesIntersect(
    FVector2D &aWorld, //edge 1
    FVector2D &bWorld,
    FVector2D &e1,     //edge 2
    FVector2D &e2
){
    //right left test by normal
    FVector2D edge = bWorld - aWorld;

    //is a clock wise rotation, (from left to right)
    //means the point with dot product < 0, is outside the polygon if shape is clock wise sorted
    FVector2D edgeNormal(edge.Y, -edge.X); 

    FVector2D e1Local = e1 - aWorld; //kante ins relative system bringen.
    FVector2D e2Local = e2 - aWorld;

    //wenn beide skalar produkte mit der normalen das selbe vorzeichen haben, ist das ergebnis positiv und
    //beide punkte liegen auf einer seite von der "edge".
    /*if(DotProduct(edgeNormal, e1Local) * DotProduct(edgeNormal, e2Local) < 0.0f){// < 0, intersection!

        


        return true;
    }*/
    FVector2D IntersectionPoint;
    if (SegmentIntersection2D(aWorld, bWorld, e1, e2, IntersectionPoint))
    {
        //since an inetrsection is detected, one point must be on the negative side of the normal.
        bool e1Outside = DotProduct(edgeNormal, e1Local) < 0.0; //assuming e1 is outside of clockwise ordered shape
        FVector2D &outsidePoint = e1Outside ? e1 : e2;

        // AB = B - A, transform needed inwards to not intersect
        //(might not be needed for the algorythm)
        moveAwayFromIntersection = IntersectionPoint - outsidePoint;

        return true;
    }

    return false;
}


float MPolygon::DotProduct(FVector2D &a, FVector2D &b){
    return a.X * b.X + a.Y * b.Y;
}


bool MPolygon::SegmentIntersection2D(
    FVector2D &a0_2D,
    FVector2D &a1_2D,
    FVector2D &b0_2D,
    FVector2D &b1_2D,
    FVector2D &outIntersection_2D
){

    FVector a0(a0_2D.X, a0_2D.Y, 0.0);
    FVector a1(a1_2D.X, a1_2D.Y, 0.0);
    FVector b0(b0_2D.X, b0_2D.Y, 0.0);
    FVector b1(b1_2D.X, b1_2D.Y, 0.0);
    

    //bool SegmentIntersection2D(
    //  const FVector& SegmentStartA, 
    //  const FVector& SegmentEndA, 
    //  const FVector& SegmentStartB, 
    //  const FVector& SegmentEndB, 
    //  FVector& out_IntersectionPoint
    //);
    FVector IntersectionPoint;
    if(FMath::SegmentIntersection2D(a0, a1, b0, b1, IntersectionPoint)){
        outIntersection_2D.X = IntersectionPoint.X;
        outIntersection_2D.Y = IntersectionPoint.Y;
        return true;
    }
    return false;
}