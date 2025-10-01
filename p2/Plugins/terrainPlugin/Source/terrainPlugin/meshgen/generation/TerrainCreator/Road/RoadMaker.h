#pragma once

#include "terrainPlugin/meshgen/generation/TerrainCreator/terrainConstants.h"
#include "terrainPlugin/meshgen/generation/bezierCurve.h"
#include "terrainPlugin/meshgen/generation/TerrainCreator/chunk.h"
#include "CoreMath/algorithm/PolygonFit/polygon/MPolygon.h"
#include "terrainPlugin/meshgen/generation/TerrainCreator/Road/InternalHelper/RoadData.h"
#include "terrainPlugin/meshgen/generation/TerrainCreator/Road/InternalHelper/Mapped/RoadIntersectionMapped.h"

class terrainCreator;
class TERRAINPLUGIN_API RoadMaker
{

public:
    RoadMaker();

    //--road--
	void createRoads(terrainCreator *ptr, UWorld *world, int chunks);
	

private:
    float getHeightFor(FVector2D &pos);

    void createRoads(
        MeshData &outmeshDataSurface,
        MeshData &outmeshDataSides,
        int count,
        int chunks
    );
    void createRoad(
        MeshData &outmeshDataSurface,
        MeshData &outmeshDataSides,
        int chunks
    );
    void processRoad(
        TVector<FVector2D> &curve,
        float roadWidth,
        MeshData &outmeshDataSurface,
        MeshData &outmeshDataSides,
        float _einheitsValue
    );
    FVector make3D(FVector2D &pos, float height);
    void lockQuadsFromParalellArrayLines(
        TArray<FVector> &line0,
        TArray<FVector> &line1
    );


    terrainCreator *terrainCreatorPtr = nullptr;
    AcustomMeshActor *getNewMeshActor();







    


    /// @brief finds intersection pairs between the curves, both must be in same logical space
    /// @param curveA 
    /// @param curveB 
    /// @param outIntersections indices to copy vertecies between, 
    /// to create convex like / hull polygons for building fitting
    void FindIntersections(
        TVector<FVector2D>& curveA, 
        TVector<FVector2D>& curveB,
        TArray<std::pair<int,int>> &outIntersections 
    );
    void AddCurveToCache(TVector<FVector2D> &curve);
    
    
    /// @brief cache to save all created curves, to find all intersection points between
    /// segments
    TArray<RoadData> createdRoadsCache;
    RoadIntersectionMapped roadIntersections;

    void CreatePolygonShapesForBuildingFittingBetweenRoadIntersections();
    void FindAllTwoRoadIntersections();


    /// @brief array to save polygons closed by roads
    TArray<MPolygon> createdPolygonsForBuildingFit;
};