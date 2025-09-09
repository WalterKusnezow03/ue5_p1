#include "MPolygon.h"
#include "Math/UnrealMathUtility.h"


MPolygon::MPolygon(){

}

MPolygon::~MPolygon(){

}
MPolygon::MPolygon(const MPolygon &other){
    if(this != &other){
        *this = other;
    }
}
MPolygon &MPolygon::operator=(const MPolygon &other){
    if(this != &other){
        myTranslation = other.myTranslation;
        myRotation = other.myRotation;

        shapeTransformed = other.shapeTransformed;
        shapeOriginal = other.shapeOriginal;

        moveAwayFromIntersection = other.moveAwayFromIntersection;
    }
    return *this;
}

void MPolygon::SetShape(const TArray<FVector2D> &shapeIn){
    shapeOriginal = shapeIn;
    UpdateTransformedShape();
}

void MPolygon::SetTranslation(const FVector2D &pos){
    MMatrix2D mat(pos);
    SetTranslation(mat);
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



bool MPolygon::DoesIntersect(
    const MPolygon &other
){
    return DoesIntersectClockwiseShape(other.shapeTransformed);
}

bool MPolygon::DoesIntersectClockwiseShape(const TArray<FVector2D> &shapeToFit){
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
    const FVector2D &aWorld, 
    const FVector2D &bWorld
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
    const FVector2D &aWorld, //edge 1
    const FVector2D &bWorld,
    const FVector2D &e1,     //edge 2
    const FVector2D &e2
){
    //right left test by normal
    FVector2D edge = bWorld - aWorld;

    //is a clock wise rotation, (from left to right)
    //means the point with dot product < 0, is outside the polygon if shape is clock wise sorted
    FVector2D edgeNormal(edge.Y, -edge.X); 

    FVector2D e1Local = e1 - aWorld; //kante ins relative system bringen.
    FVector2D e2Local = e2 - aWorld;

    FVector2D IntersectionPoint;
    if (SegmentIntersection2D(aWorld, bWorld, e1, e2, IntersectionPoint))
    {
        //since an inetrsection is detected, one point must be on the negative side of the normal.
        bool e1Outside = FVector2D::DotProduct(edgeNormal, e1Local) < 0.0; //assuming e1 is outside of clockwise ordered shape
        const FVector2D &outsidePoint = e1Outside ? e1 : e2;

        // AB = B - A, transform needed inwards to not intersect
        //(might not be needed for the algorythm)
        moveAwayFromIntersection = IntersectionPoint - outsidePoint;

        return true;
    }

    return false;
}




bool MPolygon::SegmentIntersection2D(
    const FVector2D &a0_2D,
    const FVector2D &a1_2D,
    const FVector2D &b0_2D,
    const FVector2D &b1_2D,
    FVector2D &outIntersection_2D
){
    if(IntersectsPoint(a0_2D, a1_2D, b0_2D)){
        outIntersection_2D = b0_2D;
        return true;
    }

    if(IntersectsPoint(a0_2D, a1_2D, b1_2D)){
        outIntersection_2D = b1_2D;
        return true;
    }







    FVector a0(a0_2D.X, a0_2D.Y, 0.0);
    FVector a1(a1_2D.X, a1_2D.Y, 0.0);
    FVector b0(b0_2D.X, b0_2D.Y, 0.0);
    FVector b1(b1_2D.X, b1_2D.Y, 0.0);


    //make line extended because hit thru point is not 
    //counted a intersction in FMath!
    //ExtendLineInBothDirections(a0, a1, sizeExtensionCm);
    //ExtendLineInBothDirections(b0, b1, sizeExtensionCm);

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



#include "CoreMath/Debug/CoreMathDebugHelper.h"
bool MPolygon::IntersectsPoint(
    const FVector2D &a0,
    const FVector2D &a1,
    const FVector2D &b
){
    FVector2D dir = a1 - a0;
    FVector2D dirComapre = b - a0;

    if(FVector2D::DotProduct(dir.GetSafeNormal(), dirComapre.GetSafeNormal()) <= 0.99999f){
        return false;
    }

    //-- works if paralell test is sucess! --
    //gx = a + t(b-a), t in 0, 1 for a to b range

    //a0 + t * (a1-a0) = b || solve for t
    //t * (a1-a0) = b - a0 ||^2
    //t^2 * (a1-a0)^2 = (b - a0)^2
    //t^2 = (b - a0)^2 / (a1-a0)^2
    //t1,2 = sqrt((b - a0)^2 / (a1-a0)^2)

    
    FVector2D ab = b - a0;
    float ab2 = ab.X * ab.X + ab.Y * ab.Y;

    FVector2D a0a1 = a1 - a0;
    float a0a12 = a0a1.X * a0a1.X + a0a1.Y * a0a1.Y;

    float innerSqrt = ab2 / a0a12;
    float sqrt = std::sqrt(innerSqrt);

    float t1 = sqrt;
    float t2 = -1.0f * sqrt;
    if((t1 >= 0.0f && t1 <= 1.0f) || (t2 >= 0.0f && t2 <= 1.0f)){
        CoreMathDebugHelper::logMessage(
            FString::Printf(
                TEXT("MPolygon was hitting line!%.2f, a0:%s a1:%s b:%s"),
                std::max(t1,t2), *a0.ToString(), *a1.ToString(), *b.ToString()
            )
        );
        return true;
    }
    return false;
}
