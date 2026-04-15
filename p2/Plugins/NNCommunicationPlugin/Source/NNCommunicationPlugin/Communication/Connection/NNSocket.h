#pragma once 

#include "CoreMinimal.h"

#include "Sockets.h"
#include "SocketSubsystem.h"
#include "Networking.h"

#include "NNCommunicationPlugin/Communication/Connection/base/PythonSocketBase.h"

#include "SharedMemoryPlugin/Public/SharedMemory/FSharedFrame.h"

#include "NNSocket.generated.h"

/// @brief Python socket with Shared Memory Support for Python Script!
/// ---- PATH FINDER SPECIFIC NN ----
UCLASS()
class NNCOMMUNICATIONPLUGIN_API ANNSocket : public APythonSocketBase {
    GENERATED_BODY()


private:
    static constexpr bool bDEBUGBLOCK_LAUNCH = false;

public:
    static void MakeInstance(UWorld *world);

    static ANNSocket *instance(){
        return instancePtr;
    }

private:
    
    static ANNSocket *instancePtr;
    FSharedFrame sharedMemory;
    FString frameName = "ANNSocket_Frame";

    bool frameNameSend = false;

public:

    ANNSocket();

protected:
    virtual void TickSocketConnected(float deltatime) override;
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    void OpenSharedMemory(int bytes);
    void CloseSharedMemory();
};