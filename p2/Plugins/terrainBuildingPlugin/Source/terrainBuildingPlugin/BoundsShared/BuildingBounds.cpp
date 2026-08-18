#include "BuildingBounds.h"
#include "MeshDataPlugin/Public/MeshGenBase/MeshData/MeshData.h"

void BuildingBounds::Setup(int x, int y){
    sizeX = std::max(1, std::abs(x));
    sizeY = std::max(1, std::abs(y));
}

void BuildingBounds::Setup(int x, int y, int z){
    Setup(x, y);
    SetHeight(z);
}

void BuildingBounds::Setup(const BuildingBounds &other){
    Setup(other.sizeX, other.sizeY, other.sizeZ);
}

void BuildingBounds::SetHeight(int z){
    sizeZ = std::max(1, std::abs(z));
}


int BuildingBounds::getSizeX() const {
    return sizeX;
}

int BuildingBounds::getSizeY() const {
    return sizeY;
}

void BuildingBounds::AssignTransform(FVector &posWorld, FRotator &rotation){
    assignedBottomLeftWorld = posWorld;
    assignedRoation = rotation;
    UpdateTransform();
}

void BuildingBounds::UpdateTransform(){
    MMatrix tWorld;
    tWorld.setTranslation(assignedBottomLeftWorld);

    MMatrix r;
    r.setRotation(assignedRoation);

    //M = T * R <-- lese richtung, erst rotieren
    transform = tWorld * r;

    
}

void BuildingBounds::GetOverrideBoundingVerteciesWorld(TArray<FVector> &outArray){
    outArray.SetNum(4);
    BoundVerteciesRaw(outArray);
    TransformAll(outArray, transform);
}

void BuildingBounds::BoundVerteciesRaw(TArray<FVector> &outArray){
    outArray.SetNum(4);
    /*
    1-->2
    |   |
    0<--3
    */

    //x side, y depth
    outArray[0] = FVector(0, 0, 0);
    outArray[1] = FVector(0, sizeY, 0);
    outArray[2] = FVector(sizeX, sizeY, 0);
    outArray[3] = FVector(sizeX, 0, 0);

}


void BuildingBounds::TransformAll(TArray<FVector> &outArray, MMatrix &M){
    for (int i = 0; i < outArray.Num(); i++){
        FVector local = outArray[i];
        outArray[i] = M * local; //<-- lese richtung -- M * p
    }
}

void BuildingBounds::AppendDebugCube(MeshData &data, FVector &offset){
    AppendDebugCube(data, offset, 1.0f);
}

void BuildingBounds::AppendDebugCube(MeshData &data, FVector &offset, float debugScale){
    FVector up(0, 0, 300); //debug very high

    TArray<FVector> bottomQuad;
    BoundVerteciesRaw(bottomQuad);

    MMatrix tOffset(offset);
    MMatrix s;
    s.scaleUniform(debugScale);
    

    //M = tOff*(TR)* S
    MMatrix TRS = transform * s;
    MMatrix M = tOffset * TRS;
    TransformAll(bottomQuad, M);


    up *= debugScale;
    data.appendCube(bottomQuad, up);
}

const FVector &BuildingBounds::GetPivot(){
    return assignedBottomLeftWorld;
}