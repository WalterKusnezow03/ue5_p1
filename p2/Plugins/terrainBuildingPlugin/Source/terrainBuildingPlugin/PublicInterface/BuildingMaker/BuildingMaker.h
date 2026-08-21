#pragma once

#include "CoreMinimal.h"
#include "terrainPluginBase/BaseTerrainInterface/TerrainMeshDataParsingBase/ChunkParserMapInterfaceBase.h"
#include "TerrainRoadPlugin/RoadGeneration/Road/RoadGrid/RoadQuadMeshedSurface/MeshedSurfaceGrid.h"
#include "terrainBuildingPlugin/BoundsShared/BuildingBounds.h"

class TerrainInterfaceBase;

/// @brief pblic application Interface to launch building creations
/// from terrain plugin :: terrain creator
class TERRAINBUILDINGPLUGIN_API BuildingMaker {

public:
    void Build(
        TerrainInterfaceBase *creatorIn,
        ChunkParserMapInterfaceBase &mapIn, 
        TArray<FMeshedSurfaceGrid *> &buildingAreas
    );



private:    
    //to avoid overloaded functions
    void SetTemporaryPointers(
        TerrainInterfaceBase *creatorIn,
        ChunkParserMapInterfaceBase &mapIn
    );
    void ResetTemporaryPointers();

    TerrainInterfaceBase *creator = nullptr;
    ChunkParserMapInterfaceBase *map = nullptr;
    //to avoid overloaded functions



    void BuildSingleGrid(
        FMeshedSurfaceGrid *grid,
        int buildingsWanted
    );

    void GetBuildingSizes(TArray<BuildingBounds> &outBoundsType);

    void PrepareFittedBounds(
        TArray<BuildingBounds> &boundsType,
        TArray<BuildingBounds> &outPreparedBounds,
        FMeshedSurfaceGrid *grid,
        int buildingsWanted
    );

    bool FindShape(
        FMeshedSurfaceGrid *grid,
        const BuildingBounds &prefab,
        BuildingBounds &outBounds
    );

    
    /// lock trees from the created bounds
    void LockTerrainFromQuadBounds(
        TArray<BuildingBounds> &preparedBounds
    );

    //cube generation for buildings (DEBUG)
    void CreateDebugCubesFromBoundsCreated(
        TArray<BuildingBounds> &preparedBounds
    );
    void CreateDebugCubesFromBoundCreated(
        BuildingBounds &preparedBound
    );

    //add created bounds to the collected nodes 
    //as polygons
    void AddBoundsToPathFinderGeometry(
        TArray<BuildingBounds> &preparedBounds
    );
    void AddBoundsToPathFinderGeometry(
        BuildingBounds &bound
    );



    

    void DebugAppendHull(
        FMeshedSurfaceGrid *grid
    );
};
