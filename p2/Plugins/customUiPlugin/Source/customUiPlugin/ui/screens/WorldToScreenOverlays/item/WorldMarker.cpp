#include "WorldMarker.h"
#include "customUiPlugin/baseInterface/WidgetHelper.h"
#include "GameCore/PlayerInfo/PlayerInfo.h"
#include "customUiPlugin/ui/screens/WorldToScreenOverlays/canvas/WorldMarkerCanvas.h"


void UWorldMarker::Tick(float deltatime){
    if(owningInterface != nullptr){
        owningInterface->Tick(deltatime);
    }
}



void UWorldMarker::PostInitProperties(){
    Super::PostInitProperties();
    SetMaxDistance(100);
}

void UWorldMarker::BeginDestroy(){
    RemoveSelfFromWorldMarkerCanvas(); //remove self completly from canvas, then reset pointers.
    
    //it is not clear whether the widget should be killed or not
    //widget owner ship not known here yet!
    
    widget = nullptr;
    owningInterface = nullptr;
    Super::BeginDestroy();
}

void UWorldMarker::RemoveSelfFromWorldMarkerCanvas(){
    if(parent && IsValid(parent)){
        parent->RemoveMarker(this);
        parent = nullptr;
    }
}

void UWorldMarker::SetMarkerCanvasPointer(UWorldMarkerCanvas *canvas){
    parent = canvas;
}




void UWorldMarker::SetMaxDistance(int distance){
    maxDistanceSquared = (distance * distance);
}

void UWorldMarker::SetWidget(IBaseUiInterface *interfacePtr){
    if(interfacePtr){
        if(UWidget *widgetBase = interfacePtr->baseLayoutPointer()){
            SetWidget(widgetBase); //also resets interface ptr
            owningInterface = interfacePtr;
        }
    }
}
void UWorldMarker::SetWidget(UWidget *widgetIn){
    if(widgetIn){
        owningInterface = nullptr;
        widget = widgetIn;
    }
}



void UWorldMarker::SetEnabled(bool flag){
    isEnabled = flag;
    //hide if not enabled.
    if(!flag){
        SetVisible(false);
    }
}

bool UWorldMarker::IsSetEnabled(){
    return isEnabled;
}

void UWorldMarker::UpdateWorldPosition(FVector &worldPosIn){
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
    //dont override enabled flag here at all
    //just for external use
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