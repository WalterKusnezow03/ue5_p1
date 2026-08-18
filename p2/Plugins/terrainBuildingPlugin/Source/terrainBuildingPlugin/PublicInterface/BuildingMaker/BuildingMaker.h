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
        TerrainInterfaceBase *creator,
        ChunkParserMapInterfaceBase &map, 
        TArray<FMeshedSurfaceGrid *> &buildingAreas
    );



private:
    void BuildSingleGrid(
        TerrainInterfaceBase *creator,
        ChunkParserMapInterfaceBase &map,
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

    

    void LockTerrainFromQuadBounds(
        TerrainInterfaceBase *creator,
        TArray<BuildingBounds> &preparedBounds
    );

    void CreateDebugCubesFromBoundsCreated(
        TerrainInterfaceBase *creator,
        ChunkParserMapInterfaceBase &map,
        TArray<BuildingBounds> &preparedBounds
    );
    void CreateDebugCubesFromBoundCreated(
        TerrainInterfaceBase *creator,
        ChunkParserMapInterfaceBase &map,
        BuildingBounds &preparedBound
    );

    void DebugAppendGrid(
        TerrainInterfaceBase *creator,
        ChunkParserMapInterfaceBase &map,
        FMeshedSurfaceGrid *grid
    );

    void DebugAppendHull(
        TerrainInterfaceBase *creator,
        ChunkParserMapInterfaceBase &map,
        FMeshedSurfaceGrid *grid
    );
};
