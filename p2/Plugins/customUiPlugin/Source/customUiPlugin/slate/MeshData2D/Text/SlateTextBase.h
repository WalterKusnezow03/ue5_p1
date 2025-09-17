#pragma once 

#include "CoreMinimal.h"
#include "Engine/Font.h" 
#include "Fonts/SlateFontInfo.h"

///@brief text abstraction class for easier text making.
///owned by SlatePolygonMap
class CUSTOMUIPLUGIN_API SlateTextBase {


public:
    SlateTextBase();
    ~SlateTextBase();

    void SetFont(UFont *font);
    void SetTextSizePixels(float size);
    void SetTextSize(float size);

    void SetColor(FColor colorIn);
    void SetCenteredInWidget(bool bCentered);

    void SetFitMaxWidthPixels(int widthPixels);

    void SetText(FString textIn);

    int Lenght() const;
    const FString &GetText() const;
    const FSlateFontInfo &FontInfo() const;
    FVector2D Bounds() const;
    FColor Color() const;
    bool bShouldCenteredInWidget() const;

protected:
    FVector2D BoundsSlate() const;
    void LoadDefaultFont();

    float sizeSaved = 24.0f;
    UFont *fontSet = nullptr;

    FString text;

    bool maxWidthSetup = false;
    float maxWidthPixelSaved = 100.0f;

    FSlateFontInfo fontInfo;

    FColor colorSet;

    bool bCenteredFlag = false;

    /// @brief FSlateFontInfo refreshed.
    void RefreshFont();

    float PixelToSlateSize(float pixels) const;
    float SlateToPixelSize(float size) const;
};
