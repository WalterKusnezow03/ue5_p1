#pragma once 

#include "CoreMinimal.h"

#include "Sockets.h"
#include "SocketSubsystem.h"
#include "Networking.h"

#include "NNCommunicationPlugin/Communication/Connection/base/PythonSocketBase.h"

#include "NNSocket.generated.h"


UCLASS()
class NNCOMMUNICATIONPLUGIN_API ANNSocket : public APythonSocketBase {
    GENERATED_BODY()


private:
    static constexpr bool bDEBUGBLOCK_LAUNCH = true;

public:
    static void MakeInstance(UWorld *world);

    static ANNSocket *instance(){
        return instancePtr;
    }

private:
    
    static ANNSocket *instancePtr;

public:

    ANNSocket();
    virtual void Tick(float deltatime) override;
    virtual void BeginPlay() override;
    

};