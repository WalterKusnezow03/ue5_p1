#pragma once
#include "CoreMinimal.h"
#include "customUiPlugin/ui/_baseClass/customUiComponentBase.h"
#include "customUiPlugin/ui/_baseClass/baseParent/UiComponentParent.h"

#include "AutoBoxBase.generated.h"



/// @brief ABSTRACT designed as a abstract base class for hbox and vbox containers
UCLASS()
class CUSTOMUIPLUGIN_API UAutoBoxBase : public UUiComponentParent {
    GENERATED_BODY()

public:
    using UUiComponentParent::AddChild;
    using UUiComponentParent::RemoveChild;

    /// @brief adds any child, not in click listening nor custom visibility propagation for hittest!
    /// @param any
    virtual void AddChild(UWidget *any){}; //MUST BE OVERRIDEN

    ///@brief must be overriden
    virtual void RemoveChild(UWidget *any){};

    virtual void ReplaceChild(int index, IBaseUiInterface *item) {};

    ///@brief will update the padding for all attached items (UcustomUiComponents)
    void SetPadding(FVector2D &paddingIn);

protected:
    FVector2D padding;

    FMargin makePadding();

    ///@brief must be overriden!
    virtual void UpdatePadding(UWidget *widget){};

    /// @brief doesnt have to be override, called from setpadding
    void UpdatePaddingForAllTrackedItems();



    /// @brief to be overriden.
    /// @param widget 
    virtual void UpdateSlotForCustomSlate(UWidget *widget){};

    UPROPERTY()
    TArray<UcustomUiComponentBase *> attachedItems;

    using UUiComponentParent::CanAddChild;
    bool CanAddChild(UcustomUiComponentBase *ptr);
};