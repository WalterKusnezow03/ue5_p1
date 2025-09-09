#include "WorldMarkerComponent.h"

UWorldMarkerComponent *UWorldMarkerComponent::Make(
    AActor *actorAttachTo, 
    FVector &localOffset
){
    if(actorAttachTo){
        UWorldMarkerComponent *newMarker = NewObject<UWorldMarkerComponent>(actorAttachTo);
        newMarker->RegisterComponent(); // Aktiviert die Component, actorAttachTo is getteable over *GetOwner()
        return newMarker;
    }
    return nullptr;
}



UWorldMarkerComponent::UWorldMarkerComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UWorldMarkerComponent::PostInitProperties(){
    Super::PostInitProperties();
    CreateMarker();
    CreateWidgetForUWorldMarker();
}

void UWorldMarkerComponent::CreateMarker(){
    if(!MyMarker){
        MyMarker = NewObject<UWorldMarker>(this);
    }
}

void UWorldMarkerComponent::RemoveMarkerFromWorldMarkerCanvas(){
    if(MyMarker && IsValid(MyMarker)){
        MyMarker->RemoveSelfFromWorldMarkerCanvas();
    }
}


void UWorldMarkerComponent::BeginDestroy(){
    RemoveMarkerFromWorldMarkerCanvas();
    if(MyMarker && IsValid(MyMarker)){
        MyMarker->MarkAsGarbage(); //marker will be also destroyed. (??? crash here)
    }
    MyMarker = nullptr;

    
    Super::BeginDestroy();
}

void UWorldMarkerComponent::CreateWidgetForUWorldMarker(){
    if(MyMarker){
        UTextBlock *someTextfallback = NewObject<UTextBlock>(this);
        MyMarker->SetWidget(someTextfallback);
        someTextfallback->SetText(FText::FromString("FallbackTextBaseCreateWidget"));
    }
}

UWorldMarker *UWorldMarkerComponent::GetMarkerInternal(){
    return MyMarker;
}

void UWorldMarkerComponent::SetRelativeLocation(FVector &pos){
    relativeLocation = pos;
}

void UWorldMarkerComponent::TickComponent(
    float DeltaTime,
    ELevelTick TickType,
    FActorComponentTickFunction *ThisTickFunction
){
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    AActor *parent = GetOwner();
    if (MyMarker && parent)
    {
        FVector worldLocation = parent->GetActorLocation() + relativeLocation;
        MyMarker->UpdateWorldPosition(worldLocation);

        
    }
}

void UWorldMarkerComponent::SetEnabled(bool flag){
    if(MyMarker){
        MyMarker->SetEnabled(flag);
    }
}