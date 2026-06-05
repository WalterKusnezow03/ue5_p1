#pragma once

#include "CoreMinimal.h"

class NNCOMMUNICATIONPLUGIN_API MessageDispatcher {

public:
    MessageDispatcher();
    ~MessageDispatcher();

    void Subscribe(INNPathFinderSocketMessageReceiver *other);

    void Dispatch(FString message);

private:
    TArray<INNPathFinderSocketMessageReceiver *> receiverSet;
};