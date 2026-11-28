#pragma once

#include "CoreMinimal.h"


class NNCOMMUNICATIONPLUGIN_API SocketInstanceMaker {

public:
    template<typename T>
    static T *MakeInstance(UWorld *World, T *&instancePtr)
    {
        if(instancePtr){
            return *instancePtr;
        }
        if (World){
            FActorSpawnParameters SpawnParams;
            SpawnParams.Name = TEXT("ANNPathFinderSocketActor");
            FVector Location = FVector::ZeroVector;
            FRotator Rotation = FRotator::ZeroRotator;
            T* Spawned = World->SpawnActor<T>(
                T::StaticClass(), Location, Rotation, SpawnParams
            );   
            instancePtr = Spawned;
        }
    }


};