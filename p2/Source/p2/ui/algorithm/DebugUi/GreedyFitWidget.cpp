#include "GreedyFitWidget.h"

#include "customUiPlugin/slate/MeshData2D/SlateMeshDataPolygon.h"
#include "CoreMath/algorithm/PolygonFit/GreedyFilledPolygon.h"

void UGreedyFitWidget::ConstructWidget(){
    bDebugPolygon = false;
    Super::ConstructWidget();
    
    
    
    if(polygonMap.IsValid()){

        TArray<MPolygon> created;
        TestPolygonFit(created);

        if(created.Num() > 0){

            AppendIntoLayer(created[0], 0); //different layer for outline polygon

            //create mesh data from polygon
            for (int i = 1; i < created.Num(); i++){
                AppendIntoLayer(created[i], i);
            }
        }
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
    slateMeshPolygon.SetColor(color);


}



void UGreedyFitWidget::TestPolygonFit(TArray<MPolygon> &outPolygons){

    GreedyFilledPolygon filled;
    TArray<FVector2D> shapeA{
        FVector2D(0, 0),
        FVector2D(0, 200),
        FVector2D(200, 200),
        FVector2D(200, 0)
    };
    /*
    TArray<FVector2D> shapeB = shapeA;
    MMatrix2D R;
    R.RadAdd(MMatrix2D::degToRadian(45));
    for (int i = 0; i < shapeB.Num(); i++){
        shapeB[i] = R * shapeB[i];
    };

    shapeA.Append(shapeB);
    GrahamScan2D sorter;
    sorter.ComputeConvexHull(shapeA);
    */
    filled.SetShape(shapeA, 10.0f);



    TArray<MPolygon> polygons;
    for (int i = 0; i < 10; i++){
        MPolygon _polygon;
        TArray<FVector2D> shape{
            FVector2D(0, 0),
            FVector2D(0, 50),
            FVector2D(50, 50),
            FVector2D(50, 0)
        };
        _polygon.SetShape(shape);
        polygons.Add(_polygon);
    }

    for (int i = 0; i < 20; i++){
        MPolygon _polygon;
        TArray<FVector2D> shape{
            FVector2D(0, 0),
            FVector2D(0, 10),
            FVector2D(10, 10),
            FVector2D(10, 0)
        };
        _polygon.SetShape(shape);
        polygons.Add(_polygon);
    }

    for (int i = 0; i < 15; i++){
        MPolygon _polygon;
        TArray<FVector2D> shape{
            FVector2D(0, 0),
            FVector2D(20, 30),
            FVector2D(20, 0)
        };
        _polygon.SetShape(shape);
        polygons.Add(_polygon);
    }
    
    

    filled.Add(polygons); //tries to add all polygons
    //filled.Append(polygons);

    filled.AppendAllPolygonsIncludingSelfIntoArray(outPolygons); //data copied into array
}