#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "customUiPlugin/ui/_baseClass/payload/PayLoadBase.h"
#include "BaseUiInterface.generated.h"

class UWidget;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class CUSTOMUIPLUGIN_API UBaseUiInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * interface designed to merge UcustomUiComponentBase and UWidget Derived class
 * into one interface, which allows Tick, and click dispatching manual system!
 */
class CUSTOMUIPLUGIN_API IBaseUiInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
    // pure virtual functions () = 0 Only!
public:
	virtual void Tick(float DeltaTime) = 0;
	virtual bool dispatchClick() = 0;
	virtual void SetVisible(bool flag) = 0;

	//MUST BE OVERRIDEN!!! - UWidget derived can return itself.
	virtual UWidget *baseLayoutPointer() = 0;

	///does not need to be implemented but can be.
	virtual UPayLoadBase *GetPayLoad(){
		return nullptr;
	}

};
