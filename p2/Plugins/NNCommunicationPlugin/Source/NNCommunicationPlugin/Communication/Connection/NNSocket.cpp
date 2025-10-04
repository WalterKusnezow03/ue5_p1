#include "NNSocket.h"

ANNSocket *ANNSocket::instancePtr = nullptr;

ANNSocket::ANNSocket() : Super(){
    PrimaryActorTick.bCanEverTick = true;
}

void ANNSocket::MakeInstance(UWorld* World)
{
    if(instancePtr){
        return;
    }
    if (World){
        FActorSpawnParameters SpawnParams;
        SpawnParams.Name = TEXT("ANNSocketActor");
        FVector Location = FVector::ZeroVector;
        FRotator Rotation = FRotator::ZeroRotator;
        ANNSocket* Spawned = World->SpawnActor<ANNSocket>(ANNSocket::StaticClass(), Location, Rotation, SpawnParams);   
        instancePtr = Spawned;
    }
}

void ANNSocket::LogMessage(FString msg){
    UE_LOG(LogTemp, Log, TEXT("%s"), *msg);
}





void ANNSocket::BeginPlay(){
    Super::BeginPlay();

    serverRunning = false;
    connected = false;
    LaunchPythonProcess();
    //OpenSocket();


    //send once
    //SendDataToPython();
}

void ANNSocket::LaunchPythonProcess(){
    if(serverRunning){
        return;
    }

    // Absoluter Pfad zum Plugin
    FString PluginDir = FPaths::ConvertRelativePathToFull(
        FPaths::ProjectPluginsDir() / 
        TEXT("NNCommunicationPlugin/Source/NNCommunicationPlugin")
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
        UE_LOG(LogTemp, Log, TEXT("ANNSocket::Started Python server sucess!: %s"), *PythonScript);
        serverRunning = true;
    }
    else{
        UE_LOG(LogTemp, Error, TEXT("ANNSocket::Failed to start Python server"));
    }
        
}



void ANNSocket::OpenSocket(float deltatime){
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
        LogMessage("ANNSocket::Socket connect failed");
        connected = false;
        return;
    }else{
        LogMessage("ANNSocket::Socket Connected!");
        connected = true;
    }
}

void ANNSocket::SetupSocketIfNeeded(){
    if(Socket){
        return;
    }

    Socket = FTcpSocketBuilder(TEXT("PythonClient"))
        .AsBlocking()
        .AsReusable()
        .Build();
}




void ANNSocket::EndPlay(const EEndPlayReason::Type EndPlayReason){
    instancePtr = nullptr;
    SendShutdown();
    CloseSocketOnEndPlay();
    Super::EndPlay(EndPlayReason);
}

void ANNSocket::CloseSocketOnEndPlay(){
    if(Socket){
        Socket->Close();
        ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(Socket);
    }
    Socket = nullptr;
}






void ANNSocket::Tick(float deltatime){
    Super::Tick(deltatime);
    if(serverRunning && !connected){
        OpenSocket(deltatime);
    }

    if(serverRunning && connected){
        //debug some data
        TArray<float> data = {8, 4, 3, 1};
        typeDataRandomNum++;
        typeDataRandomNum %= 3;

        Send(typeDataRandomNum, data);
    }
    LogPythonMessages();
}


void ANNSocket::LogPythonMessages(){
    if(ReadPipe){
        if(serverRunning){
            FString Output = FPlatformProcess::ReadPipe(ReadPipe);
            if (!Output.IsEmpty())
            {
                UE_LOG(LogTemp, Log, TEXT("ANNSocket::PythonPrint: %s"), *Output);
            }
        }
    }
}



void ANNSocket::SendDataToPython()
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


void ANNSocket::Send(int32 typeData, TArray<float> &data){
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

void ANNSocket::SendShutdown(){
    int32 typeData = -1;
    // shut down nn
    TArray<uint8> bytes;
    bytes.SetNum(sizeof(int) * 1);
    // FMemory::Memcpy(void* Dest, const void* Src, SIZE_T Count);
    FMemory::Memcpy(bytes.GetData(), &typeData, sizeof(int32) * 1);
    Send(bytes);
    LogMessage("ANNSocket::Socket Send Shutdown!");
}

void ANNSocket::Send(TArray<uint8> &bin){
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

        LogMessage("ANNSocket::Socket Send Data!");

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





void ANNSocket::Receive(TArray<float> &data, int32 numFloats){
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

void ANNSocket::Receive(TArray<uint8> &data, int32 numBytes, int32 &bytesread){
    data.SetNum(numBytes);
    uint8 *ptr = data.GetData();
    if(Socket->Recv( 
        ptr, //uint8* Data,
        numBytes, //int32 BufferSize,
        bytesread //actually read
    )){
        UE_LOG(LogTemp, Log, 
            TEXT("ANNSocket::Receive %d bytes"), bytesread
        );
    }
}