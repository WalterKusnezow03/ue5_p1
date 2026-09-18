#include "FlagActor.h"


AFlagActor *AFlagActor::MakeInstance(){
    return TMakeInstance<AFlagActor>(EntityAsset::Flag);
}

AFlagActor *AFlagActor::MakeInstanceAt(const FVector &pos){
    if(AFlagActor *actor = MakeInstance()){
        actor->SetActorLocation(pos);
        return actor;
    }
    return nullptr;
}

void AFlagActor::BeginPlay(){
    Super::BeginPlay();



}

