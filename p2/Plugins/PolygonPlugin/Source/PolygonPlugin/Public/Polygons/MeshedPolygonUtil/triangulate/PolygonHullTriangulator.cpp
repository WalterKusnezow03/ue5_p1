#include "PolygonHullTriangulator.h"


void FPolygonHullTriangulator::Clear(){
    triangles.Empty();
}

void FPolygonHullTriangulator::Triangulate(
    const TArray<std::pair<int, int>> indices,
    bool clockwise
){
    Clear();
    if (indices.Num() < 3)
    {
        return;
    }

    //first sign for normal direction
    //if triangle is valid
    int signWorld = clockwise ? 1 : -1;
    bool bSignWorld = signWorld >= 0;

    bool earFound = false;

    
    TArray<std::pair<int, int>> indicesCurrent = indices;
    while(indicesCurrent.Num() > 2){

        earFound = false;
        for(int i = 0; i < indicesCurrent.Num(); i++){
            int t0 = i;
            int t1 = (i + 1) % indicesCurrent.Num();
            int t2 = (i + 2) % indicesCurrent.Num();

            const std::pair<int, int> &v0 = indicesCurrent[t0];
            const std::pair<int, int> &v1 = indicesCurrent[t1];
            const std::pair<int, int> &v2 = indicesCurrent[t2];

            FPolygonTriangle triangle;
            triangle.Setup(v0, v1, v2);


            if(!triangle.IsDegenerate() && !triangle.AnyInsideIgnoreSupportPoints(indicesCurrent)){

                //dann wird geprüft wo die ebene hin aufgespannt wird
                //wenn es die selbe order ist wie world, zeigen die normalen beide nach oben
                //bzw unten. Das dreieck wird effektiv gezwungen nach innen orientiert zu sein.
                if(triangle.GetNormalSign() == bSignWorld){
                    //valid: remove t1
                    indicesCurrent.RemoveAt(t1);
                    triangles.Add(triangle);
                    //i = indicesCurrent.Num(); //start from beginning
                    earFound = true;
                }
            }
        }

        if(!earFound){
            break;
        }
    }
    DebugHelper::logMessage(
        FString::Printf(
            TEXT("FPolygonHullTriangulator::generated triangles %d, indices left %d of %d"),
            triangles.Num(),
            indicesCurrent.Num(),
            indices.Num()
        )
    );
}





bool FPolygonHullTriangulator::InsideHull(int x, int y)const{
    for (int i = 0; i < triangles.Num(); i++){
        const FPolygonTriangle &current = triangles[i];
        if(current.InsideTriangle(x,y)){
            return true; //eins anschlagen reicht.
        }
    }
    return false;
}