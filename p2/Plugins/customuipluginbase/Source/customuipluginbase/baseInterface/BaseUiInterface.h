#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "customuipluginbase/payload/PayLoadBase.h"
#include "customuipluginbase/UserInputTracking/UserInput.h"
#include "BaseUiInterface.generated.h"

class UWidget;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class CUSTOMUIPLUGINBASE_API UBaseUiInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * interface designed to merge UcustomUiComponentBase and UWidget Derived class
 * into one interface, which allows Tick, and click dispatching manual system!
 */
class CUSTOMUIPLUGINBASE_API IBaseUiInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
    // pure virtual functions () = 0 Only!
public:

	/// ---- PURE VIRTUAL ----

	virtual void Tick(float DeltaTime) = 0;
	virtual bool dispatchClick(const FVector2D &position) = 0;
	//MUST BE OVERRIDEN!!! - UWidget derived can return itself.
	virtual UWidget *baseLayoutPointer() = 0;


	/// ---- OPTIONAL VIRTUAL ----

	virtual bool dispatchHover(const FVector2D &position) { return false; };

	//can be implemented, will remove the hover state (is for game core 3D ui widgets.)
	virtual void removeHover() {};

	virtual void SetVisible(bool flag) {};
	virtual bool markedVisible() { return true; }

	//does not have to be overriden.
	virtual void dispatchUserInput(UserInput &input) {};

	

	///does not need to be implemented but can be.
	virtual UPayLoadBase *GetPayLoad(){
		return nullptr;
	}
	//debug
	virtual void enableTicklog() {};

	
	virtual void OnHover() {};




	
};
