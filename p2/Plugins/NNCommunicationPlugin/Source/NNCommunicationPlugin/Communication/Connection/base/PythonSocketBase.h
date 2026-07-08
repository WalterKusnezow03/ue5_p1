#pragma once 

#include "CoreMinimal.h"

#include "Sockets.h"
#include "SocketSubsystem.h"
#include "Networking.h"

#include "PythonPlugin/Public/base/PythonLauncher.h"
#include "NNCommunicationPlugin/Communication/Connection/base/notify/NNPathFinderSocketMessageReceiver.h"
#include "NNCommunicationPlugin/Communication/Connection/base/notify/MessageDispatcher.h"

#include "PythonSocketBase.generated.h"


UCLASS()
class NNCOMMUNICATIONPLUGIN_API APythonSocketBase : public APythonLauncher {
    GENERATED_BODY()

public:
    static int GetPort(){
        return 5050;
    }

    //must be called on begin play in derived class
    using APythonLauncher::LaunchPythonProcess;
    void LaunchPythonProcess(
        FString pluginName, 
        FString pyName, 
        FString pluginNamePython,
        FString pythonExePath
    );
    void LaunchPythonProcess(FString pluginName, FString pyName);

    void SubscribeMessageListener(INNPathFinderSocketMessageReceiver *listener);

protected:
    void OpenSocket(float deltatime);
    void SetupSocketIfNeeded();
    void CloseSocketOnEndPlay();
    void ConnectIfNeeded(float deltatime);
    bool IsConnected();

    void SetFlagsOnBeginPlay() override;

    float connectIntervall = 1.0f;
    float integratedDT = 0.0f;

    MessageDispatcher messageDispatcher;

    FSocket *Socket = nullptr;

    //bool serverRunning = false;
    bool connected = false;



    virtual void Tick(float deltatime) override;
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    //to be overriden: called on tick if socket is connected
    virtual void TickSocketConnected(float deltatime);

    void LogMessage(FString msg);

    
    virtual void OnReceivePythonPrint(FString message) override;

    //send via socket connection
public:
    void Send(FString message);
protected:
    void Send(TArray<uint8> &bin);

    void Receive(TArray<uint8> &data, int32 numBytes, int32 &bytesread);



    // ---- debug messages on tick to python----
    
    void DebugSocketMessage(); //works

private:
    bool debugMessageEnabled = true; //do not change, is needed

};