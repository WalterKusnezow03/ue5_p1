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
	
	virtual void RightHandWorld() = 0;
    virtual void LeftHandWorld() = 0;
	virtual CarriedItemPositionData &getItemPositionDataRef() = 0;
};
