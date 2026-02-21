#include "FAnimationAttachedActor.h"

FAnimationAttachedActor::FAnimationAttachedActor(){

}

FAnimationAttachedActor::~FAnimationAttachedActor(){

}

FAnimationAttachedActor::FAnimationAttachedActor(const FAnimationAttachedActor &other){
    if(this != &other){
        *this = other;
    }
}
FAnimationAttachedActor &FAnimationAttachedActor::operator=(const FAnimationAttachedActor &other){
    if(this != &other){
        ejectOnAnimationFinishFlag = other.ejectOnAnimationFinishFlag;
        actorReference = other.actorReference;
    }
    return *this;
}

void FAnimationAttachedActor::SetActorReference(AActor *actor, bool ejectOnAnimationFinish){
    ejectOnAnimationFinishFlag = ejectOnAnimationFinish;
    actorReference = actor;
}

void FAnimationAttachedActor::ResetActorReference(){
    actorReference = nullptr;
}

void FAnimationAttachedActor::UpdateActorLocation(FVector &location){
    if(actorReference){
        actorReference->SetActorLocation(location);
    }
}


//might aswell come with camera (?)
void FAnimationAttachedActor::Eject(FVector &location, FVector &direction, float velocity, float mass){
    if(!ejectOnAnimationFinishFlag){
        return;
    }
    if(actorReference){
        UpdateActorLocation(location);
        if (UPrimitiveComponent *root = ActorRoot())
        {
            if(root->IsSimulatingPhysics()){
                //p = m * v
                FVector scaledImpulse = direction * velocity * mass;
                root->AddImpulse(scaledImpulse);
            }
        }
    }
    ResetActorReference();
}

UPrimitiveComponent *FAnimationAttachedActor::ActorRoot(){
    if(actorReference){
        if(UPrimitiveComponent* root = Cast<UPrimitiveComponent>(actorReference->GetRootComponent())){
            return root;
        }
    }
    return nullptr;
}
