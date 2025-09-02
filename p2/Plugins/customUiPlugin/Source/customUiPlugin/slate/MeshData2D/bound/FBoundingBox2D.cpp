#include "FBoundingBox2D.h"

FBoundingBox2D::FBoundingBox2D(){
    bottomLeft = FVector2D(0, 0);
    topRight = FVector2D(0, 0);
}
FBoundingBox2D::~FBoundingBox2D(){}

FBoundingBox2D::FBoundingBox2D(
    TArray<FVector2D> &buffer
) {
    bottomLeft = FVector2D(0, 0);
    topRight = FVector2D(0, 0);
    Update(buffer);
}

FBoundingBox2D::FBoundingBox2D(const FBoundingBox2D & other){
    if(this != &other){
        *this = other;
    }
}

FBoundingBox2D &FBoundingBox2D::operator=(const FBoundingBox2D & other){
    if(this != &other){
        topRight = other.topRight;
        bottomLeft = other.bottomLeft;
    }
    return *this;
}

void FBoundingBox2D::Update(TArray<FVector2D> &buffer){
    for (int i = 0; i < buffer.Num(); i++){
        Update(buffer[i]);
    }
}

void FBoundingBox2D::Update(FVector2D &v0, FVector2D &v1, FVector2D &v2){
    Update(v0);
    Update(v1);
    Update(v2);
}

void FBoundingBox2D::Update(FVector2D &pos){
    bottomLeft.X = std::min(bottomLeft.X, pos.X);
    bottomLeft.Y = std::min(bottomLeft.X, pos.X);

    topRight.X = std::max(topRight.X, pos.X);
    topRight.Y = std::max(topRight.X, pos.X);
}


float FBoundingBox2D::sizeX(){
    //AB = B - A
    return topRight.X - bottomLeft.X;
}

float FBoundingBox2D::sizeY(){
    //AB = B - A
    return topRight.Y - bottomLeft.Y;
}