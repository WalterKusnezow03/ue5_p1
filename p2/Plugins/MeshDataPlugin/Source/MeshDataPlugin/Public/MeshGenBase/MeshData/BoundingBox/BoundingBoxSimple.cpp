// Copyright Walter Kusnezow All Rights Reserved.
#include "BoundingBoxSimple.h"


BoundingBoxSimple::BoundingBoxSimple(){
    Clear();
}

BoundingBoxSimple::BoundingBoxSimple(const TArray<FVector> &vertecies){
    Clear();
    updateBoundsIfNeeded(vertecies);
}

BoundingBoxSimple &BoundingBoxSimple::operator=(const BoundingBoxSimple &other){
    if(this != &other){
        bottomLeftNear = other.bottomLeftNear;
        topRightFar = other.topRightFar;
    }
    return *this;
}

BoundingBoxSimple::BoundingBoxSimple(const BoundingBoxSimple &other){
    if(this != &other){
        *this = other;
    }
}



BoundingBoxSimple::~BoundingBoxSimple(){

}

void BoundingBoxSimple::Clear(){
    //nicht 0,0, NEIN
    bottomLeftNear = FVector(FLT_MAX, FLT_MAX, FLT_MAX);
    topRightFar = FVector(-FLT_MAX, -FLT_MAX, -FLT_MAX);
}

void BoundingBoxSimple::RebuildBounds(const TArray<FVector> &vertecies){
    bottomLeftNear = FVector(0, 0, 0);
    topRightFar = FVector(0, 0, 0);
    updateBoundsIfNeeded(vertecies);
}

void BoundingBoxSimple::updateBoundsIfNeeded(const TArray<FVector> &vertecies){
    for (int i = 0; i < vertecies.Num(); i++){
        updateBoundsIfNeeded(vertecies[i]);
    }
}

void BoundingBoxSimple::updateBoundsIfNeeded(const FVector &other){
    bottomLeftNear.X = std::min(bottomLeftNear.X, other.X);
    bottomLeftNear.Y = std::min(bottomLeftNear.Y, other.Y);
    bottomLeftNear.Z = std::min(bottomLeftNear.Z, other.Z);

    topRightFar.X = std::max(topRightFar.X, other.X);
    topRightFar.Y = std::max(topRightFar.Y, other.Y);
    topRightFar.Z = std::max(topRightFar.Z, other.Z);
}

bool BoundingBoxSimple::isInsideBoundingbox(const FVector &other) const {
    return other.X > bottomLeftNear.X &&
           other.Y > bottomLeftNear.Y &&
           other.Z > bottomLeftNear.Z &&
           other.X < topRightFar.X &&
           other.Y < topRightFar.Y &&
           other.Z < topRightFar.Z;
}



FVector BoundingBoxSimple::bottomLeftNearVertex(){
    return bottomLeftNear;
}

FVector BoundingBoxSimple::topRightFarVertex(){
    return topRightFar;
}



FString BoundingBoxSimple::ToString(){
    FVector extentTmp = extent();
    FString message = FString::Printf(
        TEXT("BoundingBoxSimple To String: (%.2f, %.2f, %.2f) (%.2f, %.2f, %.2f) extent (%.2f %.2f %.2f)"),
        bottomLeftNear.X, bottomLeftNear.Y, bottomLeftNear.Z,
        topRightFar.X, topRightFar.Y, topRightFar.Z,
        extentTmp.X, extentTmp.Y, extentTmp.Z
    );
    return message;
}


void BoundingBoxSimple::GetAllCorners(
    FVector &A0,
    FVector &A1,
    FVector &A2,
    FVector &A3,
    FVector &B0,
    FVector &B1,
    FVector &B2,
    FVector &B3,
    const MMatrix &mat
){
    GetAllCorners(A0, A1, A2, A3, B0, B1, B2, B3);
    A0 = mat * A0;
    A1 = mat * A1;
    A2 = mat * A2;
    A3 = mat * A3;
    B0 = mat * B0;
    B1 = mat * B1;
    B2 = mat * B2;
    B3 = mat * B3;
}

void BoundingBoxSimple::GetAllCorners(
    FVector &A0,
    FVector &A1,
    FVector &A2,
    FVector &A3,
    FVector &B0,
    FVector &B1,
    FVector &B2,
    FVector &B3
){
    //plane intersect
    A0 = bottomLeftNearVertex();
    B2 = topRightFarVertex();

    FVector connect = B2 - A0;
    float x = connect.X;
    float y = connect.Y;
    float z = connect.Z;

    /*
    stacked:
    y
    1  2
    
    0  3x
    */

    A1 = A0 + FVector(0.0f, y, 0.0f);
    A2 = A0 + FVector(x, y, 0.0f);
    A3 = A0 + FVector(x, 0.0f, 0.0f);

    B0 = A0 + FVector(0.0f, 0.0f, z);
    B1 = A1 + FVector(0.0f, 0.0f, z);
    B3 = A3 + FVector(0.0f, 0.0f, z);
}





#include "MeshDataPlugin/Public/MeshGenBase/MeshData/intersectCache/QuadIntersectFrame.h"
bool BoundingBoxSimple::DoesIntersect(const FVector &origin, const FVector &dir){
    
    //plane intersect
    FVector A0, A1, A2, A3, B0, B1, B2, B3;
    GetAllCorners(A0, A1, A2, A3, B0, B1, B2, B3);

    FVector intersectPoint;
    FQuadIntersectFrame frame;
    //left side
    frame.Setup(A1,B1,B0, A0);
    if(frame.DoesIntersect(origin, dir, intersectPoint)){
        return true;
    }

    //right side
    frame.Setup(A3,B3,B2, A2);
    if(frame.DoesIntersect(origin, dir, intersectPoint)){
        return true;
    }

    //front side
    frame.Setup(A0,B0,B3,A3);
    if(frame.DoesIntersect(origin, dir, intersectPoint)){
        return true;
    }

    //back side
    frame.Setup(A2,B2,B1,A1);
    if(frame.DoesIntersect(origin, dir, intersectPoint)){
        return true;
    }

    //bottom side
    frame.Setup(A1,A0,A3,A2);
    if(frame.DoesIntersect(origin, dir, intersectPoint)){
        return true;
    }

    //top side
    frame.Setup(B0,B1,B2,B3);
    if(frame.DoesIntersect(origin, dir, intersectPoint)){
        return true;
    }

    return false;
    
}


bool BoundingBoxSimple::InRange(float t, float lower, float higher){
    return t >= lower && t <= higher;
}



#include "DebugPlugin/DebugHelper.h"
void BoundingBoxSimple::Test(){
    TArray<FVector> vertecies = {
        FVector(0, 0, 0),
        FVector(100, 100, 100)
    };

    BoundingBoxSimple instance(vertecies);

    //should be in bound ?
    FVector start(-50,50,50);
    FVector dir(200,0,0);
    FString inBoundResult = instance.DoesIntersect(start, dir) ? TEXT("inBound") : TEXT("outOfBound");

    FString message = FString::Printf(
        TEXT("BoundingBoxSimple::Test %s bound s(%s) dir(%s) RESULT(%s)"), 
        *instance.ToString(),
        *start.ToString(),
        *dir.ToString(),
        *inBoundResult
    );
    DebugHelper::logMessage(message);


    //should be in bound
    start = FVector(50,50,50);
    inBoundResult = instance.DoesIntersect(start, dir) ? TEXT("inBound") : TEXT("outOfBound");

    message = FString::Printf(
        TEXT("BoundingBoxSimple::Test %s in bound s(%s) dir(%s) RESULT(%s)"), 
        *instance.ToString(),
        *start.ToString(),
        *dir.ToString(),
        *inBoundResult
    );
    DebugHelper::logMessage(message);



    //should be in bound
    start = FVector(50,50,200);
    dir = FVector(0,0,-2000);
    inBoundResult = instance.DoesIntersect(start, dir) ? TEXT("inBound") : TEXT("outOfBound");

    message = FString::Printf(
        TEXT("BoundingBoxSimple::Test %s in bound s(%s) dir(%s) RESULT(%s)"), 
        *instance.ToString(),
        *start.ToString(),
        *dir.ToString(),
        *inBoundResult
    );
    DebugHelper::logMessage(message);


    //should be Not in bound
    start = FVector(200,200,200);
    dir = FVector(0,0,-2000);
    inBoundResult = instance.DoesIntersect(start, dir) ? TEXT("inBound") : TEXT("outOfBound");

    message = FString::Printf(
        TEXT("BoundingBoxSimple::Test %s in bound s(%s) dir(%s) RESULT(%s)"), 
        *instance.ToString(),
        *start.ToString(),
        *dir.ToString(),
        *inBoundResult
    );
    DebugHelper::logMessage(message);


    //should be in bound
    start = FVector(-200,-200,-200);
    dir = FVector(400,400,400);
    inBoundResult = instance.DoesIntersect(start, dir) ? TEXT("inBound") : TEXT("outOfBound");

    message = FString::Printf(
        TEXT("BoundingBoxSimple::Test %s in bound s(%s) dir(%s) RESULT(%s)"), 
        *instance.ToString(),
        *start.ToString(),
        *dir.ToString(),
        *inBoundResult
    );
    DebugHelper::logMessage(message);


    //should not be in bound
    start = FVector(-200,-200,-200);
    dir = FVector(400,-400,-400);
    inBoundResult = instance.DoesIntersect(start, dir) ? TEXT("inBound") : TEXT("outOfBound");

    message = FString::Printf(
        TEXT("BoundingBoxSimple::Test %s in bound s(%s) dir(%s) RESULT(%s)"), 
        *instance.ToString(),
        *start.ToString(),
        *dir.ToString(),
        *inBoundResult
    );
    DebugHelper::logMessage(message);
}





FVector BoundingBoxSimple::extent() const {
    //AB = B - A
    FVector result = topRightFar - bottomLeftNear;
    return result;
}

// IS TESTED ON UWIDGETCOMPONENT CUSTOM
void BoundingBoxSimple::Update(FKBoxElem *BoxElem) const {
    if(BoxElem){
        //copied from UWidgetComponent source.
        
        BoxElem->X = std::max(DepthX(), 10.0f);
        BoxElem->Y = std::max(WidthY(), 10.0f);
        BoxElem->Z = std::max(HeightZ(), 10.0f);



        BoxElem->SetTransform(FTransform::Identity);
        BoxElem->Center = center();
    }


    
}

float BoundingBoxSimple::WidthY() const {
    return std::abs(topRightFar.Y - bottomLeftNear.Y);
}

float BoundingBoxSimple::HeightZ() const {
    return std::abs(topRightFar.Z - bottomLeftNear.Z);
}

float BoundingBoxSimple::DepthX() const {
    return std::abs(topRightFar.X - bottomLeftNear.X);
}

FVector BoundingBoxSimple::center() const {
    return (topRightFar + bottomLeftNear) * 0.5f;
}



void BoundingBoxSimple::debugDrawBounds(const MMatrix &transform, UWorld *world, FColor color, float time){
    if(world){
        MMatrix mRaw = transform;
        MMatrix scale;
        scale.scaleUniform(1.1f);
        MMatrix M = mRaw * scale; //<-- lese richtung --

        FVector A0, A1, A2, A3, B0, B1, B2, B3;
        GetAllCorners(A0, A1, A2, A3, B0, B1, B2, B3, M);
        DebugHelper::showLineBetween(world, A0, A1, color, time);
        DebugHelper::showLineBetween(world, A1, A2, color, time);
        DebugHelper::showLineBetween(world, A2, A3, color, time);
        DebugHelper::showLineBetween(world, A3, A0, color, time);

        DebugHelper::showLineBetween(world, B0, B1, color, time);
        DebugHelper::showLineBetween(world, B1, B2, color, time);
        DebugHelper::showLineBetween(world, B2, B3, color, time);
        DebugHelper::showLineBetween(world, B3, B0, color, time);

        DebugHelper::showLineBetween(world, A0, B0, color, time);
        DebugHelper::showLineBetween(world, A1, B1, color, time);
        DebugHelper::showLineBetween(world, A2, B2, color, time);
        DebugHelper::showLineBetween(world, A3, B3, color, time);
    }
}