#include "BuildingMaker.h"
#include "terrainPluginBase/BaseTerrainInterface/TerrainInterfaceBase.h"
#include "DebugPlugin/DebugHelper.h"


void BuildingMaker::Build(
    TerrainInterfaceBase *creator,
    ChunkParserMapInterfaceBase &map, 
    TArray<FMeshedSurfaceGrid *> &buildingAreas
){
    DebugHelper::logMessage(
        FString::Printf(
            TEXT("BuildingMaker::Build from grids: %d"), buildingAreas.Num()
        )
    );


    int buildingsWantedPerGrid = 50;
    for (int i = 0; i < buildingAreas.Num(); i++){
        if(FMeshedSurfaceGrid *current = buildingAreas[i]){
            BuildSingleGrid(creator, map, current, buildingsWantedPerGrid);
        }
    }
}

void BuildingMaker::BuildSingleGrid(
    TerrainInterfaceBase *creator,
    ChunkParserMapInterfaceBase &map,
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
    
    if(!grid->CanFindShape()){
        return;
    }

    //fixed. Debug not needed.
    //DebugAppendGrid(creator, map, grid);
    

    //types
    TArray<BuildingBounds> boundsType;
    GetBuildingSizes(boundsType);

    //made
    TArray<BuildingBounds> preparedBounds;
    PrepareFittedBounds(boundsType, preparedBounds, grid, buildingsWanted);


    if(preparedBounds.Num() > 0){
        DebugHelper::logMessage(
            FString::Printf(
                TEXT("BuildingMaker::Build made buildings: %d"), preparedBounds.Num()
            )
        );


        //lock terrain from generated bounds
        LockTerrainFromQuadBounds(creator, preparedBounds);

        //for now
        //create debug cubes / paste inside chunk map
        //DebugAppendGrid(creator, map, grid);
        
        CreateDebugCubesFromBoundsCreated(creator, map, preparedBounds);

        //paste bounds of buildings to pathfinder once generated mesh (?)
        

        //later
        //generate individual buildings
    }

    if(true){
        //append hull AND generated polygons
        DebugAppendHull(creator, map, grid);
    }

   
}


void BuildingMaker::PrepareFittedBounds(
    TArray<BuildingBounds> &boundsType,
    TArray<BuildingBounds> &outPreparedBounds,
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







//deprecated, not needed
void BuildingMaker::DebugAppendGrid(
    TerrainInterfaceBase *creator,
    ChunkParserMapInterfaceBase &map,
    FMeshedSurfaceGrid *grid
){  
    if(grid){

        //FVector pivot = grid->BottomLeft();
        //std::pair<int, int> posIndex2D = creator->Index2DFromWorldPosition(pivot);
        //ChunkParserInterfaceBase &interfaceFound = map.findByIndexBase(posIndex2D.first, posIndex2D.second);
        ChunkParserInterfaceBase &interfaceFound = map.findByIndexBase(0,0);


        //append meshdata
        MeshData &meshDataRef = interfaceFound.findMeshDataReference(
            materialEnum::wallMaterial,
            ELod::lodNear,
            true // bool raycastOnLayer
        );

        bool removeOffset2DLocalMesh = false;
        grid->AppendMeshedSurface(meshDataRef, removeOffset2DLocalMesh);
    }
}


void BuildingMaker::DebugAppendHull(
    TerrainInterfaceBase *creator,
    ChunkParserMapInterfaceBase &map,
    FMeshedSurfaceGrid *grid
){
    if(grid){
        const FVector pivot = grid->BottomLeft();

        ChunkParserInterfaceBase &interfaceFound = map.findByWorldLocation(creator, pivot);
        
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






void BuildingMaker::LockTerrainFromQuadBounds(
    TerrainInterfaceBase *creator,
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
    TerrainInterfaceBase *creator,
    ChunkParserMapInterfaceBase &map,
    TArray<BuildingBounds> &preparedBounds 
){
    if(creator){
        for (int i = 0; i < preparedBounds.Num(); i++){
            CreateDebugCubesFromBoundCreated(
                creator,
                map,
                preparedBounds[i]
            );
        }
    }
}

void BuildingMaker::CreateDebugCubesFromBoundCreated(
    TerrainInterfaceBase *creator,
    ChunkParserMapInterfaceBase &map,
    BuildingBounds &preparedBound 
){
    if(creator){

        //bound pivot to chunk parser
        const FVector &pivot = preparedBound.GetPivot();
        DebugHelper::logMessage("BuildingMaker::Create Cube at ", pivot);

        std::pair<int, int> posIndex2D = creator->Index2DFromWorldPosition(pivot);
        ChunkParserInterfaceBase &interfaceFound = map.findByIndexBase(posIndex2D.first, posIndex2D.second);

        FVector targetLocationRemove = -1.0f * interfaceFound.GetActorLocation(); //offset to remove
        


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






//Updates the bounds with the assigned rotation and position
bool BuildingMaker::FindShape(
    FMeshedSurfaceGrid *grid,
    const BuildingBounds &prefab, 
    BuildingBounds &outBounds
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

