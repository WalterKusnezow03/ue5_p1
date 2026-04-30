#pragma once 

#include "CoreMinimal.h"

#include "Sockets.h"
#include "SocketSubsystem.h"
#include "Networking.h"

#include "NNCommunicationPlugin/Communication/Connection/base/PythonSocketBase.h"

#include "SharedMemoryPlugin/Public/SharedMemory/FSharedFrame.h"
#include "NNCommunicationPlugin/Communication/SharedFrames/FCommunicationSharedFrameManager.h"

#include "NNSocket.generated.h"

/// @brief Python socket with Shared Memory Support for Python Script!
/// ---- NN SOCKET BASE WITH SHARED MEMORY SUPPORT----
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

protected:
    virtual void SetFlagsOnBeginPlay() override;

    static ANNSocket *instancePtr;

    FCommunicationSharedFrameManager frameManager;

public:
    ANNSocket();

    void WriteData(FString name, const TArray<uint8> &data);

protected:
    virtual void TickSocketConnected(float deltatime) override;
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    void CloseSharedMemory();
};