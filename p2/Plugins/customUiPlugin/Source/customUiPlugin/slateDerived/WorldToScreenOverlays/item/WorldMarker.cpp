#include "WorldMarker.h"
#include "customUiPlugin/baseInterface/WidgetHelper.h"
#include "GameCore/PlayerInfo/PlayerInfo.h"


void UWorldMarker::Tick(float deltatime){
    if(owningInterface != nullptr){
        owningInterface->Tick(deltatime);
    }
}



void UWorldMarker::PostInitProperties(){
    Super::PostInitProperties();
    SetMaxDistance(100);
}

void UWorldMarker::SetMaxDistance(int distance){
    maxDistanceSquared = (distance * distance);
}

void UWorldMarker::TrySetWidget(IBaseUiInterface *ptr){
    if(ptr){
        if(UWidget *widgetBase = ptr->baseLayoutPointer()){
            widget = widgetBase;
            owningInterface = ptr;
        }
    }
}

bool UWorldMarker::IsSetEnabled(){
    return isEnabled;
}

void UWorldMarker::UpdatePosition(FVector &worldPosIn){
    worldPosition = worldPosIn;
}

const FVector &UWorldMarker::worldPositionRef(){
    return worldPosition;
}

FVector2D &UWorldMarker::screenPositionRef(){
    return screenPosition;
}

void UWorldMarker::SetVisible(bool flag){
    WidgetHelper::SetVisible(widget, flag);

    //enabled flag hier überschreiben oder nicht! (?unklar.)

}

bool UWorldMarker::IsSame(UWidget *ptr){
    if(ptr != nullptr && widget != nullptr){
        return ptr == widget;
    }
    return false;
}

bool UWorldMarker::IsSame(IBaseUiInterface *ptr){
    if(ptr){
        return ptr == owningInterface;
    }
    return false;
}

UWidget *UWorldMarker::widgetPointer(){
    return widget;
}

IBaseUiInterface *UWorldMarker::interfacePointer(){
    return owningInterface;
}

bool UWorldMarker::IsInLookDirAndRangeOfPlayerInfo(){
    FVector location = PlayerInfo::playerLocation();
	FVector look = PlayerInfo::playerLookDir();
    return IsInLookDirAndRange(location, look);
}

bool UWorldMarker::IsInLookDirAndRange(FVector &playerLocation, FVector &look){
    return IsInLookDir(playerLocation, look) && IsInRange(look); //quits at first statement if false.
}

bool UWorldMarker::IsInLookDir(FVector &playerLocation, FVector &look){
    FVector connect = worldPosition - playerLocation; // AB = B - A
    float dot = FVector::DotProduct(connect, look);
    bool result =  dot > 0.0f; //wenn das skalarprodukt zweier vektoren 0 ergibt sind sie 
    //orthogonal zu einander, normalisieren nicht notwendig!

    SetVisible(result);
    return result;
}

bool UWorldMarker::IsInRange(FVector &pos){
    bool result = FVector::DistSquared(pos, worldPosition) < maxDistanceSquared;
    SetVisible(result);
    return result;
}