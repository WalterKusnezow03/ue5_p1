#pragma once
#include "CoreMinimal.h"
#include "customUiPlugin/ui/_baseClass/customUiComponentBase.h"
#include "AutoBoxBase.h"
#include "Components/VerticalBox.h"


#include "Vbox.generated.h"

/// @brief vbox wrapper to track custom visibility and click listening
UCLASS()
class CUSTOMUIPLUGIN_API UVbox : public UAutoBoxBase {
    GENERATED_BODY()

public:
    
    virtual void init() override;
    virtual UWidget *baseLayoutPointer() override{
        return baseVBox; //button //scalebox(wrong)
    }

    virtual void AddChild(IBaseUiInterface *item) override;

    /// @brief adds ANY child to the vbox, no custom click or visibility listening
    /// @param any 
    virtual void AddChild(UWidget *any) override;


    ///@brief removes a child from click listening and inner container
    virtual void RemoveChild(IBaseUiInterface *item) override;

    ///@brief must be overriden - trys to remove child from container
    virtual void RemoveChild(UWidget *any) override;

    
    
    void SetItemsFillHorizontal();

protected:
    virtual void UpdatePadding(UWidget *widget) override;

    void UpdateAlignmentForAllTrackedItems();
    void UpdateAlignment(UWidget *item);

private:
    
    void initVboxIfNeeded();
    UPROPERTY()
    UVerticalBox *baseVBox = nullptr;


    // Left, Right, Center, Fill
    bool fillHorizontal = false;

    EHorizontalAlignment horizontalAlignmentForItems = EHorizontalAlignment::HAlign_Left; 
};