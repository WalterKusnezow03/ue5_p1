#pragma once

#include "CoreMinimal.h"

#include "IkHumanoidModell/carryItems/Interface/IkCarryInterface.h"

#include "IkHumanoidModell/carryItems/Interface/EmptyActor/EArmAnimationEnum.h"
#include "IkHumanoidModell/carryItems/container/CarriedItemPositionData.h"
#include "IkHumanoidModell/carryItems/Interface/EmptyActor/FArmAnimationPair.h"
#include "IkHumanoidModell/carryItems/Interface/EmptyActor/FingerPositions/CarriedItemHandComponentManager.h"
#include "GameCore/util/FVectorUtil.h"
#include "GameCore/util/ActorBase/ActorBase.h"

#include "IKCarryInterfaceAnimatedActor.generated.h"

/// --- EMPTY ARM TARGET ACTOR ---
//will hold an animation and animate the actor scene components
//instead of carrying a raw item, it will be invisible
//actors will be attachable (carry a greande, throwing, states switchable)
UCLASS()
class IKHUMANOIDMODELL_API AIKCarryInterfaceAnimatedActor : public AActorBase, public IIkCarryInterface{
    GENERATED_BODY()

public:
	AIKCarryInterfaceAnimatedActor();
	static AIKCarryInterfaceAnimatedActor *makeInstance(UWorld *world);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float deltatime) override;

	//might be changed by derived class
	virtual bool AutoInitComponents(){
		return true;
	}

	void InitComponents();

	bool logEnabled = false;
	bool drawLineOnTransformUpdate = false;

public:
	/// --- INTERFACE OVERRIDE ---

	/// @brief must provide this package with end effector hands position and more
	/// when asked!
	virtual CarriedItemPositionData &getItemPositionDataRef() override;

	virtual FIKCarryInterfaceAxisConstraint &getAxisConstraint();

	/// @brief must provide this api for outside updates of the actor (but this is just an interface.)
	/// is called on skelleton tick if item is attached!, called from carriedItemSocket!
	virtual void UpdateActorTransform(FVector &location, FRotator &rotation) override;
	

	//updates for attached item rotation
	virtual void UpdateLowerArm(EArmType typeArm, const FVector &direction) override;

	virtual FVector LocalAnimationOffset() override{
		return localAnimationOffset;
	}
	/// --- INTERFACE OVERRIDE END ---


	// animation fire
	virtual void FireAnimation(EArmAnimationEnum id);
	virtual void StopAnimation();
	bool IsAnimationActive(EArmAnimationEnum id);
	// animation fire

	//animation time rescale
public:
	void MotionTimeUpdateAnimation(EArmAnimationEnum type, float newTime);
protected:
	void MotionTimeUpdateAnimationInAnimationMap(EArmAnimationEnum type, float newTime);
	void MotionTimeUpdateAnimationIfActive(EArmAnimationEnum type, float newTime);

	//animation time rescale

public:
	//hand attached items
	IIkCarryInterface *CurrentAttachedItem();
	void InjectCarryByHandItem(IIkCarryInterface *newItem);
	void EjectCarryByHandItem();
	//hand attached items




	//picked up flag to animate or not
	void SetIsPickedUpFlag(bool flag);
	void SetDebugPlayerAnimatedActor(bool flag);



	//override throwing direction -- player only humanoid controller --
	void OverrideThrowingDirectionOfItem(FVector direction);
	//override throwing direction -- player only humanoid controller --

protected:

	bool isPickedUpFlag = false;
	void ScreenLogPickedUpState();
	bool isPickedUpByPlayerDebugFlag = false;
	void DebugDrawHandLocation(EArmType type, float deltatime, bool force);
	// picked up flag to animate or not

	bool IsHandAttachedItem(IIkCarryInterface *newItem);
	void TickUpdateAttachedItem();
	//copies own transform to actor
	void TickUpdateAttachedItemGlobalTransform();
	//update scenes to adapt to animated hand components
	void TickUpdateAttachedItemLocalTransform(EArmType type);

	//carried item by hand
	IIkCarryInterface *attachedHandCarriedItem = nullptr;

	//qeued carried item if current is thrown away by animation
	IIkCarryInterface *qeuedForPickupAttachedHandCarriedItem = nullptr;


	bool ActiveAnimationIsThrowingItemAnimation();

	//only call on any animation finish to check whether to inject the next item.
	void ApplyImpulseToCarriedItemIfThrowFinished();
	void PickupQeuedItemIfThrowFinished();




	//updates for lower arm direction resulting in 
	//attached item rotation being orthogonal to arm direction
	FVector OrthogonalLocalUpFor(const FVector &vec);



	FVector localAnimationOffset; //not needed, just on weapon kickback, not here.


	std::map<EArmAnimationEnum, FArmAnimationPair> AnimationTable;

	void TickAnimation(float deltatime);
	bool TickAnimationFor(EArmType type, float deltatime);
	bool HasAnimation(EArmAnimationEnum type);
	FArmAnimationPair &FindAnimationPair(EArmAnimationEnum type);
	FArmAnimationPair &FindAnimationPairAndResetProgress(EArmAnimationEnum type);

	//loads animation to table regardless of existence
	void LoadAnimationToAnimationTable(
		EArmAnimationEnum typeAnimation
	);
	void ReplaceAnimationPair(
		FArmAnimationPair &anim,
		EArmAnimationEnum type
	);

	FArmAnimationPair fallback;

	void InitAnimationTable();

	//animation active
	bool CurrentAnimationCanBeTicked();
	bool animationActiveFlag = false;
	FArmAnimationPair activeAnimation;


	// -- components -- 
	USceneComponent *FindHand(EArmType type);
	
	USceneComponent *RightHandComponent = nullptr;
	USceneComponent *LefttHandComponent = nullptr;

	// -- components --

	// -- components update --
	void UpdateHandComponentLocation(EArmType typeArm, FVector &location);
	void UpdateHandComponentRotation(EArmType typeArm, FVector &rotationDirection);
	void UpdateHandComponentRotation(EArmType typeArm, FRotator &rotation);

	void LogCurrentAnimationStatus();





	//position data to update based on animation
	CarriedItemHandComponentManager handAndFingerPositionManager;

	void InitAxisConstraintEmpty();
	FIKCarryInterfaceAxisConstraint axisConstraintNone;

	MMatrix internalTransform;


	//flag for ticking running animation or not
	bool hasMovedFlag = false;
	float distSquaredMovedFlag = 4.0f;
	void UpdateHasMovedFlag(const FVector &location);


	// ---- throwing direction of items: can be overriden ----
	FVector ThrowingDirectionOfItem();
	bool bItemThrowingDirectionOverriden = false;
    FVector itemThrowingDirection;
};	