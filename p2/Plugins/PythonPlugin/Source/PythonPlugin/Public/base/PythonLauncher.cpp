#include "PythonLauncher.h"
#include "DebugPlugin/DebugHelper.h"

void APythonLauncher::BeginPlay(){
    Super::BeginPlay();
    SetFlagsOnBeginPlay();
}

void APythonLauncher::SetFlagsOnBeginPlay(){
    serverRunning = false; 
}

//must be called on begin play in derived class
void APythonLauncher::LaunchPythonProcess(const FPythonSetup &setup){
    if(serverRunning){
        return;
    }
    setupCopy = setup;

    FString PythonExe = setup.PythonExe(); // TEXT("/opt/homebrew/bin/python3");
    //PythonExe = TEXT("/opt/homebrew/bin/python3");
    

    FString PluginDir = setup.PluginDir();
    FPlatformProcess::CreatePipe(ReadPipe, WritePipe); // create pipe for output python print to unreal log.
    
    
    //FString Args = FString::Printf(TEXT("\"%s\""), *setup.getPath());
    FString Args = FString::Printf(TEXT("-u \"%s\""), *setup.getPath()); //-u for print flushing into pipe instant


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
        serverRunning = true; //mark as python launched now.
    }
    else{
        UE_LOG(LogTemp, Error, TEXT("APythonLauncher::Failed to start Python server"));
    }

    //save for debug print
    pyNameSavedDebug = setup.PythonScriptName();
}

void APythonLauncher::Restart(){

    ShutDownPythonForce();
    LaunchPythonProcess(setupCopy);
}




void APythonLauncher::EndPlay(const EEndPlayReason::Type EndPlayReason){
    //close python process
    /*if (ProcHandle.IsValid()){
        FPlatformProcess::TerminateProc(ProcHandle, true); // true = force kill
        FPlatformProcess::CloseProc(ProcHandle);           // Handle freigeben
        serverRunning = false;
    }*/
    ShutDownPython();
    Super::EndPlay(EndPlayReason);
}

void APythonLauncher::ShutDownPython(){

    ShutDownPython(5.0f);
    ShutDownPythonForce();
}

void APythonLauncher::ShutDownPythonForce(){
    if (ProcHandle.IsValid()){
        FPlatformProcess::TerminateProc(ProcHandle, true);
        for (int i = 0; i < 20; i++){
            if (!FPlatformProcess::IsProcRunning(ProcHandle))
                break;

            FPlatformProcess::Sleep(0.1f);
        }
        FPlatformProcess::CloseProc(ProcHandle);
    }
    if (ReadPipe || WritePipe){
        FPlatformProcess::ClosePipe(ReadPipe, WritePipe);
        ReadPipe = nullptr;
        WritePipe = nullptr;
    }
    serverRunning = false;
}


void APythonLauncher::ShutDownPython(float timeOut){
    const double Timeout = std::abs(timeOut);
    const double StartTime = FPlatformTime::Seconds();
    bool bSafeToExit = false;
    while (FPlatformTime::Seconds() - StartTime < Timeout){
        FString Output = FPlatformProcess::ReadPipe(ReadPipe);
        if (Output.Contains(TEXT("PYTHON_SAFE_TO_EXIT"))){
            bSafeToExit = true;
            break;
        }
        FPlatformProcess::Sleep(0.1f);
    }
}

void APythonLauncher::Tick(float deltatime){
    Super::Tick(deltatime);
    LogPythonMessages();
}

void APythonLauncher::LogPythonMessages(){
    //DebugHelper::showScreenMessage("APythonLauncher::LogPythonMessages", FColor::Red);
    if (ReadPipe){
        if(serverRunning){
            //DebugHelper::showScreenMessage("APythonLauncher::LogPythonMessages Try", FColor::Red);
            FString Output = FPlatformProcess::ReadPipe(ReadPipe);
            if (!Output.IsEmpty()){
                OnReceivePythonPrint(Output);
            }
        }
    }
}

void APythonLauncher::OnReceivePythonPrint(FString Output){
    DebugHelper::logMessage(
        FString::Printf(
            TEXT("APythonLauncher::PythonPrint( %s ) %s"),
            *pyNameSavedDebug,
            *Output
        )
    );
}


