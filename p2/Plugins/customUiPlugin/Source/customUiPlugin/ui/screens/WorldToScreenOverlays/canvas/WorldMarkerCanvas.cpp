#include "WorldMarkerCanvas.h"
#include "customUiPlugin/ui/TickUpdate/UiActor.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "customUiPlugin/ui/screens/WorldToScreenOverlays/actorComponent/WorldMarkerComponent.h"

void UWorldMarkerCanvas::PostInitProperties(){
    Super::PostInitProperties();
    FindPlayerControllerAndWorld();
}

void UWorldMarkerCanvas::FindPlayerControllerAndWorld(){
    if(!worldPtr){
        //wont make any issues if playeruibase is launched.
        //is the first thing that should happen.
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

// --- tick and update ---

void UWorldMarkerCanvas::Tick(float deltatime){
    
    Super::Tick(deltatime);
    FindPlayerControllerAndWorld();
    if(PlayerControllerValid()){
        for (int i = 0; i < trackedMarkers.Num(); i++){
            UWorldMarker *current = trackedMarkers[i];
            if(current != nullptr){
                current->Tick(deltatime);
                Update(current); //update screen pos.
            }
        }

        if(blogMessage)
            UiDebugHelper::showScreenMessage("UWorldMarkerCanvas Tick", FColor::Cyan); //never called.
    }
}



void UWorldMarkerCanvas::Update(UWorldMarker *marker){
    if(marker){
        //if forced to be invisible, do not update
        if(!marker->IsSetEnabled()){
            DebugHelper::showScreenMessage("Marker Not Enabled!", FColor::Red);
            return;
        }

        //UiDebugHelper::showScreenMessage("UWorldMarkerCanvas try update marker", FColor::Yellow);
        if(marker->IsInLookDirAndRangeOfPlayerInfo()){ //hides marker if false!
            
            //UiDebugHelper::showScreenMessage("UWorldMarkerCanvas try update marker pass player check", FColor::Green);

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
        }else{
            DebugHelper::showScreenMessage("Marker Not In Look Dir!!", FColor::Red);
        }
    }
}

bool UWorldMarkerCanvas::inScreenSpace(
    const FVector &WorldLocation,
    FVector2D &ScreenPosition
){

    if(Playercontroller){

        DebugHelper::showLineBetween(
            Playercontroller->GetWorld(),
            WorldLocation,
            WorldLocation + FVector(0,0,1000),
            FColor::Yellow,
            0.1f
        );




        //debug
        FVector CamLoc;
        FRotator CamRot;
        Playercontroller->GetPlayerViewPoint(CamLoc, CamRot);
        UE_LOG(LogTemp, Warning, TEXT("WorldMarkerCanvas Camera Loc: %s %s"), 
            *CamLoc.ToString(), 
            *Playercontroller->GetName()
        );
        FVector copyWorld = WorldLocation;
        copyWorld -= CamLoc;


        /*
        //PlayerController->ProjectWorldLocationToScreen(WorldLocation, ScreenPos, bPlayerViewportRelative);
        bool bProjected = Playercontroller->ProjectWorldLocationToScreen(
            copyWorld,
            //WorldLocation,
            ScreenPosition,
            true // true = berücksichtigt DPI-Scaling
        );*/

        FVector2D WidgetPosition;
        bool bProjected = UWidgetLayoutLibrary::ProjectWorldLocationToWidgetPosition(
            Playercontroller,
            WorldLocation,
            WidgetPosition,
            true // PlayerViewportRelative
        );
        ScreenPosition = WidgetPosition;

        //all at 0,0,0 unknwon why.
        if(blogMessage){
            FString coordinate = TEXT("UWorldMarkerCanvas WorldPos: ") + WorldLocation.ToString();
            UiDebugHelper::logMessage(coordinate);

            coordinate = TEXT("UWorldMarkerCanvas ScreenPos: ") + ScreenPosition.ToString();
            UiDebugHelper::logMessage(coordinate);
        }










        /*
        if (bProjected){
            UE_LOG(LogTemp, Log, TEXT("Screen Position: X=%f, Y=%f"), ScreenPosition.X, ScreenPosition.Y);
        }else{
            UE_LOG(LogTemp, Warning, TEXT("World location could not be projected to screen!"));
        }*/
        return bProjected;
    }
    return false;
}




// --- ADD ---

void UWorldMarkerCanvas::AddMarker(UWorldMarker *marker){
    if(CanAddMarker(marker)){
        trackedMarkers.Add(marker);
        marker->SetMarkerCanvasPointer(this); //set parent, marker can remove itself externally

        //UiDebugHelper::logMessage("UWorldMarkerCanvas added UWorldMarker marker");

        //check and add widget / base ui interface
        if(IBaseUiInterface *interfaceIsPreferred = marker->interfacePointer()){
            AddChild(interfaceIsPreferred);
            UiDebugHelper::logMessage("UWorldMarkerCanvas added UWorldMarker as interface");
        }else{
            if(UWidget *innerWidget = marker->widgetPointer()){
                //AddChild(innerWidget);

                FVector2D pivot(0.5f, 0.5f);
                AddChildWithPivot(innerWidget, pivot);

                UiDebugHelper::logMessage("UWorldMarkerCanvas added UWorldMarker as uwidget"); //prints
            }
        }
    }
}

void UWorldMarkerCanvas::AddMarker(UWorldMarkerComponent *actorComponent){
    if(actorComponent){
        UWorldMarker *marker = actorComponent->GetMarkerInternal();
        if(marker){
            AddMarker(marker);
            //UiDebugHelper::logMessage("UWorldMarkerCanvas added UActor marker");
        }
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

    if(IBaseUiInterface *interface = marker->interfacePointer()){
        RemoveChild(interface); //rmeoves child and widget ptr from interface (both.)
    }else{
        if(UWidget *widget = marker->widgetPointer()){
            RemoveChild(widget); //if inetrface not used, remove uwidget (marker accepts raw uwidgets too.)
        }
    }

}

bool UWorldMarkerCanvas::CanAddMarker(UWorldMarker *marker){
    return marker != nullptr && !trackedMarkers.Contains(marker);
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
