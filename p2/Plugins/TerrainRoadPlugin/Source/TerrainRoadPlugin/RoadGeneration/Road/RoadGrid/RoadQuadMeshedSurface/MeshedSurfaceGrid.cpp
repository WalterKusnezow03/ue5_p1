#include "MeshedSurfaceGrid.h"
#include "MeshDataPlugin/Public/MeshGenBase/MeshData/BoundingBox/BoundingBoxSimple.h"
#include "terrainPluginBase/BaseTerrainInterface/TerrainInterfaceBase.h"

FMeshedSurfaceGrid::FMeshedSurfaceGrid(){

}

FMeshedSurfaceGrid::~FMeshedSurfaceGrid(){

}

FMeshedSurfaceGrid::FMeshedSurfaceGrid(const FMeshedSurfaceGrid &other){
    if(this != &other){
        *this = other;
    }
}
FMeshedSurfaceGrid &FMeshedSurfaceGrid::operator=(const FMeshedSurfaceGrid &other){
    if(this != &other){
        flagGrid = other.flagGrid;
        positionGrid = other.positionGrid;
        edgeSet = other.edgeSet;

        minSaved = other.minSaved;
        maxSaved = other.maxSaved;
        stepSizeSaved = other.stepSizeSaved;
        
    }   
    return *this;
}


void FMeshedSurfaceGrid::UpdateHeights(TerrainInterfaceBase *creator){
    if(!creator){
        return;
    }
    if(!GridValid()){
        return;
    }
    float offset = 30.0f;
    for (int i = 0; i < flagGrid.Num(); i++)
    {
        TArray<uint8> &buffer = flagGrid[i];
        for (int j = 0; j < buffer.Num(); j++){
            UpdateHeight(creator, positionGrid[i][j], offset);
        }
    }
}

void FMeshedSurfaceGrid::UpdateHeight(TerrainInterfaceBase *creator, FVector &vertex, float offset){
    if(creator){
        float zUpdate = creator->getHeightFor(vertex);
        vertex.Z = zUpdate + offset;
    }
}



void FMeshedSurfaceGrid::SplitSelfCuttingPolygonsFromOuterHull(){
    //edgeSetRawTemp.SplitSelfCuttingPolygonsFromOuterHull();
    edgeSetRawTemp.SortByArea();
}

/// --- finding shapes to fit ---
bool FMeshedSurfaceGrid::BoundsSmallerThanStepSize(int x, int y){
    FVector sizeBound = TopRight() - BottomLeft();
    return (sizeBound.X > x) && (sizeBound.Y > y);
}

bool FMeshedSurfaceGrid::CanFindShape(){
    FMeshedPolygonHullIndices &edgeIndices = GetEdgeIndicesRaw();
    return edgeIndices.Num() > 0;
}

bool FMeshedSurfaceGrid::FindShape(
    int x, //in cm
    int y, //in cm
    FVector &outBottomLeft,
    FRotator &outRotation
){
    /*if(!BoundsSmallerThanStepSize(x,y)){
        return false;
    }*/

    float insetStep = 100.0f;
    insetStep = 150.0f;

    //check edge set for a given direction whether the area
    //is free there
    //works on RAW EDGES POLYGON
    //ON LAYER 0!
    FMeshedPolygonHullIndices &edgeIndices = GetEdgeIndicesRaw();
    DebugHelper::logMessage("FMeshedSurfaceGrid::FindShape Attempt: verts: ", edgeIndices.Num());
    for (int i = 0; i < edgeIndices.Num(); i++)
    {
        //edge direction in index space
        FVector2D mainEdgeStepDir;
        
        //nach rechts zeigen für x
        FVector pivot = edgeIndices.Get(i);
        FVector next = edgeIndices.Get(i+1);
        if(edgeIndices.EdgeDirection(i + 1, i, mainEdgeStepDir)){ //i, i-1
            //DrawConnect(pivot, next, 150.0f, FColor::Green); //looks okay
            float angleDot = edgeIndices.EdgeAngleDot(i);
            if(!IsCorner(angleDot)){
                
                FVector2D inset = MakeInsetFromEdgeAngle(angleDot, insetStep);

                //first fit found:

                if (LockArea(pivot, mainEdgeStepDir, inset, x, y, outRotation)){
                    UpdateWorldHeightFor(pivot);
                    outBottomLeft = pivot;
                    return true;
                }
            }
            
            
        }else{
            //DrawConnect(pivot, next, 150.0f, FColor::Yellow); //looks okay
            //DebugHelper::logMessage("FMeshedSurfaceGrid::FindShape::DirInvalid");
            //DrawMarker(pivot, FColor::Red);
        }
    }
    return false;
}

bool FMeshedSurfaceGrid::IsCorner(float dot){
    return IsCorner(dot, cornerUntilDot);
}

bool FMeshedSurfaceGrid::IsCorner(float dot, float epsilon){
    return std::abs(dot) <= epsilon;
}

FVector2D FMeshedSurfaceGrid::MakeInsetFromEdgeAngle(float dot, float depth){
    FVector2D insetStep(0,0);
    if(IsCorner(dot)){
        //perpendicular (x and y inset)
        insetStep = FVector2D(depth, depth);

    }else{
        insetStep = FVector2D(depth, 0.0f); //is tested and correct, inset at x depth
    }
    return insetStep;
}

bool FMeshedSurfaceGrid::LockArea(
    FVector &pivot,
    const FVector2D &stepDir, //zeigt nach rechts
    const FVector2D &inset,
    int xSize, //für nach rechts
    int ySize,
    FRotator &outRotation
){
    // ---- PROBLEM NOCH SEHR UNKLAR! ----
    FVector2D orthogonal(stepDir.Y, -stepDir.X);
    FVector2D rotation = orthogonal;

    FVector copyPivot = pivot;
    TArray<FVector2D> array;
    MakeBounds(xSize, ySize, array, pivot, rotation, inset, outRotation);

    //log okay.
    /*DebugHelper::logMessage(
        FString::Printf(
            TEXT("FMeshedSurfaceGrid::LockArea Update pivot (%.2f, %.2f)->(%.2f, %.2f) (inset %.2f %.2f)"),
            copyPivot.X, copyPivot.Y, pivot.X, pivot.Y, inset.X, inset.Y
        )
    );*/

    bool visible = false;
    bool inside = false;
    if (PolygonCanBeAdded(array, inside, visible))
    {
        FMeshedPolygonHullIndices hull;
        hull.AddAll(array);
        edgeSetRawTemp.Add(hull);
        //Draw(array, FColor::Green);

        /*DebugHelper::logMessage(
            FString::Printf(
                TEXT("FMeshedSurfaceGrid::LockArea OK! dir %.2f %.2f size %d %d "),
                stepDir.X, stepDir.Y,
                xSize, ySize
            )
        );*/


        return true;
    }
    
    /*if(!inside){
        Draw(array, 100.0f, FColor::Red);
    }
    if(!visible){
        Draw(array, 90.0f, FColor::Orange);
    }*/

    
   
    /*DebugHelper::logMessage(
        FString::Printf(
            TEXT("FMeshedSurfaceGrid::LockArea FAILED! dir %.2f %.2f size %d %d "),
            stepDir.X, stepDir.Y,
            xSize, ySize
        )
    );*/
    return false;

}



void FMeshedSurfaceGrid::MakeBounds(
    int x, 
    int y,
    TArray<FVector2D> &outArray,
    FVector &pivot,
    const FVector2D &rotation,
    const FVector2D &inset,
    FRotator &outRotation
){
    pivot.Z = 0.0f;
    MakeBounds(x, y, outArray);

    FVector inset3D(inset.X, inset.Y, 0.0f);

    /*DebugHelper::logMessage(
        FString::Printf(
            TEXT("FMeshedSurfaceGrid::Inset %.2f %.2f"), inset3D.X, inset3D.Y
        )
    );*/

    FRotator rotator = MakeRotator(rotation);
    outRotation = rotator;
    MMatrix t(pivot);
    MMatrix r(rotator);
    MMatrix tInset(inset3D);

    //M = T * R * tInner <-- lese richtung --
    MMatrix M = t * (r * tInset);
    for (int i = 0; i < outArray.Num(); i++){
        outArray[i] = M * outArray[i];
    }

    //update the pivot to offseted location
    pivot = M.getTranslation();
}

void FMeshedSurfaceGrid::MakeBounds(int x, int y, TArray<FVector2D> &outArray){
    /*
    1-->2
    |   |
    0<--3
    */
    outArray.SetNum(4);
    outArray[0] = FVector2D(0, 0);
    outArray[1] = FVector2D(0, y);
    outArray[2] = FVector2D(x, y);
    outArray[3] = FVector2D(x, 0);
}

FRotator FMeshedSurfaceGrid::MakeRotator(const FVector2D &rotation){
    float YawDegrees = FMath::RadiansToDegrees(FMath::Atan2(rotation.Y, rotation.X));
    FRotator outRotator = FRotator(0.0f, YawDegrees, 0.0f);
    return outRotator;
}

void FMeshedSurfaceGrid::Draw(
    const TArray<FVector2D> &array,
    float offset,
    FColor color
){
    if(array.Num() == 4){
        Draw(
            array[0],
            array[1],
            array[2],
            array[3],
            offset,
            color
        );
    }
}


#include "GameCore/world/worldLevelBase.h"
void FMeshedSurfaceGrid::Draw(
    const FVector2D &v0_pair,
    const FVector2D &v1_pair,
    const FVector2D &v2_pair,
    const FVector2D &v3_pair,
    float offset,
    FColor color
){
    FVector a(v0_pair.X, v0_pair.Y, 0.0f);
    FVector b(v1_pair.X, v1_pair.Y, 0.0f);
    FVector c(v2_pair.X, v2_pair.Y, 0.0f);
    FVector d(v3_pair.X, v3_pair.Y, 0.0f);

    UpdateWorldHeightFor(a);
    UpdateWorldHeightFor(b);
    UpdateWorldHeightFor(c);
    UpdateWorldHeightFor(d);

    a.Z += offset;
    b.Z += offset;
    c.Z += offset;
    d.Z += offset;

    UWorld *worldPtr = AworldLevelBase::GetWorldPointer();
    DebugHelper::showQuad(
        worldPtr,
        a, b, c, d,
        color,
        200.0f
    );

    /*FVector zero(0, 0, 0);
    DebugHelper::showLineBetween(worldPtr, zero, a, color, 200.0f);
    DebugHelper::showLineBetween(worldPtr, zero, b, color, 200.0f);
    DebugHelper::showLineBetween(worldPtr, zero, c, color, 200.0f);
    DebugHelper::showLineBetween(worldPtr, zero, d, color, 200.0f);*/

}

void FMeshedSurfaceGrid::DrawMarkers(TArray<FVector> &positions, FColor color){
    for (int i = 0; i < positions.Num(); i++){
        DrawMarker(positions[i], color);
    }
}

void FMeshedSurfaceGrid::DrawMarker(FVector pos, FColor color){
    UWorld *worldPtr = AworldLevelBase::GetWorldPointer();
    UpdateWorldHeightFor(pos);
    FVector offset(0, 0, 1000);
    offset += pos;

    DebugHelper::showLineBetween(worldPtr, pos, offset, color, 200.0f);
}

void FMeshedSurfaceGrid::DrawConnect(FVector posA, FVector posB, float offset, FColor color){
    UWorld *worldPtr = AworldLevelBase::GetWorldPointer();
    UpdateWorldHeightFor(posA);
    UpdateWorldHeightFor(posB);
    FVector offset3D(0,0,offset);
    posA += offset3D;
    posB += offset3D;

    DebugHelper::showLineBetween(worldPtr, posA, posB, color, 200.0f);
}

bool FMeshedSurfaceGrid::PolygonCanBeAdded(
    const TArray<FVector2D> &array,
    bool &insidePolygon,
    bool &visible 
){
    if(array.Num() > 0){
        //visibile hull on very first hull
        //where the polygons are inserted into
        //if every vertex can see each other iterative,
        //they also must be all inside the polygon
        FMeshedPolygonHullIndices &edgeIndices = GetEdgeIndicesRaw();

        FVector notInside;
        insidePolygon = edgeIndices.IsInside(array, notInside);

        //check visibility on all!
        FVector hit;
        visible = edgeSetRawTemp.IsVisibleHull(array);
        
        if(!insidePolygon){
            //DrawMarker(notInside, FColor::Orange);
        }

        return insidePolygon && visible; // visible && inside;
    }
    return false;
    
}






void FMeshedSurfaceGrid::DebugAppendEdgeSetRawAsMeshData(
    float height, 
    MeshData &other,
    float offset 
){
    FVector offsetVector(0, 0, offset);
    DebugAppendEdgeSetRawAsMeshData(height, other, offsetVector);
}

void FMeshedSurfaceGrid::DebugAppendEdgeSetRawAsMeshData(
    float height, 
    MeshData &other, 
    const FVector offset
){
    DebugHelper::logMessage(
        "FMeshedSurfaceGrid::DebugAppendEdgeSetRawAsMeshData Num Polygons ", 
        edgeSetRawTemp.Num()
    );

    int min = 0;
    int max = 1;

    max = std::min(edgeSetRawTemp.Num(), max);
    for (int i = min; i < max; i++)
    {
        FMeshedPolygonHullIndices &edgeIndices = edgeSetRawTemp[i];
        DebugAppendEdgeSetRawAsMeshData(height, other, edgeIndices, offset);
    }
}



void FMeshedSurfaceGrid::DebugAppendEdgeSetRawAsMeshData(
    float height, 
    MeshData &other,
    FMeshedPolygonHullIndices &hull,
    const FVector offsetUp
){
    FVector up(0, 0, height);
    for (int i = 0; i < hull.Num(); i++)
    {
        int next = (i + 1) % hull.Num();
        FVector v0 = hull.Get(i);
        FVector v3 = hull.Get(next);

        //make world height
        UpdateWorldHeightFor(v0);
        UpdateWorldHeightFor(v3);

        v0 += offsetUp;
        v3 += offsetUp;

        FVector v1 = v0 + up;
        FVector v2 = v3 + up;

        other.appendDoublesided(v0, v1, v2, v3);
    }
}