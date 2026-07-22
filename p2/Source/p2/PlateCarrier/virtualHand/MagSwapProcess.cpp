#include "MagSwapProcess.h"
#include "terrainPluginBase/BaseTerrainInterface/bezier/extendedVersion/BSpline.h"
#include "p2/weapon/weapon.h"
#include "p2/PlateCarrier/MagSocket/PlateCarrierMagSocket.h"
#include "DebugPlugin/DebugHelper.h"

void MagSwapProcess::PrepareSetHandComponent(USceneComponent *handIn){
    hand = handIn;
}


void MagSwapProcess::PrepareSetEjectedWeaponMag(
    WeaponPartsCollection &weaponComponentsIn
){
    USceneComponent *ejectedMagIn = weaponComponentsIn.FindComponent(EweaponPartEnum::EMag);
    if(!ejectedMagIn){
        return;
    }
    weaponComponents = &weaponComponentsIn;
    ejectedMag = ejectedMagIn;
    ejectedMagOriginalParent = ejectedMag->GetAttachParent();

}



void MagSwapProcess::SetupAndStartAnimation(UPlateCarrierMagSocket *swapSlotIn){
    if(swapSlotIn != nullptr){
        swapSlot = swapSlotIn;

        SetupAnimations();
    }
}

void MagSwapProcess::AttachToKeepWorldSpace(USceneComponent *newChild, USceneComponent *newParent){
    //attach to new parent
    if(newChild && newParent){
        Detach(newChild);
        newChild->AttachToComponent(newParent, FAttachmentTransformRules::KeepWorldTransform);
    }
    
}

void MagSwapProcess::AttachToSnap(USceneComponent *newChild, USceneComponent *newParent){
    if(newChild && newParent){
        Detach(newChild);
        newChild->AttachToComponent(newParent, FAttachmentTransformRules::SnapToTargetIncludingScale);
    }
}


void MagSwapProcess::Detach(USceneComponent *comp){
    if(comp){
        // 1. Explicitly detach completely from the previous parent/actor, keeping its current world location
        FDetachmentTransformRules DetachRules(EDetachmentRule::KeepWorld, true);
        comp->DetachFromComponent(DetachRules);
    }
}



//get mag static mesh / scene componenent
//kontrolle wird abgegeben: externe intepolation des objektes
//eject aus parenting systems, direkte eingabe ins target system


//insert mag / scene component
//kontrolle wird übergeben: interne interpolation
//des objektes
//inject sofort an sub hierachie

//callback wenn animation fertig



void MagSwapProcess::SetupAnimations(){
    if(!swapSlot){
        return;
    }
    animationQueue.clear();

    //--> prozess kann mit payloads ggf abgebacken werden
    // die eine animation hergibt wenn sie fertig ist z.b.
    // die animation muss aus bsplines konstriert werden.

    //move hand towards 
    //the socket

    //grab mag

    //go to weapon

    //swap - with parent change

    //move empty mag to carrier

    PayloadKeyFrameAnimation handToHolsteredMag;
    PayloadKeyFrameAnimation handToWeaponMag;
    PayloadKeyFrameAnimation handSwapMag;
    PayloadKeyFrameAnimation handToHolster;
    PayloadKeyFrameAnimation handToWeaponGrip; //last step hand back


    //alle positionen relativ zur hand finden
    float offsetSize = -5.0f;
    FVector locationSwapSlotWorld = swapSlot->GetComponentLocation();
    FVector locationMagWorld = ejectedMag->GetComponentLocation();
    FVector locationMagSideWorld = locationMagWorld + ejectedMag->GetRightVector() * offsetSize;

    FVector magMeshOffset = ejectedMag->GetComponentLocation() - ejectedMag->Bounds.Origin; // AB = B - A

    bool byZOffset = false; //true
    if(byZOffset){
        //remove offset from mag to (0,0,0), because mesh might be offseted
        locationMagSideWorld.Z -= magMeshOffset.Z;
        
        //the same must be true for the mag which is ejected / target point, since we
        //are removing the offset while attaching, the target point is different!
        locationMagWorld.Z -= magMeshOffset.Z;
    }else{
        //remove offset from mag to (0,0,0), because mesh might be offseted
        FVector down = ejectedMag->GetUpVector() * -1.0f;
        locationMagSideWorld += down * std::abs(magMeshOffset.Z);

        //the same must be true for the mag which is ejected / target point, since we
        //are removing the offset while attaching, the target point is different!
        locationMagWorld += down * std::abs(magMeshOffset.Z);
    }


    
    


    FVector locationSwapSlot = RelativeLocation(hand, locationSwapSlotWorld);
    FVector locationMag = RelativeLocation(hand, locationMagWorld);
    FVector locationMagSide = RelativeLocation(hand, locationMagSideWorld);

    handOriginal = hand->GetRelativeLocation();
    FVector handLocal(0, 0, 0);

    //todo: make animations!! 
    //especially reload with support point down!

    // -- hand to mag holster --
    float timeOfAnimation = 0.3f;
    ConstructAnimationHandTowards(
        handToHolsteredMag, //PayloadKeyFrameAnimation &animation,
        handLocal,          //from
        locationSwapSlot,   //to
        payloadHandMovedToMagHolster, //payload
        timeOfAnimation
    );

    // -- mag to side of weapon --
    timeOfAnimation = 2.0f; // 0.5f
    ConstructAnimationHandTowards(
        handToWeaponMag, //PayloadKeyFrameAnimation &animation,
        locationSwapSlot,   //from
        locationMagSide,    //to
        payloadHandMovedToMagEjected, //payload
        timeOfAnimation
    );




    // -- reload process: --

    /*TArray<FVector> ReloadSupportPoints;
    FVector downOffset(0, 0, -30);
    ReloadSupportPoints.Add(locationMagSide + downOffset);
    ReloadSupportPoints.Add(locationMag + downOffset);
    //timeOfAnimation = 0.5f; // 0.5f
    timeOfAnimation = 1.0f; // 0.5f
    ConstructAnimationHandTowards(
        handSwapMag,            //PayloadKeyFrameAnimation &animation,
        locationMagSide,        //from
        ReloadSupportPoints,
        locationMag,            //to
        payloadHandReloadFinished, //payload
        timeOfAnimation
    );*/

    TArray<FVector> ReloadSupportPoints;
    FVector downOffset(0, 0, -30);
    ReloadSupportPoints.Add(locationMagSide);
    ReloadSupportPoints.Add(locationMagSide + downOffset);
    ReloadSupportPoints.Add(locationMag + downOffset);
    ReloadSupportPoints.Add(locationMag);
    timeOfAnimation = 1.0f; // 0.5f

    ConstructAnimationHandTowards(
        handSwapMag,
        ReloadSupportPoints,
        payloadHandReloadFinished,
        timeOfAnimation
    );









    timeOfAnimation = 0.3f;
    // -- hand to holster back --
    TArray<FVector> toHolsterFrames;
    toHolsterFrames.Add(locationMag);
    toHolsterFrames.Add(locationMag + magMeshOffset * 2.0f);
    toHolsterFrames.Add(locationSwapSlot);

    ConstructAnimationHandTowards(
        handToHolster, //PayloadKeyFrameAnimation &animation,
        //locationMag,        //from
        //locationSwapSlot,   //to
        toHolsterFrames,
        payloadHandToHolsterBack, //payload
        timeOfAnimation
    );

    // -- hand to original back --
    ConstructAnimationHandTowards(
        handToWeaponGrip,             //PayloadKeyFrameAnimation &animation,
        locationSwapSlot,             //from
        handLocal,                    //to
        payloadHandToOriginal,        //payload
        timeOfAnimation
    );



    animationQueue.AddAnimation(handToHolsteredMag);
    animationQueue.AddAnimation(handToWeaponMag);
    animationQueue.AddAnimation(handSwapMag);
    animationQueue.AddAnimation(handToHolster);
    animationQueue.AddAnimation(handToWeaponGrip);
    //DrawAnimationDebug();
}

void MagSwapProcess::DrawAnimationDebug(){
    if(hand){
        TArray<FVector> frames = animationQueue.GetAllKeyFrames(hand->GetComponentTransform());
        FColor color = FColor::Red;
        DebugHelper::showLine(hand->GetWorld(), frames, color);
    }
}




void MagSwapProcess::ConstructAnimationHandTowards(
    PayloadKeyFrameAnimation &animation,
    FVector handLocation, 
    FVector target,
    FString payload,
    float timeOfAnimation
){
    TArray<FVector> points;
    points.Add(handLocation);
    points.Add(target);

    ConstructAnimationHandTowards(
        animation,
        points,
        payload,
        timeOfAnimation
    );
}

void MagSwapProcess::ConstructAnimationHandTowards(
    PayloadKeyFrameAnimation &animation,
    TArray<FVector> &keyframes,
    FString payloadName,
    float timeOfAnimation
){
    animation.clear();
    if (keyframes.Num() > 0)
    {
        float timePerFrame = timeOfAnimation / (float) keyframes.Num();
        for (int i = 0; i < keyframes.Num(); i++){
            animation.addFrame(keyframes[i], timePerFrame);
        }
        
        //set payload for changing components / parents.
        Payload load(this, payloadName);
        animation.AddPayload(load);
    }
}

void MagSwapProcess::ConstructAnimationSplineHandTowards(
    PayloadKeyFrameAnimation &animation,
    FVector handLocation, 
    TArray<FVector> &supportPoints,
    FVector target,
    FString payloadName,
    float timeOfAnimation
){
    animation.clear();

    TArray<FVector> animationKeyFrames;
    MakeSpline(handLocation, target, supportPoints, animationKeyFrames);

    ConstructAnimationHandTowards(
        animation,
        animationKeyFrames,
        payloadName,
        timeOfAnimation
    );


    if(animationKeyFrames.Num() > 0){
        DebugHelper::logMessage(
            FString::Printf(
                TEXT("MagSwapProcess made keyframes %d, support points %d"),
                animationKeyFrames.Num(),
                supportPoints.Num()
            )
        );
    }
}


void MagSwapProcess::MakeSpline(
    FVector &start, 
    FVector &end, 
    TArray<FVector> &supportPoints, 
    TArray<FVector> &outSpline
){
    MakeSpline(start, end, supportPoints, outSpline, animationEinheitsValue);
}

void MagSwapProcess::MakeSpline(
    FVector &start, 
    FVector &end, 
    TArray<FVector> &supportPoints, 
    TArray<FVector> &outSpline,
    float einheitsValue
){
    TArray<FVector> anchors;
    anchors.Reserve(2 + supportPoints.Num());
    anchors.Add(start);
    anchors.Append(supportPoints);
    anchors.Add(end);

    if(supportPoints.Num() > 0){
        
        TArray<FVectorBSplinePosition> splineInternal;
        BSpline curve;
        curve.calculatecurve(
            anchors, 
            splineInternal,
            einheitsValue
        );
        //DebugHelper::logMessage("MagSwapProcess::MakeSpline Made ", splineInternal.Num());

        outSpline.SetNum(splineInternal.Num());
        for (int i = 0; i < splineInternal.Num(); i++){
            outSpline[i] = splineInternal[i].GetPosition();
        }
    }else{
        outSpline = anchors;
    }
}


FVector MagSwapProcess::RelativeLocation(USceneComponent *from, USceneComponent *target){
    if (!from || !target){
        return FVector::ZeroVector;
    }
    FVector TargetWorldLocation = target->GetComponentLocation();
    return RelativeLocation(from, TargetWorldLocation);
}

FVector MagSwapProcess::RelativeLocation(USceneComponent *from, FVector &targetWorld){
    if (!from){
        return FVector::ZeroVector;
    }
    FVector RelativeLocalLocation = from->GetComponentTransform().InverseTransformPosition(targetWorld);
    return RelativeLocalLocation;
}









void MagSwapProcess::Notify(FString message){
    //auf payload reagieren: nächsten state setzen, components trees wechseln, usw.
    //DebugHelper::logMessage("MagSwapProcess::Notify RECEIVE ", message);
    if (message == payloadHandMovedToMagHolster)
    {
        AttachHolsteredMagToHand();
    }
    if(message == payloadHandMovedToMagEjected){
        AttachEjectedMagToHand();
    }
    if(message == payloadHandReloadFinished){
        AttachHolsteredMagToWeapon();
    }
    if(message == payloadHandToHolsterBack){
        AttachEjectedMagToCarrier();
    }
    if(message == payloadHandToOriginal){

    }

    //DebugHelper::logMessage("MagSwapProcess::Notify - info: ", animationQueue.Info());
}

void MagSwapProcess::AttachHolsteredMagToHand(){
    if(swapSlot != nullptr && hand != nullptr){
        if(swapSlot->HasMag()){

            USceneComponent *magPouched = swapSlot->GetPouchedMag();
            FVector relativeLocation = magPouched->GetRelativeLocation();

            AttachToSnap(magPouched, hand);
            magPouched->SetRelativeLocation(relativeLocation);

            /*AttachToKeepWorldSpace(
                swapSlot->GetPouchedMag(), // USceneComponent *newChild,
                hand                       // USceneComponent *newParent
            );*/

            //pause = true;
        }
    }
}

void MagSwapProcess::AttachEjectedMagToHand(){
    if(hand && ejectedMag){

        //looks better than manual offset caluclation
        AttachToKeepWorldSpace(
            ejectedMag, // USceneComponent *newChild,
            hand   // USceneComponent *newParent
        );
        //pause = true;
    }
}

void MagSwapProcess::AttachHolsteredMagToWeapon(){
    if(swapSlot != nullptr && ejectedMagOriginalParent != nullptr){
        if(swapSlot->HasMag()){

            //the pouched mag, which is in the hand right now, will be inserted into the weapon
            USceneComponent *mag = swapSlot->GetPouchedMag();
            AttachToSnap(mag, ejectedMagOriginalParent);

            //TODO (?<- check needed)
            //copy transform / original mag location here?

            // ----- NOTE: -----
            //Very Important: Notify the weapon for the new mag component here!
            //it doesnt matter whether its present in the editor scene tree 
            //the mag is swapped by ptr and thats enough!

            //set new ptr inside weapon components of the weapon
            if(weaponComponents != nullptr){
                weaponComponents->OverrideComponent(EweaponPartEnum::EMag, mag);
            }

            //pause = true;
        }
    }
}

void MagSwapProcess::AttachEjectedMagToCarrier(){
    //ejected mag must be placed into mag socket by ptr aswell!
    if(swapSlot){
        swapSlot->InsertMagSnap(ejectedMag);
    }
}



void MagSwapProcess::TickAnimation(float deltatime){
    if(pause){
        return;
    }

    FVector localPosHandOut;
    if(animationQueue.TryTick(localPosHandOut, deltatime)){
        if(hand){
            //sicne the local location of the hand in 
            //the animation is at (0,0,0)
            //we add the hand original location as pivot
            FVector handAnimated = localPosHandOut + handOriginal;
            hand->SetRelativeLocation(handAnimated);
        }
        //DebugHelper::logMessage("MagSwapProcess::TickAnimation - info: ", animationQueue.InfoExtended());
    }
}


bool MagSwapProcess::IsFinished(){
    return animationQueue.IsFinished();
}
