#include "PythonSocket.h"

APythonSocket *APythonSocket::instancePtr = nullptr;

APythonSocket::APythonSocket() : Super(){
    PrimaryActorTick.bCanEverTick = true;
}

void APythonSocket::MakeInstance(UWorld* World)
{
    if(bDEBUGBLOCK_LAUNCH){
        return;
    }
    if(instancePtr){
        return;
    }
    if (World){
        FActorSpawnParameters SpawnParams;
        SpawnParams.Name = TEXT("APythonSocketActor");
        FVector Location = FVector::ZeroVector;
        FRotator Rotation = FRotator::ZeroRotator;
        APythonSocket* Spawned = World->SpawnActor<APythonSocket>(
            APythonSocket::StaticClass(), Location, Rotation, SpawnParams
        );   
        instancePtr = Spawned;
    }
}

void APythonSocket::LogMessage(FString msg){
    UE_LOG(LogTemp, Log, TEXT("%s"), *msg);
}





void APythonSocket::BeginPlay(){
    Super::BeginPlay();

    serverRunning = false;
    connected = false;
    LaunchPythonProcess();
    //OpenSocket();


    //send once
    //SendDataToPython();
}

void APythonSocket::LaunchPythonProcess(){
    if(serverRunning){
        return;
    }


    ///Users/walterk/Documents/UnrealProjects/p2/Plugins/ComputerVisionPlugin/Source/ComputerVisionPlugin/Python/nn_server.py
    //Plugins/ComputerVisionPlugin/Source/Python/nn_server.py
    // Absoluter Pfad zum Plugin
    FString PluginDir = FPaths::ConvertRelativePathToFull(
        FPaths::ProjectPluginsDir() / 
        TEXT("ComputerVisionPlugin/Source")
    );
    FString PythonExe = TEXT("/opt/homebrew/bin/python3");
    FString PythonScript = FPaths::Combine(PluginDir, TEXT("Python/nn_server.py"));

    //void* ReadPipe = nullptr;
    void* WritePipe = nullptr;
    FPlatformProcess::CreatePipe(ReadPipe, WritePipe); //create pipe for output python print to unreal log.
    FString Args = FString::Printf(TEXT("\"%s\""), *PythonScript);

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
        *PluginDir, // <- Wichtig! Arbeitsverzeichnis
        WritePipe,
        ReadPipe
    );

    if (ProcHandle.IsValid()){
        UE_LOG(LogTemp, Log, TEXT("APythonSocket::Started Python server sucess!: %s"), *PythonScript);
        serverRunning = true;
    }
    else{
        UE_LOG(LogTemp, Error, TEXT("APythonSocket::Failed to start Python server"));
    }
        
}



void APythonSocket::OpenSocket(float deltatime){
    integratedDT += deltatime;
    if (integratedDT <= connectIntervall)
    {
        return;
    }
    integratedDT = 0.0f;


    SetupSocketIfNeeded();

    ISocketSubsystem* SocketSubsystem = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);
    TSharedRef<FInternetAddr> Address = SocketSubsystem->CreateInternetAddr();

    bool bIsValid;
    Address->SetIp(TEXT("127.0.0.1"), bIsValid);
    Address->SetPort(5050);

    if (!Socket->Connect(*Address))
    {
        LogMessage("APythonSocket::Socket connect failed");
        connected = false;
        return;
    }else{
        LogMessage("APythonSocket::Socket Connected!");
        connected = true;
    }
}

void APythonSocket::SetupSocketIfNeeded(){
    if(Socket){
        return;
    }

    Socket = FTcpSocketBuilder(TEXT("PythonClient"))
        .AsBlocking()
        .AsReusable()
        .Build();
}




void APythonSocket::EndPlay(const EEndPlayReason::Type EndPlayReason){
    instancePtr = nullptr;
    SendShutdown();
    CloseSocketOnEndPlay();

    //close python process
    if (ProcHandle.IsValid())
    {
        FPlatformProcess::TerminateProc(ProcHandle, true); // true = force kill
        FPlatformProcess::CloseProc(ProcHandle);           // Handle freigeben
    }


    Super::EndPlay(EndPlayReason);
}

void APythonSocket::CloseSocketOnEndPlay(){
    if(Socket){
        Socket->Close();
        ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(Socket);
    }
    Socket = nullptr;
}






void APythonSocket::Tick(float deltatime){
    Super::Tick(deltatime);
    if(serverRunning && !connected){
        OpenSocket(deltatime);
    }

    if(serverRunning && connected){
        //debug some data
        TArray<float> data = {1,3,4,5};
        typeDataRandomNum++;
        typeDataRandomNum %= 3;

        Send(typeDataRandomNum, data);
    }
    LogPythonMessages();
}


void APythonSocket::LogPythonMessages(){
    UE_LOG(LogTemp, Log, TEXT("APythonSocket::try read pipe"));
    if(ReadPipe){
        UE_LOG(LogTemp, Log, TEXT("APythonSocket::try read pipe non nullpte"));
        if(serverRunning){
            UE_LOG(LogTemp, Log, TEXT("APythonSocket::try read pipe running server"));
            FString Output = FPlatformProcess::ReadPipe(ReadPipe);
            if (!Output.IsEmpty())
            {
                UE_LOG(LogTemp, Log, TEXT("APythonSocket::PythonPrint: %s"), *Output);
            }else{
                UE_LOG(LogTemp, Log, TEXT("APythonSocket::No Print!"));
            }
        }
    }
}



void APythonSocket::SendDataToPython()
{
    if(!Socket){
        return;
    }

    /*
    // Beispiel: 4 floats senden
    float Values[4] = { 0.1f, 0.2f, 0.3f, 0.4f };
    int32 BytesToSend = sizeof(Values);
    int32 BytesSent = 0;
    Socket->Send(reinterpret_cast<uint8*>(Values), BytesToSend, BytesSent);

    // Antwort empfangen (z. B. 4 floats)
    TArray<float> buff;
    Receive(buff, 4);

    FString msg = TEXT("");
    for (int i = 0; i < buff.Num(); i++)
    {
        msg += FString::Printf(TEXT("%.2f"), buff[i]);
    }
    UE_LOG(
        LogTemp, 
        Log,
        TEXT("ANNSocket::SendDataToPython Got response: %s"), *msg);*/
}


void APythonSocket::Send(int32 typeData, TArray<float> &data){
    if(typeData == -1){
        //shut down nn
        TArray<uint8> bytes;
        bytes.SetNum(sizeof(int32) * 1);
        //FMemory::Memcpy(void* Dest, const void* Src, SIZE_T Count);
        FMemory::Memcpy(bytes.GetData(), &typeData, sizeof(int32) * 1);
        Send(bytes);
    }else{


        //send any
        TArray<uint8> bytes;
        bytes.SetNum(sizeof(int32) + (sizeof(float) * data.Num()));
        uint8 *ptr = bytes.GetData();

        //FMemory::Memcpy(void* Dest, const void* Src, SIZE_T Count);
        FMemory::Memcpy(ptr, &typeData, sizeof(int32) * 1);
        ptr += sizeof(int32) * 1;
        FMemory::Memcpy(ptr, (uint8*)data.GetData(), sizeof(float) * data.Num());
        Send(bytes);



    }
}

void APythonSocket::SendShutdown(){
    int32 typeData = -1;
    // shut down nn
    TArray<uint8> bytes;
    bytes.SetNum(sizeof(int) * 1);
    // FMemory::Memcpy(void* Dest, const void* Src, SIZE_T Count);
    FMemory::Memcpy(bytes.GetData(), &typeData, sizeof(int32) * 1);
    Send(bytes);
    LogMessage("APythonSocket::Socket Send Shutdown!");
}

void APythonSocket::Send(TArray<uint8> &bin){
    if(bin.Num() <= 0){
        return;
    }

    //write header info num bytes
    int32 numBytes = bin.Num();
    TArray<uint8> binModified;
    binModified.SetNum(sizeof(int32));
    //FMemory::Memcpy(void* Dest, const void* Src, SIZE_T Count);
    FMemory::Memcpy(binModified.GetData(), &numBytes, sizeof(int32) * 1);
    binModified.Append(bin);

    if(Socket){

        //alle an einem stück senden
        int32 BytesToSend = sizeof(uint8) * binModified.Num();
        int32 TotalSent = 0;
        uint8 *dataPtr = binModified.GetData();

        int32 BytesSent = 0;
        Socket->Send(dataPtr, BytesToSend, BytesSent);

        LogMessage("APythonSocket::Socket Send Data!");

        /*

        //unklar ob das so auch richtig ist.
        while (TotalSent < BytesToSend)
        {
            int32 BytesSent = 0;
            Socket->Send(dataPtr, BytesToSend - TotalSent, BytesSent);

            //increase ptr after bytes send increased.
            dataPtr += BytesSent;
            TotalSent += BytesSent;

            LogMessage(FString::Printf(TEXT("ANNSocket::send (%d of %d)"), TotalSent, BytesToSend));
        }*/

        //Socket->Send(uint8*, targetSendValue, &outSendResult <= targetSendValue)
        //Socket->Send(reinterpret_cast<uint8*>(Values), BytesToSend, BytesSent);
    }
    
}





void APythonSocket::Receive(TArray<float> &data, int32 numFloats){
    TArray<uint8> rawdata;
    int32 numBytes = sizeof(float) * numFloats;
    int32 readBytes;
    Receive(rawdata, numBytes, readBytes);
    if(readBytes == numBytes){
        //FMemory::Memcpy(void* Dest, const void* Src, SIZE_T Count);

        int32 size = readBytes / sizeof(float);
        data.SetNum(size);
        float *ptr = data.GetData();

        FMemory::Memcpy(ptr, rawdata.GetData(), readBytes);
    }
}

void APythonSocket::Receive(TArray<uint8> &data, int32 numBytes, int32 &bytesread){
    data.SetNum(numBytes);
    uint8 *ptr = data.GetData();
    if(Socket->Recv( 
        ptr, //uint8* Data,
        numBytes, //int32 BufferSize,
        bytesread //actually read
    )){
        UE_LOG(LogTemp, Log, 
            TEXT("APythonSocket::Receive %d bytes"), bytesread
        );
    }
}