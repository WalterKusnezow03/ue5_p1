#pragma once


#include "customUiPlugin/ui/_baseClass/customUiComponentBase.h"
#include "Components/Button.h"
#include "Components/ScaleBox.h"
#include "customUiPlugin/ui/Widgets/callback/callback.h"
#include "customUiPlugin/ui/Widgets/buttons/ButtonBase.h"
#include "Components/TextBlock.h"

#include "customUiPlugin/slateDerived/TextBased/Text/WidgetSlateText.h"

#include "TextButton.generated.h"



UCLASS()
class CUSTOMUIPLUGIN_API UTextButton : public UButtonBase
{
    GENERATED_BODY()

public:
    virtual void init() override;

    void SetText(FString textIn);
    FString GetText();

    virtual bool dispatchClick() override;

    /// @brief slate widgets MUST be ticked if they are changed in scale to recognize the change.
    /// @param deltatime 
    virtual void Tick(float deltatime) override;

    bool CompareText(FString textIn);

    virtual UcustomUiComponentBase *DuplicateWidgetInitialized(UObject *parent) override;

    virtual UWidget *baseLayoutPointer() override{
        return TextBlock;
    }

    UWidgetSlateText *SlateTextBlock(){
        return TextBlock;
    }

private:

    UPROPERTY()
    UWidgetSlateText *TextBlock = nullptr;

    //Deprecated! 
    //UPROPERTY()
    //UTextBlock *TextBlock = nullptr;
    
    FString textInternalCopy;
    void createTextAndAddToButton();

};