#include "SlateMeshDataPolygon.h"
#include "CoreMath/Matrix/2D/MMatrix2D.h"

SlateMeshDataPolygon::SlateMeshDataPolygon(){

}

SlateMeshDataPolygon::~SlateMeshDataPolygon(){

}

void SlateMeshDataPolygon::AppendClosedShape(TArray<FVector2D> &shape){
    meshData.AppendClosedShape(shape);
}

void SlateMeshDataPolygon::AppendClosedShape(TArray<FVector2D> &shape, int detail){
    meshData.AppendClosedShape(shape, detail);
}



SlateMeshData &SlateMeshDataPolygon::MeshDataRef(){
    return meshData;
}


const SlateMeshData &SlateMeshDataPolygon::MeshDataRefConst() const {
    return meshData;
}


bool SlateMeshDataPolygon::Tick(float deltatime){
    //color animations
    //mesh data animations
    return false;
}

void SlateMeshDataPolygon::UpdateCursorPosition(FVector2D &pos){
    meshData.UpdateCursorPosition(pos, bDynamicCursorColorEnabled);
}



void SlateMeshDataPolygon::SetColor(FLinearColor color){
    meshData.SetFullColor(color);
}

void SlateMeshDataPolygon::AddAmbientUvColor(FVector2D uv, FLinearColor color){
    meshData.AddAmbientUvColor(uv, color);
}

void SlateMeshDataPolygon::SetCursorColor(FLinearColor color){
    bDynamicCursorColorEnabled = true;
    meshData.UpdateCursorColor(color);
}


void SlateMeshDataPolygon::ApplyTransformImmidiate(MMatrix2D &transform){
    meshData.ApplyTransformationImmidiate(transform);
}