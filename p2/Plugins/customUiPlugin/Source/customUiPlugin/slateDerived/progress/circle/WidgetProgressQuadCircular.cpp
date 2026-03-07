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
    SetColorsFromProperty();
    SetProgress(progressScalar); //DEBUG SHOWCASE
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
    FVector2D b = desiredResolution;

    SlateMeshDataPolygon &background = PolygonBackground();
    SlateMeshData &meshData = background.MeshDataRef();
    meshData.AppendQuad(a, b);
    
    
}



int UWidgetProgressQuadCircular::TilesPerAxisX(){
    return TilesPerAxis(desiredResolution.X, tileResolution) - 1;
}

int UWidgetProgressQuadCircular::TilesPerAxisY(){
    return TilesPerAxis(desiredResolution.Y, tileResolution) - 1;
}

int UWidgetProgressQuadCircular::TilesPerAxis(double axis, double size){
    size = std::max(size, 1.0);
    return axis / size;
}







int UWidgetProgressQuadCircular::TilesTotal(){
    return TilesPerAxisX() * 2 + TilesPerAxisY() * 2; //erstmal so lassen
}


void UWidgetProgressQuadCircular::CreateTiles(){

    //start top left
    FVector2D pivot(0, 0);
    int tileId = 0;
    //0-to->x
    for (int x = 0; x < TilesPerAxisX(); x++)
    {
        AppendTile(pivot, tileId);

        pivot += FVector2D(tileResolution, 0);
        tileId++;
    }

    //x
    //|to down
    //y
    for (int y = 0; y < TilesPerAxisY(); y++)
    {
        AppendTile(pivot, tileId);

        pivot += FVector2D(0, tileResolution);
        tileId++;
    }

    //0<--to--y on x
    for (int x = 0; x < TilesPerAxisX(); x++)
    {
        AppendTile(pivot, tileId);

        pivot -= FVector2D(tileResolution, 0);
        tileId++;
    }


    //x
    //|to up
    //y
    for (int y = 0; y < TilesPerAxisY(); y++)
    {
        AppendTile(pivot, tileId);

        pivot -= FVector2D(0, tileResolution);
        tileId++;
    }

}

void UWidgetProgressQuadCircular::AppendTile(FVector2D pivot, int tileId){
    FVector2D a = pivot + FVector2D(0, 0);
    FVector2D b = pivot + FVector2D(tileResolution, tileResolution);

    SlateMeshDataPolygon &tilePolygon = PolygonForeGround(tileId);
    SlateMeshData &meshData = tilePolygon.MeshDataRef();
    meshData.AppendQuad(a, b);    
}







void UWidgetProgressQuadCircular::SetColorsFromProperty(){
    SetColorForeground(FLinearColor(colorForeground));
    SetColorBackground(FLinearColor(colorBackground));
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
    Super::SetProgress(num);
    // scale for foreground by scalar
    EnableTileRangeByScalar(0, progressScalar, true);
    EnableTileRangeByScalar(progressScalar, 1, false);
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




