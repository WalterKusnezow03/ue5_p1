#pragma once 

#include "CoreMinimal.h"
#include "NNCommunicationPlugin/Communication/Connection/NNSocket.h"
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

protected:
    //using ANNSocket::WriteData;
    //void WriteData(const TArray<uint8> &array);

    virtual void TickSocketConnected(float deltatime) override;
    virtual void BeginPlay() override;
    
};