#include "BuildingMaker.h"
#include "terrainPluginBase/BaseTerrainInterface/TerrainInterfaceBase.h"
#include "DebugPlugin/DebugHelper.h"


//to avoid overloaded functions
void BuildingMaker::SetTemporaryPointers(
    TerrainInterfaceBase *creatorIn,
    ChunkParserMapInterfaceBase &mapIn
){
    creator = creatorIn;
    map = &mapIn;
}

void BuildingMaker::ResetTemporaryPointers(){
    creator = nullptr;
    map = nullptr;
}

//to avoid overloaded functions

void BuildingMaker::Build(
    TerrainInterfaceBase *creatorIn,
    ChunkParserMapInterfaceBase &mapIn, 
    TArray<FMeshedSurfaceGrid *> &buildingAreas
){
    SetTemporaryPointers(creatorIn, mapIn);
    DebugHelper::logMessage(
        FString::Printf(
            TEXT("BuildingMaker::Build from grids: %d"), buildingAreas.Num()
        )
    );


    int buildingsWantedPerGrid = 50;
    for (int i = 0; i < buildingAreas.Num(); i++){
        if(FMeshedSurfaceGrid *current = buildingAreas[i]){
            BuildSingleGrid(current, buildingsWantedPerGrid);
        }
    }
    ResetTemporaryPointers();
}

void BuildingMaker::BuildSingleGrid(
    FMeshedSurfaceGrid *grid,
    int buildingsWanted
){
    if(!grid){
        return;
    }
    if(buildingsWanted <= 0){
        return;
    }

    // --- TODO ---
    //clean up hulls
    grid->SplitSelfCuttingPolygonsFromOuterHull(); //sort by area only.
    
    //hull valid for fit?
    if(!grid->CanFindShape()){
        return;
    }
    

    //make types
    TArray<BuildingBounds> boundsType;
    GetBuildingSizes(boundsType);

    //try fit all types (first fit)
    TArray<BuildingBounds> preparedBounds;
    PrepareFittedBounds(boundsType, preparedBounds, grid, buildingsWanted);


    if(preparedBounds.Num() > 0){
        DebugHelper::logMessage(
            FString::Printf(
                TEXT("BuildingMaker::Build made buildings: %d"), preparedBounds.Num()
            )
        );


        //lock terrain from generated bounds
        LockTerrainFromQuadBounds(preparedBounds);

        //for now
        //create debug cubes / paste inside chunk map    
        CreateDebugCubesFromBoundsCreated(preparedBounds);

        //paste bounds of buildings to pathfinder (via chunkparser) once generated mesh
        AddBoundsToPathFinderGeometry(preparedBounds);

        //later
        //generate individual buildings
    }

    if(false){
        //append hull AND generated polygons
        DebugAppendHull(grid);
    }

   
}


void BuildingMaker::PrepareFittedBounds(
    TArray<BuildingBounds> &boundsType,
    TArray<BuildingBounds> &outPreparedBounds, //generated and fitted bounds
    FMeshedSurfaceGrid *grid,
    int buildingsWanted
){
    //is not randomized yet.
    for (int i = 0; i < buildingsWanted; i++){
        for (int j = 0; j < boundsType.Num(); j++)
        {
            BuildingBounds &prefab = boundsType[j];
            BuildingBounds generated;
            if (FindShape(grid, prefab, generated))
            {
                outPreparedBounds.Add(generated);
                j = boundsType.Num(); // skip types to end since fit found
            }
        }
    }
}


//Updates the bounds with the assigned rotation and position
bool BuildingMaker::FindShape(
    FMeshedSurfaceGrid *grid,
    const BuildingBounds &prefab, 
    BuildingBounds &outBounds //created bounds out if succeded
){
    if(grid){
        FVector posWorldOfBound;
        FRotator rotation;

        if(grid->FindShape(
            prefab.getSizeX(), // in cm
            prefab.getSizeY(), // in cm
            posWorldOfBound,
            rotation
        )){
            outBounds.Setup(prefab);
            outBounds.AssignTransform(posWorldOfBound, rotation);

            return true;
        }
    }
    return false;
}










/// @brief locks quads for tree blocking on the desired terrain creator
/// before generating the meshdata and trees procedurally for each chunk
/// @param creator 
/// @param preparedBounds 
void BuildingMaker::LockTerrainFromQuadBounds(
    TArray<BuildingBounds> &preparedBounds
){
    if(creator){
        TArray<FVector> reUsableArray;
        for (int i = 0; i < preparedBounds.Num(); i++){
            BuildingBounds &current = preparedBounds[i];
            current.GetOverrideBoundingVerteciesWorld(reUsableArray); //set to num 4 internally
            //terrainCreator::lockQuad(const TArray<FVector> &quadPositions)
            creator->lockQuad(reUsableArray);
        }
    }
}






void BuildingMaker::CreateDebugCubesFromBoundsCreated(
    TArray<BuildingBounds> &preparedBounds 
){
    if(creator && map){
        for (int i = 0; i < preparedBounds.Num(); i++){
            CreateDebugCubesFromBoundCreated(preparedBounds[i]);
        }
    }
}

void BuildingMaker::CreateDebugCubesFromBoundCreated(
    BuildingBounds &preparedBound 
){
    if(creator && map){

        //bound pivot to chunk parser
        const FVector &pivot = preparedBound.GetPivot();
        DebugHelper::logMessage("BuildingMaker::Create Cube at ", pivot);

        ChunkParserInterfaceBase &interfaceFound = map->findByWorldLocation(creator, pivot);

        //offset to remove
        //to bring the vertecies to the local coordinate system of the
        //chunk parser
        FVector targetLocationRemove = -1.0f * interfaceFound.GetActorLocation(); 
        


        //append meshdata
        MeshData &meshDataRef = interfaceFound.findMeshDataReference(
            materialEnum::wallMaterial,
            ELod::lodNear,
            true // bool raycastOnLayer
        );

        //wenn nicht darüber ist der scale sehr klein.

       
        preparedBound.AppendDebugCube(meshDataRef, targetLocationRemove); // debug
    }
}



void BuildingMaker::AddBoundsToPathFinderGeometry(
    TArray<BuildingBounds> &preparedBounds 
){
    for (int i = 0; i < preparedBounds.Num(); i++){
        AddBoundsToPathFinderGeometry(preparedBounds[i]);
    }
}

void BuildingMaker::AddBoundsToPathFinderGeometry(
    BuildingBounds &preparedBound
){
    if(map && creator){
        const FVector &pivot = preparedBound.GetPivot();
        ChunkParserInterfaceBase &interfaceFound = map->findByWorldLocation(creator, pivot);

        FGeometryCollection &collection = interfaceFound.GetGeometryCollection();
        //add world bounds. -> are added to path finder by world coordinates
        TArray<FVector> worldPositions;
        preparedBound.GetOverrideBoundingVerteciesWorld(worldPositions);
        collection.AddConvexHull(worldPositions);
    }
}








void BuildingMaker::GetBuildingSizes(TArray<BuildingBounds> &outBoundsType){

    //debug sizes!
    int meter = 100;
    int height = meter * 3;

    BuildingBounds boundsA;
    boundsA.Setup(10 * meter, 10 * meter, height); //in index space!
    outBoundsType.Add(boundsA);

    boundsA.Setup(10 * meter, 5 * meter, height);
    outBoundsType.Add(boundsA);

    boundsA.Setup(3 * meter, 4 * meter, height);
    outBoundsType.Add(boundsA);

}








/// ---------- DEBUG ----------


//not used. Debug only.
void BuildingMaker::DebugAppendHull(
    FMeshedSurfaceGrid *grid
){
    if(creator && map && grid){
        const FVector pivot = grid->BottomLeft();

        ChunkParserInterfaceBase &interfaceFound = map->findByWorldLocation(creator, pivot);
        
        //std::pair<int, int> posIndex2D = creator->Index2DFromWorldPosition(pivot);
        //ChunkParserInterfaceBase &interfaceFound = map.findByIndexBase(posIndex2D.first, posIndex2D.second);

        FVector targetLocationRemove = -1.0f * interfaceFound.GetActorLocation(); //offset to remove

        

        //ChunkParserInterfaceBase &interfaceFound = map.findByIndexBase(0,0);
        MeshData &meshDataRef = interfaceFound.findMeshDataReference(
            materialEnum::prop_alarmBoxMaterial,
            ELod::lodNear,
            true // bool raycastOnLayer
        );

        float height = 100;
        grid->DebugAppendEdgeSetRawAsMeshData(height, meshDataRef, targetLocationRemove);
    }
}