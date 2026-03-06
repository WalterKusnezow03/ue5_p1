#include "WidgetProgressQuadCircular.h"

#include "customUiPlugin/slate/MeshData2D/BaseMeshData/SlateMeshData.h"


//TODO: 

//void SlateMeshDataPolygon::EnableRender(bool bFlag)
//virtual void SetResolution(FVector2D scale);


//constructor.
void UWidgetProgressQuadCircular::ConstructWidget(){
    //bDebugPolygon = false; //from super
    //Super::ConstructWidget(); // debug

    CreateBackground();
    CreateTiles();
    SetDefaultColors();
}

//temporary reference, use one at a time!
SlateMeshDataPolygon &UWidgetProgressQuadCircular::PolygonBackground(){
    return FindFromMap(layerBackground);
}

//temporary reference, use one at a time!
SlateMeshDataPolygon &UWidgetProgressQuadCircular::PolygonForeGround(int tileId){
    tileId = VerifyTileIdToPolygonId(tileId);
    return FindFromMap(tileId);
}

int UWidgetProgressQuadCircular::VerifyTileIdToPolygonId(int tileId){
    tileId++;
    tileId = std::max(tileId, 1);
    tileId = std::min(tileId, TilesTotal()); //offset one already given.
    return tileId;
}





void UWidgetProgressQuadCircular::CreateBackground(){
    FVector2D a(0, 0);
    FVector2D b(setupResolution, setupResolution);

    SlateMeshDataPolygon &background = PolygonBackground();
    SlateMeshData &meshData = background.MeshDataRef();
    meshData.AppendQuad(a, b);
    
    
}


int UWidgetProgressQuadCircular::TilesPerAxis(){
    return setupResolution;
}

float UWidgetProgressQuadCircular::TileSize(){
    return TilesPerAxis() / setupResolution;
}

int UWidgetProgressQuadCircular::TilesTotal(){
    return TilesPerAxis() * 4; //erstmal so lassen
}


void UWidgetProgressQuadCircular::CreateTiles(){
    //start top left
    FVector2D pivot(0, 0);
    int tileId = 0;
    //0-to->x
    for (int x = 0; x < TilesPerAxis(); x++)
    {
        AppendTile(pivot, tileId);

        pivot += FVector2D(TileSize(), 0);
        tileId++;
    }

    //x
    //|to down
    //y
    for (int y = 0; y < TilesPerAxis(); y++)
    {
        AppendTile(pivot, tileId);

        pivot += FVector2D(0, TileSize());
        tileId++;
    }

    //0<--to--y
    for (int y = 0; y < TilesPerAxis(); y++)
    {
        AppendTile(pivot, tileId);

        pivot -= FVector2D(TileSize(), 0);
        tileId++;
    }


    //x
    //|to up
    //y
    for (int y = 0; y < TilesPerAxis(); y++)
    {
        AppendTile(pivot, tileId);

        pivot -= FVector2D(0, TileSize());
        tileId++;
    }

}

void UWidgetProgressQuadCircular::AppendTile(FVector2D pivot, int tileId){
    FVector2D a = pivot + FVector2D(0, 0);
    FVector2D b = pivot + FVector2D(TileSize(), TileSize());

    SlateMeshDataPolygon &tilePolygon = PolygonForeGround(tileId);
    SlateMeshData &meshData = tilePolygon.MeshDataRef();
    meshData.AppendQuad(a, b);    
}












void UWidgetProgressQuadCircular::SetDefaultColors(){
    SetColorForeground(FLinearColor::Green);
    SetColorBackground(FLinearColor::White);
}






void UWidgetProgressQuadCircular::SetColorBackground(FLinearColor color){
    SlateMeshDataPolygon &background = PolygonBackground();
    background.SetFullColor(color);
}

void UWidgetProgressQuadCircular::SetColorForeground(FLinearColor color){
    for (int i = 0; i < TilesTotal(); i++){
        SlateMeshDataPolygon &polygon = PolygonForeGround(i);
        polygon.SetFullColor(color);
    }
}






void UWidgetProgressQuadCircular::SetProgress(float num){
    // scale for foreground by scalar
    EnableTileRangeByScalar(0, num, true);
    EnableTileRangeByScalar(num, 1, false);
}

void UWidgetProgressQuadCircular::EnableTileRangeByScalar(
    float scalarStart, 
    float scalarEnd,
    bool enableRender
){
    scalarStart = ClampProgress(scalarStart);
    scalarEnd = ClampProgress(scalarEnd);
    int startTileId = TilesTotal() * scalarStart;
    int endTileId = TilesTotal() * scalarEnd;

    EnableTileRangeById(startTileId, endTileId, enableRender);
}

void UWidgetProgressQuadCircular::EnableTileRangeById(
    int startId,
    int endId,
    bool enableRender
){
    for (int i = startId; i < endId; i++){
        SlateMeshDataPolygon &tile = PolygonForeGround(i);
        tile.EnableRender(enableRender);
    }
}






void UWidgetProgressQuadCircular::ResetProgress(){
    SetProgress(1.0f);
}


float UWidgetProgressQuadCircular::ClampProgress(float num){
    num = std::max(num, 0.0f);
    num = std::min(num, 1.0f);
    return num;
}


