#pragma once 

#include "CoreMinimal.h"


class COMPUTERVISIONPLUGIN_API SharedMemory{

public:
    SharedMemory();
    ~SharedMemory();

    void Launch(int bytes);

private:


};