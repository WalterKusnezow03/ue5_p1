#include "FPathFinderNNRequestPackage.h"


FPathFinderNNRequestPackage::FPathFinderNNRequestPackage(){

}

FPathFinderNNRequestPackage::~FPathFinderNNRequestPackage(){

}


FPathFinderNNRequestPackage::FPathFinderNNRequestPackage(const FPathFinderNNRequestPackage &other){
    if(this != &other){
        *this = other;
    }
}

FPathFinderNNRequestPackage &FPathFinderNNRequestPackage::operator=(const FPathFinderNNRequestPackage &other){
    if(this != &other){
        setupActor = other.setupActor;
        subscribed = other.subscribed;
    }
    return *this;
}

FPathFinderNNRequestPackage::FPathFinderNNRequestPackage(AActor *actorRequested){
    Setup(actorRequested);
}

void FPathFinderNNRequestPackage::Setup(AActor *actorRequested){
    if(actorRequested){
        setupActor = actorRequested;
    }
}

bool FPathFinderNNRequestPackage::IsSame(AActor *actorRequested){
    if(actorRequested){
        return setupActor == actorRequested;
    }
    return false;
}

void FPathFinderNNRequestPackage::Subscribe(IPathfinderNNInterface *interface){
    if(interface && setupActor){
        if(subscribed.Contains(interface) == false){
            subscribed.Add(interface);
        }
    }
}

void FPathFinderNNRequestPackage::NotifyAll(const TArray<FVector> &predictedPositions){
    if(predictedPositions.Num() > 0){
        for (int i = 0; i < subscribed.Num(); i++){
            if(IPathfinderNNInterface *interface = subscribed[i]){
                interface->ResponseNNPositions(predictedPositions);
            }
        }
    }
}


AActor *FPathFinderNNRequestPackage::GetActor(){
    return setupActor;
}





void FPathFinderNNRequestPackage::GetRequesterVisionCones(TArray<FVisionCone*> &outPositions){
    if(subscribed.Num() > 0){
        int prevSize = outPositions.Num();
        outPositions.SetNum(prevSize + subscribed.Num());

        for (int i = 0; i < subscribed.Num(); i++)
        {
            if(IPathfinderNNInterface *interface = subscribed[i]){
                outPositions[i + prevSize] = &interface->GetVisionCone(); // GetWorldLocation();
            }
        }
    }
}

