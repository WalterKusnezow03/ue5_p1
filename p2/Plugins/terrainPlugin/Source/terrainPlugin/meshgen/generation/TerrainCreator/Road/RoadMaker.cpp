#include "RoadMaker.h"
#include "terrainPlugin/meshgen/generation/TerrainCreator/terrainCreator.h"
#include "GameCore/EntityGC/EntityManagerBase.h"
#include "CoreMath/algorithm/GrahamScan/GrahamScan2D.h"


RoadMaker::RoadMaker(){
    terrainCreatorPtr = nullptr;
}

float RoadMaker::getHeightFor(FVector2D &pos){
    if(terrainCreatorPtr){
        return terrainCreatorPtr->getHeightFor(pos);
    }
    return 0.0f;
}

void RoadMaker::MakeMeshActorFromRoadData(UWorld *world){
    if(world){
        AcustomMeshActor *currentActor = terrainCreatorPtr->getNewMeshActor(world);
        if (currentActor == nullptr)
        {
            return;
        }
        currentActor->disableDistanceListening(); //no lod change

        //MeshData meshdataSurface;
        //MeshData meshdataSides;

        //raycast on automatic in this method
        currentActor->replaceMeshData(meshdataSurface, materialEnum::stoneMaterial, ELod::lodNear);
        currentActor->replaceMeshData(meshdataSides, materialEnum::beigeStoneMaterial, ELod::lodNear);

        currentActor->ReloadMeshAndApplyAllMaterials();
    }
}



TArray<TArray<FVector2D>> &RoadMaker::BuildedSectionsRef(){
    return buildedSections;
}

void RoadMaker::createRoads(terrainCreator *ptr, int chunks){

    terrainCreatorPtr = ptr;
    createRoads(chunks);
}

void RoadMaker::createRoads(int chunks){
    int count = 3;
    createRoads(meshdataSurface, meshdataSides, count, chunks);
    meshdataSurface.calculateNormals();
    meshdataSides.calculateNormals();
}

void RoadMaker::createRoads(
    MeshData &outmeshDataSurface,
    MeshData &outmeshDataSides,
    int count,
    int chunks
){
    for(int i = 0; i < count; i++){
        createRoad(outmeshDataSurface, outmeshDataSides, chunks);
    }
    CreatePolygonShapesForBuildingFittingBetweenRoadIntersections();
}

void RoadMaker::createRoad(
    MeshData &outmeshDataSurface,
    MeshData &outmeshDataSides,
    int chunks
){
    
    int scalePerChunk = terrainConstants::CHUNKSIZE * terrainConstants::ONEMETER;
    int limitall = scalePerChunk * chunks;
	
    //eine strasse erstreckt sich immer über die ganze karte z.b.


    bezierCurve curve;
    TVector<FVector2D> output;
    float _einheitsValue = terrainConstants::ONEMETER;

    if(true){
        /*
        bezierCurve::
        createNewRandomCurve(
            FVector2D &startingPoint,
            TVector<FVector2D> &output,
            float _einheitsValue,
            float distanceBetweenAnchorsOnXAxisMin,
            float distanceBetweenAnchorsOnXAxisMax,
            float distanceBetweenAnchorsYRange,
            float max_xy_coordinate
        )
        */

        FVector2D startingPoint;
        float distanceBetweenAnchorsOnXAxisMin = scalePerChunk * 1.0f;
        float distanceBetweenAnchorsOnXAxisMax = scalePerChunk * 4.0f;
        float distanceBetweenAnchorsYRange = scalePerChunk * 2.0f;
        float max_xy_coordinate = limitall;

        curve.createNewRandomCurve(
            startingPoint,
            output,
            _einheitsValue,
            distanceBetweenAnchorsOnXAxisMin,
            distanceBetweenAnchorsOnXAxisMax,
            distanceBetweenAnchorsYRange,
            max_xy_coordinate
        );


        //add to cache
        AddCurveToCache(output);
    }

    float roadWidth = terrainConstants::ONEMETER * 5.0f;
    processRoad(
        output, 
        roadWidth, 
        outmeshDataSurface, 
        outmeshDataSides, 
        _einheitsValue
    );

    //assign data to mesh actor when done.

}

void RoadMaker::AddCurveToCache(TVector<FVector2D> &curve){
    int id = 0;
    if(createdRoadsCache.Num() > 0){
        id = createdRoadsCache.Last().Id() + 1;
    }
    RoadData r(id, curve);
    createdRoadsCache.Add(r);
}

void RoadMaker::CreatePolygonShapesForBuildingFittingBetweenRoadIntersections(){

    ///could be a giant rasterized polygon which
    ///locks all roads with -1,
    ///or makes in from polygon data / intersections

    //graph creation
    FindAllTwoRoadIntersections();
    roadIntersections.BuildGraph(); //adding nodes locked now
    edgeCache = roadIntersections.GetEdges(); //copy edges for draw

    //graph traversal, build polygons
    TArray<FRoadSectionList> &sectionsBuilded = roadIntersections.DisassembleTraverseGraph();

    

    //build polygons
    buildedSections.Empty();
    for (int i = 0; i < sectionsBuilded.Num(); i++)
    {
        FRoadSectionList &sectionList = sectionsBuilded[i];
        sectionList.BuildPolygonAutoExtract(createdRoadsCache);
        TArray<FVector2D> &builded = sectionList.BuildedFromSections();
        buildedSections.Add(builded);
    }

    DebugHelper::logMessage(
        FString::Printf(
            TEXT("RoadMaker::finished, sections builded(%d)"), //zu viele.
            buildedSections.Num()
        )
    );

    /*
    //will not be needed.
    GrahamScan2D scan2D;

    void GrahamScan2D::ComputeConvexHull(
        const TArray<FVector2D> &points,
        TArray<int> &outIndices
    )*/
}


void RoadMaker::FindAllTwoRoadIntersections(){
    //something like < O(n^2)
    for (int i = 0; i < createdRoadsCache.Num(); i++){
        for (int j = i + 1; j < createdRoadsCache.Num(); j++){
            RoadData &roadA = createdRoadsCache[i];
            RoadData &roadB = createdRoadsCache[j];

            ////add to map instead immidiatly!
            roadA.FindIntersections(roadB, roadIntersections);
        }
    }
}















void RoadMaker::processRoad(
    TVector<FVector2D> &curve,
    float roadWidth,
    MeshData &outmeshDataSurface,
    MeshData &outmeshDataSides,
    float _einheitsValue
){
    TArray<FVector> line1;
    TArray<FVector> line2;
    line1.SetNum(curve.size());
    line2.SetNum(curve.size());


    roadWidth = std::abs(roadWidth);
    float halfWidht = roadWidth / 2.0f;
    for(int i = 1; i < curve.size(); i++){
        FVector2D &prev = curve[i-1];
        FVector2D &current = curve[i];

        //an normla extenden und beide seiten vertecies suchen, higher one
        FVector2D AB = current - prev; //AB = B - A;
        FVector2D normal(AB.Y, -AB.X); 
        normal = normal.GetSafeNormal();

        FVector2D side0_2d = prev + normal * halfWidht;
        FVector2D side1_2d = prev + -1.0f * normal * halfWidht;

        float side0Heigth = getHeightFor(side0_2d);
        float side1Heigth = getHeightFor(side1_2d);
        float maxHeight = std::max(side0Heigth, side1Heigth);

        FVector side0 = make3D(side0_2d, maxHeight);
        FVector side1 = make3D(side1_2d, maxHeight);

        line1[i] = side0;
        line2[i] = side1;

    }


    //make road look less clunky
    bezierCurve bezierCurveMaker;
    int skipIndicesForSmooth = 10;
    bezierCurveMaker.afterSmoothHeight(
        line1, //TArray<FVector> &curve,
        _einheitsValue,
        skipIndicesForSmooth//int anchorSkipPerStep
    );
    bezierCurveMaker.afterSmoothHeight(
        line2, 
        _einheitsValue,
        skipIndicesForSmooth
    );



    //generate buffer:
    /*
    1->2
    |  |
    0<-3
    */

    //road surface
    outmeshDataSurface.appendParalellLinesClosedAsQuads(line1, line2);

    //TESTING NEEDED
    //extend sides to bottom
    TArray<FVector> line1Bottom = line1;
    TArray<FVector> line2Bottom = line2;
    for (int i = 0; i < line1Bottom.Num(); i++){
        FVector &current = line1Bottom[i];
        current += FVector(0, 0, -200);
    }
    for (int i = 0; i < line2Bottom.Num(); i++){
        FVector &current = line2Bottom[i];
        current += FVector(0, 0, -200);
    }

    //side of surface
    outmeshDataSides.appendParalellLinesClosedAsQuads(line1Bottom, line1);
    outmeshDataSides.appendParalellLinesClosedAsQuads(line2, line2Bottom);




    // blocks terrain
    lockQuadsFromParalellArrayLines(line1, line2);
}

FVector RoadMaker::make3D(FVector2D &pos, float height){
    return FVector(
        pos.X,
        pos.Y,
        height
    );
}



void RoadMaker::lockQuadsFromParalellArrayLines(
    TArray<FVector> &line0,
    TArray<FVector> &line1
){
    //if not initialized with terrain ptr, skipped.
    if(!terrainCreatorPtr){
        return;
    }

    /*
    array aufbau
    
    1   2-1  2

    0   3-0  3... paralelle lines, bilden quads durch adjazente indices
    
    heisst: 0 und 2 bzw 1 und 3 bilden bounding box
    */
    int limit = std::min(line0.Num(), line1.Num());
    for(int i = 1; i < limit; i++){
        FVector &v2 = line1[i];
        FVector &v0 = line0[i-1];

        if(true){
            /**
             * CAUTION: foliage block is still partially. Bugged.
             */

            FVector &v1 = line1[i-1];
            FVector &v3 = line0[i];
            TArray<FVector> positions = {v0, v1, v2, v3};
            TArray<chunk *> chunksCollected = terrainCreatorPtr->chunksAt(positions);

            for (int j = 0; j < chunksCollected.Num(); j++){
                chunk *currentChunk = chunksCollected[j];
                if(currentChunk != nullptr){
                    currentChunk->blockAreaForFoliage(v0, v1, v2, v3);
                }
            }
        
            
            /*else{
            //old
            //chunk 0
            chunk *ptr = chunkAt(
                cmToChunkIndex(v0.X),
                cmToChunkIndex(v0.Y)
            );
            if(ptr != nullptr){
                ptr->blockAreaForFoliage(v0,v2);
            }
            
            //chunk 1
            chunk *ptr1 = chunkAt(
                cmToChunkIndex(v2.X),
                cmToChunkIndex(v2.Y)
            );
            if(ptr1 != nullptr){
                ptr1->blockAreaForFoliage(v0,v2);
            }*/
        }
        
    }



}






//// debug 
TArray<TArray<std::pair<FVector2D, FVector2D>>> &RoadMaker::GetEdges(){
    return edgeCache;
}




TArray<std::vector<FVector2D>> RoadMaker::GetRoads(){
    TArray<std::vector<FVector2D>> outArray;
    for (int i = 0; i < createdRoadsCache.Num(); i++){
        RoadData &current = createdRoadsCache[i];
        outArray.Add(current.getCurve());
    }
    return outArray;
}