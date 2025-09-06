#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "CollectionProperties.h"

#include "CollectableInterface.generated.h"



// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class GCGAMECORE_API UCollectableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * interface designed make aactor or uobject pass its infos into the
 * collection interface. Will then be used 
 */
class GCGAMECORE_API ICollectableInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
    // pure virtual functions () = 0 Only!
public:
    virtual CollectionProperties &getCollectionProperties() = 0;
};