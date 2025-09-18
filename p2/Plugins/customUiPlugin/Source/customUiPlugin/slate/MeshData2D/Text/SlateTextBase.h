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

    SlateTextBase(const SlateTextBase &other);
    SlateTextBase &operator=(const SlateTextBase &other);


    void SetFont(UFont *font);
    void SetTextSizePixels(float size);
    void SetTextSize(float size);

    void SetColor(FColor colorIn);
    
    void SetCenteredInWidget(bool bCentered);
    
    /// @brief max size in pixels
    /// @param maxSize 
    void SetFitMaxSize(const FVector2D &maxSize);

    void SetText(FString textIn);
    void AppendChar(TCHAR &character);
    void RemoveChar();

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


    //new max size
    bool maxSizeSetup = false;
    FVector2D maxSizePixels;

    FSlateFontInfo fontInfo;

    FColor colorSet;

    bool bCenteredFlag = false;

    /// @brief FSlateFontInfo refreshed.
    void RefreshFont();

    float PixelToSlateSize(float pixels) const;
    float SlateToPixelSize(float size) const;
};
