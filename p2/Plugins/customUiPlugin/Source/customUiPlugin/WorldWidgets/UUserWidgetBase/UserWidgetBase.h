#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"


#include "UserWidgetBase.generated.h"

/// @brief will be the base class for 3D World User Widgets
UCLASS()
class CUSTOMUIPLUGIN_API UUserWidgetBase : public UUserWidget
{
    GENERATED_BODY()

public:
    //constructor.
    virtual void PostInitProperties() override;

    void SetLabelText(FString NewText);

    void Init();

private:
    UPROPERTY()
    UTextBlock* TextBlock;
};