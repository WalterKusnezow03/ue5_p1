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



    ///@brief dispatches a click and returns the index in IBaseUiInterface Array
    virtual bool dispatchClick(int &outIndexFirst);

    ///@brief item at index or nullptr if invalid
    IBaseUiInterface *BaseInterfaceAtIndex(int i);

    ///@brief item at index or nullptr if invalid
    UWidget *UWidgetAtIndex(int i);


    /// @brief use only for very special cases where this data is needed!
    const TArray<IBaseUiInterface *> &AccessInternalItemsTmp() const;

protected:
   

    

    /// @brief adds child tracked to Click, Tick and Visibilty Dispatch
    virtual void AddChild(IBaseUiInterface *item);

    /// @brief removes child from tracked to Click, Tick and Visibilty Dispatch if found
    virtual void RemoveChild(IBaseUiInterface *item);


    /// @brief removes child from tracked to Click, Tick and Visibilty Dispatch if found by comparing
    /// the widget ptr to the base interface widget ptrs.
    virtual void RemoveChild(UWidget *item);


    bool CanAddChild(IBaseUiInterface *item);

    
    TArray<IBaseUiInterface *> trackedChildsInterface; //cannot be marked Uproperty

    UPROPERTY()
    TArray<UWidget *> trackedChildsAsUWidget; //prevent GC, very important.


private:
    virtual void RemoveChildSwapPopBack(IBaseUiInterface *item);
    virtual void RemoveChildSwapPopBack(UWidget *item);

};