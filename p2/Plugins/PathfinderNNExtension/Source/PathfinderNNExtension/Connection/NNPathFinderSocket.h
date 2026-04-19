#pragma once 

#include "CoreMinimal.h"
#include "NNCommunicationPlugin/Communication/Connection/NNSocket.h"
#include "CoreMath/animation/timer/Timer.h"

#include "PathfinderNNExtension/PolygonCollection/NNPathFinderProxy.h"
#include "PolygonPlugin/Public/Polygons/MeshedPolygon.h"

#include "NNPathFinderSocket.generated.h"

/// @brief Python socket with Shared Memory Support for Python Script!
/// ---- NN SOCKET BASE WITH SHARED MEMORY SUPPORT----
UCLASS()
class NNCOMMUNICATIONPLUGIN_API ANNPathFinderSocket : public ANNSocket {
    GENERATED_BODY()


public:
    static void MakePathFinderSocketInstance(UWorld *world);

    static ANNPathFinderSocket *PathFinderNNinstance(){
        if(ANNPathFinderSocket *casted = Cast<ANNPathFinderSocket>(instancePtr)){
            return casted;
        }
        return nullptr;
    }

    void PredictNode(
        FVector playerPos,
        float radius
    );

protected:
    using ANNSocket::WriteData;
    void WriteData(FMeshedPolygon &polygon);

    //using ANNSocket::WriteData;
    //void WriteData(const TArray<uint8> &array);

    virtual void TickSocketConnected(float deltatime) override;
    virtual void BeginPlay() override;
    
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    NNPathFinderProxy proxy;

    
};