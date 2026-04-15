#pragma once 

#include "CoreMinimal.h"

#include "PythonPlugin/Public/ProcessSetup/FPythonSetup.h"

#include "PythonLauncher.generated.h"


UCLASS()
class PYTHONPLUGIN_API APythonLauncher : public AActor{
    GENERATED_BODY()

public:
    //must be called on begin play in derived class
    void LaunchPythonProcess(const FPythonSetup &package);
    void LogPythonMessages();

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float deltatime) override;

    FString pyNameSavedDebug;

    //process related
    FProcHandle ProcHandle;
    void* ReadPipe = nullptr;
    void* WritePipe = nullptr;

    bool serverRunning = false;

    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

};