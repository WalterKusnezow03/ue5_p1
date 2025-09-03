#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IkHumanoidModell/carryItems/container/CarriedItemPositionData.h"
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

	/// @brief must provide this api for outside updates of the actor (but this is just an interface.)
	virtual void UpdateActorTransform(FVector &location, FRotator &rotation) = 0;

	///@brief must be implemented if local actor position is changed for animations
	/// (Weapon kickback manual for example)
	virtual FVector LocalAnimationOffset();
};
