#pragma once

#include "CoreMinimal.h"
#include "Styling/SlateTypes.h"

//generates text styles
class CUSTOMUIPLUGIN_API UiTextStyle {

public:

    static FSlateFontInfo DefaultFont(){
        FSlateFontInfo fontInfo;
        fontInfo.Size = 25;

        //Roboto
        FString path = TEXT("/Engine/EngineFonts/Roboto.Roboto");
        fontInfo.FontObject = LoadObject<UObject>(nullptr, *path);
        return fontInfo;

    }

    static FTextBlockStyle DefaultTextStyle(){
        FTextBlockStyle textStyle;
        textStyle.SetFont(DefaultFont());
        textStyle.SetColorAndOpacity(FSlateColor(UiColors::buttonWhite));
        return textStyle;
    }

    static FEditableTextBoxStyle DefaultEditableTextBoxStyle(){
        FEditableTextBoxStyle textBoxStyle;
        textBoxStyle.SetBackgroundColor(FSlateColor(FLinearColor(0.0f, 0.0f, 0.0f, 0.2f)));
        textBoxStyle.SetForegroundColor(FSlateColor(UiColors::buttonWhite));

        textBoxStyle.SetTextStyle(DefaultTextStyle());
        return textBoxStyle;
    }

private:
    UiTextStyle() {};
    ~UiTextStyle() {};
};