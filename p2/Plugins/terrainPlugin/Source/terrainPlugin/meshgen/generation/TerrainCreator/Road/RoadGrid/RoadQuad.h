#pragma once

#include "CoreMinimal.h"
#include "terrainPlugin/meshgen/generation/bezier/extendedVersion/TBSplinePosition.h"
#include "GameCore/MeshGenBase/MeshData/MeshData.h"
#include "terrainPlugin/meshgen/generation/TerrainCreator/Road/RoadGrid/RoadQuadMeshedSurface/RoadQuadMeshedSurface.h"


class TERRAINPLUGIN_API RoadQuad {

public:
    RoadQuad();
    ~RoadQuad();

    RoadQuad(const RoadQuad &other);
    RoadQuad &operator=(const RoadQuad &other);

    //modify by roadmakerfrom grid
    void Append(TArray<FVectorBSplinePosition> &vertecies);
    void Append(TArray<FVector> &vertecies);


    //meshdata generation
    void AppendRoadMesh(MeshData &data, float width); //if not generated yet by width
    void AppendRoadMesh(MeshData &data); //if inner circle already generated
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
    void GenerateMeshedSurface(terrainCreator *creator, float widthOfInsideStep);

    //debug
    void AppendMeshedSurface(MeshData &data);

protected:
    bool wasModified = false;
    TArray<FVector> circle;

    TArray<FVector> innerCircle;

    //saved to push mesh data to chunkparsermap -> chunk parser at index.
    std::pair<int, int> chunkindexInTerrain;

    void UpdateCenter();
    FVector center;

    void GenerateParalellRoad(float width);

    RoadQuadMeshedSurface meshedSurface;
    
};
