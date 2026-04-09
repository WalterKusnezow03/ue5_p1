#pragma once 

#include "CoreMinimal.h"

#include "Sockets.h"
#include "SocketSubsystem.h"
#include "Networking.h"


#include "PythonSocketBase.generated.h"


UCLASS()
class NNCOMMUNICATIONPLUGIN_API APythonSocketBase : public AActor{
    GENERATED_BODY()

public:
    //must be called on begin play in derived class
    void LaunchPythonProcess(FString name);


protected:
    void OpenSocket(float deltatime);
    void SetupSocketIfNeeded();
    void CloseSocketOnEndPlay();
    void ConnectIfNeeded(float deltatime);
    bool IsConnected();

    float connectIntervall = 1.0f;
    float integratedDT = 0.0f;

    //debug
    int32 typeDataRandomNum = 0;

    //process related
    FProcHandle ProcHandle;
    void* ReadPipe = nullptr;
    void LogPythonMessages();

    FSocket *Socket = nullptr;

    bool serverRunning = false;
    bool connected = false;



    virtual void Tick(float deltatime) override;
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    
    void LogMessage(FString msg);

    void Send(int32 typeData, TArray<float> &data);
    void Send(TArray<uint8> &bin);
    void SendShutdown();

    void Receive(TArray<float> &data, int32 numFloats);
    void Receive(TArray<uint8> &data, int32 numBytes, int32 &bytesread);

};