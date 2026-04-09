#include "PythonSocketBase.h"
#include "DebugPlugin/DebugHelper.h"


void APythonSocketBase::BeginPlay(){
    Super::BeginPlay();
    serverRunning = false;
    connected = false;
    
}

//must be called on begin play in derived class
void APythonSocketBase::LaunchPythonProcess(FString name){
    if(serverRunning){
        return;
    }

    // Absoluter Pfad zum Plugin
    FString PluginDir = FPaths::ConvertRelativePathToFull(
        FPaths::ProjectPluginsDir() / 
        TEXT("NNCommunicationPlugin/Source/NNCommunicationPlugin")
    );
    FString PythonExe = TEXT("/opt/homebrew/bin/python3");

    FString innerPath = FString::Printf(TEXT("Python/%s"), *name);
    //FString PythonScript = FPaths::Combine(PluginDir, TEXT("Python/example.py"));
    FString PythonScript = FPaths::Combine(PluginDir, innerPath);


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




void APythonSocketBase::SetupSocketIfNeeded(){
    if(Socket){
        return;
    }

    Socket = FTcpSocketBuilder(TEXT("PythonClient"))
        .AsBlocking()
        .AsReusable()
        .Build();
}




void APythonSocketBase::EndPlay(const EEndPlayReason::Type EndPlayReason){
    
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

void APythonSocketBase::CloseSocketOnEndPlay(){
    if(Socket){
        Socket->Close();
        ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(Socket);
    }
    Socket = nullptr;
}






void APythonSocketBase::Tick(float deltatime){
    Super::Tick(deltatime);
    ConnectIfNeeded(deltatime);
    if (serverRunning && connected)
    {
        //debug some data
        TArray<float> data = {8, 4, 3, 1};
        typeDataRandomNum++;
        typeDataRandomNum %= 3;

        Send(typeDataRandomNum, data);
    }
    LogPythonMessages();
}

void APythonSocketBase::ConnectIfNeeded(float deltatime){
    if(serverRunning && !connected){
        OpenSocket(deltatime);
    }
}


void APythonSocketBase::OpenSocket(float deltatime){
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
        DebugHelper::logMessage("ANNSocket::Socket connect failed");
        connected = false;
        return;
    }else{
        DebugHelper::logMessage("ANNSocket::Socket Connected!");
        connected = true;
    }
}

bool APythonSocketBase::IsConnected(){
    return serverRunning && connected;
}

void APythonSocketBase::LogPythonMessages(){
    if(ReadPipe){
        if(serverRunning){
            FString Output = FPlatformProcess::ReadPipe(ReadPipe);
            if (!Output.IsEmpty())
            {
                DebugHelper::logMessage("ANNSocket::PythonPrint: ", Output);
            }
        }
    }
}





void APythonSocketBase::Send(int32 typeData, TArray<float> &data){
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

void APythonSocketBase::SendShutdown(){
    int32 typeData = -1;
    // shut down nn
    TArray<uint8> bytes;
    bytes.SetNum(sizeof(int) * 1);
    // FMemory::Memcpy(void* Dest, const void* Src, SIZE_T Count);
    FMemory::Memcpy(bytes.GetData(), &typeData, sizeof(int32) * 1);
    Send(bytes);

    DebugHelper::logMessage("APythonSocketBase::Socket Send Shutdown!");
    
}

void APythonSocketBase::Send(TArray<uint8> &bin){
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

        DebugHelper::logMessage("APythonSocket::Socket Send Data!");

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





void APythonSocketBase::Receive(TArray<float> &data, int32 numFloats){
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

void APythonSocketBase::Receive(TArray<uint8> &data, int32 numBytes, int32 &bytesread){
    data.SetNum(numBytes);
    uint8 *ptr = data.GetData();
    if(Socket->Recv( 
        ptr, //uint8* Data,
        numBytes, //int32 BufferSize,
        bytesread //actually read
    )){
        UE_LOG(LogTemp, Log, 
            TEXT("APythonSocketBase::Receive %d bytes"), bytesread
        );
    }
}