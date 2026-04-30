#include "NNPathFinderProxy.h"
#include "PathFinder/pathFinding/PathFinder.h"


#include "StoragePlugin/Storage/ImageData/ImageWriter/ImageWriter.h"
#include "Async/Async.h"

// ---- debugger ----

NNPathFinderProxy::NNPathFinderProxy(){
    
}

void NNPathFinderProxy::CollectPolygon(
    FVector playerPos,
    float radius,
    FMeshedPolygon &outData
){
    if(APathFinder *instance = APathFinder::instance()){
        float StartTime = FPlatformTime::Seconds();
        if(instance->TryGetSubGraphPolygonMesh(
            outData,
            playerPos,
            radius
        )){
            float EndTime = FPlatformTime::Seconds();
            float Duration = EndTime - StartTime;
            DebugHelper::logMessage(
                FString::Printf(TEXT("NNPathFinderProxy::CreatePolygon Try Success %.2f"), Duration)
            );

            //make distance field data
            outData.SetName(
                FString::Printf(TEXT("extracted_%d_%d"), (int) playerPos.X, (int) playerPos.Y)
            );
            
            cachedImages.Add(outData);
        }
    }
} 






void NNPathFinderProxy::EndSave(){
    if(saveOnEnd){
        for(int i = 0; i < cachedImages.Num(); i++){
            FMeshedPolygon &current = cachedImages[i];
            FString name = current.GetName();
            DebugSaveMeshedPolygonToStorage(current, name);

            //DebugHelper::logMessage(current.ToString());
        }
        DebugHelper::logMessage("NNPathFinderProxy::Saved ", cachedImages.Num());
    }else{
        DebugHelper::logMessage("NNPathFinderProxy::Save on End Disabled");
    }
}




void NNPathFinderProxy::DebugSaveMeshedPolygonToStorage(
    FMeshedPolygon &polygon,
    FString nameImage
){
    if(polygon.IsValid()){
        TArray<FColor> colors;
        FColor locked = FColor::Green;
        FColor free = FColor::Red;

        int x, y;
        polygon.GenerateColorBitmap(colors, free, locked, x, y);

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





