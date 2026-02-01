#include "MiniMapData.h"

MiniMapData::MiniMapData(){

}

MiniMapData::~MiniMapData(){

}


TArray<AActor *> &MiniMapData::Find(EMarkerType type){
    if(typeMap.find(type) == typeMap.end()){
        typeMap[type] = TArray<AActor *>();
    }
    return typeMap[type];
}

void MiniMapData::UpdatePlayerTransform(AActor *player){
    if(player){
        FRotator r = player->GetActorRotation();
        r.Pitch = 0.0f;
        r.Roll = 0.0f;
        FVector l = player->GetActorLocation();

        playerRotation.setRotation(r);
        playerLocation.setTranslation(l);
        MakePlayerInverse();
    }
}

void MiniMapData::MakePlayerInverse(){
    // M = T * R //<-- lese richtung --
    // M^-1 = R^T * T^-1 // <-- lese richtung --
    MMatrix t1 = playerLocation.invertedTranslation();
    PlayerRotationInverse = playerRotation.transposedRotation();
    PlayerTransformInverse = PlayerRotationInverse * t1; //M1 = R1 * T1 <-- lese richtung --
}



std::map<EMarkerType, TArray<FMiniMapMarkerTransform>> &MiniMapData::MapFromCollectMarkersCanvasSpace(
    const FVector2D &canvasScale
){
    /*DebugHelper::showScreenMessage(
        FString::Printf(
            TEXT("MiniMapData::Collect With scale %.2f %.2f"),
            canvasScale.X,
            canvasScale.Y
        )
    );*/

    UpdateMarkersCanvasSpace(canvasScale);
    // return cached map.
    return visibleMarkerMap;
}

void MiniMapData::UpdateMarkersCanvasSpace(const FVector2D &canvasScale){
    
    FVector2D halfScale = canvasScale / 2.0f;
    for (auto &pair : typeMap)
    {
        EMarkerType type = pair.first;
        if(visibleMarkerMap.find(type) == visibleMarkerMap.end()){
            visibleMarkerMap[type] = TArray<FMiniMapMarkerTransform>();
        }
        TArray<FMiniMapMarkerTransform> &array = visibleMarkerMap[type];
        
        //clear array before hand
        array.Empty();

        CollectMarkersCanvasSpace(type, array, canvasScale, halfScale);
    }    
}




void MiniMapData::CollectMarkersCanvasSpace(
    EMarkerType type, 
    TArray<FMiniMapMarkerTransform> &outMarkers,
    const FVector2D &canvasScale,
    const FVector2D &canvasHalfScale
){
    CollectMarkersWorld(outMarkers, type);
    MoveToCanvasSpace(outMarkers, canvasScale, canvasHalfScale);
}

void MiniMapData::MoveToCanvasSpace(
    TArray<FMiniMapMarkerTransform> &array, 
    const FVector2D &canvasScale,
    const FVector2D &canvasHalfScale
){
    for (int i = 0; i < array.Num(); i++){
        MoveToCanvasSpace(
            array[i].GetPositionRef(), //get by ref
            canvasScale, 
            canvasHalfScale
        );
    }
}

void MiniMapData::MoveToCanvasSpace(
    FVector2D &pos, 
    const FVector2D &canvasScale,
    const FVector2D &canvasHalfScale
){
    //skalar = distTarget / distAll
    //toCanvas = skalar * someNumber
    float radius2 = maxRadiusMap * 2;
    float scalarX = pos.X / radius2; // maxRadiusMap;
    float scalarY = pos.Y / radius2; //maxRadiusMap;
    pos.X = scalarX * canvasScale.X;
    pos.Y = scalarY * canvasScale.Y;

    //offset to center
    pos += canvasHalfScale;
    FlipXY(pos);
    InvertYAxis(pos, canvasScale);
}

// flip x and y, since x is forward and up, and
// not y (from ui to game perspective coordinates)
void MiniMapData::FlipXY(FVector2D &pos){
    double copy = pos.X;
    pos.X = pos.Y;
    pos.Y = copy;
}


//flip Y axis, because it goes down instead of up
void MiniMapData::InvertYAxis(FVector2D &pos, const FVector2D &canvasScale){
    pos.Y = canvasScale.Y - pos.Y;
}


void MiniMapData::CollectMarkersWorld( 
    TArray<FMiniMapMarkerTransform> &outMarkers,
    EMarkerType type
){
    TArray<AActor *> &array = Find(type);
    for (int i = 0; i < array.Num(); i++){
        if(AActor *current = array[i]){
            FVector2D result = LocationInPlayerRelativeSpace(current);
            if(InRange(result)){


                FMiniMapMarkerTransform transformResult(
                    result, 
                    DegRotationInPlayerRelativeSpace(current)
                );


                outMarkers.Add(transformResult);


                /*FString message = FString::Printf(
                    TEXT("MiniMapData::marker(%.2f %.2f)"),
                    result.X,
                    result.Y
                );
                DebugHelper::showScreenMessage(message, FColor::Yellow);*/
            }
        }
    }
}

bool MiniMapData::InRange(FVector2D &location){
    return location.Size() <= maxRadiusMap;
}


// ------ Transform inverting to relative space of player -------

FVector2D MiniMapData::LocationInPlayerRelativeSpace(AActor *actor){
    if(actor){
        return LocationInPlayerRelativeSpace(actor->GetActorLocation());
    }
    return FVector2D(0, 0);
}

FVector2D MiniMapData::LocationInPlayerRelativeSpace(FVector location){
    FVector result3D = PlayerTransformInverse * location;
    return FVector2D(result3D.X, result3D.Y);
}


float MiniMapData::DegRotationInPlayerRelativeSpace(AActor *actor){
    if(actor){
        return DegRotationInPlayerRelativeSpace(actor->GetActorRotation());
    }
    return 0.0f;
}

float MiniMapData::DegRotationInPlayerRelativeSpace(FRotator rotation){

    //player look always forward
    MMatrix rIn;
    rIn.setRotation(rotation);
    MMatrix playerSpaceRotation = PlayerRotationInverse * rIn; //<-- lese richtung --
    FVector forward(1, 0, 0);
    FVector rotated = playerSpaceRotation * forward;

    FVector2D forward2D(1, 0);
    FVector2D rotated2D(rotated.X, rotated.Y);

    //testing and all needed!
    float angle = MMatrix::signedAngleRadBetween(forward2D, rotated2D);
    float deg = MMatrix::radToDegree(angle);

    return deg;

    //return result.extractRotation();
}

// ----- add and remove ------

void MiniMapData::AddMarker(EMarkerType type, AActor *actor){
    if(actor){
        TArray<AActor *> &actors = Find(type);
        
        if(!actors.Contains(actor)){
            actors.Add(actor);
            DebugHelper::logMessage("MiniMapData::AddedActor");
        }
        
    }
}

void MiniMapData::RemoveMarker(AActor *actor){
    if(!actor){
        return;
    }

    for(auto & pair : typeMap){
        TArray<AActor *> &currentArray = pair.second;
        int index = currentArray.Find(actor);

        //swap pop back
        if (index != INDEX_NONE){
            currentArray[index] = currentArray.Last();
            currentArray.Pop();
        }
    }
}