#pragma once 

#include "customUiPlugin/ui/Widgets/item/removableBase/RemovableItem.h"
#include "customUiPlugin/ui/Widgets/buttons/subtypes/TextButton.h"
#include "RemovableTextButton.generated.h"


UCLASS()
class P2_API URemovableTextButton : public URemovableItem {
    GENERATED_BODY()
public:

    virtual void init() override;

    void SetText(FString Text);
    FString GetText();

    void SetTextButtonCallBack(FSimpleDelegate callbackIn);

private:
    void createText();

    UPROPERTY()
    UTextButton *textButton = nullptr;
};