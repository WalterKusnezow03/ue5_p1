#include "UiSimulation.h"
#include "p2/ui/3Dui/HUD/HudUiActor.h"
#include "GameCore/Ui3D/WidgetComponentModified/Actor/CustomMeshUIActor.h"

UiSimulation::UiSimulation(){

}

UiSimulation::~UiSimulation(){

}

void UiSimulation::Notify(EUiEvent event, FString message){
    if(event == EUiEvent::HudTopText){
        if(AHudUiActor *actor = AHudUiActor::GetInstance()){
            //actor->updateTopWaringElement(message);
        }
    }
}

//deprecated, has 3D Widget iNtercation Component
/*
void UiSimulation::RayCast3DUi(
    UWorld *world, 
    const FVector &pos, 
    const FVector &dir
){
    if(!world){
        return;
    }
    FVector start = pos + dir.GetSafeNormal() * 50.0f; //50cm
    FVector end = pos + dir.GetSafeNormal() * 5000.0f; //50 * 100 = 50m

    // Perform the raycast
    FHitResult HitResult;
    FCollisionQueryParams Params;
    //Params.AddIgnoredActor(this); // Ignore the character itself
    Params.bTraceComplex = false; //new lower complexity

    

    bool bHit = world->LineTraceSingleByChannel(HitResult, start, end, ECC_Visibility, Params);


    // If the raycast hit something, save hitresult and return positive
    if (bHit)
    {
        if(AActor *actor = HitResult.GetActor()){
            if(ACustomMeshUIActor *casted = Cast<ACustomMeshUIActor>(actor)){
                casted->RayIntersect(start, dir);
            }
        }
    }
}*/

        