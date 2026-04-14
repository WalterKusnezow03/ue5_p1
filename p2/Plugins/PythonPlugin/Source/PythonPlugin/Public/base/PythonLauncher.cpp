#include "PythonLauncher.h"
#include "DebugPlugin/DebugHelper.h"

void APythonLauncher::BeginPlay(){
    Super::BeginPlay();
    serverRunning = false; 
}

//must be called on begin play in derived class
void APythonLauncher::LaunchPythonProcess(const FPythonSetup &setup){
    if(serverRunning){
        return;
    }
    
    FString PythonExe = TEXT("/opt/homebrew/bin/python3");

    FString PluginDir = setup.PluginDir();
    FPlatformProcess::CreatePipe(ReadPipe, WritePipe); // create pipe for output python print to unreal log.
    FString Args = FString::Printf(TEXT("\"%s\""), *setup.getPath());

    // Python-Prozess starten
    /*FProcHandle ProcHandle = FPlatformProcess::CreateProc(*PythonExe, *Args,
        true,  // bLaunchDetached
        false, // bLaunchHidden
        false, // bLaunchReallyHidden
        nullptr, 0, nullptr, WritePipe);*/
    ProcHandle = FPlatformProcess::CreateProc(
        *PythonExe,
        *Args,
        true,   // bLaunchDetached
        false,  // bLaunchHidden
        false,  // bLaunchReallyHidden
        nullptr,
        0,
        *PluginDir, // <- Wichtig! Arbeitsverzeichnis
        WritePipe,
        ReadPipe
    );

    if (ProcHandle.IsValid()){
        UE_LOG(LogTemp, Log, TEXT("APythonLauncher::Started Python server sucess!: %s"), *setup.getPath());
        serverRunning = true;
    }
    else{
        UE_LOG(LogTemp, Error, TEXT("APythonLauncher::Failed to start Python server"));
    }
        
}







void APythonLauncher::EndPlay(const EEndPlayReason::Type EndPlayReason){
    //close python process
    if (ProcHandle.IsValid()){
        FPlatformProcess::TerminateProc(ProcHandle, true); // true = force kill
        FPlatformProcess::CloseProc(ProcHandle);           // Handle freigeben
        serverRunning = false;
    }
    Super::EndPlay(EndPlayReason);
}




void APythonLauncher::LogPythonMessages(){
    if(ReadPipe){
        if(serverRunning){
            FString Output = FPlatformProcess::ReadPipe(ReadPipe);
            if (!Output.IsEmpty()){
                DebugHelper::logMessage("APythonLauncher::PythonPrint: ", Output);
            }
        }
    }
}
