#include "WorldMarkerCanvas.h"
#include "customUiPlugin/ui/TickUpdate/UiActor.h"

void UWorldMarkerCanvas::PostInitProperties(){
    Super::PostInitProperties();
    FindPlayerControllerAndWorld();
}

void UWorldMarkerCanvas::FindPlayerControllerAndWorld(){
    if(!worldPtr){
        worldPtr = AUiActor::GetWorldFromInstance();
    }
    if(worldPtr && !Playercontroller){
        Playercontroller = worldPtr->GetFirstPlayerController();
    }
}

bool UWorldMarkerCanvas::PlayerControllerValid(){
    return Playercontroller != nullptr;
}

bool UWorldMarkerCanvas::dispatchClick(){
    return false;
}


void UWorldMarkerCanvas::Tick(float deltatime){
    FindPlayerControllerAndWorld();
    if(PlayerControllerValid()){
        for (int i = 0; i < trackedMarkers.Num(); i++){
            UWorldMarker *current = trackedMarkers[i];
            if(current != nullptr){
                current->Tick(deltatime);
                Update(current); //update screen pos.
            }
        }
    }
}



void UWorldMarkerCanvas::Update(UWorldMarker *marker){
    if(marker){
        //if forced to be invisible, do not update
        if(marker->IsSetEnabled()){
            return;
        }

        if(marker->IsInLookDirAndRangeOfPlayerInfo()){ //hides marker if false!
            
            UWidget *widget = marker->widgetPointer();
            if(widget){
                FVector2D &screenPosition = marker->screenPositionRef();
                bool bWasProjected = inScreenSpace(
                    marker->worldPositionRef(),
                    screenPosition
                );
                //todo here: if not projected, set invisible! / otherwise visible, also based on range to player!
                if(bWasProjected){
                    SetScreenPosition(widget, screenPosition);
                }
            }
        }
    }
}

bool UWorldMarkerCanvas::inScreenSpace(
    const FVector &WorldLocation,
    FVector2D &ScreenPosition
){
    if(Playercontroller){
        bool bProjected = Playercontroller->ProjectWorldLocationToScreen(
            WorldLocation,
            ScreenPosition,
            false // true = berücksichtigt DPI-Scaling
        );

        if (bProjected){
            UE_LOG(LogTemp, Log, TEXT("Screen Position: X=%f, Y=%f"), ScreenPosition.X, ScreenPosition.Y);
        }else{
            UE_LOG(LogTemp, Warning, TEXT("World location could not be projected to screen!"));
        }
        return bProjected;
    }
    return false;
}




void UWorldMarkerCanvas::RemoveMarker(IBaseUiInterface *item){
    if(item){
        UWorldMarker *marker = findMarker(item);
        if(marker != nullptr){
            RemoveMarker(marker);
        }
    }
}


UWorldMarker *UWorldMarkerCanvas::findMarker(IBaseUiInterface *item){
    if(item){
        UWidget *internalWidget = item->baseLayoutPointer();
        if(internalWidget){
            for (int i = 0; i < trackedMarkers.Num(); i++)
            {
                if(UWorldMarker *current = trackedMarkers[i]){
                    if(current->IsSame(internalWidget)){
                        return current;
                    }
                }
            }
        }
    }
    return nullptr;
}

/// ---- add section ----
void UWorldMarkerCanvas::AddMarker(UWorldMarker *marker){
    if(CanAddMarker(marker)){
        trackedMarkers.Add(marker);
    }
}

void UWorldMarkerCanvas::RemoveMarker(UWorldMarker *marker){
    if(!marker){
        return;
    }

    int32 Index = -1;
    if (trackedMarkers.Find(marker, Index)){
        //swap with back, pop
        int32 lastIndex = trackedMarkers.Num() - 1;
        trackedMarkers[Index] = trackedMarkers[lastIndex];
        trackedMarkers.Pop();
    }

    RemoveChild(marker->interfacePointer());

}



bool UWorldMarkerCanvas::CanAddMarker(UWorldMarker *marker){
    return marker != nullptr && !trackedMarkers.Contains(marker);
}

bool UWorldMarkerCanvas::CanAddMarker(IBaseUiInterface *item){
    if(item){
        for (int i = 0; i < trackedMarkers.Num(); i++){
            UWorldMarker *current = trackedMarkers[i];
            if(current != nullptr && current->IsSame(item)){
                return false;
            }
        }
        return true;
    }
    return false;
}