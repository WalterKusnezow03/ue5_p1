#pragma once

#include "CoreMinimal.h"
#include "customuipluginbase/baseInterface/BaseUiInterface.h"

#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"

#include "TextBoxWidget.generated.h"


UCLASS()
class P2_API UTextBoxWidget : public UUserWidget, public IBaseUiInterface{
    GENERATED_BODY()

public:
    

    UFUNCTION(BlueprintImplementableEvent, Category = "WidgetSetting")
    UWidget *GetTextWidget();


    //override
    virtual void Tick(float DeltaTime) override;
    virtual bool dispatchClick(const FVector2D &position) override;
    virtual bool dispatchHover(const FVector2D &position) override;

    virtual void removeHover() override;
   
    virtual void dispatchUserInput(UserInput &input) override;

    virtual UWidget *baseLayoutPointer() override { return this; };

    bool IsMarkedSelected(){
        return isSelected;
    }

    FString GetText();

    void ResetText();
    void ResetTextIfEmpty();

protected:
    UTextBlock *GetTextBlockWidget();

    bool isSelected = false;

    void SetText(FString stringin);

    //do not change
    const FString defaultText = TEXT("Type...");
};