#include "RoadIntersectWidget.h"

#include "terrainPlugin/meshgen/generation/TerrainCreator/Road/RoadMaker.h"
#include "CoreMath/vector/bound/FBoundingBox2D.h"
#include "CoreMath/Matrix/2D/MMatrix2D.h"

void URoadIntersectWidget::ConstructWidget(){
    bDebugPolygon = false;
    Super::ConstructWidget();
    
    
    
    if(polygonMap.IsValid()){

        RoadMaker roadmaker;
        roadmaker.createRoads(10); //chunks 10


        TArray<TArray<std::pair<FVector2D, FVector2D>>> edges = roadmaker.GetEdges(); //edges per intersection
        TArray<std::vector<FVector2D>> roadSplines = roadmaker.GetRoads();
        FVector2D scaleDesired(500, 500);
        MMatrix2D M = MoveAndScaleToPivot0(edges, roadSplines, scaleDesired);

        //apply M and add to meshdata
        for (int i = 0; i < edges.Num(); i++){
            AppendToPolygonAtLayer(
                edges[i],
                i + 1, // next layer after road cyan, layer 0
                M
            );
        }

        //create roads for draw
        SlateMeshDataPolygon &layer1 = FindFromMap(0);
        layer1.SetFullColor(FColor::Cyan);
        SlateMeshData &meshData1 = layer1.MeshDataRef();
        
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


        

        
        FVector2D drawOffset(520, 0);
        TArray<TArray<FVector2D>> &polygons = roadmaker.BuildedSectionsRef();
        for (int i = 0; i < polygons.Num(); i++){
            TArray<FVector2D> &polygon = polygons[i];
            
        
            //make polygon per layer
            SlateMeshDataPolygon &layer_current = FindFromMap(2 + i);

            FColor color = ColorByIndex(i);
            SlateMeshData &meshDataCurrent = layer_current.MeshDataRef();

            for (int j = 1; j < polygon.Num(); j++){
                FVector2D v0 = M * polygon[j - 1];
                FVector2D v1 = M * polygon[j];

                v0 += drawOffset;
                v1 += drawOffset;

                AppendQuadFromEdge(
                    v0,
                    v1,
                    meshDataCurrent
                );
            }

        }
    }
}

void URoadIntersectWidget::AppendQuadFromEdge(FVector2D &a, FVector2D &b, SlateMeshData &meshData){
    FVector2D &v0 = a;
    FVector2D &v1 = b;
    FVector2D dir = (v1 - v0).GetSafeNormal();

    FVector2D normal(dir.Y, -dir.X);
    int width = 2;
    FVector2D v2 = v1 + normal * width;
    FVector2D v3 = v0 + normal * width;

    meshData.Append(v0, v1, v2, v3);
}

FColor URoadIntersectWidget::ColorByIndex(int index){
    //create closed roads from traversed Polygons
    TArray<FColor> colors = {
        FColor::Green, 
        FColor::Cyan, 
        FColor::Yellow, 
        FColor::Black,
        FColor::Red,
        FColor::Blue,
        FColor::Magenta
    };
    int iMod = index % colors.Num();
    return colors[iMod];
}

void URoadIntersectWidget::AppendToPolygonAtLayer(
    TArray<std::pair<FVector2D, FVector2D>> &edges,
    int index,
    MMatrix2D &M //offset / scaling matrix
){
    SlateMeshDataPolygon &slateMeshPolygon = FindFromMap(index); //new layer for intersections plot
    SlateMeshData &meshData = slateMeshPolygon.MeshDataRef();

    //slateMeshPolygon.SetFullColor(FColor::Red);
    slateMeshPolygon.SetFullColor(ColorByIndex(index));

    for (int i = 0; i < edges.Num(); i++){

        std::pair<FVector2D, FVector2D> &current = edges[i];
        current.first = M * current.first;
        current.second = M * current.second;
        AppendQuadFromEdge(current.first, current.second, meshData);
    }
}








MMatrix2D URoadIntersectWidget::MoveAndScaleToPivot0(
    TArray<TArray<std::pair<FVector2D, FVector2D>>> &edges,
    TArray<std::vector<FVector2D>> &splines,
    FVector2D &scaleDesired
){
    FBoundingBox2D box;
    UpdateFromBatch(box, edges);
    UpdateFromBatch(box, splines);


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



void URoadIntersectWidget::UpdateFromBatch(
    FBoundingBox2D &box, 
    TArray<TArray<std::pair<FVector2D, FVector2D>>> &array
){
    for (int i = 0; i < array.Num(); i++){
        UpdateFromBatch(box, array[i]);
    }
}

void URoadIntersectWidget::UpdateFromBatch(
    FBoundingBox2D &box, 
    TArray<std::pair<FVector2D, FVector2D>> &array
){
    for (int i = 0; i < array.Num(); i++){
        std::pair<FVector2D, FVector2D> &current = array[i];
        box.Update(current.first);
        box.Update(current.second);
    }
}

void URoadIntersectWidget::UpdateFromBatch(
    FBoundingBox2D &box, 
    TArray<std::vector<FVector2D>> &splines
){
    for (int i = 0; i < splines.Num(); i++){
        const std::vector<FVector2D> &buffer = splines[i];
        box.Update(buffer);
    }
}