#pragma once

#include "CoreMinimal.h"

//CHANGE PATH NEEDED!
#include "TerrainRoadPlugin/RoadGeneration/Road/RoadGrid/RoadQuad.h"
#include "TerrainRoadPlugin/RoadGeneration/Road/RoadGrid/RoadKey.h"
#include "TerrainRoadPlugin/RoadGeneration/Road/RoadGrid/RoadKeyEnum.h"

#include "terrainPluginBase/BaseTerrainInterface/bezier/extendedVersion/TBSplinePosition.h"

#include "terrainPluginBase/BaseTerrainInterface/TerrainMeshDataParsingBase/ChunkParserInterfaceBase.h"
#include "terrainPluginBase/BaseTerrainInterface/TerrainMeshDataParsingBase/ChunkParserMapInterfaceBase.h"

#include "TerrainRoadPlugin/RoadGeneration/Road/RoadGrid/RoadQuadMeshedSurface/MeshedSurfaceGrid.h"

#include "CoreMath/Matrix/MMatrix.h"
#include <map>

/// -- 
/// creates a road mesh from a grid, displacing vertecies
/// and then interpolates with bsplines.
/// will also store the closed polygons for building areas
/// --

class TerrainInterfaceBase;

class TERRAINROADPLUGIN_API RoadMakerFromGrid {

public:
    RoadMakerFromGrid();
    ~RoadMakerFromGrid();

    ///create mesh grid with respect to step size and internal quad mesh for later extraction
    ///of polygons
    void CreateGrid(FVector2D size, float StepSize);

    // --- warp grid with different functions ---
    void WarpCirlceRandom();
    

    // --- build --- (no warp allowed anymore)
    void Build(
        TerrainInterfaceBase *creator, 
        float _einheitsValueForBsplineStepSize, 
        float roadWidth,
        ChunkParserMapInterfaceBase &map
    ); //einheitsvalue: ditsance between nodes interpolated

    // -- todo --
    //for creation of buildings
    //TArray<MPolygon> PolygonsForBuildingFit();

    

    TArray<FMeshedSurfaceGrid *> GetMeshedSurfaces();

protected:
    // ----- Base Properties -----

    //raw mesh 
    TArray<TArray<FVector>> mesh;
    
    //helper properites for distortion

    /// @brief center of mesh after setup
    FVector centerSaved;

    /// @brief original property of size on setup
    FVector2D sizeSaved;
    float stepSizeSavedBetweenMeshNodes = 0.0f; // on setup stepsize
    void SaveOriginalProperties(
        FVector2D &size,
        float stepSize
    );
    FVector &CenterOfMesh();
    void UpdateCenterOfMesh();

    // ----- private warp helpers -----
    TArray<FVector> RandomPositions(
        int maxCount, 
        float distanceFromEdges
    );
    bool RandomPosition(
        FVector2D &minPoisiton,
        FVector2D &maxPosition,
        FVector &outpos
    );
    bool RandomPosition(
        int iLower,
        int jLower,
        int iHigher,
        int jHigher,
        FVector &outpos
    );
    

    // ----- private warp -----
    void WarpCirlceByDistanceAroundCenter(
        float maxDistance,
        float angle
    );
    void WarpCirlceByDistance(
        FVector &center,
        float maxDistance,
        float angle
    );

    void MakeRotationAroundPosition(
        FVector &position,
        float angle,
        MMatrix &outMatrix
    );

    // ----- builded mesh -----
    bool wasBuilded = false;
    TArray<TArray<RoadQuad>> buildedMeshQuads; //any shape, but originally the quads
    RoadQuad fallback;

public:

    //if was build, get
    const TArray<TArray<RoadQuad>> &GetBuildedMeshQuads(){
        return buildedMeshQuads;
    }


protected:
    
    RoadQuad &GetQuadInBuildedMesh(std::pair<int, int> &posXY);
    RoadQuad &GetQuadInBuildedMesh(int i, int j);
    


    void ComputeAllYAxis(float _einheitsValue);
    void ComputeAllXAxis(float _einheitsValue);
    void ComputeYAxis(int index, float _einheitsValue);
    void ComputeXAxis(int index, float _einheitsValue);

    //builded sections unadded
    std::map<FRoadKey, TArray<FVectorBSplinePosition>> buildedSections;
    TArray<FVectorBSplinePosition> &GetSection(int index, ERoadKeyEnum key);
    TArray<FVectorBSplinePosition> &GetSection(FRoadKey &key);

    
    //as (x,y) pairs for copying axis section parts
    TArray<std::pair<int, int>> GetAllQuadShapedIndices();

    ///where the original array had its anchor (original grid), find that index
    //inside the passed array (0,1,2,3,x<-target,...) - finds: 4
    int IndexInArrayWhereOriginalAnchorIsIndex(
        int targetAnchor,
        TArray<FVectorBSplinePosition> &completeSection
    );

    TArray<FVectorBSplinePosition> CopySection(
        std::pair<int,int> &pos0,
        std::pair<int,int> &pos1
    );

    TArray<FVectorBSplinePosition> CopySection(
        ERoadKeyEnum axis,
        int axisIndex,
        int a,
        int b
    );
    TArray<FVectorBSplinePosition> CopySection(
        TArray<FVectorBSplinePosition> &array,
        int i0,
        int i1
    );

    void CopySectionTo(
        std::pair<int, int> &pos0,
        std::pair<int, int> &pos1,
        RoadQuad &quad
    );

    void Build(float detail);
    void BuildAllPolygons();
    void ApplyHeightOnRawMesh(TerrainInterfaceBase *creator);
    void ValidateSize(FVector2D &size, float &stepSize);

    /// --- update terrain and chunk parsers. ---
    void LockTerrainFromGeneratedRoadQuads(TerrainInterfaceBase *creator, float roadWidth);

    void ApplyTerrain2DIndexToRoadQuadsAndRemoveTerrainOffset(
        TerrainInterfaceBase *creator
    );
    void ApplyTerrain2DIndexToRoadQuads(
        TerrainInterfaceBase *creator);
    void RemoveTerrainOffsetFromRoadQuads(
        TerrainInterfaceBase *creator
    );
    void ApplyHeightOnRoadQuads(TerrainInterfaceBase *creator);
    void ApplyHeightForPostions(TArray<FVector> &array, TerrainInterfaceBase *creator);

    void AddRoadQuadsMeshDataToChunks(ChunkParserMapInterfaceBase &map);
    void AddRoadQuadsMeshDataToChunk(
        ChunkParserInterfaceBase &chunk,
        RoadQuad &quad
    );



    // --- generate surface data for road polygons ---
    void GenerateQuadMeshedSurfaces(TerrainInterfaceBase *creator);
    void GenerateQuadMeshedSurfacesDebugMesh();
};
