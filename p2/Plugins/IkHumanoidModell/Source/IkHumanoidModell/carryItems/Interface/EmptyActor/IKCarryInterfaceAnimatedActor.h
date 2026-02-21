#pragma once

#include "CoreMinimal.h"

#include "IkHumanoidModell/carryItems/Interface/IkCarryInterface.h"

#include "IkHumanoidModell/carryItems/Interface/EmptyActor/EArmAnimationEnum.h"
#include "IkHumanoidModell/carryItems/container/CarriedItemPositionData.h"
#include "IkHumanoidModell/carryItems/Interface/EmptyActor/FArmAnimationPair.h"
#include "IkHumanoidModell/carryItems/Interface/EmptyActor/FingerPositions/CarriedItemFingerPositionManager.h"

#include "GameCore/util/ActorBase/ActorBase.h"

#include "IKCarryInterfaceAnimatedActor.generated.h"

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
	// animation fire

	//hand attached items
	void InjectCarryByHandItem(IIkCarryInterface *newItem);
	void EjectCarryByHandItem();
	//hand attached items

protected:
	bool IsHandAttachedItem(IIkCarryInterface *newItem);
	void TickUpdateAttachedItem();
	//copies own transform to actor
	void TickUpdateAttachedItemGlobalTransform();
	//update scenes to adapt to animated hand components
	void TickUpdateAttachedItemLocalTransform(EArmType type);

	//to be replaced with eject info
	IIkCarryInterface *attachedItemDebug = nullptr;





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

	//position data to update based on animation
	CarriedItemPositionData itemPositionData;
	CarriedItemFingerPositionManager handAndFingerPositionManager;

	void InitAxisConstraintEmpty();
	FIKCarryInterfaceAxisConstraint axisConstraintNone;

	MMatrix internalTransform;


	//flag for ticking running animation or not
	bool hasMovedFlag = false;
	void UpdateHasMovedFlag(const FVector &location);
};	