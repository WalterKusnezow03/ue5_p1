#pragma once

#include "CoreMinimal.h"
#include "Styling/SlateTypes.h"
#include "customUiPlugin/ui/Widgets/buttons/colors/UiColors.h"

//generates text styles
class CUSTOMUIPLUGIN_API UiTextStyle {

public:

    static FSlateFontInfo DefaultFont(){
        FSlateFontInfo fontInfo;
        fontInfo.Size = 25;
        fontInfo.FontObject = DefaultUFont(); // Font Object also Accepts UObject*
        return fontInfo;
    }

    static UFont *DefaultUFont(){
        FString path = TEXT("/Engine/EngineFonts/Roboto.Roboto");
        UObject *ptr = LoadObject<UObject>(nullptr, *path);
        if(ptr){
            UFont *casted = Cast<UFont>(ptr);
            if(casted){
                return casted;
            }
        }
        return nullptr;
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