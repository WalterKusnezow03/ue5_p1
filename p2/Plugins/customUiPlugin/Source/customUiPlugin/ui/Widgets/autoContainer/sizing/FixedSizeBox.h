#pragma once

#include "CoreMinimal.h"
#include "customUiPlugin/ui/_baseClass/customUiComponentBase.h"


#include "FixedSizeBox.generated.h"

class USizeBox;

UCLASS()
class CUSTOMUIPLUGIN_API UFixedSizeBox : public UcustomUiComponentBase {
    GENERATED_BODY()

public:
    using UcustomUiComponentBase::init;
    virtual void init() override;

    virtual UWidget *baseLayoutPointer() override {
        return SizeBox;
    };

    virtual void Tick(float DeltaTime) override{
        if(!TICK_ENABLED){
            return;
        }
        Super::Tick(DeltaTime);
        if(child){
            child->Tick(DeltaTime);
        }
    }

    virtual bool dispatchClick() override;

    /// @brief marks self as invisible: may be needed to not dispatch a click, base layout pointer is 
    /// invisible too! - OVERRIDE THIS METHOD!
    /// @param visible
    virtual void setVisible(bool visible) override;


    void OverrideChild(UcustomUiComponentBase *item);

    void SetWidth(int pixels);

private:
    int validatePixels(int num);

    void createLayout();

    UPROPERTY()
    USizeBox *SizeBox = nullptr;

    UPROPERTY()
    UcustomUiComponentBase *child = nullptr;
};