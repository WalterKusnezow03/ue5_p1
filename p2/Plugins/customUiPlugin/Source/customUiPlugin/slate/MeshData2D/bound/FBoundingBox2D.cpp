#include "FBoundingBox2D.h"

FBoundingBox2D::FBoundingBox2D(){
    topLeft = FVector2D(0, 0);
    bottomRight = FVector2D(0, 0);
}
FBoundingBox2D::~FBoundingBox2D(){}

FBoundingBox2D::FBoundingBox2D(
    TArray<FVector2D> &buffer
) {
    topLeft = FVector2D(0, 0);
    bottomRight = FVector2D(0, 0);
    Update(buffer);
}

FBoundingBox2D::FBoundingBox2D(const FBoundingBox2D & other){
    if(this != &other){
        *this = other;
    }
}

FBoundingBox2D &FBoundingBox2D::operator=(const FBoundingBox2D & other){
    if(this != &other){
        topLeft = other.topLeft;
        bottomRight = other.bottomRight;
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
    topLeft.X = std::min(topLeft.X, pos.X);
    topLeft.Y = std::min(topLeft.X, pos.X);

    bottomRight.X = std::max(bottomRight.X, pos.X);
    bottomRight.Y = std::max(bottomRight.X, pos.X);
}


float FBoundingBox2D::sizeX(){
    //AB = B - A
    return std::abs(bottomRight.X - topLeft.X);
}

float FBoundingBox2D::sizeY(){
    //AB = B - A
    return std::abs(bottomRight.Y - topLeft.Y);
}