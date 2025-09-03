#include "FBoundingBox2D.h"

#include "customUiPlugin/slate/MeshData2D/SlateMeshData.h"
#include "customUiPlugin/slate/MeshData2D/SlateMeshDataPolygon.h"

FBoundingBox2D::FBoundingBox2D(){
    Reset();
}
FBoundingBox2D::~FBoundingBox2D(){}

void FBoundingBox2D::Reset(){
    topLeft = FVector2D(0, 0);
    bottomRight = FVector2D(0, 0);
}

FBoundingBox2D::FBoundingBox2D(const TArray<FVector2D> &buffer){
    Reset();
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

void FBoundingBox2D::Update(const TArray<FVector2D> &buffer){
    for (int i = 0; i < buffer.Num(); i++){
        Update(buffer[i]);
    }
}

void FBoundingBox2D::Update(const FVector2D &v0, const FVector2D &v1, const FVector2D &v2){
    Update(v0);
    Update(v1);
    Update(v2);
}

void FBoundingBox2D::Update(const FVector2D &pos){
    topLeft.X = std::min(topLeft.X, pos.X);
    topLeft.Y = std::min(topLeft.Y, pos.Y);

    bottomRight.X = std::max(bottomRight.X, pos.X);
    bottomRight.Y = std::max(bottomRight.Y, pos.Y);

}


FVector2D FBoundingBox2D::size() const {
    return FVector2D(sizeX(), sizeY());
}

float FBoundingBox2D::sizeX()const{
    //AB = B - A
    return std::abs(bottomRight.X - topLeft.X);
}

float FBoundingBox2D::sizeY()const{
    //AB = B - A
    return std::abs(bottomRight.Y - topLeft.Y);
}



// --- slate mesh data update ---
void FBoundingBox2D::Update(const SlateMeshData &ref){
    const TArray<FVector2D> &buffer = ref.VerteciesRefConst();
    Update(buffer);
}

void FBoundingBox2D::Update(const SlateMeshDataPolygon &ref){
    const SlateMeshData &refMeshData = ref.MeshDataRefConst();
    Update(refMeshData);
}