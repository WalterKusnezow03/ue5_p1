#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IkHumanoidModell/carryItems/Interface/EmptyActor/AxisConstraint/IKCarryInterfaceAxisConstraint.h"
#include "IkHumanoidModell/carryItems/container/CarriedItemPositionData.h"
#include "IkHumanoidModell/carryItems/Interface/EIKCarryType.h"
#include "IkCarryInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class IKHUMANOIDMODELL_API UIkCarryInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * interface designed for Ik Skelleton Interaction:
 * Carry items with right or left hand or both
 * 
 * implement in entity class
 */
class IKHUMANOIDMODELL_API IIkCarryInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
    // pure virtual functions () = 0 Only!
public:
	
	/// @brief must provide this package with end effector hands position and more
	/// when asked!
	virtual CarriedItemPositionData &getItemPositionDataRef() = 0;

	virtual FIKCarryInterfaceAxisConstraint &getAxisConstraint() = 0;

	/// @brief must provide this api for outside updates of the actor (but this is just an interface.)
	/// is called on skelleton tick if item is attached!, called from carriedItemSocket!
	virtual void UpdateActorTransform(FVector &location, FRotator &rotation) = 0;

	// --- TO BE CALLED FROM BONES ! ---
	/// update for special carried items 
	//(update passed though this function back to actor implementing the interface)
	virtual void UpdateLowerArm(EArmType typeArm, const FVector &direction);

	///@brief must be implemented if local actor position is changed for animations
	/// (Weapon kickback manual for example)
	virtual FVector LocalAnimationOffset();






	/// erstmal so machen und testen!
	///denken
	/*
	items könnten an sicht auch attached sein an
	hände zum folgen / werfen, an animation

	allerdings lässt sich dazu sagen dass
	das per eigenem actor oder flag stattfinden kann
	bei jenem actor der attachbar ist

	oder gleich alles vererbt wird und alles über hände stattfindet

	alternativ kann es eine basis throwable geben
	was injected wird für wurf. Besser(?)
	*/

	virtual EIKCarryType GetCarryType(){
		return EIKCarryType::ECarryBySocket; //default
	}

	/// -- only for hand carried items like grenade, attached to hands, not hands to item --
	/// @brief items attached to empty actor will receive the hand locations 
	/// from the actor by this method, if the carry type is ECarryByHand.
	virtual void UpdateLocalSceneTransformCarriedByHand(EArmType type, FVector &location, FRotator &rotation){
		//exapmple implementation
		/*if(GetCarryType() == EIKCarryType::ECarryByHand){

		}*/
	}


};
