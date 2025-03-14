#pragma once

#include "p2/DebugHelper.h"
#include "callback.generated.h"

UCLASS()
class P2_API UCallback : public UObject
{
    GENERATED_BODY()

public:
    // Delegate declaration
    FSimpleDelegate CallbackPassed;

    // Set the delegate (bind a function or lambda)
    void SetCallback(FSimpleDelegate Callback)
    {
        CallbackPassed = Callback;
    }

    // none of this is called...
    UFUNCTION()
    void UCallbackFunction()
    {
        if (CallbackPassed.IsBound())  // Ensure the delegate is bound before executing
        {
            CallbackPassed.Execute();  // Executes the bound function or lambda

            DebugHelper::showScreenMessage("debugCallback CALLBACK");
        }else{
            DebugHelper::showScreenMessage("debugCallback CALLBACK NOT BOUND"); //printed
        }
    }
};