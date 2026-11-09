#pragma once

#include "FNNRequestPackage.h"


class NNCOMMUNICATIONPLUGIN_API FNNRequestQueue {

protected:
    TArray<FNNRequestPackage> tasks;

    FNNRequestPackage *Find(FString name);

public:

    void PopFront();

    //notifies the front task and pop fronts it since its finished.
    void NotifyPopFront(const TArray<uint8> &predictedData);

    bool HasTasks();

    FNNRequestPackage *frontPackage();

    void Subscribe(
        IBaseNNInterface *interface,
        FString name,
        UObject *targetObject = nullptr
    ){
        if(interface){
            if(FNNRequestPackage *found = Find(name)){
                found->Setup(name, targetObject);
                found->Subscribe(interface);
            }else{
                FNNRequestPackage current;
                current.Setup(name, targetObject);

                current.Subscribe(interface);
                tasks.Add(current);
            }
            //DebugHelper::logMessage("FNNRequestQueue::AddInterface");
        }
    }


};