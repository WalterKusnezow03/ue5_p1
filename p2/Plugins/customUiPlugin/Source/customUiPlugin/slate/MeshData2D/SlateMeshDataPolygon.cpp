#include "SlateMeshDataPolygon.h"

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
    meshData.ClearAmbientColors();
    meshData.AddAmbientUvColor(FVector2D(0, 0), color);
    meshData.AddAmbientUvColor(FVector2D(1, 0), color);
    meshData.AddAmbientUvColor(FVector2D(0, 1), color);
    meshData.AddAmbientUvColor(FVector2D(1, 1), color);
}

void SlateMeshDataPolygon::SetCursorColor(FLinearColor color){
    bDynamicCursorColorEnabled = true;
    meshData.UpdateCursorColor(color);
}
