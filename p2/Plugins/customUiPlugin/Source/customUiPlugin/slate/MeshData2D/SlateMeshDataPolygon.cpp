#include "SlateMeshDataPolygon.h"

SlateMeshDataPolygon::SlateMeshDataPolygon(){

}

SlateMeshDataPolygon::~SlateMeshDataPolygon(){

}

void SlateMeshDataPolygon::AppendClosedShape(TArray<FVector2D> &shape){
    meshData.AppendClosedShape(shape);
}

SlateMeshData &SlateMeshDataPolygon::MeshDataRef(){
    return meshData;
}


const SlateMeshData &SlateMeshDataPolygon::MeshDataRefConst() const {
    return meshData;
}