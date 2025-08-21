#pragma once
#include "CoreMinimal.h"

#include "GameCore/util/FVectorTouple.h"
#include "AssetPlugin/gamestart/assetEnums/materialEnum.h"
#include "GameCore/MeshGenBase/MeshData/MeshData.h"
#include "GameCore/MeshGenBase/foliage/ETerrainType.h"
#include "terrainPlugin/meshgen/generation/helper/TerrainChunkSetup.h"
#include "terrainPlugin/meshgen/foliage/MatrixTree/MatrixTree.h"

#include "GameCore/MeshGenBase/MeshData/MeshData.h"
#include "GameCore/MeshGenBase/lodHelper/MeshDataPair/MeshDataMap.h"

#include <map>


/// @brief will parse a chunk to a terrain mesh data 
/// with all its layers for the custom mesh actor to use
/// will also write immidiate to disk
/// will be able to update a mesh actor
class TERRAINPLUGIN_API ChunkParser {

public:
    ChunkParser();
    ~ChunkParser();

    /// @brief makes setup chunk parser package for a mesh actor to use. Needs setup package and 
    /// targeted actor location!
    /// @param positionChunkActorLocation 
    /// @param package 
    void createTerrainFrom2DMap(FVector &positionChunkActorLocation, TerrainChunkSetup &package);

    FVector GetActorLocation();

    void setChunkId(int inId);

    //do not modify, do not delete, read only. Might be nullptr
    //might get invalid if other meshdata is added. Dont.
    MeshDataMap *findMeshDataMap(ELod lod);

    //can add nodes to nav mesh a single time!
    void addNodesToNavMeshIfNeeded(UWorld *world);

    //flag is default FALSE
    void flagWaterActorNeeded(FVector &location);
    bool WaterActorNeededFlagged();
    FVector GetWaterActorLocation();

    ///mark used by actor to prevent mesh update if not needed
    void SetUsedMeshDataByActorFlag(bool flag);

    /// will tell whether in use by actor is flagged
    bool IsUsedByActor();

private:
    int chunkId = 0;
    bool navmeshNodesAdded = false;

    bool waterActorNeeded = false;
    FVector waterActorLocation;

    bool currentlyUsedByActor = false;
    FVector actorLocation;

    std::vector<FVector> navmeshNodes;
    std::vector< std::vector<FVector>> navmeshConvexHulls;
    ETerrainType thisTerrainType;
    MatrixTree tree;

    // MUST BE REFACTURED WITHOUT COMPONENT PTR; A BASE CLASS!
    std::map<ELod, MeshDataMap> meshLodContainers;

    void createTerrainFrom2DMap(
        std::vector<std::vector<FVector>> &map
    );
    void appendLodTerrain(
        std::vector<std::vector<FVector>> &map,
        MeshData &grassLayer,
        MeshData &stoneLayer,
        int stepSize
    );

    void filterTouplesForVerticalVectors(
        TArray<FVectorTouple> &touples,
        std::vector<FVector> &potentialLocations
    );

    void createFoliageAndPushNodesAroundFoliageToNavMesh(
        TArray<FVectorTouple> &touples,
        float treeDensitySkalar
    );

    void createTreeAndSaveToMesh(FVector &location);

    void addRandomNodesToNavmesh(TArray<FVectorTouple> &touples);

    MeshData &findMeshDataReference(
        materialEnum type,
        ELod lodLevel,
        bool raycastOnLayer
    );

    std::vector<ELod> lodVector(){
        std::vector<ELod> types = {
            ELod::lodNear,
            ELod::lodMiddle,
            ELod::lodFar
        };
        return types;
    }
};