#pragma once

#include "CoreMinimal.h"
#include "customUiPlugin/ui/_baseClass/customUiComponentBase.h"
#include "customUiPlugin/baseInterface/BaseUiInterface.h"

#include "UiComponentParent.generated.h"

/// @brief Abstract Class to track children
UCLASS()
class CUSTOMUIPLUGIN_API UUiComponentParent : public UcustomUiComponentBase{

    GENERATED_BODY()

public:
    // --- CUSTOM DISPATCHER -> OVERRIDE ---
    virtual bool dispatchClick() override;
    virtual void SetVisible(bool visible) override;
    virtual void Tick(float deltatime) override;


    /// @brief adds child tracked to Click, Tick and Visibilty Dispatch
    virtual void AddChild(IBaseUiInterface *item);

    /// @brief removes child from tracked to Click, Tick and Visibilty Dispatch if found
    virtual void RemoveChild(IBaseUiInterface *item);


    /// @brief removes child from tracked to Click, Tick and Visibilty Dispatch if found by comparing
    /// the widget ptr to the base interface widget ptrs.
    virtual void RemoveChild(UWidget *item);

protected:
    bool CanAddChild(IBaseUiInterface *item);

    
    TArray<IBaseUiInterface *> trackedChildsInterface; //cannot be marked Uproperty

    UPROPERTY()
    TArray<UWidget *> trackedChildsAsUWidget; //prevent GC, very important.


private:
    virtual void RemoveChildSwapPopBack(IBaseUiInterface *item);
    virtual void RemoveChildSwapPopBack(UWidget *item);

};