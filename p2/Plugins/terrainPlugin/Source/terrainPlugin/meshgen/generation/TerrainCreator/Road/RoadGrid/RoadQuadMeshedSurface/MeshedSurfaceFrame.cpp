#include "MeshedSurfaceFrame.h"
#include "terrainPlugin/meshgen/generation/TerrainCreator/terrainCreator.h"
#include "GameCore/MeshGenBase/MeshData/MeshData.h"


FMeshedSurfaceFrame::FMeshedSurfaceFrame(){

}

FMeshedSurfaceFrame::~FMeshedSurfaceFrame(){

}

FMeshedSurfaceFrame::FMeshedSurfaceFrame(const FMeshedSurfaceFrame &other){
    if(this != &other){
        *this = other;
    }
}

//copied from Quad SurfaceFrame copy constructor
FMeshedSurfaceFrame &FMeshedSurfaceFrame::operator=(const FMeshedSurfaceFrame &other){
    if(this != &other){
        v0 = other.v0;
        v1 = other.v1;
        v2 = other.v2;
        v3 = other.v3;
    }
    return *this;
}

FMeshedSurfaceFrame::FMeshedSurfaceFrame(
    const FVector &v0In,
    const FVector &v1In,
    const FVector &v2In,
    const FVector &v3In
){
    Setup(
        v0In,
        v1In,
        v2In,
        v3In
    );
}


void FMeshedSurfaceFrame::Setup(
    const FVector &v0In,
    const FVector &v1In,
    const FVector &v2In,
    const FVector &v3In
){
    v0 = v0In;
    v1 = v1In;
    v2 = v2In;
    v3 = v3In;
}

void FMeshedSurfaceFrame::UpdateHeightOnVertecies(terrainCreator *creator){

    float heightOffset = 30.0f;
    UpdateHeight(creator, v0, heightOffset);
    UpdateHeight(creator, v1, heightOffset);
    UpdateHeight(creator, v2, heightOffset);
    UpdateHeight(creator, v3, heightOffset);
}

void FMeshedSurfaceFrame::UpdateHeight(terrainCreator *creator, FVector &vertex, float offset){
    if(creator){
        float zUpdate = creator->getHeightFor(vertex);
        vertex.Z = zUpdate + offset;
    }
}
    



void FMeshedSurfaceFrame::AppendMeshedSurface(MeshData &data){
    data.append(v0,v1,v2,v3);
}

