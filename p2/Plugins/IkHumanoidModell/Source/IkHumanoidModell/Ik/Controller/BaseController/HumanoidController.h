#pragma once

#include "CoreMinimal.h"
#include "IkHumanoidModell/carryItems/Interface/IkCarryInterface.h"
#include "IkHumanoidModell/Ik/Controller/HipController/HipController.h"
#include "IkHumanoidModell/Ik/Controller/TorsoController/TorsoController.h"
#include "IkHumanoidModell/Ik/CarriedItem/CarriedItemSocket.h"
#include "IkHumanoidModell/Ik/CarriedItem/Animated/AnimCarriedItemSocket.h"
#include "IkHumanoidModell/carryItems/enum/ECarriedItemPosition.h"
#include "GameCore/Raycast/query/CollisionTracker.h"
#include "GameCore/interfaces/Damageinterface.h"


/// @brief will control the skelleton, hip and torso controller
class IKHUMANOIDMODELL_API HumanoidController {

public:
    HumanoidController();
    virtual ~HumanoidController();

    void EnableDebugLogExtended();

    virtual void defaultSetup(UWorld *world);
    virtual void defaultSetup(UWorld *world, bool flagWantedHands);
   

    void ResetAndRebuild();

    /// @brief call tick function to run locomotion, hip and torso controller, aswell as
    /// update all carried items.
    /// @param deltatime 
    virtual void Tick(float deltatime);
    void attachOrReplaceCarriedItem(IIkCarryInterface *ptr);
    void dropCarriedItem();

    //add owning actor to be ignored from collision for trajectory grounding
    void raycastIgnoreOwner(AActor *actor);

    //rotation change of hipcontroller
    void LookAt(FVector &location);

    void setupRotationForNextStep(float radianYaw);
    void forceYawAdd(float degree);

    FVector GetLocation();
    FRotator GetRotation();
    FVector lookDirection();

    /// @brief will place skelleton at a target, foot on ground
    void SetLocation(FVector &target);

    //sets locomotion enabled immidiate
    void setStateWalking();

    //sets stop locomotion immidiate
    void stopLocomotion();

    //will wait for rotation to finish and then stop locomotion, unless forced locomotion 
    //any time inbetween
    void stopLocomotionOnceRotationHasFinished();

    //human entity bot api for carried item
    //will change the item the asigned position, lerp if needed
    void changeCarriedItemSocket(ECarriedItemPosition type);


    //set damaged owner api
    /// Call after AFTER Setup - Internal Bones not setup before!
    void setDamagedOwner(IDamageinterface *damagedOwnerIn);


protected:

    //collision params tracking
    CollisionTracker collisionParamsProvider;

    //legs, locomotion
    HipController hipController;


    //arms and torso
    TorsoController torsoController;


    //main attached weapon / item

    //add or remove a item from collison params
    void updateCollisionParams(IIkCarryInterface *ptr, bool bAdd);
    void updateCollisionParams(AActor *ptr, bool bAdd);


    void addAllActorsInChildrenToRaycastExclude();
    // actor collect for set damaged owner and exclude from raycast here
    TArray<AActor *> actorInChildrenArray();


    //CarriedItemSocket mainItemSocket;
    AnimCarriedItemSocket mainItemSocket;
    virtual void TickMainCarriedItemSocket(float deltatime);

   
    
    //empty actor to pickup, attach hand carried items to
    void OnDropUpdateAnimation();
    void SetupEmptyArmAnimationActor(UWorld *world);

    AIKCarryInterfaceAnimatedActor *emptyArmTargetActor = nullptr;
    IIkCarryInterface *CurrentPickedUpItem();
    bool EmptyActorIsPickedUp();
    void UpdateEmptyArmTargetActorPickedUpFlag();
    void dropItemFromEmptyActorHand();
    bool TryInjectIntoEmptyIkCarryInterface(IIkCarryInterface *newItem);
    //empty actor to pickup, attach hand carried items to


};