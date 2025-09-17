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



void SlateMeshDataPolygon::SetFullColor(FLinearColor color){
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

void SlateMeshDataPolygon::SetRuntimeTransformation(MMatrix2D &transform){
    meshData.SetRuntimeTransformation(transform);
}




/// @brief marks for not filled drawing
/// USE FOR DEBUG ONLY
void SlateMeshDataPolygon::DrawOutLineOnly(){
    bDrawOutlineOnly = true;
}

bool SlateMeshDataPolygon::IsFlaggedDrawOutlineOnly() const {
    return bDrawOutlineOnly;
}



//text
const SlateTextBase &SlateMeshDataPolygon::GetSlateTextConst() const{
    return internalText;
}

SlateTextBase &SlateMeshDataPolygon::GetSlateText(){
    return internalText;
}

bool SlateMeshDataPolygon::bHasText()const{
    return internalText.Lenght() > 0;
}

void SlateMeshDataPolygon::SetText(FString textIn){
    internalText.SetText(textIn);
}