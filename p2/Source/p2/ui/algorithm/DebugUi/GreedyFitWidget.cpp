#include "GreedyFitWidget.h"

#include "customUiPlugin/slate/MeshData2D/BaseMeshData/MeshDataWrap/SlateMeshDataPolygon.h"

void UGreedyFitWidget::ConstructWidget(){
    bDebugPolygon = false;
    Super::ConstructWidget();
    
    
    
    if(polygonMap.IsValid()){

        //rasterize fit
        TestPolygonFitRasterized();
    }
}

void UGreedyFitWidget::AppendIntoLayer(MPolygon &polygon, int layer){
    //temporary reference, use one at a time!
    SlateMeshDataPolygon &slateMeshPolygon = FindFromMap(layer);
    TArray<FVector2D> &internalShape = polygon.internalTransformed();

    if(layer == 0){
        slateMeshPolygon.DrawOutLineOnly(); //DEBUG
    }

    FLinearColor color = 
    layer % 2 == 0 ?
    FLinearColor::Red :
    FLinearColor::Green;

    slateMeshPolygon.AppendClosedShape(internalShape);
    slateMeshPolygon.SetFullColor(color);


}







#include "CoreMath/algorithm/PolygonFitRasterized/MPolygonRasterizer.h"
#include "CoreMath/algorithm/PolygonFitRasterized/MPolygonRaster.h"

void UGreedyFitWidget::TestPolygonFitRasterized(){
    
    //bool MPolygonRaster::Fit(MPolygon &polygon)
    TArray<MPolygon> polygons;
    {
        MPolygon _polygon;
        TArray<FVector2D> shape{
            FVector2D(0, 0),
            FVector2D(0, 20),
            FVector2D(70, 50),
            FVector2D(50, 0)
        };
        _polygon.SetShape(shape);
        polygons.Add(_polygon);
    }
    {
        MPolygon _polygon;
        TArray<FVector2D> shape{
            FVector2D(0, 0),
            FVector2D(0, 40),
            FVector2D(40, 20),
            FVector2D(40, 0)
        };
        _polygon.SetShape(shape);
        polygons.Add(_polygon);
        polygons.Add(_polygon);
        polygons.Add(_polygon);
        
    }
    /*
    {
        MPolygon _polygon;
        TArray<FVector2D> shape{
            FVector2D(0, 0),
            FVector2D(12, 40),
            FVector2D(10, 50),
            FVector2D(10, 0)
        };
        _polygon.SetShape(shape);
        polygons.Add(_polygon);
    }
    */





    MPolygon outer;
    TArray<FVector2D> shape{
        FVector2D(0, 0),
        FVector2D(0, 50),
        FVector2D(100, 100),
        FVector2D(100, 0)
    };
    outer.SetShape(shape);
    MPolygonRasterizer maker;
    MPolygonRaster rasterizedOuter = maker.MakeRasterized(outer);

    int pixelStep = 20;
    TArray<FVector2D> quadsOuter = rasterizedOuter.createQuadMeshData(pixelStep);
    AppendIntoLayer(quadsOuter, 0, FColor::Green);

    int loops = 3;
    for (int i = 0; i < loops; i++){
        rasterizedOuter.TryFitAll(polygons);
    }

    
    TArray<TArray<FVector2D>> quadPolygons = rasterizedOuter.createQuadMeshDataChildren(pixelStep);
    for (int i = 0; i < quadPolygons.Num(); i++){
        TArray<FVector2D> &currentQuadShapedBuffer = quadPolygons[i];
        AppendIntoLayer(currentQuadShapedBuffer, i + 1);
    }
    DebugHelper::logMessage("UGreedyFitWidget MPolygonRaster created childs ", quadPolygons.Num());

    //rasterizedOuter.Log();
}



void UGreedyFitWidget::AppendIntoLayer(TArray<FVector2D> &currentQuadShapedBuffer, int layer){
    TArray<FColor> colors = {FColor::Yellow, FColor::Red, FColor::Blue};
    FColor color = colors[layer % colors.Num()];
    AppendIntoLayer(currentQuadShapedBuffer, layer, color);
}


void UGreedyFitWidget::AppendIntoLayer(
    TArray<FVector2D> &currentQuadShapedBuffer, 
    int layer, 
    FColor color
){
    DebugHelper::logMessage("UGreedyFitWidget MPolygonRaster child buffer count ", currentQuadShapedBuffer.Num());
    //temporary reference, use one at a time!
    SlateMeshDataPolygon &polygon = FindFromMap(layer);
    SlateMeshData &mesh = polygon.MeshDataRef();
    mesh.AppendEfficentQuadShapedBuffer(currentQuadShapedBuffer);
    polygon.SetFullColor(color);

}