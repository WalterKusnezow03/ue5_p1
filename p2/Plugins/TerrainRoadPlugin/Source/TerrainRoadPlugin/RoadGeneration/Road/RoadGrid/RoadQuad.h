#pragma once

#include "CoreMinimal.h"
#include "terrainPluginBase/BaseTerrainInterface/bezier/extendedVersion/TBSplinePosition.h"
#include "MeshDataPlugin/Public/MeshGenBase/MeshData/MeshData.h"
#include "TerrainRoadPlugin/RoadGeneration/Road/RoadGrid/RoadQuadMeshedSurface/RoadQuadMeshedSurface.h"

class TERRAINROADPLUGIN_API RoadQuad {

public:
    RoadQuad();
    ~RoadQuad();

    RoadQuad(const RoadQuad &other);
    RoadQuad &operator=(const RoadQuad &other);

    //modify by roadmakerfrom grid
    void Append(TArray<FVectorBSplinePosition> &vertecies);
    void Append(TArray<FVector> &vertecies);


    //meshdata generation
    //void AppendRoadMesh(MeshData &data, float width); //if not generated yet by width
    void AppendRoadMesh(MeshData &data, TerrainInterfaceBase *creator); //if inner circle already generated
    void GenerateParalellRoad(
        TArray<FVector> &outerOut,
        TArray<FVector> &innerOut,
        float width
    );

    //Paralell roads must be created first or meshdata.
    TArray<FVector> &GetCirlce();
    TArray<FVector> &GetInnerCircle();
    FVector &GetCenter();

    void UpdateChunkIndexInTerrain(std::pair<int,int> &indexIn){
        chunkindexInTerrain = indexIn;
    }

    std::pair<int, int> &GetChunkIndex(){
        return chunkindexInTerrain;
    }

    void RemoveOffset(FVector &offset);

    //meshed surface for building gen
    //generates the flag grid and the MeshedSurfaceGrid to align buildings on.
    void GenerateMeshedSurface(TerrainInterfaceBase *creator, float widthOfInsideStep);

    //debug
    void AppendMeshedSurface(MeshData &data);

    FMeshedSurfaceGrid *GetFMeshedSurfaceGridPointer(){
        return meshedSurface.GetFMeshedSurfaceGridPointer();
    }

protected:
    bool wasModified = false;
    TArray<FVector> circle;

    TArray<FVector> innerCircle;

    //saved to push mesh data to chunkparsermap -> chunk parser at index.
    std::pair<int, int> chunkindexInTerrain;

    void UpdateCenter();
    FVector center;

    void GenerateParalellRoad(float width);

    //meshed surface for building floor / and alignment
    RoadQuadMeshedSurface meshedSurface;
    
};
