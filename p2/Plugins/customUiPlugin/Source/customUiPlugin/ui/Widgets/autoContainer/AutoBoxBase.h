#pragma once
#include "CoreMinimal.h"
#include "customUiPlugin/ui/_baseClass/customUiComponentBase.h"

#include "AutoBoxBase.generated.h"



/// @brief ABSTRACT designed as a abstract base class for hbox and vbox containers
UCLASS()
class CUSTOMUIPLUGIN_API UAutoBoxBase : public UcustomUiComponentBase {
    GENERATED_BODY()

public:
    /*
    ABSTRACT
    virtual void init() override;
    virtual UWidget *baseLayoutPointer() override{
        return button; //button //scalebox(wrong)
    }
    */

    /// @brief dispatches to all children added!
    /// @return any item hit
    virtual bool dispatchClick() override;

    /// @brief sets Base Widget invisble and flags all childs invisble
    /// @param flag 
    void setVisible(bool flag) override;

    
    /// @brief dipatches tick to all children!
    /// @param deltatime 
    void Tick(float deltatime) override;

    /// @brief adds a child to the layout and adds to custom click listening!
    /// @param item 
    virtual void AddChild(UcustomUiComponentBase *item);

    /// @brief adds any child, not in click listening nor custom visibility propagation for hittest!
    /// @param any
    virtual void AddChild(UWidget *any){}; //MUST BE OVERRIDEN


    ///@brief removes a child from click listening
    virtual void RemoveChild(UcustomUiComponentBase *item);

    ///@brief must be overriden
    virtual void RemoveChild(UWidget *any){};

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

    bool CanAddChild(UcustomUiComponentBase *ptr);
};