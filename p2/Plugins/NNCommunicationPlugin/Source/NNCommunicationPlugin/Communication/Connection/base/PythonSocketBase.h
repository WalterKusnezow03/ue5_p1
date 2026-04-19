#pragma once 

#include "CoreMinimal.h"

#include "Sockets.h"
#include "SocketSubsystem.h"
#include "Networking.h"

#include "PythonPlugin/Public/base/PythonLauncher.h"


#include "PythonSocketBase.generated.h"


UCLASS()
class NNCOMMUNICATIONPLUGIN_API APythonSocketBase : public APythonLauncher {
    GENERATED_BODY()

public:
    //must be called on begin play in derived class
    using APythonLauncher::LaunchPythonProcess;
    void LaunchPythonProcess(
        FString pluginName, 
        FString pyName, 
        FString pluginNamePython,
        FString pythonExePath
    );
    void LaunchPythonProcess(FString pluginName, FString pyName);

protected:
    void OpenSocket(float deltatime);
    void SetupSocketIfNeeded();
    void CloseSocketOnEndPlay();
    void ConnectIfNeeded(float deltatime);
    bool IsConnected();

    void SetFlagsOnBeginPlay() override;

    float connectIntervall = 1.0f;
    float integratedDT = 0.0f;

    //debug
    int32 typeDataRandomNum = 0;

    

    FSocket *Socket = nullptr;

    //bool serverRunning = false;
    bool connected = false;



    virtual void Tick(float deltatime) override;
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    //to be overriden: called on tick if socket is connected
    virtual void TickSocketConnected(float deltatime);

    void LogMessage(FString msg);

    //send via socket connection
    void Send(FString message);
    void Send(TArray<uint8> &bin);

    void Receive(TArray<uint8> &data, int32 numBytes, int32 &bytesread);



    // ---- debug messages on tick to python----
    bool debugMessageEnabled = true;
    void DebugSocketMessage(); //works
};