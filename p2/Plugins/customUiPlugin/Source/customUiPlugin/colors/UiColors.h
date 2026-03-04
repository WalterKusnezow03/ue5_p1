#pragma once

#include "CoreMinimal.h"

class CUSTOMUIPLUGIN_API UiColors{
public:
    //Red
    static constexpr FLinearColor buttonDefaultRed = FLinearColor(
        231.0f / 255.0f, 29.0f / 255.0f, 54.0f / 255.0f, 1.0f
    ); // RGB = (1, 0.5, 0, 1)

    //green
    static constexpr FLinearColor buttonDefaultGreen = FLinearColor(
        27.0f / 255.0f, 153.0f / 255.0f, 139.0f / 255.0f, 1.0f
    );

    //vanilla
    static constexpr FLinearColor buttonDefaultVanilla = FLinearColor(
        236.0f / 255.0f, 228.0f / 255.0f, 183.0f / 255.0f, 1.0f
    );

    //white
    static constexpr FLinearColor buttonWhite = FLinearColor(
        253.0f / 255.0f, 246.0f / 255.0f, 237.0f / 255.0f ,1.0f
    );

    //black
    static constexpr FLinearColor buttonBlack = FLinearColor(
        10.0f / 255.0f, 10.0f / 255.0f, 10.0f / 255.0f, 1.0f
    );

    //black transculent
    static constexpr FLinearColor backgroundBlackTransculent = FLinearColor(
        10.0f / 255.0f, 10.0f / 255.0f, 10.0f / 255.0f, 0.5f
    );
};