#include "MeshedSurfaceGrid.h"
#include "MeshDataPlugin/Public/MeshGenBase/MeshData/BoundingBox/BoundingBoxSimple.h"
#include "terrainPluginBase/BaseTerrainInterface/TerrainInterfaceBase.h"

FMeshedSurfaceGrid::FMeshedSurfaceGrid(){

}

FMeshedSurfaceGrid::~FMeshedSurfaceGrid(){

}

FMeshedSurfaceGrid::FMeshedSurfaceGrid(const FMeshedSurfaceGrid &other){
    if(this != &other){
        *this = other;
    }
}
FMeshedSurfaceGrid &FMeshedSurfaceGrid::operator=(const FMeshedSurfaceGrid &other){
    if(this != &other){
        flagGrid = other.flagGrid;
        positionGrid = other.positionGrid;
        edgeIndices = other.edgeIndices;

        minSaved = other.minSaved;
        maxSaved = other.maxSaved;
        stepSizeSaved = other.stepSizeSaved;
        
    }   
    return *this;
}


void FMeshedSurfaceGrid::UpdateHeights(TerrainInterfaceBase *creator){
    if(!creator){
        return;
    }
    if(!GridValid()){
        return;
    }
    float offset = 30.0f;
    for (int i = 0; i < flagGrid.Num(); i++)
    {
        TArray<bool> &buffer = flagGrid[i];
        for (int j = 0; j < buffer.Num(); j++){
            if(buffer[j]){
                UpdateHeight(creator, positionGrid[i][j], offset);
            }
        }
    }
}

void FMeshedSurfaceGrid::UpdateHeight(TerrainInterfaceBase *creator, FVector &vertex, float offset){
    if(creator){
        float zUpdate = creator->getHeightFor(vertex);
        vertex.Z = zUpdate + offset;
    }
}





/// --- finding shapes to fit ---


/*
We need to iterate over the edge and find some
which matches the desired width x
Once found, the rotated area must be checked for all flags false
updated and then return whether the result was sucessfull
*/

// is NOT tested!
bool FMeshedSurfaceGrid::FindShape(
    int x, //in cm
    int y, //in cm
    FVector &outBottomLeft,
    FVector &outRotation
){
    //align rotation included, y is always depth, x side

    for (int i = 0; i < edgeIndices.Num(); i++){
        int j = (i + 1) % edgeIndices.Num();

        const std::pair<int, int> &posA = edgeIndices[i];
        const std::pair<int, int> &posB = edgeIndices[j];
        if(DistanceEnough(edgeIndices[i], edgeIndices[j], x)){
            
            FVector bottomLeft = GetPositionAt(posA);
            FVector rotation = Rotation(posA, posB);
            if(LockArea(posA, posB, rotation, y)){ //y depth along edge to inside
                return true;
            }
        }
    }
    return false;
}

bool FMeshedSurfaceGrid::DistanceEnough(
    const std::pair<int, int> &posA,
    const std::pair<int, int> &posB,
    int desired
){
    return Distance(posA, posB) <= desired;
}

int FMeshedSurfaceGrid::Distance(
    const std::pair<int, int> &posA,
    const std::pair<int, int> &posB
){
    FVector convertedA(posA.first, posA.second, 0.0f);
    FVector convertedB(posB.first, posB.second, 0.0f);
    return FVector::Dist(convertedA, convertedB);
}




// lock area 
bool FMeshedSurfaceGrid::LockArea(
    const std::pair<int, int> &posA,
    const std::pair<int, int> &posB,
    FVector &rotation,
    int depth //in cm
){
    /*
    prüfen ob die vertecies an den interpolierten 
    stellen frei sind
    */
    FVector orthogonal(rotation.Y, -rotation.X, 0.0f);
    orthogonal = orthogonal.GetSafeNormal() * depth;

    FVector aPosVector = GetPositionAt(posA);
    FVector bPosVector = GetPositionAt(posB);

    FVector extendedA = aPosVector + orthogonal;
    FVector extendedB = bPosVector + orthogonal;


    // ---- MOVE TO FVECTOR UTIL ? ----
    FVector dirStepAB;
    int stepsAB = 0;
    GenerateStepDirectionForInterpolation(aPosVector, bPosVector, stepsAB, dirStepAB);

    FVector dirStepAextendA;
    int stepsAextendA = 0;
    GenerateStepDirectionForInterpolation(aPosVector, bPosVector, stepsAextendA, dirStepAextendA);


    //bilinear interpolieren und prüfen! 
    TArray<std::pair<int, int>> markedForLock;
    for (int i = 0; i < stepsAB + 1; i++)
    {
        //step along ab
        FVector stepABWorld = aPosVector + dirStepAB * i;
        for (int j = 0; j < stepsAextendA + 1; j++){
            FVector stepLocal = dirStepAextendA * j;
            FVector bilinearInterpolate = stepABWorld + stepLocal;

            //generate index
            int x = 0;
            int y = 0;
            ToIndexBounded(bilinearInterpolate, x, y);
            if(FlagAt(x,y)){
                //mark for remove
                markedForLock.Add(std::pair<int, int>(x, y));
            }else{
                //at least one is not enough
                return false;
            }
        }
    }
    SetFlag(markedForLock, false);
    return true;
}



FVector FMeshedSurfaceGrid::Rotation(
    const std::pair<int, int> &posAPair,
    const std::pair<int, int> &posBPair
){
    // --- TODO ---
    FVector posAVec = GetPositionAt(posAPair);
    FVector posBVec = GetPositionAt(posBPair);
    posAVec.Z = 0.0f;
    posBVec.Z = 0.0f;
    FVector dir = posBVec - posAVec; // AB = B - A
    return dir.GetSafeNormal();
}