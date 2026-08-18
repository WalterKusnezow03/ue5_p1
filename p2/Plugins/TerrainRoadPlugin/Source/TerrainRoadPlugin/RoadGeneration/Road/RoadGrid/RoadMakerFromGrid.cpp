#include "RoadMakerFromGrid.h"

#include "terrainPluginBase/BaseTerrainInterface/TerrainInterfaceBase.h"


#include <algorithm>
#include "terrainPluginBase/BaseTerrainInterface/bezier/extendedVersion/BSpline.h"
#include "Algo/Reverse.h"

#include "AssetEnumCollection/assetEnums/materialEnum.h"
#include "GameCore/MeshGenBase/ELod.h"
#include "DebugPlugin/DebugHelper.h"
#include "GameCore/util/FVectorUtil.h"

#include "terrainPluginBase/BaseTerrainInterface/terrainConstants.h"


RoadMakerFromGrid::RoadMakerFromGrid(){

}

RoadMakerFromGrid::~RoadMakerFromGrid(){

}

FVector &RoadMakerFromGrid::CenterOfMesh(){
    return centerSaved;
}

void RoadMakerFromGrid::UpdateCenterOfMesh(){
    if(mesh.Num() > 0){
        int num = mesh.Num() * mesh[0].Num(); //same size on all arrays
        FVector centerUpdate;
        for (int i = 0; i < mesh.Num(); i++){
            TArray<FRoadMakerPosition> &array = mesh[i];
            for (int j = 0; j < array.Num(); j++){
                centerUpdate += array[j].GetPosition();
            }
        }
        centerUpdate /= num;
        centerUpdate.Z = 0.0f;
        centerSaved = centerUpdate;
    }
}

void RoadMakerFromGrid::SaveOriginalProperties(
    FVector2D &size,
    float stepSize
){
    sizeSaved = size;
    stepSizeSavedBetweenMeshNodes = stepSize;
}


void RoadMakerFromGrid::ValidateSize(FVector2D &size, float &stepSize){
    size.X = std::max(1.0, std::abs(size.X));
    size.Y = std::max(1.0, std::abs(size.Y));
    stepSize = std::max(1.0f, std::abs(stepSize));
}

void RoadMakerFromGrid::CreateGrid(FVector2D size, float StepSize){
    mesh.Empty();
    ValidateSize(size, StepSize);
    SaveOriginalProperties(size, StepSize);


    int countNodesX = size.X / StepSize;
    int countNodesY = size.Y / StepSize;

    DebugHelper::logMessage(
        FString::Printf(TEXT("RoadMakerFromGrid::CreateGrid %d, %d"), countNodesX, countNodesY)
    );

    mesh.SetNum(countNodesX);
    buildedMeshQuads.SetNum(countNodesX);
    for (int i = 0; i < mesh.Num(); i++){
        float offsetX = i * StepSize;

        TArray<FRoadMakerPosition> &currentGridPositions = mesh[i];
        currentGridPositions.SetNum(countNodesY);

        TArray<RoadQuad> &buildedMeshCurrent = buildedMeshQuads[i];
        buildedMeshCurrent.SetNum(countNodesY);

        for (int j = 0; j < currentGridPositions.Num(); j++){
            float offsetY = j * StepSize;
            FVector &vec = currentGridPositions[j].GetPosition();
            vec.X = offsetX;
            vec.Y = offsetY;
            vec.Z = 0.0f;
        }
    }
    UpdateCenterOfMesh();
}


// --- helper for warp ---
TArray<FVector> RoadMakerFromGrid::RandomPositions(
    int maxCount, 
    float distanceFromEdges
){
    FVector2D maxPosition = sizeSaved - FVector2D(distanceFromEdges, distanceFromEdges);
    FVector2D minPosition = FVector2D(distanceFromEdges, distanceFromEdges);
    TArray<FVector> outArray;
    while(maxCount > 0){
        FVector created;
        if(RandomPosition(minPosition, maxPosition, created)){
            outArray.Add(created);
        }
        maxCount--;
    }
    return outArray;
}

bool RoadMakerFromGrid::RandomPosition(
    FVector2D &minPosition, 
    FVector2D &maxPosition, 
    FVector &outpos
){
    int minX = minPosition.X / stepSizeSavedBetweenMeshNodes;
    int minY = minPosition.Y / stepSizeSavedBetweenMeshNodes;

    int maxX = maxPosition.X / stepSizeSavedBetweenMeshNodes;
    int maxY = maxPosition.Y / stepSizeSavedBetweenMeshNodes;
    return RandomPosition(
        minX, minY, maxX, maxY, outpos
    );
}

bool RoadMakerFromGrid::RandomPosition(
    int iLower, 
    int jLower, 
    int iHigher, 
    int jHigher,
    FVector &outpos
){
    iLower = std::max(0, iLower);
    jLower = std::max(0, jLower);

    if(mesh.Num() > 0){
        iHigher = std::min(iHigher, mesh.Num() - 1);
        jHigher = std::min(jHigher, mesh[0].Num() - 1);

        int randomI = FVectorUtil::randomNumber(iLower, iHigher);
        int randomJ = FVectorUtil::randomNumber(jLower, jHigher);
        if(randomI >= 0 && randomI < mesh.Num()){
            TArray<FRoadMakerPosition> &array = mesh[randomI];
            if (randomJ >= 0 && randomJ < array.Num())
            {
                outpos = array[randomJ].GetPosition();
                return true;
            }
        }
    }
    return false;
}

// --- warp and Randomize grid ---




void RoadMakerFromGrid::WarpCirlceRandom(){
    int iterations = 2;
    int warpCirclesPerIteration = 3;
    while(iterations > 0){
        float randomScalar = FVectorUtil::randomFloatNumber(0, 1);
        float maxDistance = sizeSaved.Size() * randomScalar;
        float angle = FVectorUtil::randomFloatNumber(0, 30);
        //FVectorUtil::randomFloatNumber(-30, 30);

        TArray<FVector> randomPositions = RandomPositions(
            warpCirclesPerIteration, 
            maxDistance
        );
        for(int i = 0; i < randomPositions.Num(); i++){
            WarpCirlceByDistance(
                randomPositions[i],
                maxDistance,
                angle
            );
        }

        //WarpCirlceByDistanceAroundCenter(maxDistance, angle);
        iterations--;
    }
}

void RoadMakerFromGrid::WarpCirlceByDistanceAroundCenter(
    float maxDistance,
    float angle
){
    WarpCirlceByDistance(centerSaved, maxDistance, angle);
}

void RoadMakerFromGrid::WarpCirlceByDistance(
    FVector &center, 
    float maxDistance,
    float angle
){
    

    float maxDistance2 = maxDistance * maxDistance;
    if(maxDistance2 < stepSizeSavedBetweenMeshNodes){
        return;
    }
    if(std::abs(angle) < 0.1f){
        return;
    }

    MMatrix M;
    for (int i = 0; i < mesh.Num(); i++){
        TArray<FRoadMakerPosition> &current = mesh[i];
        for (int j = 0; j < current.Num(); j++){
            FRoadMakerPosition &currenPosition = current[j];
            FVector &currentPositionVector = currenPosition.GetPosition();

            float currentDistance = FVector::DistSquared(currentPositionVector, center);
            if(currentDistance <= maxDistance2){
                float skalar = currentDistance / maxDistance2; // distTarget / distAll
                float scaledAngle = skalar * angle;
                MakeRotationAroundPosition(
                    center,
                    scaledAngle,
                    M
                );
                
                currentPositionVector = M * currentPositionVector;
            }
        }
    }
}

void RoadMakerFromGrid::MakeRotationAroundPosition(
    FVector &position,
    float angle,
    MMatrix &outMatrix
){
    MMatrix T;
    T.setTranslation(position);
    MMatrix T1 = T.invertedTranslation();

    MMatrix R;
    R.yawRadAdd(MMatrix::degToRadian(angle));

    //M = T * R * T^-1 <-- lese richtung --
    MMatrix RT1 = R * T1;
    outMatrix = T * RT1;
}









// --- build --- (no warp allowed anymore)

void RoadMakerFromGrid::ApplyHeightOnRawMeshAndInBoundStatusOfPositions(TerrainInterfaceBase *creator){
    if(creator){
        float heightOffset = 30.0f;
        for (int i = 0; i < mesh.Num(); i++){
            TArray<FRoadMakerPosition> &current = mesh[i];
            for (int j = 0; j < current.Num(); j++){
                FRoadMakerPosition &currentPos = current[j];
                currentPos.UpdateHeightAndMarkInBoundStatus(creator, heightOffset);
            }
        }
    }
}





void RoadMakerFromGrid::Build(
    TerrainInterfaceBase *creator, 
    float _einheitsValueForBsplineStepSize, 
    float roadWidth,
    ChunkParserMapInterfaceBase &map
){
    ApplyHeightOnRawMeshAndInBoundStatusOfPositions(creator);
    Build(_einheitsValueForBsplineStepSize);
    LockTerrainFromGeneratedRoadQuads(creator, roadWidth);
    //ApplyHeightOnRoadQuads(creator); <-- moved down --
    
    //quads surface gen for terrain and locking foliage
    //in world space

    GenerateQuadMeshedSurfaces(creator);
    GenerateQuadMeshedSurfacesDebugMesh();



    ApplyHeightOnRoadQuads(creator);


    //move road quads to local space
    ApplyTerrain2DIndexToRoadQuadsAndRemoveTerrainOffset(creator);
    AddRoadQuadsMeshDataToChunks(map, creator);

    


    wasBuilded = true;
}




void RoadMakerFromGrid::Build(float _einheitsValue){
    //interpolate with bsplines
    
    //bspline extended klasse, outindex array mit 
    //index positionen im bspline wo die originalen knoten lagen

    // ---- TODO ----
    //compute all vertical and horizontal splines
    //store into map

    //build and add segments
    //build in x, x+1, x+1,y+1, y+1,x, y+1,x
    //add to ordered quad.

    ComputeAllYAxis(_einheitsValue);
    ComputeAllXAxis(_einheitsValue);
    BuildAllPolygons();
}

void RoadMakerFromGrid::ComputeAllYAxis(float _einheitsValue){
    for (int i = 0; i < mesh.Num(); i++){
        ComputeYAxis(i, _einheitsValue);
    }
}

void RoadMakerFromGrid::ComputeAllXAxis(float _einheitsValue){
    if(mesh.Num() > 0){
        //compute all x axis, along y height, which will be the same 
        //across the rectangular map.
        int yHeight = mesh[0].Num();
        for (int j = 0; j < yHeight; j++){
            ComputeXAxis(j, _einheitsValue);
        }
    }
}

void RoadMakerFromGrid::ComputeYAxis(int index, float _einheitsValue){
    if(index >= 0 && index < mesh.Num()){
        TArray<FRoadMakerPosition> &yAxisRoadMakerPositions = mesh[index];

        //convert to raw FVector, needed here. 
        //original mesh not needed anymore anyway after bspline build
        TArray<FVector> yAxis;
        for(int i = 0; i < yAxisRoadMakerPositions.Num(); i++){
            FRoadMakerPosition &meshPosition = yAxisRoadMakerPositions[i];
            
            if(meshPosition.IsInsideTerrain()){
                yAxis.Add(meshPosition.GetPosition());
            }
            //yAxis.Add(meshPosition.GetPosition());
        }

        //from 2 anchors generate
        if(yAxis.Num() > 1){
            BSpline splineMaker;
            TArray<FVectorBSplinePosition> &outArray = GetSection(index, ERoadKeyEnum::E_yaxis);
            splineMaker.calculatecurve(
                yAxis,
                outArray,
                _einheitsValue
            );

            DebugHelper::logMessage(
                FString::Printf(
                    TEXT("RoadMakerFromGrid::ComputeYAxis %d, old size %d "),
                    outArray.Num(),
                    yAxis.Num()
                )
            );
        }
    }
}

void RoadMakerFromGrid::ComputeXAxis(int index, float _einheitsValue){
    if(mesh.Num() > 0 && index >= 0){
        TArray<FVector> copiedXAxis;
        for (int j = 0; j < mesh.Num(); j++){
            TArray<FRoadMakerPosition> &yAxisArray = mesh[j];
            if(index < yAxisArray.Num()){
                FRoadMakerPosition &meshPosition = yAxisArray[index];
                
                if(meshPosition.IsInsideTerrain()){
                    //copy in x y-vertical the desired y value for x
                    copiedXAxis.Add(meshPosition.GetPosition());
                }
                //copiedXAxis.Add(meshPosition.GetPosition());
                /*
                0000
                xxxx <--copy--
                0000
                0000
                */
            }
        }
        if(copiedXAxis.Num() > 1){
            BSpline splineMaker;
            TArray<FVectorBSplinePosition> &outArray = GetSection(index, ERoadKeyEnum::E_xaxis);
            splineMaker.calculatecurve(
                copiedXAxis,
                outArray,
                _einheitsValue
            );
            

            DebugHelper::logMessage(
                FString::Printf(
                    TEXT("RoadMakerFromGrid::ComputeXAxis %d, old size %d "),
                    outArray.Num(),
                    copiedXAxis.Num()
                )
            );
        }
    }
}


//std::map<FRoadKey, TArray<FVectorBSplinePosition>> buildedSections;



TArray<FVectorBSplinePosition> &RoadMakerFromGrid::GetSection(int index, ERoadKeyEnum key){
    FRoadKey k(index, key);
    return GetSection(k);
}

TArray<FVectorBSplinePosition> &RoadMakerFromGrid::GetSection(FRoadKey &key){
    if(buildedSections.find(key) == buildedSections.end()){
        buildedSections[key] = TArray<FVectorBSplinePosition>();
    }
    return buildedSections[key];
}





void RoadMakerFromGrid::CopySectionTo(
    std::pair<int,int> &pos0,
    std::pair<int,int> &pos1,
    RoadQuad &quad
){
    TArray<FVectorBSplinePosition> positions = CopySection(pos0, pos1);
    quad.Append(positions);

    /*DebugHelper::logMessage(
        FString::Printf(
            TEXT("RoadMakerFromGrid::CopySectionTo (%d, %d) (%d, %d) count %d"), 
            pos0.first, 
            pos0.second,
            pos1.first, 
            pos1.second,
            positions.Num()
        )
    );*/
}

//do not paste diagonals. Not supported!
TArray<FVectorBSplinePosition> RoadMakerFromGrid::CopySection(
    std::pair<int,int> &pos0,
    std::pair<int,int> &pos1
){
    /*
    xx or (xaxis)

    x (yaxis)
    x
    */
    ERoadKeyEnum axis = pos0.first == pos1.first ? ERoadKeyEnum::E_yaxis : ERoadKeyEnum::E_xaxis;
    int axisIndex = axis == ERoadKeyEnum::E_xaxis ? pos0.second : pos0.first; //yes

    int pos0Index = axis == ERoadKeyEnum::E_xaxis ? pos0.first : pos0.second; // the other index, not axis
    int pos1Index = axis == ERoadKeyEnum::E_xaxis ? pos1.first : pos1.second; //the other index, not axis
    return CopySection(axis, axisIndex, pos0Index, pos1Index);
}

TArray<FVectorBSplinePosition> RoadMakerFromGrid::CopySection(
    ERoadKeyEnum axis,
    int axisIndex,
    int a,
    int b
){
    TArray<FVectorBSplinePosition> outArray;
    if(a != b){
        bool reverseSection = b < a; //clock wise order of sections, is flipped sometimes
        FRoadKey key(axisIndex, axis);
        TArray<FVectorBSplinePosition> &completeSection = GetSection(key); 

        //copy between a and b and reverse if needed
        int start = std::min(a, b);
        int end = std::max(a, b);

        int startFoundIndex = IndexInArrayWhereOriginalAnchorIsIndex(start, completeSection);
        int endFoundIndex = IndexInArrayWhereOriginalAnchorIsIndex(end, completeSection);
        outArray = CopySection(completeSection, startFoundIndex, endFoundIndex);
        if(reverseSection){
            Algo::Reverse(outArray);
        }
    }else{
        DebugHelper::logMessage("RoadMakerFromGrid::Could not copy section");
    }
    return outArray;
}

TArray<FVectorBSplinePosition> RoadMakerFromGrid::CopySection(
    TArray<FVectorBSplinePosition> &array,
    int i0,
    int i1
){
    TArray<FVectorBSplinePosition> copiedPart;
    if (i0 >= 0 && i1 > i0 && i1 < array.Num())
    {
        for (int i = i0; i <= i1; i++){
            copiedPart.Add(array[i]);
        }
    }
    return copiedPart;
}

///where the original array had its anchor (original grid), find that index
//inside the passed array (0,1,2,3,x<-target,...) - finds: 4
int RoadMakerFromGrid::IndexInArrayWhereOriginalAnchorIsIndex(
    int targetAnchor,
    TArray<FVectorBSplinePosition> &completeSection
){
    int innerCount = 0;
    for (int i = 0; i < completeSection.Num(); i++){
        FVectorBSplinePosition &current = completeSection[i];
        if(current.IsOriginalAnchor()){
            //is original index from raw mesh - return index in section
            if(current.IsSameIndex(targetAnchor)){
                return i;
            }
            innerCount++; //reached anchor but not the targeted one
        }
    }
    return -1;
}

//Todo: get keys in quad shape order!!
void RoadMakerFromGrid::BuildAllPolygons(){
    TArray<std::pair<int, int>> quads = GetAllQuadShapedIndices();
    for (int i = 3; i < quads.Num(); i += 4){
        //copy sections.
        //always targeted the 0,0 tl corner in
        //builded quad mesh / map
        std::pair<int, int> &p0 = quads[i - 3];
        std::pair<int, int> &p1 = quads[i - 2];
        std::pair<int, int> &p2 = quads[i - 1];
        std::pair<int, int> &p3 = quads[i];

        //add sections to p0 index
        RoadQuad &addToQuadRef = GetQuadInBuildedMesh(p0);

        //copy all sections
        CopySectionTo(p0, p1, addToQuadRef);
        CopySectionTo(p1, p2, addToQuadRef);
        CopySectionTo(p2, p3, addToQuadRef);
        CopySectionTo(p3, p0, addToQuadRef);
    }
}



RoadQuad &RoadMakerFromGrid::GetQuadInBuildedMesh(std::pair<int,int> &posXY){
    return GetQuadInBuildedMesh(posXY.first, posXY.second);
}

RoadQuad &RoadMakerFromGrid::GetQuadInBuildedMesh(int i, int j){
    if(i >= 0 && i < buildedMeshQuads.Num()){
        TArray<RoadQuad> &yAxis = buildedMeshQuads[i];
        if(j >= 0 && j < yAxis.Num()){
            return yAxis[j];
        }
    }
    return fallback;
}

//as (x,y) pairs for copying axis section parts
TArray<std::pair<int,int>> RoadMakerFromGrid::GetAllQuadShapedIndices(){
    TArray<std::pair<int,int>> outArray;
    if (mesh.Num() > 0){
        int yArrays = mesh.Num();
        int xArrays = mesh[0].Num(); //along vertical, how many horizontal (refacture for function!)
        
        //COUNTER CLOCK WISE ORDER

        // -- comment on overall objective: --
        //build and add segments
        //build in x, x+1, x+1,y+1, y+1,x, y+1,x
        /*
        0(x,y)----1(x+1,y)
        |             |
        3(x,y+1)--2(x+1,y+1)
        */

        /*
        1-->2
        
        0<--3
        */

        //add to ordered quad.

        for(int i = 1; i < yArrays; i++){
            for(int j = 1; j < xArrays; j++){
                std::pair<int,int> p0(j-1, i-1);
                std::pair<int,int> p1(j, i-1);
                std::pair<int,int> p2(j, i);
                std::pair<int,int> p3(j-1, i);

                //in this case:

                //this is counter clock wise
                /*outArray.Add(p0);
                outArray.Add(p1);
                outArray.Add(p2);
                outArray.Add(p3);*/

                //clockwise
                outArray.Add(p3);
                outArray.Add(p2);
                outArray.Add(p1);
                outArray.Add(p0);
            }
        }
    }
    return outArray;
}





void RoadMakerFromGrid::LockTerrainFromGeneratedRoadQuads(
    TerrainInterfaceBase *creator,
    float roadWidth
){
    //DebugHelper::logMessage("RoadMakerFromGrid::LockTerrainFromGeneratedRoadQuads A");
    if(creator){
        //DebugHelper::logMessage("RoadMakerFromGrid::LockTerrainFromGeneratedRoadQuads B");
        for (int i = 0; i < buildedMeshQuads.Num(); i++)
        {
            TArray<RoadQuad> &current = buildedMeshQuads[i];
            for (int j = 0; j < current.Num(); j++){
                RoadQuad &currentQuad = current[j];

                TArray<FVector> inner;
                TArray<FVector> outer;
                currentQuad.GenerateParalellRoad(
                    outer,
                    inner,
                    roadWidth
                );
                creator->lockQuadsFromParalellArrayLines(
                    outer,
                    inner
                    //inner,
                    //outer
                );
            }
        }
    }
}


void RoadMakerFromGrid::ApplyHeightOnRoadQuads(TerrainInterfaceBase *creator){
    if(creator){
        for (int i = 0; i < buildedMeshQuads.Num(); i++){
            TArray<RoadQuad> &current = buildedMeshQuads[i];
            for (int j = 0; j < current.Num(); j++){
                RoadQuad &currentQuad = current[j];
                ApplyHeightForPostions(currentQuad.GetCirlce(), creator);
                ApplyHeightForPostions(currentQuad.GetInnerCircle(), creator);                
            }
        }

    }
}

void RoadMakerFromGrid::ApplyHeightForPostions(TArray<FVector> &array, TerrainInterfaceBase *creator){
    if(creator){
        for (int i = 0; i < array.Num(); i++){
            FVector &current = array[i];
            float height = creator->getHeightFor(current);
            current.Z = height + 30.0f;
        }
    }
}





void RoadMakerFromGrid::ApplyTerrain2DIndexToRoadQuadsAndRemoveTerrainOffset(
    TerrainInterfaceBase *creator
){
    ApplyTerrain2DIndexToRoadQuads(creator);
    RemoveTerrainOffsetFromRoadQuads(creator);
}

void RoadMakerFromGrid::ApplyTerrain2DIndexToRoadQuads(
    TerrainInterfaceBase *creator
){
    if(creator){
        for (int i = 0; i < buildedMeshQuads.Num(); i++){
            TArray<RoadQuad> &current = buildedMeshQuads[i];
            for (int j = 0; j < current.Num(); j++){
                RoadQuad &currentQuad = current[j];

                std::pair<int, int> posIndex = creator->Index2DFromWorldPosition(currentQuad.GetCenter());
                currentQuad.UpdateChunkIndexInTerrain(posIndex);
            }
        }
    }
}

//validation needed
void RoadMakerFromGrid::RemoveTerrainOffsetFromRoadQuads(
    TerrainInterfaceBase *creator
){
    if(creator){
        for (int i = 0; i < buildedMeshQuads.Num(); i++){
            TArray<RoadQuad> &current = buildedMeshQuads[i];
            for (int j = 0; j < current.Num(); j++){
                RoadQuad &currentQuad = current[j];

                std::pair<int, int> &index = currentQuad.GetChunkIndex();
                FVector removeOffset;
                if (creator->ChunkPositionFromIndexPair(removeOffset, index)){
                    currentQuad.RemoveOffsetFromInnerAndOuterCircle(removeOffset);
                }

            }
        }
    }
}




void RoadMakerFromGrid::AddRoadQuadsMeshDataToChunks(
    ChunkParserMapInterfaceBase &map,
    TerrainInterfaceBase *creator
){
    
    
    for (int i = 0; i < buildedMeshQuads.Num(); i++){
        TArray<RoadQuad> &array = buildedMeshQuads[i];
        for (int j = 0; j < array.Num(); j++){
            RoadQuad &current = array[j];
            std::pair<int, int> &indexPair = current.GetChunkIndex();

            ChunkParserInterfaceBase &parser = map.findByIndexBase(indexPair.first, indexPair.second);
            AddRoadQuadsMeshDataToChunk(parser, current, creator);
        }
    }



}

void RoadMakerFromGrid::AddRoadQuadsMeshDataToChunk(
    ChunkParserInterfaceBase &chunk,
    RoadQuad &quad,
    TerrainInterfaceBase *creator
){
    //used by meshdata saving, dont use manually if not needed
    //only public for RoadMakerFromGrid!
    bool raycastOnLayer = true;
    MeshData &data = chunk.findMeshDataReference(
        materialEnum::stoneMaterial,
        ELod::lodNear,
        raycastOnLayer
    );
    quad.AppendRoadMesh(data, creator);
}






// --- generate road polygon surface data ---

//must happen BEFORE any offset removal on road quads!
void RoadMakerFromGrid::GenerateQuadMeshedSurfaces(TerrainInterfaceBase *creator){
    if(creator){
        
        
        float widthOfInsideStep = terrainConstants::ONEMETER;
        
        
        
        /*bool debugOnly = false;
        if(debugOnly){
            if(buildedMeshQuads.Num() > 0 && buildedMeshQuads[0].Num() > 0){
                RoadQuad &quad = buildedMeshQuads[0][0];
                quad.GenerateMeshedSurface(creator, widthOfInsideStep);
            }
            return;
        }*/

        //all
        for (int i = 0; i < buildedMeshQuads.Num(); i++)
        {
            TArray<RoadQuad> &currentArray = buildedMeshQuads[i];
            for (int j = 0; j < currentArray.Num(); j++){
                RoadQuad &current = currentArray[j];
                current.GenerateMeshedSurface(creator, widthOfInsideStep);
            }
        }
    }
}



// ---- debug ----

//#include "terrainPlugin/meshgen/customMeshActor.h"
//#include "GameCore/MeshGenBase/customMeshActorBase.h"
//#include "GameCore/world/worldLevelBase.h"
void RoadMakerFromGrid::GenerateQuadMeshedSurfacesDebugMesh(){
    /*
    if(UWorld *worldPtr = AworldLevelBase::GetWorldPointer()){
        AcustomMeshActor *actor = AcustomMeshActor::makeInstance(worldPtr);
        if(actor){
            MeshData &data = actor->findMeshDataReference(
                materialEnum::wallMaterial,
                ELod::lodNear,
                true
            );
            for (int i = 0; i < buildedMeshQuads.Num(); i++)
            {
                TArray<RoadQuad> &currentArray = buildedMeshQuads[i];
                for (int j = 0; j < currentArray.Num(); j++){
                    RoadQuad &current = currentArray[j];
                    current.AppendMeshedSurface(data);
                }
            }

            //might be refactured to auto tick.
            actor->ReloadMeshAndApplyAllMaterials();
            actor->SetActorLocation(FVector(0, 0, 0));

            DebugHelper::logMessage("RoadMakerFromGrid::GenerateQuadMeshedSurfacesDebugMesh");
        }
    }*/
}




/// ---- GET MESHED DATA FOR BUILDING PLUGIN ----
TArray<FMeshedSurfaceGrid *> RoadMakerFromGrid::GetMeshedSurfaces(){
    TArray<FMeshedSurfaceGrid *> outArray;

    for (int i = 0; i < buildedMeshQuads.Num(); i++){
        TArray<RoadQuad> &arrayCurrent = buildedMeshQuads[i];
        for (int j = 0; j < arrayCurrent.Num(); j++){
            RoadQuad &current = arrayCurrent[j];
            if(FMeshedSurfaceGrid *grid = current.GetFMeshedSurfaceGridPointer()){
                outArray.Add(grid);
            }
        } 
    }

    return outArray;
}