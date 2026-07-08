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


    //check ob script gefunden
    FString ScriptPath = setup.getPath();
    if (!FPaths::FileExists(ScriptPath))
    {
        UE_LOG(LogTemp, Error, TEXT("APythonLauncher::SKRIPT DATEI EXISTIERT NICHT: %s"), *ScriptPath);
        return;
    }else{
        UE_LOG(LogTemp, Error, TEXT("APythonLauncher::SKRIPT DATEI WURDE GEFUNDEN: %s"), *ScriptPath);
    }


    //new
    //saved ordner des spiels statt PluginDir für Working Dir python,
    //sonst muss python in .app schreiben.
    //FString SafeWorkingDir = PluginDir
    FString SafeWorkingDir = PluginDir; // FPaths::ConvertRelativePathToFull(FPaths::ProjectSavedDir());

    bool useProjectSavedDirInsteadOfPluginDir = false;
    if(useProjectSavedDirInsteadOfPluginDir){
        SafeWorkingDir = FPaths::ConvertRelativePathToFull(FPaths::ProjectSavedDir());
    }

    //optional not nesecarry
    //CopyPythonDataToTempDirectory(setup, SafeWorkingDir, Args);

    // Python-Prozess starten
    /*FProcHandle ProcHandle = FPlatformProcess::CreateProc(*PythonExe, *Args,
        true,  // bLaunchDetached
        false, // bLaunchHidden
        false, // bLaunchReallyHidden
        nullptr, 0, nullptr, WritePipe);*/
    ProcHandle = FPlatformProcess::CreateProc(
        *PythonExe,
        *Args,
        false,//true,   // bLaunchDetached
        false,  // bLaunchHidden
        false,  // bLaunchReallyHidden
        nullptr,
        0,
        *SafeWorkingDir, //*PluginDir, // <- Wichtig! Arbeitsverzeichnis
        WritePipe,
        ReadPipe
    );

    
    if (!ProcHandle.IsValid())
    {
        serverRunning = false;
        UE_LOG(LogTemp, Error, TEXT("APythonLauncher::Python-Prozess konnte nicht gestartet werden!"));
        return; // Verhindert den Absturz
    }

    if (ProcHandle.IsValid()){
        UE_LOG(LogTemp, Log, TEXT("APythonLauncher::Started Python server sucess!: %s"), *setup.getPath());
        serverRunning = true; //mark as python launched now.
        for (int i = 0; i < 100; i++){
            DebugHelper::showScreenMessage("PYTHON LAUNCH");
        }
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

//escape from app bundle: copy code to public dir
//to launch the script
void APythonLauncher::CopyPythonDataToTempDirectory(
    const FPythonSetup &setup,
    FString &outSafeWorkingDir,
    FString &outArgs
){
    // 1. Pfade definieren
    FString OriginalScriptPath = setup.getPath(); // Z.B. .../Plugins/PathFinderNNExtension/Python/nnpathfinder_server.py
    FString SourcePythonDir = FPaths::GetPath(OriginalScriptPath); // Der gesamte Quellordner im App-Bundle

    FString SafeSavedDir = FPaths::ConvertRelativePathToFull(FPaths::ProjectSavedDir());
    FString TargetPythonDir = FPaths::Combine(SafeSavedDir, TEXT("PythonServer")); // Zielordner im beschreibbaren Bereich
    FString TargetScriptPath = FPaths::Combine(TargetPythonDir, FPaths::GetCleanFilename(OriginalScriptPath));

    /*// 2. Den gesamten Ordner kopieren (falls er im App-Bundle existiert)
    IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
    if (PlatformFile.DirectoryExists(*SourcePythonDir))
    {
        // Kopiert den gesamten Ordner inklusive aller Unterordner und Dateien (.py, etc.)
        if (PlatformFile.CopyDirectoryTree(*TargetPythonDir, *SourcePythonDir, true))
        {
            UE_LOG(LogTemp, Log, TEXT("APythonLauncher:: Ganzer Python-Ordner erfolgreich nach %s kopiert!"), *TargetPythonDir);
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("APythonLauncher:: Fehler beim Kopieren des Python-Ordners!"));
        }
    }*/
   // 2. Den Ordner selektiv kopieren (ohne .pth Dateien)
    IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
    if (PlatformFile.DirectoryExists(*SourcePythonDir))
    {
        // Wir nutzen einen Lambda-Visitor, der jede Datei und jeden Ordner im Quellverzeichnis scannt
        bool bCopySuccess = true;
        
        PlatformFile.IterateDirectoryRecursively(*SourcePythonDir, [&](const TCHAR* SourcePath, bool bIsDirectory) -> bool
        {
            // Erstelle den Zielpfad, indem wir den Quellpfad-Teil durch den Zielordner-Teil ersetzen
            FString RelativePath = FString(SourcePath).RightChop(SourcePythonDir.Len());
            FString DestPath = FPaths::Combine(TargetPythonDir, RelativePath);

            if (bIsDirectory)
            {
                // Wenn es ein Ordner ist, erstellen wir ihn im Zielverzeichnis
                PlatformFile.CreateDirectoryTree(*DestPath);
            }
            else
            {
                // Wenn es eine Datei ist, prüfen wir die Endung
                FString Extension = FPaths::GetExtension(SourcePath).ToLower();
                
                if (Extension == TEXT("pth"))
                {
                    // Überspringe Checkpoint-Dateien!
                    UE_LOG(LogTemp, Log, TEXT("APythonLauncher:: Überspringe Checkpoint-Kopie: %s"), SourcePath);
                }
                else
                {
                    // Jede andere Datei (.py, .txt, etc.) wird kopiert
                    if (!PlatformFile.CopyFile(*DestPath, SourcePath))
                    {
                        bCopySuccess = false;
                    }
                }
            }
            return true; // true bedeutet: Weiterlesen im Verzeichnis
        });

        if (bCopySuccess)
        {
            UE_LOG(LogTemp, Log, TEXT("APythonLauncher:: Python-Ordner (ohne .pth) erfolgreich nach %s kopiert!"), *TargetPythonDir);
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("APythonLauncher:: Fehler beim selektiven Kopieren des Python-Ordners!"));
        }
    }
    outSafeWorkingDir = TargetPythonDir;

    // 3. Den Start-Prozess mit den neuen, sicheren Pfaden füttern!
    outArgs = FString::Printf(TEXT("-u \"%s\""), *TargetScriptPath);
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


