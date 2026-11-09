#include "FNNRequestPackage.h"




void FNNRequestPackage::Subscribe(IBaseNNInterface *interface){
    if(interface){
        if(subscribed.Contains(interface) == false){
            subscribed.Add(interface);
        }
    }
}

void FNNRequestPackage::NotifyAll(const TArray<uint8> &predictedPositions){
    if(predictedPositions.Num() > 0){
        for (int i = 0; i < subscribed.Num(); i++){
            if(IBaseNNInterface *interface = subscribed[i]){
                interface->ResponseNNData(predictedPositions);
            }
        }
    }
}


TArray<IBaseNNInterface *> &FNNRequestPackage::GetSubscribedActors(){
    return subscribed;
}


void FNNRequestPackage::Setup(FString name, UObject *newTargetObject){
    taskName = name;
    if(newTargetObject){
        targetObject = newTargetObject;
    }
}
bool FNNRequestPackage::IsSame(FString name){
    return name.Equals(taskName);
}

