#include "PythonSocketBase.h"
#include "DebugPlugin/DebugHelper.h"


void APythonSocketBase::BeginPlay(){
    Super::BeginPlay();
    connected = false;
}

//must be called on begin play in derived class
void APythonSocketBase::LaunchPythonProcess(FString pyName){
    if(serverRunning){
        return;
    }

    //launch process by plugin name and py name 
    FPythonSetup package;
    package.Setup(
        "NNCommunicationPlugin",
        pyName
    );
    LaunchPythonProcess(package);
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
    
    
    CloseSocketOnEndPlay();
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
        //send data here
        TickSocketConnected(deltatime);
    }
    DebugSocketMessage();
    // LogPythonMessages(); //happens in super
}

//if (serverRunning && connected)
void APythonSocketBase::TickSocketConnected(float deltatime){
    //is connected: Tick
}

void APythonSocketBase::ConnectIfNeeded(float deltatime){
    if(serverRunning && !connected){
        OpenSocket(deltatime);
    }
}


void APythonSocketBase::OpenSocket(float deltatime){
    SetupSocketIfNeeded();
    if(connected){
        return;
    }

    integratedDT += deltatime;
    if (integratedDT <= connectIntervall)
    {
        return;
    }
    integratedDT = 0.0f;


    

    ISocketSubsystem* SocketSubsystem = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);
    TSharedRef<FInternetAddr> Address = SocketSubsystem->CreateInternetAddr();

    bool bIsValid;
    Address->SetIp(TEXT("127.0.0.1"), bIsValid);
    Address->SetPort(5050);

    if (!Socket->Connect(*Address))
    {
        DebugHelper::logMessage("APythonSocketBase::Socket connect failed");
        connected = false;
        return;
    }else{
        DebugHelper::logMessage("APythonSocketBase::Socket Connected!");
        connected = true;
    }
}

bool APythonSocketBase::IsConnected(){
    return serverRunning && connected;
}



void APythonSocketBase::Send(FString message){
    if(message.Len() <= 0){
        return;
    }
    FTCHARToUTF8 Converter(*message);
    TArray<uint8> Bytes;
    Bytes.Append((uint8*)Converter.Get(), Converter.Length());

    Send(Bytes);
}


void APythonSocketBase::Send(TArray<uint8> &bin){
    
    if (bin.Num() <= 0){
        DebugHelper::logMessage("APythonSocketBase::SendBytes FAILED, no bytes");
        return;
    }
    if (!Socket){
        DebugHelper::logMessage("APythonSocketBase::SendBytes FAILED, no socket");
        return;
    }




    DebugHelper::logMessage("APythonSocketBase::SendBytes ", bin.Num());

    int32 numBytes = bin.Num();

    TArray<uint8> packet;
    packet.Reserve(sizeof(int32) + numBytes);

    // header
    packet.Append((uint8*)&numBytes, sizeof(int32));

    // data
    packet.Append(bin);

    int32 totalSize = packet.Num();
    int32 sent = 0;

    while (sent < totalSize){
        int32 bytesSent = 0;

        bool success = Socket->Send(
            packet.GetData() + sent,
            totalSize - sent,
            bytesSent
        );

        if (!success || bytesSent <= 0)
        {
            UE_LOG(LogTemp, Warning, TEXT("APythonSocketBase::Socket send failed"));
            break;
        }

        sent += bytesSent;
    }
    
    
    
    
    /*
    if(bin.Num() <= 0){
        return;
    }

    //write header info num bytes
    int32 numBytes = bin.Num();
    TArray<uint8> binModified;
    binModified.SetNum(sizeof(int32));
    //FMemory::Memcpy(void* Dest, const void* Src, SIZE_T Count);
    FMemory::Memcpy(binModified.GetData(), &numBytes, sizeof(int32) * 1);

    //append real data
    binModified.Append(bin);

    if(Socket){

        //alle an einem stück senden
        int32 BytesToSend = sizeof(uint8) * binModified.Num();
        int32 TotalSent = 0;
        uint8 *dataPtr = binModified.GetData();

        int32 BytesSent = 0;
        Socket->Send(dataPtr, BytesToSend, BytesSent);

        DebugHelper::logMessage("APythonSocket::Socket Send Data!");

    }*/
    
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



void APythonSocketBase::DebugSocketMessage(){
    //works as expected
    if(debugMessageEnabled){
        Send(TEXT("APythonSocketBase Debug Python Message!"));
    }
}
