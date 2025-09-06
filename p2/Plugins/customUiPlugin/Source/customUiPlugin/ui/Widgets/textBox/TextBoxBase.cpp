#include "TextBoxBase.h"
#include "GameCore/DebugHelper.h"

#include "customUiPlugin/ui/Widgets/Text/UiTextStyle.h"



void UTextBoxBase::init(){
    if(WAS_INIT_FLAG){
        return;
    }
    createTextBox();
    setDesignDefault();
    setupCallback();
}

void UTextBoxBase::createTextBox(){
    WAS_INIT_FLAG = true;
    TextBox = NewObject<UEditableTextBox>(this);
    SetHintText("Type Something");

    //scalebox = NewObject<UScaleBox>();
    //scalebox->SetStretch(EStretch::ScaleToFit);
    //scalebox->AddChild(TextBox);

    sizeBox = NewObject<USizeBox>();
    sizeBox->SetContent(TextBox);
    SetSize(FVector2D(300, 70));
}

void UTextBoxBase::setDesignDefault(){
    if(TextBox){
        TextBox->WidgetStyle = UiTextStyle::DefaultEditableTextBoxStyle();

        //UiColors::buttonBlack
        
    
    }
}

void UTextBoxBase::SetSize(FVector2D size){
    if(sizeBox){
        sizeBox->SetWidthOverride(size.X);   // fester Wert
        sizeBox->SetHeightOverride(size.Y);   // optional
    }
    
}





void UTextBoxBase::setupCallback(){
    //Keyboard events
    if(TextBox){
        TextBox->OnTextChanged.AddDynamic(this, &UTextBoxBase::OnTextChangedHandler);
        TextBox->OnTextCommitted.AddDynamic(this, &UTextBoxBase::OnTextCommittedHandler);
    }

    //finished event
    callbackPointer = NewObject<UCallback>(this);
    callbackPointerTextChanged = NewObject<UCallback>(this);
}





void UTextBoxBase::OnTextChangedHandler(const FText &Text){
    innerTextSaved = Text.ToString();
    DebugHelper::showScreenMessage(FString::Printf(TEXT("Text: %s"), *innerTextSaved), FColor::Green);


    if(callbackPointerTextChanged){
        callbackPointerTextChanged->UCallbackFunction();
    }
}

void UTextBoxBase::OnTextCommittedHandler(
    const FText &Text, 
    ETextCommit::Type CommitMethod
){
    innerTextSaved = Text.ToString();

    if(callbackPointer){
        callbackPointer->UCallbackFunction();
    }
}



/// ---- API -----
void UTextBoxBase::SetHintText(FString hint){
    if(TextBox){
        FText asText = FText::FromString(hint);
        TextBox->SetHintText(asText);
    }
}

void UTextBoxBase::SetCallBack(FSimpleDelegate onFinished){
    if(callbackPointer){
        callbackPointer->SetCallback(onFinished);
    }
}

void UTextBoxBase::SetCallBackTextUpdate(FSimpleDelegate onChanged){
    if(callbackPointerTextChanged){
        callbackPointerTextChanged->SetCallback(onChanged);
    }
}


FString UTextBoxBase::GetText(){
    return innerTextSaved;
}