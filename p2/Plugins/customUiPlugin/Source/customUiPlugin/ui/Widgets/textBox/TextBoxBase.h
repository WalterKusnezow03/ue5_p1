
#pragma once


#include "customUiPlugin/ui/_baseClass/customUiComponentBase.h"
#include "customUiPlugin/ui/Widgets/callback/callback.h"
#include "Components/ScaleBox.h"
#include "Components/EditableTextBox.h"

#include "TextBoxBase.generated.h"

/// @brief is a textbox abstracktion, saves the current string typed.
UCLASS()
class CUSTOMUIPLUGIN_API UTextBoxBase : public UcustomUiComponentBase {
    GENERATED_BODY()

public:
    //call needed on construct
    virtual void init() override;

    virtual UWidget *baseLayoutPointer() override{
        return sizeBox; //button //scalebox(wrong)
    }

    void SetHintText(FString hint);

    /// set up delegate when finished writing
    void SetCallBack(FSimpleDelegate onFinished);

    /// set up delegate for text changed
    void SetCallBackTextUpdate(FSimpleDelegate onChanged);

    /// gets the current typed text
    FString GetText();

    void SetSize(FVector2D size);

protected:

private:
    void createTextBox();
    void setDesignDefault();
    void setupCallback();

    //callback on click
    UPROPERTY()
    UCallback *callbackPointer = nullptr;

    UPROPERTY()
    UCallback *callbackPointerTextChanged = nullptr;

    //ui components
    UPROPERTY()
    UEditableTextBox *TextBox = nullptr;

    UPROPERTY()
    USizeBox *sizeBox = nullptr;


    FString innerTextSaved;

    UFUNCTION()
    void OnTextChangedHandler(const FText &Text);

    UFUNCTION()
    void OnTextCommittedHandler(const FText &Text, ETextCommit::Type CommitMethod);

};

