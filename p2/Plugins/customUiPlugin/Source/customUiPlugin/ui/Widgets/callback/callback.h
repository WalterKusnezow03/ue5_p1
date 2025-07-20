#pragma once

#include "GameCore/DebugHelper.h"
#include "callback.generated.h"

UCLASS()
class CUSTOMUIPLUGIN_API UCallback : public UObject
{
    GENERATED_BODY()

public:

    // Set the delegate (bind a function or lambda)
    ///@brief OVERRIDES the current callback delegate!
    void SetCallback(FSimpleDelegate Callback)
    {
        CallbackPassed = Callback;
    }

    //works fine, button sometimes buggy
    UFUNCTION()
    void UCallbackFunction()
    {
        if (CallbackPassed.IsBound())  // Ensure the delegate is bound before executing
        {
            CallbackPassed.Execute();  // Executes the bound function or lambda

            DebugHelper::showScreenMessage("debugCallback CALLBACK");
        }
    }

private:
    FSimpleDelegate CallbackPassed;

};