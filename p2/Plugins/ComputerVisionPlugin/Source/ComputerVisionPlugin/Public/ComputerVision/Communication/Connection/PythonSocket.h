#pragma once 

#include "CoreMinimal.h"

#include "Sockets.h"
#include "SocketSubsystem.h"
#include "Networking.h"

#include "PythonSocket.generated.h"

class ImagePatch;

UCLASS()
class COMPUTERVISIONPLUGIN_API APythonSocket : public AActor{
    GENERATED_BODY()


private:
    static constexpr bool bDEBUGBLOCK_LAUNCH = false;

public:
    static void MakeInstance(UWorld *world);

    static APythonSocket *instance(){
        return instancePtr;
    }

    void ExposeSiftToPython(TArray<ImagePatch> &images);

private:
    void OpenSocket(float deltatime);
    void SetupSocketIfNeeded();
    void CloseSocketOnEndPlay();
    void LaunchPythonProcess();

    float connectIntervall = 1.0f;
    float integratedDT = 0.0f;

    //debug
    int32 typeDataRandomNum = 0;

    //process related
    FProcHandle ProcHandle;
    void* ReadPipe = nullptr;
    void LogPythonMessages();

    FSocket *Socket = nullptr;
    static APythonSocket *instancePtr;

    bool serverRunning = false;
    bool connected = false;

public:

    APythonSocket();
    virtual void Tick(float deltatime) override;
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
    void LogMessage(FString msg);

    void SendDataToPython();

    void Send(int32 typeData, TArray<float> &data);
    void Send(TArray<uint8> &bin);
    void SendShutdown();

    void Receive(TArray<float> &data, int32 numFloats);
    void Receive(TArray<uint8> &data, int32 numBytes, int32 &bytesread);
};