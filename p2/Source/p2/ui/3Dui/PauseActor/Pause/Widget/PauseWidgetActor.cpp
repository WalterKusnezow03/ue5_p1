#include "PauseWidgetActor.h"


APauseWidgetActor::APauseWidgetActor() : Super() {
    
}


//init on beginplay
void APauseWidgetActor::CreateWidgetMeshData(){
    if(!bMeshDataCreated && Widget){
        float xMax = 300.0f; // 1920.0f;
        float yMax = 600.0f; // 1080.0f;

        /*
        1-->2
        |   |
        0<--3
        */

        FVector v0(0, 0, 0);
        FVector v1(0, 0, yMax);
        FVector v2(0, xMax, yMax);
        FVector v3(0, xMax, 0);

        FVector2D uv0(0, 1);
        FVector2D uv1(0, 0);
        FVector2D uv2(1, 0);
        FVector2D uv3(1, 1);

        MeshData &data = Widget->GetMeshDataRef();
        data.appendVerteciesAndUvs(
            v0, v1, v2, v3, 
            uv0, uv1, uv2, uv3
        );

        data.centerMesh();

        MMatrix scale;
        float factor = 0.1f;
        scale.scale(factor, factor, factor);
        data.transformAllVertecies(scale);
        data.calculateNormals();
        //data.flipNormals(); //shit happens
        data.flipWindingOrder();

        bMeshDataCreated = true;

        SetDrawSize(FVector2D(xMax, yMax));

        ScaleMeshDataToDesiredScale();
    }
}

void APauseWidgetActor::ScaleMeshDataToDesiredScale(){
    ScaleMeshDataToMaxCm(scaleCmMeshDataTargeted);
}
