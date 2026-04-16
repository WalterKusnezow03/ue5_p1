#include "NNPathFinderProxy.h"
#include "PathFinder/pathFinding/PathFinder.h"


#include "StoragePlugin/Storage/ImageData/ImageWriter/ImageWriter.h"
#include "Async/Async.h"

// ---- debugger ----

NNPathFinderProxy::NNPathFinderProxy(){
    timer.Begin(1.0f);
}

void NNPathFinderProxy::DebugSaveImage(
    FVector playerPos,
    float radius,
    float deltatime
){
    timer.Tick(deltatime);
    if (timer.timesUp()){
        DebugSaveImage(playerPos, radius);
        timer.Begin(1.0f);
    }
}


void NNPathFinderProxy::DebugSaveImage(
    FVector playerPos,
    float radius
){
    
    if(APathFinder *instance = APathFinder::instance()){

        /// ---- doesnt create too much of a overhead but very light stutter ----
        if(instance->TryGetSubGraphPolygonMesh(
            meshedPolygonStatic,
            playerPos,
            radius
        )){
            DebugHelper::logMessage("NNPathFinderProxy::CreatePolygon Try Success");
            if(false){
                FString nameImage = FString::Printf(
                    TEXT("extracted_%d_%d"), (int) playerPos.X, (int) playerPos.Y
                );
                DebugSaveMeshedPolygonToStorage(meshedPolygonStatic, nameImage);
            }
        }
        
        /*Async(EAsyncExecution::ThreadPool, [instance, playerPos, radius, this]()
        {
            DebugHelper::logMessage("NNPathFinderProxy::CreatePolygon Try (Async)");

            FMeshedPolygon outData;

            if(instance->TryGetSubGraphPolygonMesh(
                outData,
                playerPos,
                radius
            )){
                DebugHelper::logMessage("NNPathFinderProxy::CreatePolygon Try Success (Async)");


                if(false){
                    FString nameImage = FString::Printf(
                        TEXT("extracted_%d_%d"), (int) playerPos.X, (int) playerPos.Y
                    );
                    this->DebugSaveMeshedPolygonToStorage(outData, nameImage);
                }
            }
        });*/
        
    }

}


void NNPathFinderProxy::DebugSaveMeshedPolygonToStorage(
    FMeshedPolygon &polygon,
    FString nameImage
){
    if(polygon.IsValid()){
        TArray<FColor> colors;
        FColor locked = FColor::Red;
        FColor free = FColor::Cyan;

        int x, y;
        polygon.GenerateColorBitmap(colors, locked, free, x, y);

        if (colors.Num() > 0){
            ImageWriter::SaveColorBufferAsPngFromName(
                (uint8*)colors.GetData(),
                x,
                y,
                "NNPathFinderProxyDebug",
                nameImage
            );
        }
    }
}







// ---- default code ----

bool NNPathFinderProxy::PredictNode(
    FVector playerPos,
    FVector playerDirection,
    float timeSinceSeen,
    FVector ownPositon,
    FRotator ownRotation,
    float radius,
    FVector &outPrediction,
    int botId
){
    if(APathFinder *instance = APathFinder::instance()){

        /// --- MAKE ASNCY ? ---

        // --- Returns for bot with id ---


        FMeshedPolygon outData;
        if(instance->TryGetSubGraphPolygonMesh(
            outData,
            playerPos,
            radius
        )){

        }
    }


    return false;
}

void NNPathFinderProxy::CollectNodePositionsLocalSpace(
    const FVector &pos, 
    float radius,
    TArray<FVector> &outArray,
    FVector ownPositon,
    FRotator rotation
){
    ownPositon *= -1.0f;
    MMatrix t1(ownPositon);

    MMatrix r1(rotation);
    r1.transpose();

    //M = T * R <-- lese richtung --
    //M^-1 = R^T * T^-1
    MMatrix m1 = r1 * t1;
    CollectNodePositionsLocalSpace(pos, radius, outArray, m1);
}

void NNPathFinderProxy::CollectNodePositionsLocalSpace(
    const FVector &pos, 
    float radius,
    TArray<FVector> &outArray,
    MMatrix &MInverse
){
    if(APathFinder *instance = APathFinder::instance()){
        instance->CollectNodePositions(pos, radius, outArray);
        
        for (int i = 0; i < outArray.Num(); i++){
            outArray[i] = MInverse * outArray[i];
        }
    }
}

