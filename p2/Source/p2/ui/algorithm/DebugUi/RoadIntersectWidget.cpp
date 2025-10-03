#include "RoadIntersectWidget.h"

#include "terrainPlugin/meshgen/generation/TerrainCreator/Road/RoadMaker.h"
#include "CoreMath/vector/bound/FBoundingBox2D.h"
#include "CoreMath/Matrix/2D/MMatrix2D.h"

void URoadIntersectWidget::ConstructWidget(){
    bDebugPolygon = false;
    Super::ConstructWidget();
    
    
    
    if(polygonMap.IsValid()){

        RoadMaker roadmaker;
        roadmaker.createRoads(10);

        //temporary reference, use one at a time!
        SlateMeshDataPolygon &slateMeshPolygon = FindFromMap(0);
        slateMeshPolygon.SetFullColor(FColor::Red);
        //slateMeshPolygon.DrawOutLineOnly(); //not needed.

        TArray<std::pair<FVector2D, FVector2D>> edges = roadmaker.GetEdges();
        FVector2D scaleDesired(300, 300);
        MMatrix2D M = MoveAndScaleToPivot0(edges, scaleDesired);

        //apply M and add to meshdata
        SlateMeshData &meshData = slateMeshPolygon.MeshDataRef();
        for (int i = 0; i < edges.Num(); i++){
            std::pair<FVector2D, FVector2D> &current = edges[i];
            current.first = M * current.first;
            current.second = M * current.second;
            AppendQuadFromEdge(current.first, current.second, meshData);
        }



        //create roads for draw
        SlateMeshDataPolygon &layer1 = FindFromMap(1);
        layer1.SetFullColor(FColor::Cyan);
        SlateMeshData &meshData1 = layer1.MeshDataRef();
        TArray<std::vector<FVector2D>> roadSplines = roadmaker.GetRoads();
        for (int i = 0; i < roadSplines.Num(); i++){
            std::vector<FVector2D> &vec = roadSplines[i];
            
            //apply M
            for (int j = 0; j < vec.size(); j++){
                FVector2D &current = vec[j];
                current = M * current;
            }
            
            for (int j = 1; j < vec.size(); j++){
                FVector2D &a = vec[j - 1];
                FVector2D &b = vec[j];
                AppendQuadFromEdge(a, b, meshData1);
            }
        }
    }
}

void URoadIntersectWidget::AppendQuadFromEdge(FVector2D &a, FVector2D &b, SlateMeshData &meshData){
    FVector2D &v0 = a;
    FVector2D &v1 = b;
    FVector2D dir = (v1 - v0).GetSafeNormal();

    FVector2D normal(dir.Y, -dir.X);
    int width = 5;
    FVector2D v2 = v1 + normal * width;
    FVector2D v3 = v0 + normal * width;

    meshData.Append(v0, v1, v2, v3);
}






MMatrix2D URoadIntersectWidget::MoveAndScaleToPivot0(
    TArray<std::pair<FVector2D, FVector2D>> &edges,
    FVector2D &scaleDesired
){
    FBoundingBox2D box;
    for (int i = 0; i < edges.Num(); i++){
        std::pair<FVector2D, FVector2D> &current = edges[i];
        box.Update(current.first);
        box.Update(current.second);
    }

    FVector2D translation = box.min() * -1.0f;
    FVector2D scaleCurrent = box.size();

    MMatrix2D t;
    t.SetTranslation(translation);


    //skalar = distTarget / distAll


    //x * current = desired
    //x = desired / current
    MMatrix2D s; //Scale(float scaleX, float scaleY)
    s.Scale(
        scaleDesired.X / scaleCurrent.X,
        scaleDesired.Y / scaleCurrent.Y
    );

    //M = s * t <-- lese richtung --
    MMatrix2D M = s * t;
    return M;
}
