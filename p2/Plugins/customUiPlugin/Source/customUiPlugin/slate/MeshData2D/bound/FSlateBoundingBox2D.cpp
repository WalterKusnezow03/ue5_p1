#include "FSlateBoundingBox2D.h"

#include "customUiPlugin/slate/MeshData2D/SlateMeshData.h"
#include "customUiPlugin/slate/MeshData2D/SlateMeshDataPolygon.h"

FSlateBoundingBox2D::FSlateBoundingBox2D(){
    Reset();
}
FSlateBoundingBox2D::~FSlateBoundingBox2D(){}

FSlateBoundingBox2D::FSlateBoundingBox2D(const FSlateBoundingBox2D & other){
    if(this != &other){
        *this = other;
    }
}


FSlateBoundingBox2D &FSlateBoundingBox2D::operator=(const FSlateBoundingBox2D & other){
    if(this != &other){
        FBoundingBox2D::operator=(other);
    }
    return *this;
}

// --- slate mesh data update ---
void FSlateBoundingBox2D::Update(const SlateMeshData &ref){
    const TArray<FVector2D> &buffer = ref.VerteciesRefConst();
    Update(buffer);
}

void FSlateBoundingBox2D::Update(const SlateMeshDataPolygon &ref){
    const SlateMeshData &refMeshData = ref.MeshDataRefConst();
    Update(refMeshData);
}