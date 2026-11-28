#pragma once

#include "CoreMinimal.h"

class NNCOMMUNICATIONPLUGIN_API MessageDispatcher {

public:
    MessageDispatcher();
    ~MessageDispatcher();

    void Subscribe(INNSocketMessageReceiver *other);

    void Dispatch(FString message);

private:
    TArray<INNSocketMessageReceiver *> receiverSet;
};