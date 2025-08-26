#pragma once

#include "CoreMinimal.h"

#include "customUiPlugin/ui/Widgets/buttons/subtypes/TextButton.h"
#include "customUiPlugin/ui/Widgets/autoContainer/Hbox.h"

#include "RemovableItem.generated.h"

///removeable item with layout [[HboxLeft][hbox[delete,(other options might follow)]]]
UCLASS()
class CUSTOMUIPLUGIN_API URemovableItem : public UcustomUiComponentBase {

    GENERATED_BODY()

public:
    virtual void init() override;

    virtual bool dispatchClick() override;

    /// @brief marks button as invisible: may be needed to not dispatch a click, base layout pointer is 
    /// invisible too!
    /// @param visible
    virtual void setVisible(bool visible) override;

    virtual UWidget *baseLayoutPointer() override{
        if(baseHBox){
            return baseHBox->baseLayoutPointer();
        }
        return nullptr;
    }

    void SetRemoveCallback(FSimpleDelegate delegate);

    /// @brief adds a child to the left hbox
    /// @param widget not nullptr, click dispatched if needed
    void AddChild(UcustomUiComponentBase *widget);

private:
    void createLayout();
    void createRemoveButton();

    UPROPERTY()
    UHbox *baseHBox = nullptr;

    UPROPERTY()
    UHbox *hboxLeft = nullptr;

    UPROPERTY()
    UHbox *hboxRight = nullptr; //locks remove button on right side.

    UPROPERTY()
    UTextButton *removeButton = nullptr;
};